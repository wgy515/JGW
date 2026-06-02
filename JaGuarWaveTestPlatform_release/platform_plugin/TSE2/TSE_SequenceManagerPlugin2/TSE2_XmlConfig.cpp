#include "StdAfx.h"
#include "TSE2_XmlConfig.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE2_XmlConfig::CTSE2_XmlConfig(CTSE_LogServicesImpl* pLogServicesImpl) : mpLogServicesImpl(pLogServicesImpl)
    {
    }


    CTSE2_XmlConfig::~CTSE2_XmlConfig(void)
    {
    }

    bool CTSE2_XmlConfig::LoadXmlConfig(const std::string& strXmlConfig,S_TSE2_STRUCT& sTse2Struct)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        if (!ptrXmlDoc->LoadFile(strXmlConfig.c_str()))
        {
            mpLogServicesImpl->ErrorLogFormat("load tse2 xml config(%s) fail",strXmlConfig.c_str());
            return false;
        }

        TiXmlElement* xmlRoot = ptrXmlDoc->RootElement();
        if (!xmlRoot)
        {
            mpLogServicesImpl->ErrorLogFormat("tse2 xml config format fail");
            return false;
        }
        //! load GlobalVariable
        ParseGlobalVariableNode(xmlRoot,sTse2Struct,mstrGlobalEnvironmentConfigXmlPath);

        TiXmlElement* pRootTestElement = xmlRoot->FirstChildElement("RootTest");
        if (NULL == pRootTestElement)
        {
            mpLogServicesImpl->ErrorLogFormat("tse2 xml config format fail");
            return false;
        }
        ParseRootTestNode(pRootTestElement,sTse2Struct.msRootTestStruct);

        TiXmlElement* pUIConfigTestElement = xmlRoot->FirstChildElement("UIConfigTest");
        if (NULL != pUIConfigTestElement)
        {
            ParseRootTestNode(pUIConfigTestElement,sTse2Struct.msUiConfigTestStruct);
        }

        TiXmlElement* pAutoTestElement = xmlRoot->FirstChildElement("AutoTest");
        if (NULL != pAutoTestElement)
        {
            ParseRootTestNode(pAutoTestElement,sTse2Struct.msAutoTestStruct);
        }

        if (!LoadGlobalEnvironmentConfigXml(sTse2Struct))
        {
            mpLogServicesImpl->ErrorLogFormat(GetString(E_XML_FAILED_PARSE_CONFIG),mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }

        return true;
    }

    void CTSE2_XmlConfig::ParseRootTestNode(TiXmlElement* rootTestElement,S_ROOT_TEST_STRUCT& sRootTse2Struct)
    {

    }

    bool CTSE2_XmlConfig::LoadGlobalEnvironmentConfigXml(S_TSE2_STRUCT& sTse2Struct)
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
            mpLogServicesImpl->ErrorLogFormat(L"Global Environment Config Load Error(XML:%s)\n",mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }

        xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            mpLogServicesImpl->ErrorLogFormat(GetString(E_XML_FAILED_PARSE_CONFIG),mstrGlobalEnvironmentConfigXmlPath.c_str());
            return false;
        }	
        ParseGlobalVariableNode(xmlRoot,sTse2Struct,mstrGlobalEnvironmentConfigXmlPath);

        return true;
    }

    void CTSE2_XmlConfig::ParseGlobalVariableNode(TiXmlElement* xmlRoot,S_TSE2_STRUCT& sTse2Struct,std::wstring& strEnvConfigFilePath)
    {
        const TiXmlElement*	pXmlTemp = NULL;
        const char* pszTemp = NULL;
        std::wstring strkey,strvalue;
        TiXmlElement* pGlobalVariables = xmlRoot->FirstChildElement("GlobalVariables");

        if ( !pGlobalVariables ) return ;
        pXmlTemp = pGlobalVariables->FirstChildElement();

        while (pXmlTemp)
        {
            pszTemp = pXmlTemp->GetText();
            //strvalue = pszTemp?a2w_a(W(pszTemp)):L"";
            strvalue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pszTemp = pXmlTemp->Value();
            //strkey = pszTemp?a2w_a(W(pszTemp)):L"";
            strkey = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

            pXmlTemp = pXmlTemp->NextSiblingElement();

            if (0 == JGW_WStrComparenoCaseWStr(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",strkey.c_str()))
            {
                strEnvConfigFilePath = JGW_RealativePathToAbsPath(strvalue.c_str());
                //! mstrGlobalEnvironmentConfigXmlPath = JGW_RealativePathToAbsPath(strvalue.c_str());
                strvalue = strEnvConfigFilePath;
                //_mcommontestconfig::iterator it = m_sSuiteTestConfig.m_mCommonTestParam.find(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH");
                //if (it != m_sSuiteTestConfig.m_mCommonTestParam.end())
                //{
                //    it->second = mstrGlobalEnvironmentConfigXmlPath;
                //}
            }
            std::map<std::wstring,std::wstring>::iterator it = sTse2Struct.mmapGlobalVariables.find(strkey);
            if (it != sTse2Struct.mmapGlobalVariables.end())
            {
                it->second = strvalue;
            }
            else
            {
                sTse2Struct.mmapGlobalVariables.insert(std::make_pair<std::wstring,std::wstring>(strkey,strvalue));  
            }
        }
    }
}
