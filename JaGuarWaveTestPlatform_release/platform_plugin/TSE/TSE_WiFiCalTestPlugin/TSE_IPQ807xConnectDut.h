#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_IPQ807xDefine.h"
#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
namespace JGW
{
    class CTSE_IPQ807xConnectDut : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xConnectDut(void);
        ~CTSE_IPQ807xConnectDut(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbUseQPST;
        int mnWlanID;
        HANDLE mhResourceContext;
        QLIB_TargetType_Enum meType;
        std::string mstrComPort;
        jgw_device_api mcDeviceApi;
    };
}

