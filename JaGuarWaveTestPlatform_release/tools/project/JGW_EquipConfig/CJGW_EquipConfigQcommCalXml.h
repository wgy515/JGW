#pragma once
#include "CJGW_EquipConfigImpl.h"

namespace JGW
{
    class CCJGW_EquipConfigQcommCalXml : public CCJGW_EquipConfigImpl
    {
    public:
        CCJGW_EquipConfigQcommCalXml(void);
        ~CCJGW_EquipConfigQcommCalXml(void);
    private:
        bool PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
        bool ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
    };
}