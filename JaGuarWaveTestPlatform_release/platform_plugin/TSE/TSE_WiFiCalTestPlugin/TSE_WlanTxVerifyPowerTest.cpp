#include "StdAfx.h"
#include "TSE_WlanTxVerifyPowerTest.h"
#include <algorithm>
#include "CJGW_EquipBase.h"

namespace JGW
{
    CTSE_WlanTxVerifyPowerTest::CTSE_WlanTxVerifyPowerTest(void) : m_channel(5180),m_powerLevel(16),m_wlanRate(RATE_11AC_HT20_MCS8),m_numOfMIMOStream(1),m_vsaTriggerLevel(-25),m_shortGuardInterval(true),m_numAverages(3),m_txChainMask(WLAN_CHAIN_1)
    {
        m_txPowerAvg[0] = 0;
        m_txPowerAvg[1] = 30;
        m_txPowerAvgDelta[0] = -4;
        m_txPowerAvgDelta[1] = 4;
        m_txPowerMax[0] = 0;
        m_txPowerMax[1] = 100;
        m_txPowerMin[0] = 0;
        m_txPowerMin[1] = 100;
        m_txPowerPeak[0] = 0;
        m_txPowerPeak[1] = 100;
        m_txPowerPeakMax[0] = 0;
        m_txPowerPeakMax[1] = 100;
        m_txPowerPeakMin[0] = 0;
        m_txPowerPeakMin[1] = 100;
        m_cableLoss[0] = 0;
        m_cableLoss[0] = 100;
    }


    CTSE_WlanTxVerifyPowerTest::~CTSE_WlanTxVerifyPowerTest(void)
    {
    }

    const wchar_t* CTSE_WlanTxVerifyPowerTest::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_WlanTxVerifyPowerTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"numOfMIMOStream",strParamName))
        {
            m_numOfMIMOStream = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"vsaTriggerLevel",strParamName))
        {
            m_vsaTriggerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"shortGuardInterval",strParamName))
        {
            m_shortGuardInterval = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"numAverages",strParamName))
        {
            m_numAverages = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txChainMask",strParamName))
        {
            m_txChainMask = (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerAvg",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerAvg[0] = vTemp[0];
                m_txPowerAvg[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerAvgDelta",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerAvgDelta[0] = vTemp[0];
                m_txPowerAvgDelta[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerMax",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerMax[0] = vTemp[0];
                m_txPowerMax[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerMin",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerMin[0] = vTemp[0];
                m_txPowerMin[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerPeak",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerPeak[0] = vTemp[0];
                m_txPowerPeak[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerPeakMax",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerPeakMax[0] = vTemp[0];
                m_txPowerPeakMax[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txPowerPeakMin",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_txPowerPeakMin[0] = vTemp[0];
                m_txPowerPeakMin[1] = vTemp[1];
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"cableLoss",strParamName))
        {
            std::vector<float> vTemp;
            JGW_ParserFloatW(strParamValue,L",",vTemp);
            if (2 == vTemp.size())
            {
                m_cableLoss[0] = vTemp[0];
                m_cableLoss[1] = vTemp[1];
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    double GetAdjQuarterRateFreq(WLAN_RATE wlanRate)
    {
        if (wlanRate >= RATE_11P_BW5_1p5_MBPS && wlanRate <= RATE_11P_BW5_13p5_MBPS)
        {
            return 0.5;
        }
        return 0.0;
    }

    void AverageTestResult(const std::vector<double>& resultArray, int averageTimes, AvgType logType, double averageResult,  double& maxResult,  double& minResult)
    {
        averageResult = 0.0;
        maxResult = (minResult = resultArray[0]);
        double num = 20.0;
        if (logType == Log10)
        {
            num = 10.0;
        }
        /*if (averageTimes == 0)
        {
        ///throw new WLANCustomException("AverageTestResult(): can not divided by zero");
        return false;
        }*/
        if (logType == Linear)
        {
            for (int i = 0; i < averageTimes; i++)
            {
                maxResult = ((maxResult > resultArray[i]) ? maxResult : resultArray[i]);
                minResult = ((minResult < resultArray[i]) ? minResult : resultArray[i]);
                averageResult += resultArray[i];
            }
            averageResult /= (double)averageTimes;
            return;
        }
        for (int i = 0; i < averageTimes; i++)
        {
            maxResult = ((maxResult > resultArray[i]) ? maxResult : resultArray[i]);
            minResult = ((minResult < resultArray[i]) ? minResult : resultArray[i]);
            averageResult += std::pow(10.0, resultArray[i] / num);
        }
        averageResult /= (double)averageTimes;
        if (0.0 != averageResult)
        {
            averageResult = num * std::log10(averageResult);
            return;
        }
        averageResult = 0.0;
    }

    bool CTSE_WlanTxVerifyPowerTest::TSE_Run()
    {
        double txPowerAvg, txPowerAvgDelta, txPowerMax, txPowerMin,txPowerPeak, txPowerPeakMax, txPowerPeakMin, cableLoss;
        txPowerAvg = (txPowerAvgDelta = (txPowerMax = (txPowerMin = (txPowerPeak = (txPowerPeakMax = (txPowerPeakMin = (cableLoss = 999.99)))))));
        double freq = m_channel + GetAdjQuarterRateFreq(m_wlanRate);

        std::vector<double> vLoss;
        cableLoss = 1.0;
        vLoss.push_back(cableLoss);

        std::vector<double> vVsaTriggerLevel;
        vVsaTriggerLevel.push_back(m_vsaTriggerLevel);

        std::vector<double> vPowerAvEachBurst,vPowerPkEachBurst,array,array2;
        vPowerAvEachBurst.push_back(-999.0);
        vPowerPkEachBurst.push_back(-999.0);
        int i = 0;
        for (i = 0;i < m_numAverages;i ++)
        {
            EQUIP.MeasurePower(m_wlanRate,freq,m_powerLevel,(ChainMask)m_txChainMask,Edge,vLoss,vVsaTriggerLevel,m_shortGuardInterval,vPowerAvEachBurst,vPowerPkEachBurst);
            array.push_back(vPowerAvEachBurst[0]);
            array2.push_back(vPowerPkEachBurst[0]);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Average Power:%0.2f dBm\t Peak Power:%.2f dBm", vPowerAvEachBurst[0], vPowerPkEachBurst[0]);
        }
        AverageTestResult(array, i, Log10,  txPowerAvg,  txPowerMax,  txPowerMin);
        AverageTestResult(array2, i, Log10,  txPowerPeak,  txPowerPeakMax, txPowerPeakMin);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Power Avg:%.2f\tMin:%.2f\tmax%.2f",txPowerAvg,txPowerMin,  txPowerMax); 
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Power Peak:%.2f\tMin:%.2f\tmax%.2f",txPowerPeak,txPowerPeakMin,  txPowerPeakMax); 
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Power Avg Delta:%.2f",txPowerAvg - m_powerLevel);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Path loss (from cable):%.2f",cableLoss);

        if (m_txPowerAvg[0] > txPowerAvg || m_txPowerAvg[1] < txPowerAvg
            || m_txPowerMin[0] > txPowerMin || m_txPowerMin[1] < txPowerMin
            || m_txPowerMax[0] > txPowerMax || m_txPowerMax[1] < txPowerMax)
        {
            LogE_F(L"Fail => Tx Power Avg:%.2f\tMin:%.2f\tmax%.2f",txPowerAvg,txPowerMin,  txPowerMax); 
            return false;
        }
        if (m_txPowerPeak[0] > txPowerPeak || m_txPowerPeak[1] < txPowerPeak
            || m_txPowerPeakMin[0] > txPowerPeakMin || m_txPowerPeakMin[1] < txPowerPeakMin
            || m_txPowerPeakMax[0] > txPowerPeakMax || m_txPowerPeakMax[1] < txPowerPeakMax)
        {
            LogE_F(L"Fail => Tx Power Peak:%.2f\tMin:%.2f\tmax%.2f",txPowerPeak,txPowerPeakMin,  txPowerPeakMax); 
            return false;
        }
        if (m_txPowerAvgDelta[0] > txPowerAvg - m_powerLevel || m_txPowerAvgDelta[1] < txPowerAvg - m_powerLevel)
        {
            LogE_F(L"Fail => Tx Power Avg Delta:%.2f",txPowerAvg - m_powerLevel);
            return false;
        }
        if (m_cableLoss[0] > cableLoss || m_cableLoss[1] < cableLoss)
        {
            LogE_F(L"Fail => Path loss (from cable):%.2f",cableLoss);
            return false;
        }
        return true;
    }
}

