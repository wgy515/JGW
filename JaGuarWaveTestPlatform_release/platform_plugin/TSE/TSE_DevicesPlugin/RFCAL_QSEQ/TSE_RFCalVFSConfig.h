#pragma once
#include <tinyxml/tinyxml.h>
#include <vector>
#include "TSE_RFCalVFS_Define.h"

namespace JGW
{
	class CTSE_RFCalVFSConfig
	{
	public:
		CTSE_RFCalVFSConfig(std::vector<S_QCOMM_FRCALVFS_CONFIG>& vsQcommRFCalVFSConfig);
		~CTSE_RFCalVFSConfig(void);
		bool PareseXmlToXttConfig(const std::string strXmlFilePath);
	private:
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
		bool PareseSingleProjectNode(const TiXmlElement* xmlNode);
		bool PareseParametersNode(SParameters_STRUCT& sParameter,const TiXmlElement* pNode);
		std::vector<S_QCOMM_FRCALVFS_CONFIG>& mvsQcommRFCalVFSConfig;
	};
}
