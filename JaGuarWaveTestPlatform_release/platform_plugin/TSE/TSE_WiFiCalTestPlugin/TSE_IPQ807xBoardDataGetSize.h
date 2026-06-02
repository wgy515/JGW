#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_IPQ807xBoardDataGetSize : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xBoardDataGetSize(void);
        ~CTSE_IPQ807xBoardDataGetSize(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    };
}


