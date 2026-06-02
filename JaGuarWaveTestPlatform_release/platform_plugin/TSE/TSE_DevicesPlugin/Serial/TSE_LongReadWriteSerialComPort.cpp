#include "StdAfx.h"
#include "TSE_LongReadWriteSerialComPort.h"
#include "../TSE_DevicesGlobalResource.h"
#include <regex>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
    static CCJGW_SimpleThread gSimpleThread;
    bool CTSE_LongReadWriteSerialComPort::mbStartReadThread = false;
    bool gbIsHexReadWrite = false;
    std::wstring gstrReadSerialBuffer;
    CCJGW_CriticalSectionLock gCriticalSectionLock;

    CTSE_LongReadWriteSerialComPort::CTSE_LongReadWriteSerialComPort(void): mnCheckResultTimeOutSec(10),mnTestWaitResultMS(0),mnWriteTimeOutSec(10),mbIsHexReadWrite(false),mnReadResultRegexSearchIndex(0),mbIsClearReadBuffer(true),mbReadOnly(false)
    {
    }


    CTSE_LongReadWriteSerialComPort::~CTSE_LongReadWriteSerialComPort(void)
    {
    }

    const wchar_t* CTSE_LongReadWriteSerialComPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_LongReadWriteSerialComPort\",\
                \"TestName\": \"读写串行USB设备\",\
                \"Write\": \"写相关信息\",\
                \"ReadOnly\":\"只读处理\",\
                \"ErrorHandler\":\"错误处理字符串\",\
				\"StartRspCommand\":\"起始响应命令\",\
				\"EndRspCommand\":\"结束响应命令\",\
                \"IsHexReadWrite\": \"是否以十六进制方式解析读取,中间以空格作为分隔符\",\
                \"IsClearReadBuffer\": \"是否清空读缓冲区\",\
                \"WriteTimeOutSec\":\"写入超时时间,单位秒\",\
                \"TestWaitResultMS\":\"发送测试信息等待结果生产时间，单位毫秒\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"ReadResultRegexSearch\": \"搜索匹配字符串\",\
                \"ReadResultRegexSearchIndex\": \"搜索到的匹配字符串索引\",\
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

    bool CTSE_LongReadWriteSerialComPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Write"))
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsClearReadBuffer"))
        {
            mbIsClearReadBuffer = _ttoi(strParamValue) == 1;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadOnly"))
        {
            mbReadOnly = _ttoi(strParamValue) == 1;
        }
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"StartRspCommand"))
		{
			mstrStartRspCommand = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EndRspCommand"))
		{
			mstrEndRspCommand = strParamValue;
		}
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ErrorHandler"))
        {
            mstrErrorHandler = strParamValue;
            JGW_ReplaceStringW(mstrErrorHandler,L"\\r",L"\r");
            JGW_ReplaceStringW(mstrErrorHandler,L"\\n",L"\n");
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultRegexSearch"))
        {
            mstrReadResultRegexSearch = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultRegexSearchIndex"))
        {
            mnReadResultRegexSearchIndex =  _ttoi(strParamValue);
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_LongReadWriteSerialComPort::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        //! 处理写入信息
        JGW_ReplaceStringW(mstrWrite,L"\\r",L"\r");
        JGW_ReplaceStringW(mstrWrite,L"\\n",L"\n");
        if (0 == mstrWrite.length())
        {
            mstrWrite = L"\n";
        }
        else if (1 == mstrWrite.length() && 0 == mstrWrite.compare(L" "))
        {
            mstrWrite = L"\r";
        }
        return true;
    }

    bool CTSE_LongReadWriteSerialComPort::WriteSerialBuffer()
    {
        std::wstring strWrite (mstrWrite);
        GetEnvironmentCommandString(strWrite);
        LogI_F(L"Serial Write : %s",strWrite.c_str());

        size_t nWriteBufferLen = 0;
        const char* pWriteBuffer;
        std::string strWriteA;
        std::vector<char> vBuffer;
        //! 是否使用十六进制读写方案
        if (mbIsHexReadWrite)
        {
            std::vector<size_t> vTemp;
            JGW_ParserIntAToHex(JGW_W2A(strWrite).c_str()," ",vTemp);
            for (size_t i = 0;i < vTemp.size();i ++) vBuffer.push_back(vTemp[i]);
            pWriteBuffer = &vBuffer[0];
            nWriteBufferLen = vBuffer.size();
        }
        else
        {
            strWriteA = (JGW_W2A(strWrite));
            pWriteBuffer = strWriteA.c_str();
            nWriteBufferLen = strWriteA.length();
        }

        if (!CTSE_DevicesGlobalResource::GetSerialComPort().WriteBinaryFileBufferToComPort(pWriteBuffer,nWriteBufferLen,mnWriteTimeOutSec * 1000))
        {
            GetLogServices()->ErrorLogFormat(L"Write Serial Failed .(%s)",strWrite.c_str());
            return false;
        }

        return true;
    }

    void CTSE_LongReadWriteSerialComPort::ReadSerialThreadCallback(void *lpParam)
    {
        CTSE_LongReadWriteSerialComPort::mbStartReadThread = true;
        std::wstring strTemp;
        char szBuf[4096] = {0};
        while (CTSE_DevicesGlobalResource::GetSerialComPort().IsOpenComPort())
        {
            memset(szBuf,0x00,4096);
            int len = CTSE_DevicesGlobalResource::GetSerialComPort().ReadAsyncCommand(szBuf,4096,3 * 1000);
            if (len < 0 )
            {
                break;
            }
            //! 是否是二进制读写
            if (gbIsHexReadWrite)
            {
                for (int i = 0;i < len;i ++)
                {
                    if (gstrReadSerialBuffer.empty() && strTemp.empty())
                    {
                        strTemp += JGW_GetFormatWString(L"%02X",szBuf[i]);
                    }
                    else
                    {
                        strTemp += JGW_GetFormatWString(L" %02X",szBuf[i]);
                    }	
                }
            }
            else
            {
                strTemp = JGW_A2W_A(szBuf);
            }
            {
                CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
                gstrReadSerialBuffer += strTemp;
            }
            if (!strTemp.empty())
            {
                LogI_F(L"%s",strTemp.c_str());
            } 
        }
        CTSE_LongReadWriteSerialComPort::mbStartReadThread = false;
    }

    bool CTSE_LongReadWriteSerialComPort::TSE_Run()
    {
        //! check serial com port
        if (!CTSE_DevicesGlobalResource::GetSerialComPort().IsOpenComPort())
        {
            GetLogServices()->ErrorLog(L"Serial Com Port Is Not Open .");
            return false;
        }

        if (!gSimpleThread.IsThreadRun())
        {
            mbStartReadThread = false;
        }

        if (!mbStartReadThread)
        {
            if (!gSimpleThread.CreateMessageThread(ReadSerialThreadCallback,NULL,true))
            {
                GetLogServices()->ErrorLog(L"Description Failed to create the serial port read thread");
                return false;
            }
        }
        gbIsHexReadWrite = mbIsHexReadWrite;
        //! 在写入之前清空缓冲区
        if (mbIsClearReadBuffer)
        {
            CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
            gstrReadSerialBuffer = L"";
        }
        //! write serial
        if (!WriteSerialBuffer())
        {
            return false;
        }
        //! 写入命令后可能需要等待一段时间在读取
        Sleep(mnTestWaitResultMS);
        //! 检查读取结果
        if (!CheckReadSerialResult())
        {
            return false;
        }

        return true;
    }

    bool CTSE_LongReadWriteSerialComPort::CheckReadSerialResult()
    {
        //! 是否保存读取结果，或者进行比对
        if (mstrReadResultEnvironment.empty() && !mCheckTestResult.IsCompareResult())
        {
            return true;
        }

        jgw_timer jt;
        std::wstring strReadBuf;
        //! 检查读取结果超时
        while (jt.elapsed() <= mnCheckResultTimeOutSec)
        {
            Sleep(100);
            {
                CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
                strReadBuf = gstrReadSerialBuffer;
            }
			if (!mstrStartRspCommand.empty() && !mstrEndRspCommand.empty())
			{
				JGW_ReplaceStringW(strReadBuf,L"\r",L"");
				JGW_ReplaceStringW(strReadBuf,L"\n",L"");
				strReadBuf = JGW_GetSubStrToStartEndStrW(strReadBuf,mstrStartRspCommand,mstrEndRspCommand,L"Error : Not Find");
				JGW_EraseLastAndFristTwoCharsW(strReadBuf);
				JGW_ReplaceStringW(strReadBuf,L" ",L"");
			}
            //! 检查结果
            if (!mCheckTestResult.CheckCompareResult(strReadBuf))
            {
                Sleep(200);
                continue;
            }

            if (mstrReadResultEnvironment.empty())
            {
                return true;
            }
            //! 正则表达式搜索
            std::wstring strReadResultRegexSearchValue(strReadBuf);
            if (!mstrReadResultRegexSearch.empty())
            {
                std::wsmatch match;
                std::wregex pattern(mstrReadResultRegexSearch);
                if (!regex_search(strReadResultRegexSearchValue,match,pattern))
                {
                    Sleep(200);
                    continue;
                }

                if (mnReadResultRegexSearchIndex < match.size())
                {
                    strReadResultRegexSearchValue = match[mnReadResultRegexSearchIndex];
                }
                else
                {
                    Sleep(200);
                    continue;
                }
            }

            GetGlobalEnvironment()->PutString(mstrReadResultEnvironment,strReadResultRegexSearchValue);
            return true;
        }
        if (!mstrErrorHandler.empty() && std::wstring::npos != strReadBuf.find(mstrErrorHandler))
        {
            char pWriteBuffer[2] = {0x03,0x0D};
            CTSE_DevicesGlobalResource::GetSerialComPort().WriteBinaryFileBufferToComPort(pWriteBuffer,2,mnWriteTimeOutSec * 1000);
            //CTSE_DevicesGlobalResource::GetSerialComPort().ReadAsyncCommand(szBuf,4096,mnCheckResultTimeOutSec * 1000);
        }

        return false;
    }


}
