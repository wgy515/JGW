#include "StdAfx.h"
#include "TSE_DisConnectPeraso.h"
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_DisConnectPeraso::CTSE_DisConnectPeraso(void)
    {
    }


    CTSE_DisConnectPeraso::~CTSE_DisConnectPeraso(void)
    {
    }

    const wchar_t* CTSE_DisConnectPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectPeraso\",\
                \"TestName\": \"断开PERASO设备连接\"\
                }";
    }

    bool CTSE_DisConnectPeraso::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->CloseDonglesComPort();
        return true;
    }
}