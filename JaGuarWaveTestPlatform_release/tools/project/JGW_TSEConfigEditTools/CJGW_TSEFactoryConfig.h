#pragma once
#include "JGW_TSEConfigEditDefine.h"
#include "CJGW_TSEConfigImpl.h"
namespace JGW
{
    class CCJGW_TSEFactoryConfig
    {
    public:
        CCJGW_TSEFactoryConfig(void);
        ~CCJGW_TSEFactoryConfig(void);
    public:
        bool LoadTSEFactoryConfig(const std::string& strTSEConfigPath);
        bool SaveTSEFactoryConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode);
        LPS_TSE_CONFIG_INFOS GetTSEConfigInfos();
    private:
        S_TSE_CONFIG_INFOS msTSEConfigInfos;
        CCJGW_TSEConfigImpl* mpTSEConfigImpl;
    };
}

