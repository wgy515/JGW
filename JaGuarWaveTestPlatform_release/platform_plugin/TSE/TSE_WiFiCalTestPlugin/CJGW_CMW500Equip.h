#pragma once
#include <JGW_Device_RohdeSchwarz_CMW500/JGW_CMW500EquipmentControll.h>
#define INVAILD_WLAN_RESULT -999.99

namespace JGW
{
    enum E_B_FETCH_TYPE
    {
        E_B_ModType = 0,
        E_B_BurstType,
        E_B_DataLength,
        E_B_BurstPower,
        E_B_EVMPeak,
        E_B_EVMRMS,
        E_B_FreqError,
        E_B_ClockError,
        E_B_IQOffset,
        E_B_GainImbal,
        E_B_QuadError,
        E_B_OutOfTol
    };
    enum E_WLAN_ANG_MOD_FETCH
    {
        E_ModType_Rata = 0,
        E_DataLength,
        E_BurstPower,
        E_EVMAllCarr,
        E_EVMDataCarr,
        E_EVMPilotCarr,
        E_FreqError,
        E_SymClockError,
        E_IQOffset,
        E_GainImbal,
        E_QuadError,
        E_OutOfTol,
        E_GuardInterval
    };

    enum E_WLAN_AC_MOD_FETCH
    {
        E_AC_ModType_Rata = 0,
        E_AC_BurstType,
        E_AC_DataLength,
        E_AC_BurstPower,
        E_AC_EVMAllCarr,
        E_AC_EVMDataCarr,
        E_AC_EVMPilotCarr,
        E_AC_FreqError,
        E_AC_SymClockError,
        E_AC_IQOffset,
        E_AC_GainImbal,
        E_AC_QuadError,
        E_AC_OutOfTol,
        E_AC_GuardInterval
    };

    class CCJGW_CMW500Equip
    {
    public:
        CCJGW_CMW500Equip(void);
        ~CCJGW_CMW500Equip(void);
    public:
        double GetCWFreqErr(int port,double freq,WLAN_MODE wifiMode,double loss,double powerLevelDBm,double powerRange);
        double GetPower(int port,double freq,WLAN_MODE wifiMode,double loss,double powerLevelDBm,double powerRange);
    private:
        E_WLAN_HT_MODE GetWlanHtModeToWlanMode(WLAN_MODE wifiMode);
        E_WLAN_MODE GetWlanModeToWLAN_MODE(WLAN_MODE wifiMode);
    private:
        JGW_CMW500EquipmentControll mEquipmentControll;
    };
}


