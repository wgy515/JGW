#pragma once
#include "CJGW_EquipConfigImpl.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_EquipConfigCustomIni : public CCJGW_EquipConfigImpl
    {
    public:
        CCJGW_EquipConfigCustomIni(void);
        ~CCJGW_EquipConfigCustomIni(void);
    public:
        bool LoadCustomIni(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo);
        bool SaveCustomIni(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo);
    private:
        bool LoadMonitorConfig(CCJGW_ConfigIni& m_pConfigIniFile,S_MONITOR_INFO& sMonitorInfo);
        bool ChangeMonitorConfig(CCJGW_ConfigIni& m_pConfigIniFile,S_MONITOR_INFO& sMonitorInfo);
    };
}