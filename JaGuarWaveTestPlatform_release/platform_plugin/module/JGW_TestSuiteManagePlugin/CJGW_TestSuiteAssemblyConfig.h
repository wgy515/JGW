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
#include "CJGW_TestSuiteManage_Define.h"
namespace JGW
{
	class CCJGW_TestSuiteAssemblyConfig
	{
	public:
		CCJGW_TestSuiteAssemblyConfig(s_suite_test_config& suitetestconfig);
		~CCJGW_TestSuiteAssemblyConfig(void);

		bool PareseAssemblyConfigToBuffer(const std::string& strXMLBuffer);

		bool PareseAssemblyConfig(const std::string& strConfigXml);
	private:
		//! 解析所有测试节点
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
		//! 解析Common节点参数
		void PareseCommonNode(const TiXmlElement* xmlNode);
		//! 解析单个Item节点
		void PareseItemNode(const TiXmlElement* xmlNode);

		s_suite_test_config& m_sSuiteTestConfig;
	};
}


