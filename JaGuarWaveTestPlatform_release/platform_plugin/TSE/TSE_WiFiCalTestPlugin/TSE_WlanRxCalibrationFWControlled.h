#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <vector>
namespace JGW
{
    class CTSE_WlanRxCalibrationFWControlled : public CTSE_TestBase
    {
    public:
        CTSE_WlanRxCalibrationFWControlled(void);
        ~CTSE_WlanRxCalibrationFWControlled(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnRadioId;
        std::vector<int> mvChainToCalibrate;
    };
}


