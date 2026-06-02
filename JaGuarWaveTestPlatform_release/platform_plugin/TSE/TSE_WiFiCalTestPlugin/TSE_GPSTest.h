#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/JGW_QMSLFunc.h>
namespace JGW
{
    enum EGPS_COLLECT_MODE
    {
        EGPS_PRIMARY_RX = 1,
        EGPS_GLONASS_PRIMARY_RX = 3,
        EGPS_L5 = 4,
        EGPS_BDS = 17,
        EGPS_GAL = 18
    };

    class CTSE_GPSTest : public CTSE_TestBase
    {
    public:
        CTSE_GPSTest(void);
        ~CTSE_GPSTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit(); 
    private:
        bool GetGPSCNR(float fMin, float fMax,float& dCNR);
        bool GPS_NS_StartCall();
    private:
        //! 
        HANDLE mhResourceContext;
        //! 
        int mnRFPort;
        //! GPS-100;GLO-50;BDS-200; GSL-764;Specifies the frequency offset in kHz from the Center Frequency
        int mnSGFrequencyOffset;
        //! cell power
        float mfSignalPower;
        std::wstring mstrSignalPowerEnvironment;
        //! 1 GPS_Freq 1575.42,3 GLO_Freq 1602.00,4 GPS_L5_Freq1176.45,17 BDS_Freq1561.098,18 GAL_Freq1575.42
        double mdCenterFreqMhz;
        //! Specifies the source of I/Q samples.
        EGPS_COLLECT_MODE meCollectMode;
        //! Specifies the size of I/Q capture in kilosamples. It must be {1, 2, 4, 8, 16, 32} kilosamples. (1 kilosample = 1024 samples)
        int mnCaptureSize;
        //!
        int mnSingleTestErrorRetryCount;
        //! Specifies the number of captures (aka FFT integrations). It must be within 1 to 10.
        int mnNumberofCaptures;
        //! Frequency error: upper limit
        float mfFreqErrorUpperLimit; 
        //!
        std::wstring mstrGPSCNRStandardEnvironment;
        //!
        std::wstring mstrResourceContextEnvironment;
        //!
        CJGW_QMSLFunc mQMSLFunc;
    };
}


