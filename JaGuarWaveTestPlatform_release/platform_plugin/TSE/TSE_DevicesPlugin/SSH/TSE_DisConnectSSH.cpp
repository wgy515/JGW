#include "StdAfx.h"
#include "TSE_DisConnectSSH.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_DisConnectSSH::CTSE_DisConnectSSH(void)
    {
    }


    CTSE_DisConnectSSH::~CTSE_DisConnectSSH(void)
    {
    }

    const wchar_t* CTSE_DisConnectSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectSSH\",\
                \"TestName\": \"¶Ï¿ªSSHÁ¬½Ó(AP)\"\
                }";
    }

    bool CTSE_DisConnectSSH::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetSSHShell()->DisConnectSSHShell();
        return true;
    }
}