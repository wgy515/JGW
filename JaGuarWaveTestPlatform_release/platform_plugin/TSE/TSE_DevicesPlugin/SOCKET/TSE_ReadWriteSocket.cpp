#include "StdAfx.h"
#include "TSE_ReadWriteSocket.h"
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_ReadWriteSocket::CTSE_ReadWriteSocket(void) : mstrConnectSocketInstanceEnvironment(L"TSE_SOCKET"),mnCheckResultTimeOutSec(10),mnTestWaitResultMS(0),mnWriteTimeOutSec(10),mbIsHexReadWrite(false),mbReadOnly(false)
    {
    }


    CTSE_ReadWriteSocket::~CTSE_ReadWriteSocket(void)
    {
    }

    const wchar_t* CTSE_ReadWriteSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadWriteSerialComPort\",\
                \"TestName\": \"读写SOCKET\",\
                \"ConnectSocketInstanceEnvironment\": \"保存连接SOCKET实例变量名称\",\
                \"Write\": \"写相关信息\",\
				\"ReadOnly\": \"只读\",\
                \"IsHexReadWrite\": \"是否以十六进制方式解析读取,中间以空格作为分隔符\",\
                \"WriteTimeOutSec\":\"写入超时时间,单位秒\",\
                \"TestWaitResultMS\":\"发送测试信息等待结果生产时间，单位毫秒\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"CheckResultTimeOutSec\": \"检查测试结果超时时间,包括读取时间,单位秒\",\
                \"CompareStringCount\": \"比较多少项字符串\",\
                \"CompareStringName1\": \"key值 1是索引\",\
                \"CompareStringValue1\": \"与KEY值对应VALUE进行比对\",\
                \"CompareStringErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"CompareIntCount\": \"比对多少项INT类型\",\
                \"CompareIntName1\": \"key值 1是索引\",\
                \"CompareIntValue1\": \"与KEY值对应VALUE进行比对,A~C,当前值在大于等于A小于等于C即可\",\
                \"CompareIntErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"FindStringCount\": \"查找字符串,需要匹配多少项\",\
                \"FindStringName1\": \"key值 1是索引\",\
                \"FindStringValue1\": \"查找当前值是否在key值对应字符串中\",\
                \"FindStringErrorMsg1\": \"比对失败后显示的错误字符串\"\
                }";
    }

    bool CTSE_ReadWriteSocket::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ConnectSocketInstanceEnvironment"))
        {
            mstrConnectSocketInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Write"))
        {
            mstrWrite = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteTimeOutSec"))
        {
            mnWriteTimeOutSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsHexReadWrite"))
        {
            mbIsHexReadWrite = _ttoi(strParamValue) == 1;
        }
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadOnly"))
		{
			mbReadOnly = _ttoi(strParamValue) == 1;
		}
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestWaitResultMS"))
        {
            mnTestWaitResultMS = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckResultTimeOutSec"))
        {
            mnCheckResultTimeOutSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultEnvironment"))
        {
            mstrReadResultEnvironment = strParamValue;
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_ReadWriteSocket::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        return true;
    }

    bool CTSE_ReadWriteSocket::TSE_Run()
    {
        if (mstrConnectSocketInstanceEnvironment.empty())
        {
            LogE(L"Error : Socket Instance Config Environment Name is empty");
            return false;
        }

        CCJGW_SyncSocket* pSyncSocket = (CCJGW_SyncSocket*)(GetGlobalEnvironment()->GetInt(mstrConnectSocketInstanceEnvironment));
        if (NULL == pSyncSocket)
        {
            LogE(L"Error : Socket Instance is empty");
            return false;
        }

        if (0 == mstrWrite.length())
        {
            mstrWrite = L"\n";
        }
        else if (1 == mstrWrite.length() && 0 == mstrWrite.compare(L" "))
        {
            mstrWrite = L"\r";
        }

        //! write socket
        if (mstrWrite.length() > 0 && !mbReadOnly)
        {
            std::wstring strWrite (mstrWrite);
            GetEnvironmentCommandString(strWrite);
            LogI_F(L"Serial Write : %s",strWrite.c_str());

            if (mbIsHexReadWrite)
            {
                char szBuf[256] = {0};
                std::vector<size_t> vTemp;
                JGW_ParserIntAToHex(JGW_W2A(strWrite).c_str()," ",vTemp);
                for (size_t i = 0;i < vTemp.size();i ++)
                {
                    szBuf[0] = vTemp[i]/* - '0'*/;
                }
                if (!pSyncSocket->SendSocket(szBuf,vTemp.size()))
                {
                    GetLogServices()->ErrorLogFormat(L"Write Socket Failed .(%s)",strWrite.c_str());
                    return false;
                }
            }
            else
            {
                std::string strWriteA = JGW_W2A(strWrite);
                if (!pSyncSocket->SendSocket(strWriteA.c_str(),strWriteA.size()))
                {
                    GetLogServices()->ErrorLogFormat(L"Write Socket Failed .(%s)",strWrite.c_str());
                    return false;
                }
            }
        }

        //! 写入命令后可能需要等待一段时间在读取
        Sleep(mnTestWaitResultMS);
        //! 是否读取SOCKET
        if (!mstrReadResultEnvironment.empty() || mCheckTestResult.IsCompareResult())
        {
            jgw_timer jt;
            std::wstring strReadBuf,strTemp;

            while (jt.elapsed() <= mnCheckResultTimeOutSec)
            {
                std::string strRead;
                char szBuf[256] = {0};
                int len = pSyncSocket->RecvSocket(szBuf,256);
                if (len < 0 )
                {
                    return false;
                }

                if (mbIsHexReadWrite)
                {

                    for (int i = 0;i < len;i ++)
                    {
                        if (strReadBuf.empty() && strRead.empty())
                        {
                            strRead += JGW_GetFormatString("%02X",szBuf[i]);
                        }
                        else
                        {
                            strRead += JGW_GetFormatString(" %02X",szBuf[i]);
                        }	
                    }
                }

                strTemp = JGW_A2W_A(szBuf);
                strReadBuf += strTemp;
                LogI_F(L"Socket Read : %s",strTemp.c_str());
                //! save read 
                if (!mstrReadResultEnvironment.empty())
                {
                    GetGlobalEnvironment()->PutString(mstrReadResultEnvironment,strReadBuf);
                }

                if (mCheckTestResult.CheckCompareResult(strReadBuf))
                {
                    return true;
                }
                Sleep(200);
            }
            return false;
        }

        return true;
    }
}

