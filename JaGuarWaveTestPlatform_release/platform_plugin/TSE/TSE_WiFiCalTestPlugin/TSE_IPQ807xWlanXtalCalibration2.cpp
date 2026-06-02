#include "StdAfx.h"
#include "TSE_IPQ807xWlanXtalCalibration2.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanXtalCalibration2::CTSE_IPQ807xWlanXtalCalibration2(void)
    {
    }


    CTSE_IPQ807xWlanXtalCalibration2::~CTSE_IPQ807xWlanXtalCalibration2(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanXtalCalibration2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanXtalCalibration2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyId = (PhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("Channel"),strParamName))
        {
            mnChannel = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("TxChainMask"),strParamName))
        {
            meTxChainMask = (WLAN_SINGLE_CHAIN_MASK)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("GainIndex"),strParamName))
        {
            mnGainIndex = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("DacGain"),strParamName))
        {
            mnDacGain = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("PaCfg"),strParamName))
        {
            mnPaCfg = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("NumAverages"),strParamName))
        {
            mnNumAverages = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanXtalCalibration2::TSE_Run()
    {
        size_t xtalCapIn = 0,xtalCapOut = 0;
        double freqErrPPM = -999.0;;
        bool status = false;
        bool result = CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanXtalCalibration2(mePhyId,mnChannel,meTxChainMask,mnGainIndex,mnDacGain,mnPaCfg,mnNumAverages,xtalCapIn,xtalCapOut,freqErrPPM,status);
        //! FreqErr:-8.28   Power: -9.14   ChannelFreq: 5500    txchainMask: 1    capOutValue:176    capInValue:560
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"WlanXtalCalibration2 xtalCapIn: %u,xtalCapOut: %u,FreqErrPPM: %.2f",xtalCapIn,xtalCapOut,freqErrPPM);
        return result;

        //PHONE->XtalCalibration(mnChannel,meTxChainMask,mGainIndex,mDacGain,mPaCfg,mnNumAverages,xtalCap,freqErrPPM);
        //LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"WlanXtalCalibration2 XtalCap: %u,FreqErrPPM: %.2f(Min:%d,Max:%d)",xtalCap,freqErrPPM,((long)PHONE->mXtalTargetPPM - (long)((unsigned long)PHONE->mXtalTolerancePPM)),((long)PHONE->mXtalTargetPPM + (long)((unsigned long)PHONE->mXtalTolerancePPM)));
        //return freqErrPPM >= (double)((long)PHONE->mXtalTargetPPM - (long)((unsigned long)PHONE->mXtalTolerancePPM)) && freqErrPPM <= (double)((long)PHONE->mXtalTargetPPM + (long)((unsigned long)PHONE->mXtalTolerancePPM));
    }
}
