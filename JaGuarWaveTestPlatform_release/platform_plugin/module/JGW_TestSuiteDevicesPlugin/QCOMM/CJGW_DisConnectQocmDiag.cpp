#include "StdAfx.h"
#include "CJGW_DisConnectQocmDiag.h"
#include "../CJGW_DevicesGlobalResource.h"
namespace JGW
{
    bool CCJGW_DisConnectQocmDiag::Run()
    {
        CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()->DisConnectDevice();
        return true;
    }
}
