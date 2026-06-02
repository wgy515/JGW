#pragma once
#include <tinyxml/tinyxml.h>
#include <UiLib/UIlib.h>

namespace JGW
{
    class CTSE_LoadUIConfig
    {
    public:
        CTSE_LoadUIConfig(void);
        ~CTSE_LoadUIConfig(void);
    public:
        bool LoadUIConfigToXml(const std::string& strUIConfigXmlPath);
        const std::vector<S_PLUGIN_GROUP_INFO>& GetPluginGroupInfos();
    private:
        bool ParesePluginNode( TiXmlElement* pPluginNode );
        bool GetPlnNodeInfo(TiXmlElement* pPlnNode,S_PLUGIN_INFO& sPluginInfo);
    private:
        std::vector<S_PLUGIN_GROUP_INFO> mvsPluginGroupInfo;
    };
}