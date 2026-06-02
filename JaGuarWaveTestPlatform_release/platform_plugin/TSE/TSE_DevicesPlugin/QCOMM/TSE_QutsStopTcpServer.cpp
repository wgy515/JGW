#include "StdAfx.h"
#include "TSE_QutsStopTcpServer.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_QutsStopTcpServer::CTSE_QutsStopTcpServer(void) : mnQutsTcpPort(5000)
    {
    }


    CTSE_QutsStopTcpServer::~CTSE_QutsStopTcpServer(void)
    {
    }

    const wchar_t* CTSE_QutsStopTcpServer::TSE_GetParamDescription()
    {
        return L"";
    }
    
    bool CTSE_QutsStopTcpServer::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"QutsTcpPort"))
        {
            mnQutsTcpPort = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QutsStopTcpServer::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,QLIB_LIB_MODE_QUTS)->QUTS_Stop_TCP_Server(mnQutsTcpPort); 
        return true;
    }
}
