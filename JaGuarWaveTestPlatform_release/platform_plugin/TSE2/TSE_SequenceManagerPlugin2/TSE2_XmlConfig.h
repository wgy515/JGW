#pragma once
#include "TSE2_SequenceManagerPluginDefine.h"
#include <tinyxml/tinyxml.h>
#include <TSE_SequenceManagerPlugin/TSE_LogServicesImpl.h>

namespace JGW
{
    class CTSE2_XmlConfig
    {
    public:
        CTSE2_XmlConfig(CTSE_LogServicesImpl* pLogServicesImpl);
        ~CTSE2_XmlConfig(void);
    public:
        bool LoadXmlConfig(const std::string& strXmlConfig,S_TSE2_STRUCT& sTse2Struct);
    private:
        bool LoadGlobalEnvironmentConfigXml(S_TSE2_STRUCT& sTse2Struct);
        void ParseRootTestNode(TiXmlElement* rootTestElement,S_ROOT_TEST_STRUCT& sRootTse2Struct);
        void ParseGlobalVariableNode(TiXmlElement* xmlRoot,S_TSE2_STRUCT& sTse2Struct,std::wstring& strEnvConfigFilePath);
    private:
        CTSE_LogServicesImpl* mpLogServicesImpl;
        std::wstring mstrGlobalEnvironmentConfigXmlPath;
    };

}

