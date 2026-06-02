#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_IPQ807xDefine.h"
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
namespace JGW
{
    class CTSE_IPQ807xWlanTxCalibrationFWControlled2 : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanTxCalibrationFWControlled2(void);
        ~CTSE_IPQ807xWlanTxCalibrationFWControlled2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbVsaOnePort;
        WLAN_UniversalPhyID mePhyId;
        WLAN_TpcCalScheme meCalScheme;
        WLAN_TpcBand meBand;
        size_t mnTotalNumOfChains;
        size_t mnMultiChainMask;
        size_t mnNumAverages;
        
    };
}


