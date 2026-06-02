#include "StdAfx.h"
#include "TSE_LoadTSESuiteConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>

namespace JGW
{
    CTSE_LoadTSESuiteConfig::CTSE_LoadTSESuiteConfig(S_TSE_SequenceConfig& sTSESequenceConfig) : msTSESequenceConfig(sTSESequenceConfig)
    {
    }


    CTSE_LoadTSESuiteConfig::~CTSE_LoadTSESuiteConfig(void)
    {
    }

    bool CTSE_LoadTSESuiteConfig::SaveAsTSESuiteConfig(const std::string& strSaveXmlFilePath)
    {
        return mptrXmlDoc->SaveFile(strSaveXmlFilePath);
    }

    bool CTSE_LoadTSESuiteConfig::SaveTSESuiteConfig()
    {
        return mptrXmlDoc->SaveFile();
    }

    bool CTSE_LoadTSESuiteConfig::PareseTSESuiteConfig(const std::string& strConfigXml)
    {
        mptrXmlDoc.reset ( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;
        msTSESequenceConfig.m_mCommonTestParam.mpCommonXmlNode = NULL;
        msTSESequenceConfig.m_mCommonTestParam.mEnvironment.clear();
        msTSESequenceConfig.m_vSuiteTestConfig.clear();

        if (!strConfigXml.empty() && '<' == strConfigXml[0])
        {
            if ( !mptrXmlDoc->LoadFileBuffer( strConfigXml.c_str() ) )
            {
                PELOG4WW(L"XML Buf Parese Fail\n");
                return false;
            }
        }
        else
        {
            if ( !mptrXmlDoc->LoadFile( strConfigXml.c_str() ) )
            {
                PELOG4WW_F(L"Config Not Exist(XML:%s)\n",JGW_A2W(strConfigXml).c_str());
                return false;
            }
        }

        if( !( xmlRoot = mptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
        {
            PELOG4WW_F(L"解析配置文件失败!!!(XML:%s)\n",JGW_A2W(strConfigXml).c_str());
            return false;
        }	

        return true;
    }

    bool CTSE_LoadTSESuiteConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot)
    {
        PareseCommonNode(xmlRoot);
        PareseConfigNode(xmlRoot);
        PareseAutoNode(xmlRoot);

        return PareseSuiteProject(xmlRoot);
    }

    bool CTSE_LoadTSESuiteConfig::PareseSuiteProject(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement* pTempXmlNode = xmlRoot->FirstChildElement("SuiteProject");
        if (!pTempXmlNode)
        {    
            PELOG4WW(L"当前配置文件无测试项目\n");
            return false;
        }

        while (pTempXmlNode)
        {
            PareseItemNode(pTempXmlNode,msTSESequenceConfig.m_vSuiteTestConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("SuiteProject");
        }
        return true;
    }

    void CTSE_LoadTSESuiteConfig::PareseCommonNode(const TiXmlElement* xmlNode)
    {
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = NULL;
        const TiXmlElement*	pTempXmlNode = NULL;

        pTempXmlNode = xmlNode->FirstChildElement("common");
        if ( !pTempXmlNode ) return ;
        msTSESequenceConfig.m_mCommonTestParam.mpCommonXmlNode = const_cast<TiXmlElement*>(pTempXmlNode);
        pXmlTemp = pTempXmlNode->FirstChildElement();
        std::wstring strkey,strvalue;

        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->GetText();
            strvalue = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Value();
            strkey = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            msTSESequenceConfig.m_mCommonTestParam.mEnvironment.insert(std::make_pair<std::wstring,std::wstring>(strkey,strvalue));
        }

        return ;
    }

    void CTSE_LoadTSESuiteConfig::PareseConfigNode(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement*	pTempXmlNode = xmlRoot->FirstChildElement("config");
        while (pTempXmlNode)
        {
            PareseItemNode(pTempXmlNode,msTSESequenceConfig.m_vConfigViewTestConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("config");
        }
    }

    void CTSE_LoadTSESuiteConfig::PareseAutoNode(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement*	pTempXmlNode = xmlRoot->FirstChildElement("auto");
        while (pTempXmlNode)
        {
            PareseItemNode(pTempXmlNode,msTSESequenceConfig.m_vAutoTestConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("auto");
        }
    }

    void CTSE_LoadTSESuiteConfig::PareseItemNode(const TiXmlElement* xmlNode,_vsuitetestconfig& vsuitetTestConfig)
    {
        if (!xmlNode) return;
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = xmlNode->FirstChildElement();

        std::shared_ptr<s_suite_item_test_config> ptrConfig(new s_suite_item_test_config);
        ptrConfig->mpSuiteXmlNode = const_cast<TiXmlElement*>(xmlNode);;
        //! 是否测试
        pszTemp = xmlNode->Attribute("IsTest");
        ptrConfig->mbIsTest = pszTemp?(atoi(pszTemp) != 0):true;

        //! 结束测试项目
        pszTemp = xmlNode->Attribute("IsFinalize");
        ptrConfig->m_bFinalizeTest = pszTemp?(atoi(pszTemp)!=0):false;
        //! 测试优先级
        pszTemp = xmlNode->Attribute("priority");
        ptrConfig->mstrPriority = pszTemp?pszTemp:"0";
        //! 测试组名称
        pszTemp = xmlNode->Attribute("groupname");
        ptrConfig->mstrGroupName = pszTemp?pszTemp:"default";

        s_suite_single_item_test_config s_single_item;
        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->Value();
            s_single_item.mParamName = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("paramVal");
            s_single_item.mParamValue = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("paramType");
            s_single_item.meParamType = pszTemp?(E_PARAM_TYPE)atoi(pszTemp):E_PARAM_STRING;

            pszTemp = pXmlTemp->Attribute("description");
            s_single_item.mParamDescription = pszTemp?JGW::JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            ptrConfig->m_mTestParam.push_back(s_single_item);
        }
        vsuitetTestConfig.push_back(ptrConfig);
    }
}