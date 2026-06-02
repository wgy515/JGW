#include "StdAfx.h"
#include "TSE_QutsStartTcpServer.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_QutsStartTcpServer::CTSE_QutsStartTcpServer(void) : mnQutsType(0),mnQutsTcpPort(5000)
    {
    }


    CTSE_QutsStartTcpServer::~CTSE_QutsStartTcpServer(void)
    {
    }

    const wchar_t* CTSE_QutsStartTcpServer::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_QutsStartTcpServer::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"QutsType"))
        {
            mnQutsType = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"QutsTcpPort"))
        {
            mnQutsTcpPort = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QutsStartTcpServer::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,QLIB_LIB_MODE_QUTS)->QUTS_Start_TCP_Server(mnQutsType,mnQutsTcpPort); 
        return true;
    }
}
