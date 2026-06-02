#include "StdAfx.h"
#include "CJGW_TSEFactoryConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "CJGW_TSE2Config.h"
#include "CJGW_TSEConfig.h"

namespace JGW
{
    CCJGW_TSEFactoryConfig::CCJGW_TSEFactoryConfig(void) : mpTSEConfigImpl(NULL)
    {
    }


    CCJGW_TSEFactoryConfig::~CCJGW_TSEFactoryConfig(void)
    {
    }

    LPS_TSE_CONFIG_INFOS CCJGW_TSEFactoryConfig::GetTSEConfigInfos()
    {
        return &msTSEConfigInfos;
    }

    bool CCJGW_TSEFactoryConfig::LoadTSEFactoryConfig(const std::string& strTSEConfigPath)
    {
        if (NULL != mpTSEConfigImpl)
        {
            delete mpTSEConfigImpl;
            mpTSEConfigImpl = NULL;
            msTSEConfigInfos.m_vConfigViewTestConfig.clear();
            msTSEConfigInfos.mmapGlobalVariables.clear();
            msTSEConfigInfos.m_vAutoTestConfig.clear();
            msTSEConfigInfos.m_vSuiteTestConfig.clear();
        }
        //! <RootTest>
        if (NULL != JGW_StrCaseStr(strTSEConfigPath.c_str(),"_tse2.") || NULL != JGW_StrCaseStr(strTSEConfigPath.c_str(),"<RootTest>"))
        {
            mpTSEConfigImpl = new CCJGW_TSE2Config();
        }
        else
        {
            mpTSEConfigImpl = new CCJGW_TSEConfig();
        }

        return mpTSEConfigImpl->LoadTSEConfig(strTSEConfigPath,msTSEConfigInfos);
    }

    bool CCJGW_TSEFactoryConfig::SaveTSEFactoryConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode)
    {
        if (NULL != mpTSEConfigImpl)
        {
            return mpTSEConfigImpl->SaveTSEConfig(strTSEConfigPath,pHomeTreeNode);
        }
        return false;
    }
}

