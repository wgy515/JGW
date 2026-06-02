#include "StdAfx.h"
#include "TSE_DisConnectDMGPeraso.h"
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>

namespace JGW
{
    CTSE_DisConnectDMGPeraso::CTSE_DisConnectDMGPeraso(void)
    {
    }


    CTSE_DisConnectDMGPeraso::~CTSE_DisConnectDMGPeraso(void)
    {
    }

    const wchar_t* CTSE_DisConnectDMGPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectDMGPeraso\",\
                \"TestName\": \"DisConnect DMG Peraso\",\
                \"GuSerialComPortEnv\": \"Golden SerailComPort\",\
                \"DutSerialComPortEnv\": \"Dut SerailComPort\"\
                }";
    }

    bool CTSE_DisConnectDMGPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"GuSerialComPortEnv"))
        {
            mstrGuSerialComPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DutSerialComPortEnv"))
        {
            mstrDutSerialComPortEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DisConnectDMGPeraso::TSE_Run()
    {
        CCJGW_CSerialComPort* guSerialComPort = NULL,*dutSerialComPort = NULL;

        guSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrGuSerialComPortEnv);
        if (NULL != guSerialComPort)
        {
            guSerialComPort->CloseSerialComPort();
        }

        dutSerialComPort = (CCJGW_CSerialComPort*)GetGlobalEnvironment()->GetInt(mstrDutSerialComPortEnv);
        if (NULL != dutSerialComPort)
        {
            dutSerialComPort->CloseSerialComPort();
        }
        return true;
    }
}

