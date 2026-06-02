#pragma once
#include "CJGW_EquipConfigImpl.h"

namespace JGW
{
    class CCJGW_QCOMMWlanXml : public CCJGW_EquipConfigImpl
    {
    public:
        CCJGW_QCOMMWlanXml(void);
        ~CCJGW_QCOMMWlanXml(void);
    private:
        bool PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
        bool ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo);
    private:
        std::wstring GetResourceIDListItem(const TiXmlElement *pInstrumentNode);
    };

}

