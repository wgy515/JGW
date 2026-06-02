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
        sTse2Struct.mmapGlobalVariables.insert(std::make_pair<std::wstring,std::wstring>(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH,JGW_A2W(strXmlConfig)));
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

    void CTSE2_XmlConfig::ParseRootTestNode(TiXmlElement* testElement,S_ROOT_TEST_STRUCT& sRootTse2Struct)
    {
        TiXmlElement* pTestElement = testElement->FirstChildElement();
        std::string strElementName;
        while (pTestElement)
        {
            S_TSE2_TEST sTseTest;
            ParseTestNode(pTestElement,sTseTest);
            sRootTse2Struct.msTse2Test.push_back(sTseTest);
            pTestElement = pTestElement->NextSiblingElement();
        }
    }

    void CTSE2_XmlConfig::GetTestNodeParam(TiXmlElement* pTestElement,std::map<std::wstring,std::wstring>& mapParam)
    {
        TiXmlElement* pParamElement = pTestElement->FirstChildElement();
        const char* pszTemp = NULL;
        while (pParamElement)
        {
            pszTemp = pParamElement->Value();
            std::wstring strParamName = pszTemp ? JGW_A2W_A(pszTemp,CP_UTF8) : L"";

            pszTemp = pParamElement->Attribute("paramVal");
            std::wstring strParamValue = pszTemp ? JGW_A2W_A(pszTemp,CP_UTF8) : L"";

            mapParam[strParamName] = strParamValue;
            pParamElement = pParamElement->NextSiblingElement();
        }
    }

    void CTSE2_XmlConfig::ParseTestNode(TiXmlElement* pTestElement,S_TSE2_TEST& sTseTest)
    {
        std::string strElementName = pTestElement->Value();
        std::wstring strTemp;
        const char* pszTemp = NULL;
        std::map<std::wstring,std::wstring> mapParam;
        GetTestNodeParam(pTestElement,mapParam);

        if (0 == JGW_StrComparenoCaseStr(strElementName.c_str(),"FolderTest"))
        {
            sTseTest.mbIsFolderTest = true;
            sTseTest.msFolderTest.mstrModuleName = mapParam[L"ModuleName"];
            sTseTest.msFolderTest.mstrRealName = mapParam[L"RealName"];
            sTseTest.msFolderTest.mstrTestName = mapParam[L"TestName"];
            sTseTest.msFolderTest.mnParamCount = _ttoi(mapParam[L"ParamCount"].c_str());
            sTseTest.msFolderTest.mstrParamHeader = mapParam[L"ParamHeader"];
            sTseTest.msFolderTest.mstrParamHeaderShow = mapParam[L"ParamHeaderShow"];
            sTseTest.msFolderTest.mstrParamHeaderEnvironment = mapParam[L"ParamHeaderEnvionment"];
            //! if (sTseTest.msFolderTest.mstrParamHeaderShow.empty()) sTseTest.msFolderTest.mstrParamHeaderShow = sTseTest.msFolderTest.mstrParamHeader;
            for (size_t i = 0;i < sTseTest.msFolderTest.mnParamCount;i++)
            {
                JGW_FormatWString(strTemp,L"ParamVaule%d",i + 1);
                sTseTest.msFolderTest.mvstrParamValues.push_back(mapParam[strTemp]);
            }
        }
        else
        {
            sTseTest.mbIsFolderTest = false;
            sTseTest.mnTestIndex = 0;
            //sTseTest.msTestBase.mnErrorRetryCount = 1;
            pszTemp = pTestElement->Attribute("IsFinalize");
            sTseTest.msTestBase.mbIsFinalize = pszTemp ? (atoi(pszTemp) != 0) : false;
            //! 
            pszTemp = pTestElement->Attribute("ReversedResult");
            sTseTest.msTestBase.mbReversedResult = pszTemp ? (atoi(pszTemp) != 0) : false;
            //!
            pszTemp = pTestElement->Attribute("IfFalse");
            sTseTest.msTestBase.mbIfFalse = pszTemp ? (atoi(pszTemp) != 0) : false;

            for (std::map<std::wstring,std::wstring>::iterator it = mapParam.begin();it != mapParam.end();++it)
            {
                if (0 == JGW_WStrComparenoCaseWStr(L"ModuleName",it->first.c_str()))
                {
                    sTseTest.msTestBase.mstrModuleName = it->second;
                }
                else if (0 == JGW_WStrComparenoCaseWStr(L"RealName",it->first.c_str()))
                {
                    sTseTest.msTestBase.mstrRealName = it->second;
                }
                else if (0 == JGW_WStrComparenoCaseWStr(L"TestName",it->first.c_str()))
                {
                    sTseTest.msTestBase.mstrTestName = it->second;
                }
                else
                {
                    sTseTest.msTestBase.mmapParam[it->first] = it->second;
                }
            }
        }
        //! 解析子节点
        TiXmlElement* pChildElement = pTestElement->FirstChildElement();
        while (pChildElement)
        {
            pszTemp = pChildElement->Value();
            std::wstring strParamName = pszTemp ? JGW_A2W_A(pszTemp,CP_UTF8) : L"";
            if (0 == JGW_WStrComparenoCaseWStr(strParamName.c_str(),L"FolderTest"))
            {
                S_TSE2_TEST sTseChild;
                ParseTestNode(pChildElement,sTseChild);
                sTseTest.msFolderTest.mvTseTest.push_back(sTseChild);
            }
            else if (0 == JGW_WStrComparenoCaseWStr(strParamName.c_str(),L"Test"))
            {
                S_TSE2_TEST sTseChild;
                const char* pszTemp = NULL;
                //! 是否测试
                pszTemp = pChildElement->Attribute("IsTest");
                if ( pszTemp && 0 == atoi(pszTemp) ) 
                {
                    pChildElement = pChildElement->NextSiblingElement();
                    continue;
                }
                //! 结束测试项目
                //pszTemp = pChildElement->Attribute("IsFinalize");
                //sTseChild.msTestBase.mbIsFinalize = pszTemp ? (atoi(pszTemp) != 0) : false;
                ////! 
                //pszTemp = pChildElement->Attribute("ReversedResult");
                //sTseChild.msTestBase.mbReversedResult = pszTemp ? (atoi(pszTemp) != 0) : false;
                ////!
                //pszTemp = pChildElement->Attribute("IfFalse");
                //sTseChild.msTestBase.mbIfFalse = pszTemp ? (atoi(pszTemp) != 0) : false;

                ParseTestNode(pChildElement,sTseChild);
                if (sTseTest.mbIsFolderTest)
                {
                    sTseTest.msFolderTest.mvTseTest.push_back(sTseChild);
                }
                else
                {
                    sTseTest.msTestBase.mvTseTest.push_back(sTseChild);
                } 
            }
            pChildElement = pChildElement->NextSiblingElement();
        }
        
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
            TiXmlElement* pSuiteNode = new TiXmlElement("JGW");
            TiXmlElement* pCommonNode1 = new TiXmlElement("GlobalVariables");
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

            if (0 == JGW_WStrComparenoCaseWStr(TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH,strkey.c_str()))
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
