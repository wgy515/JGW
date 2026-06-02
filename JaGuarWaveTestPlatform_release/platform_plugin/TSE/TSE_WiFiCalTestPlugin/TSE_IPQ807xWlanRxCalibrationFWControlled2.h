#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_IPQ807xDefine.h"
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>

namespace JGW
{
    class CTSE_IPQ807xWlanRxCalibrationFWControlled2 : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanRxCalibrationFWControlled2(void);
        ~CTSE_IPQ807xWlanRxCalibrationFWControlled2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        WLAN_UniversalPhyID mePhyId;
        BAND meBand;
        WLAN_Action mePromiscuousMode;
        std::string mstrRxMacAddress;
        std::string mstrBssId;
        std::vector<size_t> mvChainToCalibrate;
    };

}

