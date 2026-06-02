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
#include <TSE_SequenceManagerPlugin/TSE_Define.h>


namespace JGW
{
	class CTSE_SequenceTestConfig
	{
	public:
		CTSE_SequenceTestConfig(S_TSE_SequenceConfig& suitetestconfig);
		~CTSE_SequenceTestConfig(void);

		bool PareseAssemblyConfig(const std::string& strConfigXml);
	private:
		//! 解析所有测试节点
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
		//! 解析Common节点参数
		void PareseCommonNode(const TiXmlElement* xmlRoot);
		//! 解析单个Item节点
		void PareseItemNode(const TiXmlElement* xmlNode,_vsuitetestconfig& vsuitetTestConfig);
		//! 解析单个Item节点
		void PareseItemNode(const TiXmlElement* xmlNode,s_suite_item_test_config& suiteItemTestConfig);
		//! 解析config节点
		void PareseConfigNode(const TiXmlElement* xmlRoot);
		//! 解析auto
		void PareseAutoNode(const TiXmlElement* xmlRoot);
		//! 解析SuiteProject
		bool PareseSuiteProject(const TiXmlElement* xmlRoot);
	private:
        //!
        bool LoadGlobalEnvironmentConfigXml();

		void PareseChildSuiteProject(const TiXmlElement* pChildSuiteProjectNode,std::vector<std::shared_ptr<S_Test_SuiteProjectConfig>>& vChildTestSuiteProjectConfig);
    private:
        std::string mstrConfigXml;
		S_TSE_SequenceConfig& m_sSuiteTestConfig;
        std::wstring mstrGlobalEnvironmentConfigXmlPath;
	};
}


