#pragma once
#include <tinyxml/tinyxml.h>
#include <vector>
#include "CJGW_RFCalVFS_Define.h"

namespace JGW
{
	class CCJGW_RFCalVFSConfig
	{
	public:
		CCJGW_RFCalVFSConfig(void);
		~CCJGW_RFCalVFSConfig(void);
		bool PareseXmlToXttConfig(const std::string strXmlFilePath);
		std::vector<STestProject_STRUCT>& GetTestProject(){return m_vTestProject;}
	private:
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
		bool PareseSingleProjectNode(const TiXmlElement* xmlNode);
		bool PareseParametersNode(SParameters_STRUCT& sParameter,const TiXmlElement* pNode);
		std::vector<STestProject_STRUCT> m_vTestProject;
	};
}
