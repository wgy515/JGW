#pragma once
#include "JGW_EquipConfig_Define.h"
#include <tinyxml/tinyxml.h>

namespace JGW
{
    class CCJGW_EquipConfigImpl
    {
    public:
        CCJGW_EquipConfigImpl(void);
        ~CCJGW_EquipConfigImpl(void);
    public:
        virtual bool LoadEquipConfigXml(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo);
        virtual bool SaveEquipConfigXml(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo);
    protected:
        virtual void GetEquipConfigXmlPath();
        virtual bool PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
        virtual bool ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
    protected:
        LPS_EQUIP_PROJECT_CONFIG mpsEquipProjectConfig;
        std::string mstrConfigFilePath;     
    };
}