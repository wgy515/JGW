#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CloseSerialComPort : public CTSE_TestBase
    {
    public:
        CTSE_CloseSerialComPort(void);
        ~CTSE_CloseSerialComPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}

