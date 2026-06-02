#include "StdAfx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "TSE_GlobalEnvironmentDefault.h"
#include "TSE_LogServicesDefault.h"
#include "TSE_TestStatusServiceDefault.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_GlobalEnvironmentDefault* GetGlobalEnvironmentDefault();
    CTSE_LogServicesDefault* GetLogServicesDefault();
    CTSE_TestStatusServiceImpl* GetTestStatusServiceDefault();


    CTSE_GlobalEnvironmentImpl* CTSE_GlobalResources::mpGlobalEnvironmentImpl = GetGlobalEnvironmentDefault();
    CTSE_LogServicesImpl* CTSE_GlobalResources::mpLogServicesImpl = GetLogServicesDefault();
    CTSE_TestStatusServiceImpl* CTSE_GlobalResources::mpTestStatusServiceImpl = GetTestStatusServiceDefault();

    CTSE_GlobalEnvironmentDefault* GetGlobalEnvironmentDefault()
    {
        static CTSE_GlobalEnvironmentDefault gGlobalEnvironmentDefault;
        return &gGlobalEnvironmentDefault;
    }

    CTSE_LogServicesDefault* GetLogServicesDefault()
    {
        static CTSE_LogServicesDefault gLogServicesDefault;
        return &gLogServicesDefault;
    }

    CTSE_TestStatusServiceImpl* GetTestStatusServiceDefault()
    {
        static CTSE_TestStatusServiceDefault gTestStatusServiceDefault;
        return &gTestStatusServiceDefault;
    }

    CTSE_GlobalResources::CTSE_GlobalResources()
    {

    }

    const wchar_t* CTSE_GlobalResources::TSE_GetParamDescription()
    {
        return L"{\"RealName\":\"CTSE_GlobalResources\",\"TestName\":\"全局资源实例配置,主要用于资源回调使用(内部使用)\",\"TSE_GLOBAL_ENVIRONMENT\":\"全局环境变量实例名称\",\"TSE_LOG_SERVICES\":\"全局log服务实例\",\"TSE_TESTSTATUS_SERVICES\":\"全局测试状态上报服务\"}";
    }

    bool CTSE_GlobalResources::TSE_AddParam(const wchar_t* strName,const wchar_t* strValue)
    {
        if (TSE_PARAM_NAME_EQUAL(strName,TSE_GLOBAL_ENVIRONMENT))
        {
            if (NULL == strValue) mpGlobalEnvironmentImpl = GetGlobalEnvironment();
            else mpGlobalEnvironmentImpl = (CTSE_GlobalEnvironmentImpl*)strValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strName,TSE_LOG_SERVICES))
        {
            if (NULL == strValue) mpLogServicesImpl = GetLogServicesDefault();
            else mpLogServicesImpl = (CTSE_LogServicesImpl*)strValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strName,TSE_TESTSTATUS_SERVICES))
        {
            if (NULL == strValue) mpTestStatusServiceImpl = GetTestStatusServiceDefault();
            else mpTestStatusServiceImpl = (CTSE_TestStatusServiceImpl*)strValue;
        }
        else
        {
            if(CTSE_TestBase::TSE_AddParam(strName,strValue)) return true;
        }
        return true;
    }

    CTSE_LogServicesImpl* GetLogServices()
    {
        return CTSE_GlobalResources::mpLogServicesImpl;
    }

    CTSE_GlobalEnvironmentImpl* GetGlobalEnvironment()
    {
        return CTSE_GlobalResources::mpGlobalEnvironmentImpl;
    }

    CTSE_TestStatusServiceImpl* GetTestStatusService()
    {
        return CTSE_GlobalResources::mpTestStatusServiceImpl;
    }

    /*extern*/ HWND GetGlobalMainHwnd()
    {
        HWND mMainHwnd = (HWND)GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
        if (NULL == mMainHwnd) mMainHwnd = FindWindow(NULL,NULL);
        GetGlobalEnvironment()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)mMainHwnd);
        return mMainHwnd;
    }

    void GetEnvironmentCommandString(std::wstring& strCommand)
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;

        while (std::wstring::npos != nStart)
        {
            nEnd = strTemp.find(L"_$$",nStart + 3);
            if (nEnd == std::wstring::npos) return ;
            strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
            strEnvironmentValue = GetGlobalEnvironment()->GetString(strEnvironmentName);
            JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
            JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
            nStart = strTemp.find(L"$$_",nEnd + 3);
        } 
        GetLogServices()->DebugLogFormat(L"Environment Command : %s",strCommand.c_str());
    }
}