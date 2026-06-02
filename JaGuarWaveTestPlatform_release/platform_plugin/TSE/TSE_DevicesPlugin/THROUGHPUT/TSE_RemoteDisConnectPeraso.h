#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_RemoteDisConnectPeraso : public CTSE_TestBase
    {
    public:
        CTSE_RemoteDisConnectPeraso(void);
        ~CTSE_RemoteDisConnectPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


