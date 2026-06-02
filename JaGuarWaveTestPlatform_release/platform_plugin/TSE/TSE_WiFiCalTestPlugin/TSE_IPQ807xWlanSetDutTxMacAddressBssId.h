#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_IPQ807xWlanSetDutTxMacAddressBssId : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanSetDutTxMacAddressBssId(void);
        ~CTSE_IPQ807xWlanSetDutTxMacAddressBssId(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        uint mnDeviceIndex;
        std::string mstrRxStation;
        std::string mstrTxStation;
        std::string mstrbssId; 
    };

}

