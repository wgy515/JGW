#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "RKUpgradeDll.h"


namespace JGW
{
    class CTSE_ScanRKDevice : public CTSE_TestBase
    {
    public:
        CTSE_ScanRKDevice(void);
        ~CTSE_ScanRKDevice(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int mnScanDeviceTimeoutSec;
        //STRUCT_DEVICE_DESC_W m_deviceSet_W[MAX_DEVICE];
        std::wstring mstrLayerIDEnvironment;
        std::wstring mstrContextHandleEnvironment;
    };
}

