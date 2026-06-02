#include "StdAfx.h"
#include "TSE_GlobalEnvironmentConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    CTSE_GlobalEnvironmentConfig::CTSE_GlobalEnvironmentConfig(CTSE_LogListener* pLogListener) : mpLogListener(pLogListener)
    {
    }


    CTSE_GlobalEnvironmentConfig::~CTSE_GlobalEnvironmentConfig(void)
    {
    }

    bool CTSE_GlobalEnvironmentConfig::LoadGlobalEnvironmentConfigXml(std::wstring strGlobalEnvironmentConfigXmlPath,std::map<std::wstring,std::wstring>& mapNameValue)
    {
        mapNameValue.clear();
        if (strGlobalEnvironmentConfigXmlPath.empty())
        {
            return true;
        }
        
        if (!JGW_PathFileExists(strGlobalEnvironmentConfigXmlPath.c_str()))
        {
            JGW_CreateDirectory(strGlobalEnvironmentConfigXmlPath.c_str(),false);
            std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
            TiXmlDeclaration *pDec = new TiXmlDeclaration("1.0", "utf-8", "");
            ptrXmlDoc->LinkEndChild(pDec);
            TiXmlElement* pSuiteNode = new TiXmlElement("Suite");
            TiXmlElement* pCommonNode1 = new TiXmlElement("common");
            pSuiteNode->LinkEndChild(pCommonNode1);
            ptrXmlDoc->LinkEndChild(pSuiteNode);
            ptrXmlDoc->SaveFile(JGW_W2A(strGlobalEnvironmentConfigXmlPath).c_str());
            return true;
        }

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strGlobalEnvironmentConfigXmlPath).c_str()))
        {
            mpLogListener->OnErrorLog(JGW_GetFormatWString(L"Global Environment Config Load Error(XML:%s)\n",strGlobalEnvironmentConfigXmlPath.c_str()).c_str());
            return false;
        }

        xmlRoot = ptrXmlDoc->RootElement();
        if (NULL == xmlRoot)
        {
            mpLogListener->OnErrorLog(JGW_GetFormatWString(GetString(E_XML_FAILED_PARSE_CONFIG),strGlobalEnvironmentConfigXmlPath.c_str()).c_str());
            return false;
        }	
        PareseCommonNode(xmlRoot,mapNameValue);
        return true;
    }

    void CTSE_GlobalEnvironmentConfig::PareseCommonNode(const TiXmlElement* xmlNode,std::map<std::wstring,std::wstring>& mapNameValue)
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

            if (0 == JGW_WStrComparenoCaseWStr(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",strkey.c_str()))
            {
                mstrGlobalEnvironmentConfigXmlPath = JGW_RealativePathToAbsPath(strvalue.c_str());
                strvalue = mstrGlobalEnvironmentConfigXmlPath;
                //_mcommontestconfig::iterator it = m_sSuiteTestConfig.m_mCommonTestParam.find(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH");
                //if (it != m_sSuiteTestConfig.m_mCommonTestParam.end())
                //{
                //    it->second = mstrGlobalEnvironmentConfigXmlPath;
                //}
            }
            std::map<std::wstring,std::wstring>::iterator it = mapNameValue.find(strkey);
            if (it != mapNameValue.end())
            {
                it->second = strvalue;
            }
            else
            {
                mapNameValue.insert(std::make_pair<std::wstring,std::wstring>(strkey,strvalue));  
            }
        }

        return ;
    }
}