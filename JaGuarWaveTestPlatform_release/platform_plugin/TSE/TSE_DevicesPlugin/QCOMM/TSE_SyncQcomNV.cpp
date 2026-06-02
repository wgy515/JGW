#include "StdAfx.h"
#include "TSE_SyncQcomNV.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    bool CTSE_SyncQcomNV::TSE_Run()
    {
        return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->SyncNv();
    }

    const wchar_t* CTSE_SyncQcomNV::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SyncQcomNV\",\
                \"TestName\": \"Í¬²½NV(Qualcomm)\"\
                }";
    }
}
