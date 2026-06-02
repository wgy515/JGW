#pragma once
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <tinyxml/tinyxml.h>
#include <UiLib/UIlib.h>

namespace JGW
{
	class CCJGWViewLayoutConfig
	{
	public:
		CCJGWViewLayoutConfig(void);
		~CCJGWViewLayoutConfig(void);
        //! 起始页插件信息
        LPS_PLUGIN_INFO GetWelcomPluginInfo();
        //!
        PS_UI_PROJECT_INFO GetUIProjectInfo();
        //! 线程池配置相关信息
        LPS_DEVICE_POOL_MODULE_INFO GetDevicePoolModuleInfo();
        //! 获取MENU EXEC
        const std::map<std::wstring,std::vector<S_EXEC_MENUELEMENT_INFO>>& GetExecMeunInfos();
		//! 加载视图布局配置文件
		bool LoadViewLayoutConfig(std::string& strViewConfigPath);
        //! 保存视图配置
        void SaveViewLayoutConfig(std::string& strViewConfigPath/*,size_t nPrevPluginInfoIndex*/);
	private:
		bool PareseUINode( TiXmlElement* pUINode );
		bool ParesePluginNode( TiXmlElement* pPluginNode );
        bool GetPlnNodeInfo(TiXmlElement* pPlnNode,S_PLUGIN_INFO& sPluginInfo);
	private:
        S_PLUGIN_INFO msWelcomPluginInfo; //! 起始页插件信息
        S_DEVICE_POOL_MODULE_INFO msDevicePoolModuleInfo;//! 线程池配置相关信息
        S_UI_PROJECT_INFO msUIProjectInfo;
        std::map<std::wstring,std::vector<S_EXEC_MENUELEMENT_INFO>> msExecMenuInfos;
	};
}


