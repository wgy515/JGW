#include "StdAfx.h"
#include "TSE_WlanXtalCalibration.h"

namespace JGW
{
    CTSE_WlanXtalCalibration::CTSE_WlanXtalCalibration(void) : mnChannel(5180),meTxChainMask(WLAN_SINGLE_CHAIN_1),mGainIndex(10),mDacGain(0),mPaCfg(0),mnNumAverages(3)
    {
    }


    CTSE_WlanXtalCalibration::~CTSE_WlanXtalCalibration(void)
    {
    }

    const wchar_t* CTSE_WlanXtalCalibration::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanXtalCalibration\",\
                \"TestName\": \"Wlan Xtal Calibration\",\
                \"Channel\": \"Center freqMHz or channel number of tx output\",\
                \"TxChainMask\": \"Tx ChainMask to turn on WLAN_CHAIN_1=1;WLAN_CHAIN_2=2;WLAN_CHAIN_3=4;WLAN_CHAIN_4=8\",\
                \"GainIndex\": \"gain index\",\
                \"DacGain\": \"Digital gain\",\
                \"PaCfg\": \"PA configuration\",\
                \"NumAverages\": \"Number of power averages\"\
                }";
    }

    bool CTSE_WlanXtalCalibration::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Channel",strParamName))
        {
            mnChannel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxChainMask",strParamName))
        {
            meTxChainMask = (WLAN_SINGLE_CHAIN_MASK)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"GainIndex",strParamName))
        {
            mGainIndex = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"DacGain",strParamName))
        {
            mDacGain = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PaCfg",strParamName))
        {
            mPaCfg = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumAverages",strParamName))
        {
            mnNumAverages = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanXtalCalibration::TSE_Run()
    {
        size_t xtalCap = 0;
        double freqErrPPM = -999.0;

        PHONE->XtalCalibration(mnChannel,meTxChainMask,mGainIndex,mDacGain,mPaCfg,mnNumAverages,xtalCap,freqErrPPM);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"WlanXtalCalibration XtalCap: %u,FreqErrPPM: %.2f(Min:%d,Max:%d)",xtalCap,freqErrPPM,((long)PHONE->mXtalTargetPPM - (long)((unsigned long)PHONE->mXtalTolerancePPM)),((long)PHONE->mXtalTargetPPM + (long)((unsigned long)PHONE->mXtalTolerancePPM)));
        return freqErrPPM >= (double)((long)PHONE->mXtalTargetPPM - (long)((unsigned long)PHONE->mXtalTolerancePPM)) && freqErrPPM <= (double)((long)PHONE->mXtalTargetPPM + (long)((unsigned long)PHONE->mXtalTolerancePPM));
    }
}

