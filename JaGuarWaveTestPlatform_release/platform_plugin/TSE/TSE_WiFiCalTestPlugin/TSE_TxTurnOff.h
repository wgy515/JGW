#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_TxTurnOff : public CTSE_TestBase
    {
    public:
        CTSE_TxTurnOff(void);
        ~CTSE_TxTurnOff(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t m_channel;
        WLAN_CHAIN_MASK2 m_chain;
    };
}


