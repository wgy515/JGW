#include "StdAfx.h"
#include "TSE_DisconnectDiagPhone.h"
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>
namespace JGW
{
    CTSE_DisconnectDiagPhone::CTSE_DisconnectDiagPhone(void) : mstrResourceContextEnv(L"TSE_RESOURCE_CONTEXT")
    {
    }


    CTSE_DisconnectDiagPhone::~CTSE_DisconnectDiagPhone(void)
    {
    }

    const wchar_t* CTSE_DisconnectDiagPhone::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisconnectDiagPhone\",\
                \"TestName\": \"Disconnect Diag Port\",\
                \"ResourceContextEnv\": \"get open port handle environment\"\
                }";
    }

    bool CTSE_DisconnectDiagPhone::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DisconnectDiagPhone::TSE_Run()
    {
        HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnv);
        if (NULL != hResourceContext)
        {
            QLIB_DisconnectServer(hResourceContext);
            GetGlobalEnvironment()->PutInt(mstrResourceContextEnv,NULL);
        }  
        return true;
    }
}
