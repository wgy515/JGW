#include "StdAfx.h"
#include "TSE_DisConnectSFTP.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_DisConnectSFTP::CTSE_DisConnectSFTP(void)
    {
    }


    CTSE_DisConnectSFTP::~CTSE_DisConnectSFTP(void)
    {
    }

    const wchar_t* CTSE_DisConnectSFTP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectSFTP\",\
                \"TestName\": \"¶Ï¿ªSFTPÁ¬½Ó(AP)\"\
                }";
    }

    bool CTSE_DisConnectSFTP::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetSSHSFTP()->DisConnectSSHSFTP();
        return true;
    }
}