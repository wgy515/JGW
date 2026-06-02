#include "StdAfx.h"
#include "TSE_BuildTestLog.h"
#include <fstream>

namespace JGW
{
    CTSE_BuildTestLog::CTSE_BuildTestLog(void)
    {
    }


    CTSE_BuildTestLog::~CTSE_BuildTestLog(void)
    {
    }

    const wchar_t* CTSE_BuildTestLog::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildTestLog\",\
                \"TestName\": \"构造log文件\",\
                \"LogFilePath\": \"log文件路径 %SN% %DATE% %PROJECT% %TIMESTAMP%\",\
                \"LogEnvironment\": \"文件保存路径环境变量\",\
                \"SNEnvironment\": \"SN环境变量名称\",\
                \"Log\": \"log $$_xxxx_$$ xxxx表示环境变量\"\
                }";
    }

    bool CTSE_BuildTestLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"LogFilePath",strParamName))
        {
            mstrLogFilePath = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"LogEnvironment",strParamName))
        {
            mstrLogEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Log",strParamName))
        {
            mstrLog = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildTestLog::TSE_Run()
    {
        std::wstring strLogFilePath = mstrLogFilePath,strLog = mstrLog;

        JGW_ReplaceStringW(strLogFilePath,L"%SN%",GetGlobalEnvironment()->GetString(mstrSNEnvironment));
        JGW_ReplaceStringW(strLogFilePath,L"%DATE%",JGW_GetTimeFolder().c_str());
        JGW_ReplaceStringW(strLogFilePath,L"%PROJECT%",GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME));
        JGW_ReplaceStringW(strLogFilePath,L"%TIMESTAMP%",JGW_GetCurrentTimeStampYMDHMS());
        strLogFilePath = JGW_RealativePathToAbsPath(strLogFilePath.c_str());
        JGW_CreateDirectory(strLogFilePath.c_str(),false);
        GetGlobalEnvironment()->PutString(mstrLogEnvironment,strLogFilePath);

        GetEnvironmentCommandString(strLog);

        std::wofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(strLogFilePath.c_str(),std::ios_base::app);
        if (!fileStream.is_open()) return false;

        fileStream << strLog;
        fileStream.close();
        return true;
    }
}

