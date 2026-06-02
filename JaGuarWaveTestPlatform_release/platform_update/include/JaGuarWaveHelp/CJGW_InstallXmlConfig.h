#pragma once
#include <tinyxml/tinyxml.h>
#include "CJGW_HelpUpdateDefine.h"

namespace JGW
{
    class CCJGW_InstallXmlConfig
    {
    public:
        CCJGW_InstallXmlConfig(void);
        ~CCJGW_InstallXmlConfig(void);
    public:
        bool LoadInstallXmlConfig(const std::wstring& strInstallXmlFilePath,S_JGW_HELP_INSTALL_XML_CONFIG& sInstallXmlConfig);
    private:
        void LoadInstallNode(TiXmlElement* pInstallNode,S_JGW_HELP_INSTALL& sInstallInfo);
    };
}