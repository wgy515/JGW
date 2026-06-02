#pragma once
#include "CJGW_CMW500Equip.h"
//! #define EQUIP CCJGW_EquipBase::GetEquip()
namespace JGW
{
    class CCJGW_EquipBase
    {
    private:
        CCJGW_EquipBase(void);
        ~CCJGW_EquipBase(void);
    public:
        static CCJGW_EquipBase& GetEquip();
    public:
        //! 
        double GetCWFreqErr(double chFreqMHz, double powerLevelDBm);
        //! 
        double GetPowerDelegate(WLAN_DataRate rate, bool short11b_nGuard, double chFreqMHz, double powerLevelDBm);
        //!
        double GetCWPowerDelegate(double chFreqMHz, double powerLevelDBm);
        //!
        void MeasurePower(WLAN_RATE rate, double channelFreqMhz, double powerLevelDBm, ChainMask chainMask, TriggerType triggerType,const std::vector<double>& vPathLossDB, const std::vector<double>& vVsaTriggerLevelOffsetDbr , bool shortGuardInterval, std::vector<double>& vPowerAvEachBurst, std::vector<double>& vPowerPkEachBurst);
    public:
        bool GetWifiMode(WLAN_RATE rate, WLAN_MODE& wifiMode, double& peakToAvg);
    private:
        double m_extraUserMargin;
        CCJGW_CMW500Equip mEquip;
    };
}


