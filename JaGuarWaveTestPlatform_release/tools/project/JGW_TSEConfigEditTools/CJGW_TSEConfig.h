#pragma once
#include "CJGW_TSEConfigImpl.h"
#include <tinyxml/tinyxml.h>


namespace JGW
{
    class CCJGW_TSEConfig : public CCJGW_TSEConfigImpl
    {
    public:
        CCJGW_TSEConfig(void);
        ~CCJGW_TSEConfig(void);
    private:
        bool LoadTSEConfig(const std::string& strTSEConfigPath,S_TSE_CONFIG_INFOS& sTSEConfigInfos);
        bool SaveTSEConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode);
    private:
        //! 解析所有测试节点
        bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot,S_TSE_CONFIG_INFOS& sTSEConfigInfos);
        //!
        bool PareseGlobalVariablesNode(const TiXmlElement* pGlobalVariablesNode,_mMapGlobalVariables& mapGlobalVariables);
        //!
        bool PareseSuiteTestNode(const TiXmlElement* pSuiteTestNode,const char* nodeName,_vsuitetestconfig& vSuiteTestConfig);
    private:
        bool PareseItemNode(const TiXmlElement* pItemNode,s_single_item_test_config& sSingleItemTestConfig);
    private:
        bool SaveEnvironmentTreeNode(CTreeNodeUI* pEnvironment,TiXmlElement* pEnvironmentNode);
        bool SaveConfigTreeNode(CTreeNodeUI* pConfigTreeNode,TiXmlElement* pConfigNode);
        bool SaveAutoTreeNode(CTreeNodeUI* pAutoTreeNode,TiXmlElement* pAutoNode);
        bool SaveSuiteProjectTreeNode(CTreeNodeUI* pSuiteProjectTreeNode,TiXmlElement* pSuiteProjectNode);
        bool AddSuiteProjectNodeToTreeNodeData(CTreeNodeUI* pSuiteProjectTreeNode,TiXmlElement* pSuiteProjectNode,const std::string& strNodeName);
    private:
        std::wstring mstrErrorMsg;
    };
}
