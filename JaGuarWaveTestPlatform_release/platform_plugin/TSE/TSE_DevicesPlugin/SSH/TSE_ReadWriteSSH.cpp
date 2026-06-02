#include "StdAfx.h"
#include "TSE_ReadWriteSSH.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <regex>
namespace JGW
{
    CTSE_ReadWriteSSH::CTSE_ReadWriteSSH(void) : mnCheckResultTimeOut(3000),mnTestWaitResult(100),mbWriteOnly(false),mnReadResultRegexSearchIndex(0)
    {
    }


    CTSE_ReadWriteSSH::~CTSE_ReadWriteSSH(void)
    {
    }

    const wchar_t* CTSE_ReadWriteSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadWriteSSH\",\
                \"TestName\": \"SSH读写命令(AP)\",\
                \"Tip\": \"测试提示\",\
                \"Command\": \"命令\",\
                \"TestWaitResult\": \"发送测试信息等待结果生产时间MS\",\
                \"ReadResultEnvironment\": \"保存命令返回值到环境变量中\",\
				\"ReadResultRegexSearch\": \"搜索匹配字符串\",\
				\"ReadResultRegexSearchIndex\": \"搜索到的匹配字符串索引\",\
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

    bool CTSE_ReadWriteSSH::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultRegexSearch"))
		{
			mstrReadResultRegexSearch = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultRegexSearchIndex"))
		{
			mnReadResultRegexSearchIndex =  _ttoi(strParamValue);
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

    bool CTSE_ReadWriteSSH::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        return true;
    }

    bool CTSE_ReadWriteSSH::TSE_Run()
    {
        std::wstring strCommand = JGW_A2W(mstrCommand);
        GetEnvironmentCommandString(strCommand);
        LogD(mstrCommand.c_str());
        //! bool bRead = true;
        //! if (0 == mnCompareIntCount && 0 == mnCompareStringCount && 0 == mnFindStringCount) bRead = false;
        if (strCommand.empty())
        {
            strCommand = L"\n";
        }

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShell(JGW_W2A(strCommand)))
        {
            LogE(L"Sending the SSH command failed\n");
            return false;
        }

        if (mbWriteOnly)
        {
            return true;
        }

        /*std::string strRead;
        if (!CTSE_DevicesGlobalResource::GetSSHShell()->RWSSHShell(JGW_W2A(strCommand),strRead,!mbWriteOnly,mnTestWaitResult,mnCheckResultTimeOut))
        {
        LogE(L"发送读取SSH命令失败\n");
        return false;
        }*/

        if (!mstrTip.empty() && IDOK  != MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"Test Tip",MB_OKCANCEL))
        {
            return false;
        }

        Sleep(mnTestWaitResult);
        std::string strRead;
        CTSE_DevicesGlobalResource::GetSSHShell()->RecvSSHShell(strRead,mnCheckResultTimeOut);
        JGW_EraseFristToRemoveChar(strRead,L'\n');
        JGW_EraseAfterToRemoveChar(strRead,L'\n'); 
        JGW_EraseLastAndFristTwoChars(strRead);
        LogI(JGW_A2W(strRead).c_str());
        //         if (!CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShell(mstrCommand))
        //         {
        //             LogE(L"发送SSH命令失败\n");
        //             return false;
        //         }
        /*Sleep(mnTestWaitResult);*/

		if (!TSE_CheckTestResult(strRead))
		{
			return false;
		}

        if (!mstrReadResultEnvironment.empty())
        {
#if 0
            LogI_F(L"%s:%s",mstrReadResultEnvironment.c_str(),JGW_A2W(strRead).c_str());
            GetGlobalEnvironment()->PutAnsiString(mstrReadResultEnvironment,strRead);
#else
			//! 正则表达式搜索
			std::wstring strReadResultRegexSearchValue(JGW_A2W(strRead));
			if (!mstrReadResultRegexSearch.empty())
			{
				std::wsmatch match;
				std::wregex pattern(mstrReadResultRegexSearch);
				if (!regex_search(strReadResultRegexSearchValue,match,pattern))
				{
					return false;
				}

				if (mnReadResultRegexSearchIndex < match.size())
				{
					strReadResultRegexSearchValue = match[mnReadResultRegexSearchIndex];
				}
				else
				{
					return false;
				}
			}
			LogI_F(L"%s:%s",mstrReadResultEnvironment.c_str(),strReadResultRegexSearchValue.c_str());
			GetGlobalEnvironment()->PutString(mstrReadResultEnvironment,strReadResultRegexSearchValue);
#endif
        }

        return true;
    }

    bool CTSE_ReadWriteSSH::TSE_CheckTestResult(const std::string& strRead)
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