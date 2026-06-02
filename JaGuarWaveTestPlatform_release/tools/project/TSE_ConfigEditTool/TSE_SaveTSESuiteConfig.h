#pragma once
#include <string>
#include <UiLib/UiLibDefine.h>
#include <tinyxml/tinyxml.h>

namespace JGW
{
    class CTSE_SaveTSESuiteConfig
    {
    public:
        CTSE_SaveTSESuiteConfig(void);
        ~CTSE_SaveTSESuiteConfig(void);
    public:
        bool SaveTSESuiteConfig(const std::string& strTSEFilePath,CTreeViewUI* mpTreeViewUI);
    private:
        CTreeNodeUI* mpTreeNodeUI;
    };
}