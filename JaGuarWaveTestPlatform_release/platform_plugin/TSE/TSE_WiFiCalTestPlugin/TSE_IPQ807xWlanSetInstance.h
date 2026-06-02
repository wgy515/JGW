#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
namespace JGW
{
    class CTSE_IPQ807xWlanSetInstance : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanSetInstance(void);
        ~CTSE_IPQ807xWlanSetInstance(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    private:
        WLAN_INSTANCE meWlanInstance;
    };

}

