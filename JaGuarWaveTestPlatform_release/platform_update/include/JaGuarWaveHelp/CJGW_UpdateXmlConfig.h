#pragma once
#include <tinyxml/tinyxml.h>
#include "JaGuarWaveHelpDefine.h"

namespace JGW
{
    class CCJGW_UpdateXmlConfig
    {
    public:
        CCJGW_UpdateXmlConfig(void);
        ~CCJGW_UpdateXmlConfig(void);
    public:
        bool LoadUpdateXmlConfig(const std::wstring& strUpdateXmlFilePath,S_JGW_HELP_UPDATE_XML_CONFIG& sUpdateXmlConfig);
    private:
        void LoadPackageNode(TiXmlElement* pPackageNode,S_JGW_HELP_UPDATE_PACKAGE& sUpdatePackage);
    };
}

