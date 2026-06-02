#pragma once
/***************************************************************
 * Name:      ctf_parese_suite_assembly_config.h
 * Purpose:   解析序列接口配置文件
 * Author:    zhaowc (@zhaowc)
 * Created:   2016-09-2
 * Copyright: ctf
 * License:
 **************************************************************/
#include <tinyxml/tinyxml.h>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageImpl.h>
#include <map>
namespace JGW
{
	class CTSE_GlobalEnvironmentConfig
	{
	public:
		CTSE_GlobalEnvironmentConfig(CTSE_LogListener* pLogListener);
		~CTSE_GlobalEnvironmentConfig(void);
    public:
        bool LoadGlobalEnvironmentConfigXml(std::wstring strGlobalEnvironmentConfigXmlPath,std::map<std::wstring,std::wstring>& mapNameValue);
	private:
		//! 解析Common节点参数
		void PareseCommonNode(const TiXmlElement* xmlRoot,std::map<std::wstring,std::wstring>& mapNameValue);
    private:
        CTSE_LogListener* mpLogListener;
        std::wstring mstrGlobalEnvironmentConfigXmlPath;
	};
}


