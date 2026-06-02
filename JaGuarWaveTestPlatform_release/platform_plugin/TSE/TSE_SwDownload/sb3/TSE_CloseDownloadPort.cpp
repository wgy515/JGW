#include "StdAfx.h"
#include "TSE_CloseDownloadPort.h"
#include "CJGW_QcommDownloadSerialPort.h"
namespace JGW
{
    CTSE_CloseDownloadPort::CTSE_CloseDownloadPort(void) : mstrSaharaSerialClassEnv(L"TSE_SERIALPORT_CONTEXT")
    {
    }


    CTSE_CloseDownloadPort::~CTSE_CloseDownloadPort(void)
    {
    }

    const wchar_t* CTSE_CloseDownloadPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseDownloadPort\",\
                \"TestName\": \"close download port\",\
                \"SaharaSerialClassEnv\": \"get sahara serial class environment\"\
                }";
    }

    bool CTSE_CloseDownloadPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaharaSerialClassEnv"))
        {
            mstrSaharaSerialClassEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CloseDownloadPort::TSE_Run()
    {
        CCJGW_QcommDownloadSerialPort* pSerialPort = (CCJGW_QcommDownloadSerialPort*)GetGlobalEnvironment()->GetInt(mstrSaharaSerialClassEnv);
        if (NULL != pSerialPort)
        {
            pSerialPort->CloseQcommDownloadSerialPort();
        }
        GetGlobalEnvironment()->PutInt(mstrSaharaSerialClassEnv,NULL);
        return true;
    }
}
