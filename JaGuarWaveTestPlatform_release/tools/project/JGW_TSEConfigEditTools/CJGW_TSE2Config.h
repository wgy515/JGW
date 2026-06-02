#pragma once
#include "CJGW_TSEConfigImpl.h"

namespace JGW
{
    class CCJGW_TSE2Config : public CCJGW_TSEConfigImpl
    {
    public:
        CCJGW_TSE2Config(void);
        ~CCJGW_TSE2Config(void);
    private:
        bool LoadTSEConfig(const std::string& strTSEConfigPath,S_TSE_CONFIG_INFOS& sTSEConfigInfos);
        bool SaveTSEConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode);
    };
}
