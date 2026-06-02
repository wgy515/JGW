#include "StdAfx.h"
#include "CJGW_CMW500Equip.h"

namespace JGW
{
    CCJGW_CMW500Equip::CCJGW_CMW500Equip(void)
    {
    }


    CCJGW_CMW500Equip::~CCJGW_CMW500Equip(void)
    {
    }

    E_WLAN_HT_MODE CCJGW_CMW500Equip::GetWlanHtModeToWlanMode(WLAN_MODE wifiMode)
    {
        switch (wifiMode)
        {
            // B
        case WIFI_11B :
            return E_WLAN_CCK;
            //! G
        case WIFI_11AG:
            return E_WLAN_NO_HT_DSRC10;
            //! M
        case WIFI_11N_HT20:
            return E_WLAN_HT20;
        case WIFI_11N_HT40Minus:
            return E_WLAN_HT40_DEC;
        case WIFI_11N_HT40Plus:
            return E_WLAN_HT40_ADD;
            //! 11AC
        case WIFI_11AC_HT20:
            return E_WLAN_VHT20;
        case WIFI_11AC_HT40Minus:
            return E_WLAN_VHT40_DEC;
        case WIFI_11AC_HT40Plus:
            return E_WLAN_VHT40_ADD;
        case WIFI_11AC_HT80_40low_20low:
            return E_WLAN_VHT80_0;
        case WIFI_11AC_HT80_40low_20high:
            return E_WLAN_VHT80_1;
        case WIFI_11AC_HT80_40high_20low:
            return E_WLAN_VHT80_2;
        case WIFI_11AC_HT80_40high_20high:
            return E_WLAN_VHT80_3;
        case WIFI_11P_BW5:
            return E_WLAN_11P_BW5;
        case WIFI_11P_BW10:
            return E_WLAN_11P_BW10;
        case WIFI_11P_BW20:
            return E_WLAN_11P_BW20;
        }
        return E_WLAN_CCK;
    }

    E_WLAN_MODE CCJGW_CMW500Equip::GetWlanModeToWLAN_MODE(WLAN_MODE wifiMode)
    {
        switch (wifiMode)
        {
        case WIFI_11B:
            return E_B_WLAN_MODE;
        case WIFI_11AG:
            return E_G_WLAN_MODE;
        case WIFI_11N_HT20:
        case WIFI_11N_HT40Minus:
        case WIFI_11N_HT40Plus:
            return E_N_WLAN_MODE;
        case WIFI_11AC_HT20:
        case WIFI_11AC_HT40Minus:
        case WIFI_11AC_HT40Plus:
        case WIFI_11AC_HT80_40low_20low:
        case WIFI_11AC_HT80_40low_20high:
        case WIFI_11AC_HT80_40high_20low:
        case WIFI_11AC_HT80_40high_20high:
            return E_AC_WLAN_MODE;
        case WIFI_11P_BW5:
        case WIFI_11P_BW10:
        case WIFI_11P_BW20:
            return E_P_WLAN_MODE;
        }
        return E_UNKNOW_MODE;
    }

    double CCJGW_CMW500Equip::GetCWFreqErr(int port,double freq,WLAN_MODE wifiMode,double loss,double powerLevelDBm,double powerRange)
    {
        mEquipmentControll.SetInstrumentSys(E_WLAN_NAME);
        mEquipmentControll.SetRxInPutPort(port);
        mEquipmentControll.SetRxLoss(static_cast<float>(loss));
        mEquipmentControll.WLAN_SetExpectedPowerAndUserMarg(static_cast<float>(powerLevelDBm),static_cast<float>(powerRange));
        mEquipmentControll.WLAN_SetWlanMode(GetWlanHtModeToWlanMode(wifiMode));

        float fModuleData[20] = {0};
        float fSpeMask[20] = {0};
        E_WLAN_MODE wlanMode = GetWlanModeToWLAN_MODE(wifiMode);
        mEquipmentControll.WLAN_GetTestResult(wlanMode,fModuleData,fSpeMask);
        if (E_B_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_B_FreqError];
        }
        else if (E_G_WLAN_MODE == wlanMode || E_N_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_FreqError];
        }
        else if (E_AC_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_AC_FreqError];
        }
        return INVAILD_WLAN_RESULT;
    }

    double CCJGW_CMW500Equip::GetPower(int port,double freq,WLAN_MODE wifiMode,double loss,double powerLevelDBm,double powerRange)
    {
        mEquipmentControll.SetInstrumentSys(E_WLAN_NAME);
        mEquipmentControll.SetRxInPutPort(port);
        mEquipmentControll.SetRxLoss(static_cast<float>(loss));
        mEquipmentControll.WLAN_SetExpectedPowerAndUserMarg(static_cast<float>(powerLevelDBm),static_cast<float>(powerRange));
        mEquipmentControll.WLAN_SetWlanMode(GetWlanHtModeToWlanMode(wifiMode));

        float fModuleData[20] = {0};
        float fSpeMask[20] = {0};
        E_WLAN_MODE wlanMode = GetWlanModeToWLAN_MODE(wifiMode);
        mEquipmentControll.WLAN_GetTestResult(wlanMode,fModuleData,fSpeMask);
        if (E_B_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_B_BurstPower];
        }
        else if (E_G_WLAN_MODE == wlanMode || E_N_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_B_BurstPower];
        }
        else if (E_AC_WLAN_MODE == wlanMode)
        {
            return fModuleData[E_AC_BurstPower];
        }
        return INVAILD_WLAN_RESULT;
    }
}
