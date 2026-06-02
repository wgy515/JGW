#include "StdAfx.h"
#include "CJGW_EquipBase.h"
#include "JGW_WiFiLossConfig.h"

namespace JGW
{
    CCJGW_EquipBase::CCJGW_EquipBase(void) : m_extraUserMargin (4.0)
    {
    }


    CCJGW_EquipBase::~CCJGW_EquipBase(void)
    {
    }

    CCJGW_EquipBase& CCJGW_EquipBase::GetEquip()
    {
        static CCJGW_EquipBase equip;
        return equip;
    }

    double CCJGW_EquipBase::GetCWFreqErr(double chFreqMHz, double powerLevelDBm)
    {
        //! freq loss port expect_power power_range rate
        return 0;
    }

    double CCJGW_EquipBase::GetPowerDelegate(WLAN_DataRate rate, bool short11b_nGuard, double chFreqMHz, double powerLevelDBm)
    {
        WLAN_MODE wifiMode;
        double peakToAvg = INVAILD_WLAN_RESULT;
        int vsaPort = ERFAC;
        int vsgPort = ERFAC;
        float fLoss = 1.00;
        WLAN_RATE wlanRate;
        WLAN_Gen6_PHYDBG_PreambleRate wlan_Gen6_PHYDBG_PreambleRate;

        LOSS.GetLossVsaVsgPortToFreq((WLAN_CHAIN_MASK2)PHONE.m_txChainMask,static_cast<int>(chFreqMHz),fLoss,vsaPort,vsgPort);
        mapWlanRate(rate, short11b_nGuard,wlanRate, wlan_Gen6_PHYDBG_PreambleRate, 0u);

        GetWifiMode(wlanRate,wifiMode,peakToAvg);
        return mEquip.GetPower(vsaPort,chFreqMHz,wifiMode,fLoss,powerLevelDBm,peakToAvg);
    }

    double CCJGW_EquipBase::GetCWPowerDelegate(double chFreqMHz, double powerLevelDBm)
    {
        int vsaPort = ERFAC;
        int vsgPort = ERFAC;
        float fLoss = 1.00;
        LOSS.GetLossVsaVsgPortToFreq((WLAN_CHAIN_MASK2)PHONE.m_txChainMask,static_cast<int>(chFreqMHz),fLoss,vsaPort,vsgPort);

        WLAN_MODE wifiMode;
        double peakToAvg = INVAILD_WLAN_RESULT;
        WLAN_RATE wlanRate = RATE_11A_36_MBPS;
        GetWifiMode(wlanRate,wifiMode,peakToAvg);

        return mEquip.GetPower(vsaPort,chFreqMHz,wifiMode,fLoss,powerLevelDBm,peakToAvg);
    }

    void CCJGW_EquipBase::MeasurePower(WLAN_RATE rate, double channelFreqMhz, double powerLevelDBm, ChainMask chainMask, TriggerType triggerType,const std::vector<double>& vPathLossDB, const std::vector<double>& vVsaTriggerLevelOffsetDbr , bool shortGuardInterval, std::vector<double>& vPowerAvEachBurst, std::vector<double>& vPowerPkEachBurst)
    {

    }

    bool CCJGW_EquipBase::GetWifiMode(WLAN_RATE rate, WLAN_MODE& wifiMode, double& peakToAvg)
    {
        peakToAvg = 10.0 + m_extraUserMargin;
        switch (rate)
        {
        case RATE_11B_LONG_1_MBPS:
        case RATE_11B_LONG_2_MBPS:
        case RATE_11B_LONG_5_5_MBPS:
        case RATE_11B_LONG_11_MBPS:
            wifiMode = WIFI_11B;
            peakToAvg = 2.0 + m_extraUserMargin;
            break;
        case RATE_11B_SHORT_2_MBPS:
        case RATE_11B_SHORT_5_5_MBPS:
        case RATE_11B_SHORT_11_MBPS:
            wifiMode = WIFI_11B;
            peakToAvg = 2.0 + m_extraUserMargin;
            break;
        case RATE_11A_6_MBPS:
        case RATE_11A_9_MBPS:
        case RATE_11A_12_MBPS:
        case RATE_11A_18_MBPS:
        case RATE_11A_24_MBPS:
        case RATE_11A_36_MBPS:
        case RATE_11A_48_MBPS:
        case RATE_11A_54_MBPS:
            wifiMode = WIFI_11AG;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11N_HT20_MCS0:
        case RATE_11N_HT20_MCS1:
        case RATE_11N_HT20_MCS2:
        case RATE_11N_HT20_MCS3:
        case RATE_11N_HT20_MCS4:
        case RATE_11N_HT20_MCS5:
        case RATE_11N_HT20_MCS6:
        case RATE_11N_HT20_MCS7:
            wifiMode = WIFI_11N_HT20;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11N_HT40_MCS0:
        case RATE_11N_HT40_MCS1:
        case RATE_11N_HT40_MCS2:
        case RATE_11N_HT40_MCS3:
        case RATE_11N_HT40_MCS4:
        case RATE_11N_HT40_MCS5:
        case RATE_11N_HT40_MCS6:
        case RATE_11N_HT40_MCS7:
            wifiMode = WIFI_11N_HT40Minus;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11AC_HT20_MCS0:
        case RATE_11AC_HT20_MCS1:
        case RATE_11AC_HT20_MCS2:
        case RATE_11AC_HT20_MCS3:
        case RATE_11AC_HT20_MCS4:
        case RATE_11AC_HT20_MCS5:
        case RATE_11AC_HT20_MCS6:
        case RATE_11AC_HT20_MCS7:
        case RATE_11AC_HT20_MCS8:
        case RATE_11AC_HT20_MCS9:
            wifiMode = WIFI_11AC_HT20;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11AC_HT40_MCS0:
        case RATE_11AC_HT40_MCS1:
        case RATE_11AC_HT40_MCS2:
        case RATE_11AC_HT40_MCS3:
        case RATE_11AC_HT40_MCS4:
        case RATE_11AC_HT40_MCS5:
        case RATE_11AC_HT40_MCS6:
        case RATE_11AC_HT40_MCS7:
        case RATE_11AC_HT40_MCS8:
        case RATE_11AC_HT40_MCS9:
            wifiMode = WIFI_11AC_HT40Minus;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11AC_HT80_MCS0:
        case RATE_11AC_HT80_MCS1:
        case RATE_11AC_HT80_MCS2:
        case RATE_11AC_HT80_MCS3:
        case RATE_11AC_HT80_MCS4:
        case RATE_11AC_HT80_MCS5:
        case RATE_11AC_HT80_MCS6:
        case RATE_11AC_HT80_MCS7:
        case RATE_11AC_HT80_MCS8:
        case RATE_11AC_HT80_MCS9:
            wifiMode = WIFI_11AC_HT80_40low_20low;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11P_BW5_1p5_MBPS:
        case RATE_11P_BW5_2p25_MBPS:
        case RATE_11P_BW5_3_MBPS:
        case RATE_11P_BW5_4p5_MBPS:
        case RATE_11P_BW5_6_MBPS:
        case RATE_11P_BW5_9_MBPS:
        case RATE_11P_BW5_12_MBPS:
        case RATE_11P_BW5_13p5_MBPS:
            wifiMode = WIFI_11P_BW5;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11P_BW10_3_MBPS:
        case RATE_11P_BW10_4p5_MBPS:
        case RATE_11P_BW10_6_MBPS:
        case RATE_11P_BW10_9_MBPS:
        case RATE_11P_BW10_12_MBPS:
        case RATE_11P_BW10_18_MBPS:
        case RATE_11P_BW10_24_MBPS:
        case RATE_11P_BW10_27_MBPS:
            wifiMode = WIFI_11P_BW10;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        case RATE_11P_BW20_6_MBPS:
        case RATE_11P_BW20_9_MBPS:
        case RATE_11P_BW20_12_MBPS:
        case RATE_11P_BW20_18_MBPS:
        case RATE_11P_BW20_24_MBPS:
        case RATE_11P_BW20_36_MBPS:
        case RATE_11P_BW20_48_MBPS:
        case RATE_11P_BW20_54_MBPS:
            wifiMode = WIFI_11P_BW20;
            peakToAvg = 10.0 + m_extraUserMargin;
            break;
        default:
            LogE_F("WLAN RATE not supported, Rate: %d" , (int)rate);
            return false;
            //throw new WCNCustomException("WLAN RATE not supported, Rate: " + rate.ToString());
        }
        return true;
    }
}

