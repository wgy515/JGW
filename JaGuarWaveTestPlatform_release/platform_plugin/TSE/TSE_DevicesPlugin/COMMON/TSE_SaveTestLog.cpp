#include "StdAfx.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "TSE_SaveTestLog.h"

namespace JGW
{
    CTSE_SaveTestLog::CTSE_SaveTestLog(void)
    {
    }


    CTSE_SaveTestLog::~CTSE_SaveTestLog(void)
    {
    }

    const wchar_t* CTSE_SaveTestLog::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaveTestLog\",\
                \"TestName\": \"将log保存到当前程序目录下TestLog目录下\",\
                \"SNEnvironment\": \"SN保存的环境变量名\",\
                \"LogEnvironment\": \"要保存的log路径环境变量名\"\
                }";
    }

    bool CTSE_SaveTestLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"LogEnvironment"))
        {
            mstrLogEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaveTestLog::TSE_Run()
    {
        //! TSE_TEST_RESULT
        bool bResult = GetGlobalEnvironment()->GetBool(TSE_TEST_RESULT);
        std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
        std::wstring strLogPath = GetGlobalEnvironment()->GetString(mstrLogEnvironment);
        std::wstring strTestName = GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME);

        LogD_F(L"Save Test Log SN : %s,LogPath : %s,Test Name : %s",strSN.c_str(),strLogPath.c_str(),strTestName.c_str());
        mMoveTestLogging.MoveTestLogging(strTestName.c_str(),strSN.c_str(),strLogPath.c_str(),bResult);
        JGW_DeleteFile(strLogPath);

        return true;
    }
}