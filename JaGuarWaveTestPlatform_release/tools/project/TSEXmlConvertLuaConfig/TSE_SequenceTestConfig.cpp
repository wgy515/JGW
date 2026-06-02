#include "StdAfx.h"
#include "TSE_SequenceTestConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include <iostream>
namespace JGW
{
    CTSE_SequenceTestConfig::CTSE_SequenceTestConfig(S_TSE_SequenceConfig& suitetestconfig):m_sSuiteTestConfig(suitetestconfig)
    {
    }


    CTSE_SequenceTestConfig::~CTSE_SequenceTestConfig(void)
    {
    }

    bool CTSE_SequenceTestConfig::PareseAssemblyConfig(const std::string& strConfigXml)
    {
        mstrConfigXml = strConfigXml;
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;
        m_sSuiteTestConfig.m_mCommonTestParam.clear();
        m_sSuiteTestConfig.mvsSuiteProjectConfig.clear();
        mstrGlobalEnvironmentConfigXmlPath = L"";
        //m_sSuiteTestConfig.m_mCommonTestParam.insert(std::make_pair<std::wstring,std::wstring>(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",JGW_A2W(strConfigXml)));
        if (!strConfigXml.empty() && '<' == strConfigXml[0])
        {
            if ( !ptrXmlDoc->LoadFileBuffer( strConfigXml.c_str() ) )
            {
                std::wcout << L"XML Buf Parese Fail\n" << std::endl;;
                return false;
            }
        }
        else
        {
            if ( !ptrXmlDoc->LoadFile( strConfigXml.c_str() ) )
            {
                std::wcout << L"Config Not Exist(XML:" << JGW_A2W(strConfigXml).c_str() << std::endl;;
                return false;
            }
        }

        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
        {
            //! E_XML_FAILED_PARSE_CONFIG
			std::wcout << L"Failed to parse the configuration file : "<< JGW_A2W(strConfigXml).c_str() << std::endl;;
            //mLogServices.ErrorLogFormat(GetString(E_XML_FAILED_PARSE_CONFIG),JGW_A2W(strConfigXml).c_str());
            return false;
        }	

        //if (!LoadGlobalEnvironmentConfigXml())
        //{
        //    std::wcout << L"Failed to parse the configuration file : "<< JGW_A2W(strConfigXml).c_str() << std::endl;;
        //    return false;
        //}

        return true;
    }
#if 0
    bool CTSE_SequenceTestConfig::LoadGlobalEnvironmentConfigXml()
    {
        if (mstrGlobalEnvironmentConfigXmlPath.empty())
        {
            return true;
        }
        if (!JGW_PathFileExists(mstrGlobalEnvironmentConfigXmlPath.c_str()))
        {
            JGW_CreateDirectory(mstrGlobalEnvironmentConfigXmlPath.c_str(),false);
            std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
            TiXmlDeclaration *pDec = new TiXmlDeclaration("1.0", "utf-8", "");
            ptrXmlDoc->LinkEndChild(pDec);
            TiXmlElement* pSuiteNode = new TiXmlElement("Suite");
            TiXmlElement* pCommonNode1 = new TiXmlElement("common");
            pSuiteNode->LinkEndChild(pCommonNode1);
            ptrXmlDoc->LinkEndChild(pSuiteNode);
            ptrXmlDoc->SaveFile(JGW_W2A(mstrGlobalEnvironmentConfigXmlPath).c_str());
            return true;
        }

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;
        if (!ptrXmlDoc->LoadFile(JGW_W2A(mstrGlobalEnvironmentConfigXmlPath).c_str()))
        {
            mLogServices.ErrorLogFormat(L"Global Environment Config Load Error(XML:%s)\n",mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }

        xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            mLogServices.ErrorLogFormat(GetString(E_XML_FAILED_PARSE_CONFIG),mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }	
        PareseCommonNode(xmlRoot);
        return true;
    }
#endif

    bool CTSE_SequenceTestConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot)
    {
        PareseCommonNode(xmlRoot);
        PareseConfigNode(xmlRoot);
        PareseAutoNode(xmlRoot);

        return PareseSuiteProject(xmlRoot);
    }

    bool CTSE_SequenceTestConfig::PareseSuiteProject(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement* pTempXmlNode = xmlRoot->FirstChildElement("SuiteProject");
        if (!pTempXmlNode)
        {    
			std::wcout << L"当前配置文件无测试项目" << std::endl;
            ///mLogServices.ErrorLog(GetString(E_TSE_XMLCONFIG_NO_TEST_PROJECT));
            return false;
        }

        while (pTempXmlNode)
        {
            std::shared_ptr<S_Test_SuiteProjectConfig> ptrsSuiteProjectConfig(new S_Test_SuiteProjectConfig);
            PareseItemNode(pTempXmlNode,ptrsSuiteProjectConfig->msSuiteItemTestConfig);
            PareseChildSuiteProject(pTempXmlNode,ptrsSuiteProjectConfig->m_vChildTestSuiteProjectConfig);
            m_sSuiteTestConfig.mvsSuiteProjectConfig.push_back(ptrsSuiteProjectConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("SuiteProject");
        }
        return true;
    }

    void CTSE_SequenceTestConfig::PareseChildSuiteProject(const TiXmlElement* pChildSuiteProjectNode,std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>& vChildTestSuiteProjectConfig)
    {
        const TiXmlElement* pTempXmlNode = pChildSuiteProjectNode->FirstChildElement("SuiteProject");
        while (pTempXmlNode)
        {
            std::shared_ptr<S_Test_SuiteProjectConfig> ptrSTestSuiteProjectConfig (new S_Test_SuiteProjectConfig());
            PareseItemNode(pTempXmlNode,ptrSTestSuiteProjectConfig->msSuiteItemTestConfig);
            PareseChildSuiteProject(pTempXmlNode,ptrSTestSuiteProjectConfig->m_vChildTestSuiteProjectConfig);
            vChildTestSuiteProjectConfig.push_back(ptrSTestSuiteProjectConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("SuiteProject");
        }
    }

    void CTSE_SequenceTestConfig::PareseCommonNode(const TiXmlElement* xmlNode)
    {
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = NULL;
        const TiXmlElement*	pTempXmlNode = NULL;

        pTempXmlNode = xmlNode->FirstChildElement("common");
        if ( !pTempXmlNode ) return ;
        pXmlTemp = pTempXmlNode->FirstChildElement();
        std::wstring strkey,strvalue;

        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->GetText();
            //strvalue = pszTemp?a2w_a(W(pszTemp)):L"";
            strvalue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Value();
            //strkey = pszTemp?a2w_a(W(pszTemp)):L"";
            strkey = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();
#if 0
            if (0 == JGW_WStrComparenoCaseWStr(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",strkey.c_str()))
            {
#if 0
                mstrGlobalEnvironmentConfigXmlPath = JGW_RealativePathToAbsPath(strvalue.c_str());
                strvalue = mstrGlobalEnvironmentConfigXmlPath;
                //_mcommontestconfig::iterator it = m_sSuiteTestConfig.m_mCommonTestParam.find(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH");
                //if (it != m_sSuiteTestConfig.m_mCommonTestParam.end())
                //{
                //    it->second = mstrGlobalEnvironmentConfigXmlPath;
                //}
#else
               if (strvalue.empty())
               {
                   std::string strEnvrionmentConfig (mstrConfigXml);
                   JGW_ReplaceStringA(strEnvrionmentConfig,"\\cache\\","\\config\\");
                   strvalue = JGW_A2W(strEnvrionmentConfig);
				   mstrGlobalEnvironmentConfigXmlPath = strvalue;
               }
               else
               {
                   mstrGlobalEnvironmentConfigXmlPath = JGW_RealativePathToAbsPath(strvalue.c_str());
                   strvalue = mstrGlobalEnvironmentConfigXmlPath;
               }
#endif
            }
#endif
            _mcommontestconfig::iterator it = m_sSuiteTestConfig.m_mCommonTestParam.find(strkey);
            if (it != m_sSuiteTestConfig.m_mCommonTestParam.end())
            {
                it->second = strvalue;
            }
            else
            {
                m_sSuiteTestConfig.m_mCommonTestParam.insert(std::make_pair<std::wstring,std::wstring>(strkey,strvalue));  
            }
        }

        return ;
    }

    void CTSE_SequenceTestConfig::PareseConfigNode(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement*	pTempXmlNode = xmlRoot->FirstChildElement("config");
        while (pTempXmlNode)
        {
            PareseItemNode(pTempXmlNode,m_sSuiteTestConfig.m_vConfigViewTestConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("config");
        }
    }

    void CTSE_SequenceTestConfig::PareseAutoNode(const TiXmlElement* xmlRoot)
    {
        const TiXmlElement*	pTempXmlNode = xmlRoot->FirstChildElement("auto");
        while (pTempXmlNode)
        {
            PareseItemNode(pTempXmlNode,m_sSuiteTestConfig.m_vAutoTestConfig);
            pTempXmlNode = pTempXmlNode->NextSiblingElement("auto");
        }
    }

    void CTSE_SequenceTestConfig::PareseItemNode(const TiXmlElement* xmlNode,s_suite_item_test_config& suiteItemTestConfig)
    {
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = xmlNode->FirstChildElement();
        //! 是否测试
        pszTemp = xmlNode->Attribute("IsTest");
        if ( pszTemp && 0 == atoi(pszTemp) ) return;

        //! 结束测试项目
        pszTemp = xmlNode->Attribute("IsFinalize");
        suiteItemTestConfig.m_bFinalizeTest = pszTemp?(atoi(pszTemp)!=0):false;
        //! 
        pszTemp = xmlNode->Attribute("ReversedResult");
        suiteItemTestConfig.mbReversedResult = pszTemp?(atoi(pszTemp)!=0):false;
        //!
        pszTemp = xmlNode->Attribute("IfFalse");
        suiteItemTestConfig.mbIfFalse = pszTemp?(atoi(pszTemp)!=0):false;
        //! 测试优先级
        pszTemp = xmlNode->Attribute("priority");
        suiteItemTestConfig.mstrPriority = pszTemp?pszTemp:"0";
        //! 测试组名称
        pszTemp = xmlNode->Attribute("groupname");
        suiteItemTestConfig.mstrGroupName = pszTemp?pszTemp:"default";

        s_suite_single_item_test_config s_single_item;
        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->Value();
            //s_single_item.mParamName = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamName = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("paramVal");
            s_single_item.mParamValue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("description");
            s_single_item.mParamDescription = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            suiteItemTestConfig.m_mTestParam.push_back(s_single_item);
        }
        //vsuitetTestConfig.push_back(ptrConfig);
    }

    void CTSE_SequenceTestConfig::PareseItemNode(const TiXmlElement* xmlNode,_vsuitetestconfig& vsuitetTestConfig)
    {
        if (!xmlNode) return;
        const char* pszTemp = NULL;
        const TiXmlElement*	pXmlTemp = xmlNode->FirstChildElement();
        //! 是否测试
        pszTemp = xmlNode->Attribute("IsTest");
        if ( pszTemp && 0 == atoi(pszTemp) ) return;

        std::shared_ptr<s_suite_item_test_config> ptrConfig(new s_suite_item_test_config);
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
            //s_single_item.mParamName = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamName = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("paramVal");
            //s_single_item.mParamValue = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamValue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Attribute("description");
            //s_single_item.mParamDescription = pszTemp?a2w_a(W(pszTemp)):L"";
            s_single_item.mParamDescription = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            ptrConfig->m_mTestParam.push_back(s_single_item);
        }
        vsuitetTestConfig.push_back(ptrConfig);
    }
}