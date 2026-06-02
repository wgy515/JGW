#pragma once
#include <tinyxml/tinyxml.h>
#include "..\9x25_download_define.h"

namespace JGW
{
	class CParsePartitionXMLConfig
	{
	public:
		CParsePartitionXMLConfig( std::vector<S_PARTITION_INFO>& vPartitionInfo );
		~CParsePartitionXMLConfig(void);

		bool ParesePartitionXmlConfig( const std::string strXmlFilePath );
	private:
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
	private:
		std::vector<S_PARTITION_INFO>& mvPartitionInfo;
	};
}

