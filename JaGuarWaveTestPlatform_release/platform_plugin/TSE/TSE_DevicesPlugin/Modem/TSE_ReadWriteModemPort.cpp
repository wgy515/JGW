#include "StdAfx.h"
#include "TSE_ReadWriteModemPort.h"
#include "TSE_OpenModemPort.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_ReadWriteModemPort::CTSE_ReadWriteModemPort(void) : mnCheckResultTimeOut(3000),mnTestWaitResult(100),mbWriteOnly(false)
    {
    }


    CTSE_ReadWriteModemPort::~CTSE_ReadWriteModemPort(void)
    {
    }


    const wchar_t* CTSE_ReadWriteModemPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadWriteModemPort\",\
                \"TestName\": \"Read Write Modem Device\",\
                \"Tip\": \"测试提示\",\
                \"Command\": \"命令\",\
                \"TestWaitResult\": \"发送测试信息等待结果生产时间MS\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"WriteOnly\": \"只写不读标记位\",\
                \"CheckResultTimeOut\": \"检查测试结果超时时间\",\
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

    bool CTSE_ReadWriteModemPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Tip"))
        {
            mstrTip = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Command"))
        {
            mstrCommand = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestWaitResult"))
        {
            mnTestWaitResult = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckResultTimeOut"))
        {
            mnCheckResultTimeOut = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultEnvironment"))
        {
            mstrReadResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteOnly"))
        {
            mbWriteOnly = (1 == _ttoi(strParamValue));
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_ReadWriteModemPort::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        return true;
    }

    bool CTSE_ReadWriteModemPort::TSE_Run()
    {
        std::wstring strCommand = JGW_A2W(mstrCommand);
        GetEnvironmentCommandString(strCommand);
        LogD(mstrCommand.c_str());

        CCJGW_CSerialModemPort* pSerialModemPort =  GetSerialModemPort();
        if (NULL == pSerialModemPort)
        {
            LogE(L"Get Serial Modem Class Fail");
            return false;
        }

        if (!pSerialModemPort->WriteAsyncATModem(JGW_W2A(strCommand).c_str(),strCommand.length()))
        {
            LogE(L"write AT command fail");
            return false;
        }

        if (mbWriteOnly)
        {
            return true;
        }

        if (!mstrTip.empty() && IDOK  != MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"测试提示",MB_OKCANCEL))
        {
            return false;
        }
        Sleep(mnTestWaitResult);

        std::string strRead;
        if (!pSerialModemPort->ReadATAsyncModem(strRead,mnCheckResultTimeOut))
        {
            LogE(L"read AT command fail");
            return false;
        }

        LogI_F(L"shell: %s",JGW_A2W(strRead).c_str());
        if (!mstrReadResultEnvironment.empty())
        {
            LogI_F(L"%s:%s",mstrReadResultEnvironment.c_str(),JGW_A2W(strRead).c_str());
            GetGlobalEnvironment()->PutAnsiString(mstrReadResultEnvironment,strRead);
        }

        return TSE_CheckTestResult(strRead);
    }

    bool CTSE_ReadWriteModemPort::TSE_CheckTestResult(const std::string& strRead)
    {
        mstrReadBuf = JGW_A2W(strRead);
        LogD(mstrReadBuf.c_str());

        if(!mCheckTestResult.CheckCompareResult(mstrReadBuf))
        {
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }
}
