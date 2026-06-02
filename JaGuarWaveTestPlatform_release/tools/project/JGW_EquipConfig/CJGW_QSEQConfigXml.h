#pragma once
#include "CJGW_EquipConfigImpl.h"

namespace JGW
{
    class CCJGW_QSEQConfigXml : public CCJGW_EquipConfigImpl
    {
    public:
        CCJGW_QSEQConfigXml(void);
        ~CCJGW_QSEQConfigXml(void);
    private:
        bool PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
        bool ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);  
    };
}