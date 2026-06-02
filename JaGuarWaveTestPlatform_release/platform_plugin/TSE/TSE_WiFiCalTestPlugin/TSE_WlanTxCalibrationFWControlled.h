#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <vector>
namespace JGW
{
    class CTSE_WlanTxCalibrationFWControlled : public CTSE_TestBase
    {
    public:
        CTSE_WlanTxCalibrationFWControlled(void);
        ~CTSE_WlanTxCalibrationFWControlled(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! Enable all chains if AllChainOn = true, Disable all chains if AllChainOn = false
        bool mbAllChainOn;
        //! Collect debug info if DebugInfo = true, donot collect debug info if DebugInfo = false
        bool mbDebugInfo; 
        //! maximum calibration iteration
        std::vector<int> mviMaxIteration;
        //! Number of power averages
        int mnNumAverages;
        //!
        int mnVsaAverages;
        //! Single VSA port if true, Multiple VSA ports if false
        bool mbVsaOnePort;
    };
}
