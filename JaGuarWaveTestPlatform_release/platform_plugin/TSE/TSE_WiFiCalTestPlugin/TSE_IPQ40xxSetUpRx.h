#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "JGW_Phone.h"

namespace JGW
{
    class CTSE_IPQ40xxSetUpRx : public CTSE_TestBase
    {
    public:
        CTSE_IPQ40xxSetUpRx(void);
        ~CTSE_IPQ40xxSetUpRx(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        WLAN_CHAIN_MASK2 meRxChainMask;
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        WLAN_RATE meRate;
        WLAN_Action mePromiscuousMode;
        size_t mnChannel;
        size_t mnChannel2;
        std::string mstrRxMacAddress;
        S_RxCalTestParam msRxCalTestParam;
    };
}


