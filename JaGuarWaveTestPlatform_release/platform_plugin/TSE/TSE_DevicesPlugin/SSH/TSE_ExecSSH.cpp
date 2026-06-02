#include "StdAfx.h"
#include "TSE_ExecSSH.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_ExecSSH::CTSE_ExecSSH(void) : mnCheckResultTimeOut(3000),mnTestWaitResult(100)
    {
    }


    CTSE_ExecSSH::~CTSE_ExecSSH(void)
    {
    }

    const wchar_t* CTSE_ExecSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ExecSSH\",\
                \"TestName\": \"SSH执行命令(AP)\",\
                \"Tip\": \"测试提示\",\
                \"Command\": \"命令\",\
                \"TestWaitResult\": \"发送测试信息等待结果生产时间MS\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
                \"KillExecString\": \"exec执行过程中出现10次指定字符串自动KILL exec命令\",\
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

    bool CTSE_ExecSSH::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"KillExecString"))
        {
            mstrKillExecString = JGW_W2A_W(strParamValue); 
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_ExecSSH::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        return true;
    }

    bool CTSE_ExecSSH::TSE_Run()
    {
        std::wstring strCommand = JGW_A2W(mstrCommand);
        GetEnvironmentCommandString(strCommand);
        LogD(mstrCommand.c_str());

        std::string strRead;
        int i = 0;

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShell(JGW_W2A(strCommand),10))
        {
            LogE(L"Send SSH Command Fail");
            return false;
        }
        Sleep(mnTestWaitResult);

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->RecvSSHShellCallBack(strRead,[&](const std::string& strRead,int len)-> bool{
            LogI_F(strRead.c_str());
            if (!mstrKillExecString.empty())
            {
                if (NULL != JGW_StrCaseStr(strRead.c_str(),"Server listening"))
                {
                    i = 0;
                }

                if (NULL != JGW_StrCaseStr(strRead.c_str(),mstrKillExecString.c_str()))
                {
                    i ++;
                    if (i > 10)
                    {
                        char szBuf[3] = {0x03,'\n','\0'};
                        CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShellCharacter(szBuf,2);
                    }  
                }
            }
            return true;
        }))
        {
            LogE(L"Recv SSH Command Fail");
            return false;
        }

        if (!mstrTip.empty() && IDOK  != MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"测试提示",MB_OKCANCEL))
        {
            return false;
        }

        Sleep(mnTestWaitResult);

        if (!mstrReadResultEnvironment.empty())
        {
            LogI_F(L"%s:%s",mstrReadResultEnvironment.c_str(),JGW_A2W(strRead).c_str());
            GetGlobalEnvironment()->PutAnsiString(mstrReadResultEnvironment,strRead);
        }

        return TSE_CheckTestResult(strRead);
    }


    bool CTSE_ExecSSH::TSE_CheckTestResult(const std::string& strRead)
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