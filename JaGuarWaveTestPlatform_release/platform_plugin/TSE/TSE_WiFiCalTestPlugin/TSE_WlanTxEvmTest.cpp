#include "StdAfx.h"
#include "TSE_WlanTxEvmTest.h"

namespace JGW
{
    CTSE_WlanTxEvmTest::CTSE_WlanTxEvmTest(void) : m_channel(5180),m_powerLevel(16),m_wlanRate(RATE_11AC_HT20_MCS8),m_chEstimationMethod(preambleOnly),m_numAverages(5),m_numOfMIMOStream(1),m_vsaTriggerLevel(0),m_shortGuardInterval(true),m_txChainMask(WLAN_CHAIN_1)
    {
        m_evm[0] = -99;
        m_evm[1] = -31;
         m_amplErr[0] = -99;
         m_amplErr[1] = 999;
         m_dataRateMB[0] = -99;
         m_dataRateMB[1] = 999;
         m_symbolClockError[0] = -50;
         m_symbolClockError[1] = 50;
         m_freqErr[0] = -99;
         m_freqErr[1] = 999;
         m_phaseErr[0] = -99;
         m_phaseErr[1] = 999;
         m_avgTxPower[0] = 0;
         m_avgTxPower[1] = 30;
         m_avgTxPowerDelta[0] = -4;
         m_avgTxPowerDelta[1] = 4;
         m_loLeakage[0] = -120;
         m_loLeakage[1] = -10;
         m_minEvm[0] = -99;
         m_minEvm[1] = 99;
         m_maxEvm[0] = -99;
         m_maxEvm[1] = 99;
    }


    CTSE_WlanTxEvmTest::~CTSE_WlanTxEvmTest(void)
    {
    }

    const wchar_t* CTSE_WlanTxEvmTest::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_WlanTxEvmTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (0 == wcslen(strParamValue))
        {
            return true;
        }
        if (TSE_PARAM_NAME_EQUAL(L"channel",strParamName))
        {
            m_channel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"powerLevel",strParamName))
        {
            m_powerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"wlanRate",strParamName))
        {
            m_wlanRate = (WLAN_RATE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"chEstimationMethod",strParamName))
        {
            m_chEstimationMethod = (ChannelEstimationOption)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"numOfMIMOStream",strParamName))
        {
            m_numOfMIMOStream = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"numAverages",strParamName))
        {
            m_numAverages = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"vsaTriggerLevel",strParamName))
        {
            m_vsaTriggerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"shortGuardInterval",strParamName))
        {
            m_shortGuardInterval = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txChainMask",strParamName))
        {
            m_txChainMask = (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"evm",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_evm[0] = vTemp[0];
                m_evm[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"amplErr",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_amplErr[0] = vTemp[0];
                m_amplErr[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"dataRateMB",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_dataRateMB[0] = vTemp[0];
                m_dataRateMB[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"symbolClockError",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_symbolClockError[0] = vTemp[0];
                m_symbolClockError[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"freqErr",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_freqErr[0] = vTemp[0];
                m_freqErr[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"phaseErr",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_phaseErr[0] = vTemp[0];
                m_phaseErr[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"avgTxPower",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_avgTxPower[0] = vTemp[0];
                m_avgTxPower[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"avgTxPowerDelta",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_avgTxPowerDelta[0] = vTemp[0];
                m_avgTxPowerDelta[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"loLeakage",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_loLeakage[0] = vTemp[0];
                m_loLeakage[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"minEvm",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_minEvm[0] = vTemp[0];
                m_minEvm[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"maxEvm",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_maxEvm[0] = vTemp[0];
                m_maxEvm[1] = vTemp[1];
            }
        }
        else
        {
            return false;
        }

        return true;
    }

    /*double GetAdjQuarterRateFreq(WLAN_RATE wlanRate)
    {
        if (wlanRate >= RATE_11P_BW5_1p5_MBPS && wlanRate <= RATE_11P_BW5_13p5_MBPS)
        {
            return 0.5;
        }
        return 0.0;
    }*/

    bool CTSE_WlanTxEvmTest::TSE_Run()
    {
        double evm, amplErr,  dataRateMB,  symbolClockError,  freqErr,  phaseErr,  loLeakage,  minEvm,  maxEvm;
        double avgTxPower = 999.99,avgTxPowerDelta = 999.99;
        evm = (amplErr = (phaseErr = (freqErr = (symbolClockError = (dataRateMB = (loLeakage = (minEvm = (maxEvm = 999.99))))))));
        ///double freq = m_channel + GetAdjQuarterRateFreq(m_wlanRate);




        return false;
    }
}

