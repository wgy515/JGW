#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_DisConnectQocmDiag:public CTSE_TestBase
    {
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };

}

