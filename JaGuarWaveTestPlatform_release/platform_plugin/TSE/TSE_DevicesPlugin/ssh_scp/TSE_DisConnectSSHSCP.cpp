#include "StdAfx.h"
#include "TSE_DisConnectSSHSCP.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_DisConnectSSHSCP::CTSE_DisConnectSSHSCP(void)
    {
    }


    CTSE_DisConnectSSHSCP::~CTSE_DisConnectSSHSCP(void)
    {
    }

    const wchar_t* CTSE_DisConnectSSHSCP::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_DisConnectSSHSCP\",\
				\"TestName\": \"DisConnect SCP(AP)\"\
				}";
    }

    bool CTSE_DisConnectSSHSCP::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetSSHSCP()->DisConnectSSHSCP();
        return true;
    }
}

