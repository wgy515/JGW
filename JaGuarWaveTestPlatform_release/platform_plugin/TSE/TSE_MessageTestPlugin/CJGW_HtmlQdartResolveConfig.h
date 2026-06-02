#pragma once
#include "HtmlQdartResolveDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_HtmlQdartResolveConfig
    {
    public:
        CCJGW_HtmlQdartResolveConfig(void);
        ~CCJGW_HtmlQdartResolveConfig(void);
    public:
        bool LoadHtmlQdartResolveConfig(const std::wstring& strHtmlQdartResolveConfigPath,S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveInfo);
    private:
        bool LoadTestProjectConfig(const std::string& strTestProjectName,S_HTML_QDART_PROJECT_CONFIG_INFO& sHtmlQdartProjectInfo,CCJGW_ConfigIni& configIni);
    };

}

