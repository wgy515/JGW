#include "StdAfx.h"
#include "TSE_GPSTest.h"
#include "CJGW_EquipBase.h"
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>
#include <algorithm>
namespace JGW
{
    CTSE_GPSTest::CTSE_GPSTest(void) : mnSGFrequencyOffset(-100),mfSignalPower(-130),mdCenterFreqMhz(1575.42),meCollectMode(EGPS_PRIMARY_RX),mnCaptureSize(32),mnNumberofCaptures(10),mnRFPort(ERFAC),mhResourceContext(NULL),mQMSLFunc(mhResourceContext),mfFreqErrorUpperLimit(1000),mnSingleTestErrorRetryCount(10)
    {
    }


    CTSE_GPSTest::~CTSE_GPSTest(void)
    {
    }

    const wchar_t* CTSE_GPSTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GPSTest\",\
                \"TestName\": \"GPS Test\",\
                \"SignalPower\": \"Cell Power\",\
                \"SignalPowerEnvironment\": \"Cell Power Environment\",\
                \"RFPort\": \"ERFAC 1,ERFAO 2,ERFBC 3  def 1\",\
                \"SGFrequencyOffset\": \"GPS-100;GLO-50;BDS-200; GSL-764;Specifies the frequency offset in kHz from the Center Frequency .\",\
                \"CenterFreqMhz\": \"1 GPS_Freq 1575.42,3 GLO_Freq 1602.00,4 GPS_L5_Freq1176.45,17 BDS_Freq1561.098,18 GAL_Freq1575.42\",\
                \"CollectMode\": \"GPS Primary Rx=1;GLONASS Primary Rx=3;GPS_L5=4;BDS=17;GAL=18 Specifies the source of I/Q samples.\",\
                \"CaptureSize\": \"Specifies the size of I/Q capture in kilosamples. It must be {1, 2, 4, 8, 16, 32} kilosamples. (1 kilosample = 1024 samples).\",\
                \"NumberofCaptures\": \"Specifies the number of captures (aka FFT integrations). It must be within 1 to 10.\",\
                \"SingleTestErrorRetryCount\": \"Single Test Error Retry Count\",\
                \"GPSCNRStandardEnvironment\": \"GPS CNR Standard Environment\",\
                \"FreqErrorUpperLimit\": \"Specifies the upper limit on the frequency error (Hz). This limit is applied on abs(CW_freq_offset_Hz - mobile_reported_freq_Hz) def 1000\",\
                \"ResourceContextEnvironment\": \"QCOMM Diag Resource Context Envrionment\"\
                }";
    }

    bool CTSE_GPSTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SignalPower",strParamName))
        {
            mfSignalPower = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SignalPowerEnvironment",strParamName))
        {
            mstrSignalPowerEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RFPort",strParamName))
        {
            mnRFPort = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SGFrequencyOffset",strParamName))
        {
            mnSGFrequencyOffset = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CollectMode",strParamName))
        {
            meCollectMode = (EGPS_COLLECT_MODE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CaptureSize",strParamName))
        {
            mnCaptureSize = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumberofCaptures",strParamName))
        {
            mnNumberofCaptures = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SingleTestErrorRetryCount",strParamName))
        {
            mnSingleTestErrorRetryCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"GPSCNRStandardEnvironment",strParamName))
        {
            mstrGPSCNRStandardEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ResourceContextEnvironment",strParamName))
        {
            mstrResourceContextEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GPSTest::TSE_Run()
    {
        float fSignalPower = mfSignalPower;
        if (!mstrSignalPowerEnvironment.empty())
        {
            fSignalPower = GetGlobalEnvironment()->GetFloat(mstrSignalPowerEnvironment);
        }
        //! 
        if (!GETINSTRUMENT->CMW_GPS_InitBSE(mnRFPort,fSignalPower,mdCenterFreqMhz + mnSGFrequencyOffset / 1000.00))
        {
            LogE(L"Init GPS BSE Fail");
            return false;
        }

        mhResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnvironment);
        if (NULL == mhResourceContext)
        {
            LogE(L"Qualcomm Diag Connect Fail");
            return false;
        }

        float fMin = 0, fMax = 0,fValue = 0;
        std::wstring strTestRange = GetGlobalEnvironment()->GetString(mstrGPSCNRStandardEnvironment);
        if (std::wstring::npos == strTestRange.find(L"~"))
        {
            LogE(L"Test Range Config Fail");
            return false;
        }
        _stscanf_s(strTestRange.c_str(),L"%f~%f",&fMin,&fMax);

        if (!GetGPSCNR(fMin,fMax,fValue))
        {
            LogE(L"Get GPS CNR Fail");
            return false;
        }
        return true;
    }

    bool CTSE_GPSTest::GPS_NS_StartCall()
    {
        bool bOk = true;
        long EngineRevision = 0;
        unsigned char piStatus = 0;

        bOk &= mQMSLFunc.GPS_GEN8_Get_GNSS_Engine_Revision(EngineRevision);
        if (!bOk)
        {
            LogE(L"QLIB_GPS_GEN8_Get_GNSS_Engine_Revision Call Fail");
            return false;
        }

        unsigned char  Requested_Constellation_Mask = 0;//GPS
        unsigned char  Supportted_Contellation_Mask = 0;
        unsigned char  Actual_Constellsation_Mask = 0;

        bOk &= mQMSLFunc.GPS_GEN8_GNSS_NAV_SET_CONFIG(Requested_Constellation_Mask,Supportted_Contellation_Mask,Actual_Constellsation_Mask);
        if (!bOk)
        {
            LogE(L"QLIB_GPS_GEN8_GNSS_NAV_SET_CONFIG Call Fail");
            return false;
        }

        for (int nIndex = 0;nIndex < 50;nIndex ++)
        {
            bOk &= mQMSLFunc.DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(GPS_SA_RF_VERIF_MODE_SWITCH_ENTER_MODE,piStatus);
            if (bOk && !piStatus)
            {
                return true;
            }
            Sleep(100);
        }
        LogE(L"DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait Call Fail");
        return false;
    }

    bool CTSE_GPSTest::GetGPSCNR(float fMin, float fMax,float& dCNR)
    {
        if (!GPS_NS_StartCall())
        {
            return false;
        }

        double pdCNoDBHz = 0;
        int piFreqInHz = 0,freqInHz = 0;
        unsigned char iCollectMode = meCollectMode;//gps Prx
        unsigned short iCaptureSize_kSamples = mnCaptureSize;
        unsigned char  iNumberOfIntegrations = mnNumberofCaptures;

        for (int nIndex = 0;nIndex < mnSingleTestErrorRetryCount;nIndex ++)
        {
            if( mQMSLFunc.DIAG_GEN8_GET_EMBEDDED_CALCULATED_CTON(iCollectMode,iCaptureSize_kSamples,iNumberOfIntegrations,pdCNoDBHz,piFreqInHz))
            {
                dCNR = static_cast<float>(pdCNoDBHz);
                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"GPS CNR:%.3f(%.3f-%.3f)",dCNR,fMin,fMax);
                freqInHz = std::abs(mnSGFrequencyOffset * 1000 - piFreqInHz);
                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"FREQ ERROR:%d(0-%.3f)",freqInHz,mfFreqErrorUpperLimit);
                if (dCNR >= fMin && dCNR <= fMax && freqInHz < mfFreqErrorUpperLimit)
                {
                    return true;
                }
            }

            Sleep(1000);
        }

        return false;
    }

    bool CTSE_GPSTest::TSE_Exit()
    {
        unsigned char piStatus = false;
        for (int nIndex = 0;nIndex < 50;nIndex ++)
        {
            if (mQMSLFunc.DIAG_GPS_SA_RF_VERIF_MODE_SWITCH_WithWait(GPS_SA_RF_VERIF_MODE_SWITCH_EXIT_MODE,piStatus) && !piStatus)
            {
                break;
            }
            Sleep(100);
        } 
        GETINSTRUMENT->CMW_GPS_FinalizeBSE();
        return true;
    }
}

