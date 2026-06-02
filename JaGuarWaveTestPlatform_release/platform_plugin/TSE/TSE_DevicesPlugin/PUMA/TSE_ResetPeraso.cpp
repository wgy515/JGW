#include "StdAfx.h"
#include "TSE_ResetPeraso.h"
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ResetPeraso::CTSE_ResetPeraso(void)
    {
    }


    CTSE_ResetPeraso::~CTSE_ResetPeraso(void)
    {
    }

    const wchar_t* CTSE_ResetPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ResetPeraso\",\
                \"TestName\": \"ÖØÆôPeraso(Peraso)\"\
                }";
    }

    bool CTSE_ResetPeraso::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ResetDongleDevice();
        return true;
    }
}