#include "StdAfx.h"
#include "CJGW_TestSuiteAssemblyConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>

namespace JGW
{
	CCJGW_TestSuiteAssemblyConfig::CCJGW_TestSuiteAssemblyConfig(s_suite_test_config& suitetestconfig):m_sSuiteTestConfig(suitetestconfig)
	{
	}


	CCJGW_TestSuiteAssemblyConfig::~CCJGW_TestSuiteAssemblyConfig(void)
	{
	}

	bool CCJGW_TestSuiteAssemblyConfig::PareseAssemblyConfigToBuffer(const std::string& strXMLBuffer)
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* xmlRoot = NULL;
		m_sSuiteTestConfig.m_mCommonTestParam.clear();
		m_sSuiteTestConfig.m_vSuiteTestConfig.clear();

		if ( !ptrXmlDoc->LoadFileBuffer(strXMLBuffer) )
		{
            PELOG4WW(L"没有找到XML配置文件");
            return false;
		}

		if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
		{
			PELOG4WW(L"解析XML文件失败");
            return false;
		}	

		return true;
	}

	bool CCJGW_TestSuiteAssemblyConfig::PareseAssemblyConfig(const std::string& strConfigXml)
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* xmlRoot = NULL;
		m_sSuiteTestConfig.m_mCommonTestParam.clear();
		m_sSuiteTestConfig.m_vSuiteTestConfig.clear();

		if ( !ptrXmlDoc->LoadFile( strConfigXml.c_str() ) )
		{
            PELOG4WA_F("Config Not Exist(XML:%s)",strConfigXml.c_str());
            return false;
		}

		if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
		{
            PELOG4WA_F("解析配置文件失败!!!(XML:%s)",strConfigXml.c_str());
            return false;
		}	

		return true;
	}

	bool CCJGW_TestSuiteAssemblyConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot)
	{
		PareseCommonNode(xmlRoot);

		const TiXmlElement*	pXmlTemp = NULL;
		const TiXmlElement*	pTempXmlNode = NULL;
		pTempXmlNode = xmlRoot->FirstChildElement("SuiteProject");
		if ( !pTempXmlNode )
        {    
            PELOG4WW(L"当前配置文件无测试项目");
            return false;
        }

		while ( pTempXmlNode )
		{
			PareseItemNode(pTempXmlNode);
			pTempXmlNode = pTempXmlNode->NextSiblingElement();
		}

		return true;
	}

	void CCJGW_TestSuiteAssemblyConfig::PareseCommonNode(const TiXmlElement* xmlNode)
	{
		const char* pszTemp = NULL;
		const TiXmlElement*	pXmlTemp = NULL;
		const TiXmlElement*	pTempXmlNode = NULL;

		pTempXmlNode = xmlNode->FirstChildElement("common");
		if ( !pTempXmlNode )
		{
			return ;
		}
		pXmlTemp = pTempXmlNode->FirstChildElement();
		std::wstring strkey,strvalue;

		while ( pXmlTemp )
		{
			pszTemp = pXmlTemp->GetText();
			strvalue = pszTemp?a2w_a(W(pszTemp)):L"";

			pszTemp = pXmlTemp->Value();
			strkey = pszTemp?a2w_a(W(pszTemp)):L"";

			pXmlTemp = pXmlTemp->NextSiblingElement();

			m_sSuiteTestConfig.m_mCommonTestParam.insert(std::make_pair<std::wstring,std::wstring>(strkey,strvalue));
		}

		return ;
	}

	void CCJGW_TestSuiteAssemblyConfig::PareseItemNode(const TiXmlElement* xmlNode)
	{
		const char* pszTemp = NULL;
		const TiXmlElement*	pXmlTemp = xmlNode->FirstChildElement();
		//! 是否测试
		pszTemp = xmlNode->Attribute("IsTest");
		if ( pszTemp && 0 == atoi(pszTemp) ) return;


		std::shared_ptr<s_suite_item_test_config> ptrConfig(new s_suite_item_test_config);
		//! 结束测试项目
		pszTemp = xmlNode->Attribute("IsFinalize");
		ptrConfig->m_bFinalizeTest = pszTemp?(atoi(pszTemp)!=0):false;

		s_suite_single_item_test_config s_single_item;

		while ( pXmlTemp )
		{
			pszTemp = pXmlTemp->Value();
			s_single_item.mParamName = pszTemp?a2w_a(W(pszTemp)):L"";

			pszTemp = pXmlTemp->Attribute("paramVal");
			s_single_item.mParamValue = pszTemp?a2w_a(W(pszTemp)):L"";

			pszTemp = pXmlTemp->Attribute("description");
			s_single_item.mParamDescription = pszTemp?a2w_a(W(pszTemp)):L"";

			pXmlTemp = pXmlTemp->NextSiblingElement();

			ptrConfig->m_mTestParam.push_back(s_single_item);
		}
		m_sSuiteTestConfig.m_vSuiteTestConfig.push_back(ptrConfig);
		return ;
	}
}
