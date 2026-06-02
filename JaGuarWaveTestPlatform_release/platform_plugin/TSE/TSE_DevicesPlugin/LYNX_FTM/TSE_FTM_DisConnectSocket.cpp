#include "stdafx.h"
#include "TSE_FTM_DisConnectSocket.h"
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_FTM_DisConnectSocket::CTSE_FTM_DisConnectSocket()
    {

    }

    const wchar_t* CTSE_FTM_DisConnectSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_DisConnectSocket\",\
                \"TestName\": \"端口设备连接(LYNX FTM)\"\
                }";
    }

    bool CTSE_FTM_DisConnectSocket::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetFTMClientSocket()->CloseSocket();
        return true;
    }
}