#include "StdAfx.h"
#include "TSE_CloseModemPort.h"
#include "TSE_OpenModemPort.h"
namespace JGW
{
    CTSE_CloseModemPort::CTSE_CloseModemPort(void)
    {
    }


    CTSE_CloseModemPort::~CTSE_CloseModemPort(void)
    {
    }

    const wchar_t* CTSE_CloseModemPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseModemPort\",\
                \"TestName\": \"Close Modem Port\"\
                }";
    }

    bool CTSE_CloseModemPort::TSE_Run()
    {
        CCJGW_CSerialModemPort* pSerialModemPort = GetSerialModemPort();
        if (pSerialModemPort)
        {
            pSerialModemPort->CloseModemPort();
        }
        return true;
    }
}

