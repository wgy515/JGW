#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
namespace JGW
{
    class CTSE_WlanXtalCalibration : public CTSE_TestBase
    {
    public:
        CTSE_WlanXtalCalibration(void);
        ~CTSE_WlanXtalCalibration(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnChannel;
        WLAN_SINGLE_CHAIN_MASK meTxChainMask;
        int mGainIndex;
        int mDacGain;
        int mPaCfg;
        int mnNumAverages;
    };
}


