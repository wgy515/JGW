#include "StdAfx.h"
#include "TSE_CloseSerialComPort.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_CloseSerialComPort::CTSE_CloseSerialComPort(void)
    {
    }


    CTSE_CloseSerialComPort::~CTSE_CloseSerialComPort(void)
    {
    }

    const wchar_t* CTSE_CloseSerialComPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseSerialComPort\",\
                \"TestName\": \"关闭串行USB设备\"\
                }";
    }

    bool CTSE_CloseSerialComPort::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetSerialComPort().CloseSerialComPort();
        return true;
    }
}