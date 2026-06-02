#include "StdAfx.h"
#include "CJGW_SyncQcomNV.h"
#include "../CJGW_DevicesGlobalResource.h"

namespace JGW
{
    bool CCJGW_SyncQcomNV::Run()
    {
        return CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()->SyncNv();
    }
}
