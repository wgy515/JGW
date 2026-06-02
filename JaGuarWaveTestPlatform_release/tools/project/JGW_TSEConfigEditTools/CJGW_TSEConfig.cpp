#include "StdAfx.h"
#include "CJGW_TSEConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CCJGW_TSEConfig::CCJGW_TSEConfig(void)
    {
    }


    CCJGW_TSEConfig::~CCJGW_TSEConfig(void)
    {
    }

    bool CCJGW_TSEConfig::LoadTSEConfig(const std::string& strTSEConfigPath,S_TSE_CONFIG_INFOS& sTSEConfigInfos)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!strTSEConfigPath.empty() && 0 == _strnicmp(strTSEConfigPath.c_str(),"<?xml",strlen("<?xml")))
        {
            if ( !ptrXmlDoc->LoadFileBuffer(strTSEConfigPath.c_str()) )
            {
                mstrErrorMsg = L"XML Buf Parese Fail";
                return false;
            }
        }
        else
        {
            if ( !ptrXmlDoc->LoadFile(strTSEConfigPath.c_str()) )
            {
                JGW_FormatWString(mstrErrorMsg,L"Config Not Exist(XML:%s)\n",JGW_A2W(strTSEConfigPath).c_str());
                return false;
            }
        }
        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot,sTSEConfigInfos))
        {
            //! E_XML_FAILED_PARSE_CONFIG
            JGW_FormatWString(mstrErrorMsg,GetString(E_XML_FAILED_PARSE_CONFIG),JGW_A2W(strTSEConfigPath).c_str());
            return false;
        }	
#if 0
        if (!LoadGlobalEnvironmentConfigXml())
        {
            mLogServices.ErrorLogFormat(GetString(E_XML_FAILED_PARSE_CONFIG),mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }
#endif

        return true;
    }

    bool CCJGW_TSEConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot,S_TSE_CONFIG_INFOS& sTSEConfigInfos)
    {
        const TiXmlElement*	pGlobalVariablesNode = xmlRoot->FirstChildElement("common");
        if (NULL != pGlobalVariablesNode)
        {
            PareseGlobalVariablesNode(pGlobalVariablesNode,sTSEConfigInfos.mmapGlobalVariables);
        }

        const TiXmlElement*	pConfigSuiteTestNode = xmlRoot->FirstChildElement("config");
        if (NULL != pConfigSuiteTestNode)
        {
            //PareseSuiteTestNode(pConfigSuiteTestNode,"config",sTSEConfigInfos.m_vConfigViewTestConfig);
            while (pConfigSuiteTestNode)
            {
                if (!PareseSuiteTestNode(pConfigSuiteTestNode,"config",sTSEConfigInfos.m_vConfigViewTestConfig))
                {
                    return false;
                }
                pConfigSuiteTestNode = pConfigSuiteTestNode->NextSiblingElement("config");
            }
        }

        const TiXmlElement*	pAutoSuiteTestNode = xmlRoot->FirstChildElement("auto");
        if (NULL != pAutoSuiteTestNode)
        {
            while (pAutoSuiteTestNode)
            {
                if (!PareseSuiteTestNode(pAutoSuiteTestNode,"auto",sTSEConfigInfos.m_vAutoTestConfig))
                {
                    return false;
                }
                pAutoSuiteTestNode = pAutoSuiteTestNode->NextSiblingElement("auto");
            }
            //PareseSuiteTestNode(pAutoSuiteTestNode,"auto",sTSEConfigInfos.m_vAutoTestConfig);
        }

        const TiXmlElement*	pSuiteTestNode = xmlRoot->FirstChildElement("SuiteProject");
        if (NULL != pSuiteTestNode)
        {
            // 解析当前节点的兄弟节点
            //pSuiteTestNode = pSuiteTestNode->NextSiblingElement(nodeName);
            while (pSuiteTestNode)
            {
                if (!PareseSuiteTestNode(pSuiteTestNode,"SuiteProject",sTSEConfigInfos.m_vSuiteTestConfig))
                {
                    return false;
                }
                pSuiteTestNode = pSuiteTestNode->NextSiblingElement("SuiteProject");
            }
            //PareseSuiteTestNode(pSuiteTestNode,"SuiteProject",sTSEConfigInfos.m_vSuiteTestConfig);
        }

        return true;
    }

    bool CCJGW_TSEConfig::PareseGlobalVariablesNode(const TiXmlElement* pGlobalVariablesNode,_mMapGlobalVariables& mapGlobalVariables)
    {
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = pGlobalVariablesNode->FirstChildElement();
        std::wstring pNodeValue,pNodeName;

        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->Value();
            pNodeName = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";
            //! GetElementTextValue(pXmlTemp,pNodeName,pNodeValue);
            pszTemp = pXmlTemp->GetText();
            pNodeValue = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            _mMapGlobalVariables::iterator it = mapGlobalVariables.find(pNodeName);
            if (it != mapGlobalVariables.end())
            {
                it->second = pNodeValue;
            }
            else
            {
                mapGlobalVariables.insert(std::make_pair<std::wstring,std::wstring>(pNodeName,pNodeValue));  
            }
            pXmlTemp = pXmlTemp->NextSiblingElement();
        }
        return true;
    }

    bool CCJGW_TSEConfig::PareseSuiteTestNode(const TiXmlElement* pSuiteTestNode,const char* nodeName,_vsuitetestconfig& vSuiteTestConfig)
    {
        if (NULL == pSuiteTestNode)
        {
            mstrErrorMsg = L"解析测试节点失败...";
            return false;
        }

        std::shared_ptr<s_suite_test_config> ptrSuiteTestConfig(new s_suite_test_config);
        // 解析当前测试节点
        PareseItemNode(pSuiteTestNode,ptrSuiteTestConfig->msItemTestConfig);
        // 检查当前节点是否测试
        //if (ptrSuiteTestConfig->msItemTestConfig.mbIsTest)
        {
            // 解析当前节点的子节点
            const TiXmlElement*	pXmlTemp = pSuiteTestNode->FirstChildElement(nodeName);
            while (pXmlTemp)
            {
                if (!PareseSuiteTestNode(pXmlTemp,nodeName,ptrSuiteTestConfig->mvChildrenSuiteTest))
                {
                    return false;
                }
                pXmlTemp = pXmlTemp->NextSiblingElement(nodeName);
            }
            vSuiteTestConfig.push_back(ptrSuiteTestConfig);
        }

        return true;
    }

    bool CCJGW_TSEConfig::PareseItemNode(const TiXmlElement* pItemNode,s_single_item_test_config& sSingleItemTestConfig)
    {
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = pItemNode->FirstChildElement();
        //! 是否测试
        pszTemp = pItemNode->Attribute("IsTest");
        sSingleItemTestConfig.mbIsTest = true;
        if ( pszTemp && 0 == atoi(pszTemp) ) sSingleItemTestConfig.mbIsTest = false;

        //! 结束测试项目
        pszTemp = pItemNode->Attribute("IsFinalize");
        sSingleItemTestConfig.mbFinalizeTest = pszTemp?(atoi(pszTemp)!=0):false;
        //! 翻转测试结果
        pszTemp = pItemNode->Attribute("ReversedResult");
        sSingleItemTestConfig.mbReversedResult = pszTemp?(atoi(pszTemp)!=0):false;
        //! 如果测试结果为false 则运行子测试序列
        pszTemp = pItemNode->Attribute("IfFalse");
        sSingleItemTestConfig.mbIfFalse = pszTemp?(atoi(pszTemp)!=0):false;

        s_tse_test_param s_single_item;
        s_single_item.meParamType = E_PARAM_STRING;
        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->Value();
            //s_single_item.mParamName = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamName = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";
            if (0 == JGW_WStrComparenoCaseWStr(s_single_item.mParamName.c_str(),L"SuiteProject"))
            {
                pXmlTemp = pXmlTemp->NextSiblingElement();
                continue;
            }

            pszTemp = pXmlTemp->Attribute("paramVal");
            //s_single_item.mParamValue = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamValue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";
            if (0 ==JGW_WStrComparenoCaseWStr(s_single_item.mParamName.c_str(),L"TestName")) sSingleItemTestConfig.mstrTestName = s_single_item.mParamValue;

            pszTemp = pXmlTemp->Attribute("description");
            //s_single_item.mParamDescription = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamDescription = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            sSingleItemTestConfig.mvTestConfigs.push_back(s_single_item);
        }
        return true;
    }

    bool CCJGW_TSEConfig::SaveTSEConfig(const std::string& strTSEConfigPath,CTreeNodeUI* pHomeTreeNode)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlDeclaration *pDec = new TiXmlDeclaration("1.0", "utf-8", "");
        ptrXmlDoc->LinkEndChild(pDec);
        TiXmlElement* pSuiteNode = new TiXmlElement("Suite");
        TiXmlElement* pCommonNode1 = new TiXmlElement("common");
        pSuiteNode->LinkEndChild(pCommonNode1);
        ptrXmlDoc->LinkEndChild(pSuiteNode);

        for (int index = 0;index < pHomeTreeNode->GetCountChild();index++)
        {
            // save GlobalVariables
            CTreeNodeUI* pTreeNodeUI = pHomeTreeNode->GetChildNode(index);
            if (pTreeNodeUI->GetItemText() == L"Environment")
            {
                if (!SaveEnvironmentTreeNode(pTreeNodeUI,pCommonNode1)) return false;
            }
            else if (pTreeNodeUI->GetItemText() == L"Config")
            {
                // config
                if (!SaveConfigTreeNode(pTreeNodeUI,pSuiteNode)) return false;
            }
            else if (pTreeNodeUI->GetItemText() == L"Test Automation")
            {
                // auto
                if (!SaveAutoTreeNode(pTreeNodeUI,pSuiteNode)) return false;
            }
            else if (pTreeNodeUI->GetItemText() == L"Test Main")
            {
                // SuiteProject
                if (!SaveSuiteProjectTreeNode(pTreeNodeUI,pSuiteNode)) return false;
            }
            else
            {
                return false;
            }
        }
        return ptrXmlDoc->SaveFile(strTSEConfigPath);
    }

    bool CCJGW_TSEConfig::SaveEnvironmentTreeNode(CTreeNodeUI* pEnvironment,TiXmlElement* pEnvironmentNode)
    {
        for (int index = 0;index < pEnvironment->GetCountChild();index++)
        {
            CTreeNodeUI* pTreeNodeUI = pEnvironment->GetChildNode(index);
            if (pTreeNodeUI->GetItemText() == L"GlobalVariables")
            {
                _mMapGlobalVariables* pMapGlobalVariables = (_mMapGlobalVariables*)pTreeNodeUI->GetDate();
                for (_mMapGlobalVariables::iterator it = pMapGlobalVariables->begin();it != pMapGlobalVariables->end();++it)
                {
                    AddElementTextValue(pEnvironmentNode,JGW_W2A(it->first,CP_UTF8).c_str(),it->second);
                }
                return true;
            }
        }
        return false;
    }

    bool CCJGW_TSEConfig::SaveConfigTreeNode(CTreeNodeUI* pConfigTreeNode,TiXmlElement* pConfigNode)
    {
        bool result = true;
        for (int index = 0;index < pConfigTreeNode->GetCountChild();index++)
        {
            CTreeNodeUI* pTreeNodeUI = pConfigTreeNode->GetChildNode(index);
            result &= AddSuiteProjectNodeToTreeNodeData(pTreeNodeUI,pConfigNode,"config");
        }
        return result;
    }

    bool CCJGW_TSEConfig::SaveAutoTreeNode(CTreeNodeUI* pAutoTreeNode,TiXmlElement* pAutoNode)
    {
        bool result = true;
        for (int index = 0;index < pAutoTreeNode->GetCountChild();index++)
        {
            CTreeNodeUI* pTreeNodeUI = pAutoTreeNode->GetChildNode(index);
            if (pTreeNodeUI->GetItemText() == L"GlobalVariables")
            {
                CTreeNodeUI* pTreeNodeUI = pAutoTreeNode->GetChildNode(index);
                result &= AddSuiteProjectNodeToTreeNodeData(pTreeNodeUI,pAutoNode,"auto");
            }
        }
        return result;
    }

    bool CCJGW_TSEConfig::SaveSuiteProjectTreeNode(CTreeNodeUI* pSuiteProjectTreeNode,TiXmlElement* pSuiteProjectNode)
    {
        bool result = true;
        for (int index = 0;index < pSuiteProjectTreeNode->GetCountChild();index++)
        {
            CTreeNodeUI* pTreeNodeUI = pSuiteProjectTreeNode->GetChildNode(index);
            result &= AddSuiteProjectNodeToTreeNodeData(pTreeNodeUI,pSuiteProjectNode,"SuiteProject");
        }
        return result;
    }

    void AddSuiteProjectChildNode(s_tse_test_param* psTSETestParam,TiXmlElement* pParentNode)
    {
        TiXmlElement *pSubNode = new TiXmlElement(JGW_W2A(psTSETestParam->mParamName,CP_UTF8));
        pSubNode->SetAttribute("paramVal",JGW_W2A(psTSETestParam->mParamValue,CP_UTF8));
        pSubNode->SetAttribute("description",JGW_W2A(psTSETestParam->mParamDescription,CP_UTF8));
        //pSubNode->SetAttribute("paramType",(psTSETestParam->meParamType));
        pParentNode->LinkEndChild(pSubNode);
    }

    bool CCJGW_TSEConfig::AddSuiteProjectNodeToTreeNodeData(CTreeNodeUI* pSuiteProjectTreeNode,TiXmlElement* pSuiteProjectNode,const std::string& strNodeName)
    {
        TiXmlElement* pNode = new TiXmlElement(strNodeName);
        s_single_item_test_config* pItemTestConfig = (s_single_item_test_config*)pSuiteProjectTreeNode->GetDate();
        if (NULL == pItemTestConfig) return false;

        pItemTestConfig->mbIsTest = pSuiteProjectTreeNode->GetCheckBox()->GetCheck();
        if (!pItemTestConfig->mbIsTest) pNode->SetAttribute("IsTest",0);
        if (pItemTestConfig->mbFinalizeTest) pNode->SetAttribute("IsFinalize",1);
        if (pItemTestConfig->mbIfFalse) pNode->SetAttribute("IfFalse",1);
        if (pItemTestConfig->mbReversedResult) pNode->SetAttribute("ReversedResult",1);

        for (std::vector<s_tse_test_param>::iterator it = pItemTestConfig->mvTestConfigs.begin();it != pItemTestConfig->mvTestConfigs.end();++it)
        {
            AddSuiteProjectChildNode(&it[0],pNode);
        }

        for (int index = 0;index < pSuiteProjectTreeNode->GetCountChild();index++)
        {
            CTreeNodeUI* pTreeNodeUI = pSuiteProjectTreeNode->GetChildNode(index);
            AddSuiteProjectNodeToTreeNodeData(pTreeNodeUI,pNode,strNodeName);
        }

        pSuiteProjectNode->LinkEndChild(pNode);
        return true;
    }
}
