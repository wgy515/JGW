#include "StdAfx.h"
#include "TSE_DisConnectQocmDiag.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    bool CTSE_DisConnectQocmDiag::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->DisConnectDevice();
        return true;
    }

    const wchar_t* CTSE_DisConnectQocmDiag::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectQocmDiag\",\
                \"TestName\": \"断开高通DIAG连接(Qualcomm)\"\
                }";
    }
}
