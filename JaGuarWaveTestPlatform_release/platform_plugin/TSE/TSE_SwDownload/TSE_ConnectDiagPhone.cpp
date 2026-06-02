#include "StdAfx.h"
#include "TSE_ConnectDiagPhone.h"
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>

namespace JGW
{
    CTSE_ConnectDiagPhone::CTSE_ConnectDiagPhone(void) : mstrResourceContextEnv(L"TSE_RESOURCE_CONTEXT"),mbUseQpst(false),mnTimeOutSec(10)
    {
    }


    CTSE_ConnectDiagPhone::~CTSE_ConnectDiagPhone(void)
    {
    }

    const wchar_t* CTSE_ConnectDiagPhone::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectDiagPhone\",\
                \"TestName\": \"Connect Diag Phone\",\
                \"ResourceContextEnv\": \"get open port handle environment\",\
                \"PortEnv\": \"is empty : auto select port\",\
                \"UseQpst\": \"0: NO 1: YES\",\
                \"TimeOutSec\": \"connect diag phone time out sec\"\
                }";
    }

    bool CTSE_ConnectDiagPhone::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PortEnv"))
        {
            mstrPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UseQpst"))
        {
            mbUseQpst = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectDiagPhone::TSE_Init()
    {
        if (mbUseQpst)
        {
            QLIB_SetLibraryMode(QLIB_LIB_MODE_QPST);
        }
        else
        {
            QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
        }
        return true;
    }

    bool CTSE_ConnectDiagPhone::TSE_Run()
    {
        int nPort = -1;
        HANDLE hResourceContext = NULL;
        if (!mstrPortEnv.empty()) nPort = GetGlobalEnvironment()->GetInt(mstrPortEnv);
        if (nPort <= 0)
        {
            nPort = QLIB_COM_AUTO_DETECT;
        }
        if (QLIB_COM_AUTO_DETECT == nPort)
        {
            hResourceContext = QLIB_ConnectServerWithWait(nPort,mnTimeOutSec * 1000);
        }
        else
        {
            hResourceContext = QLIB_ConnectServer(nPort);
        }
       
        if (NULL == hResourceContext)
        {
            LogE(L"Could not connect to the COMPORT");
            return false;
        }
        GetGlobalEnvironment()->PutInt(mstrResourceContextEnv,(int)hResourceContext);

        return true;
    }
}

