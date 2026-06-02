#pragma once
#include <tinyxml/tinyxml.h>
#include <map>
#include "..\9x25_download_define.h"

namespace JGW
{
	class CParseContentsXMLConfig
	{
	public:
		CParseContentsXMLConfig(void);
		~CParseContentsXMLConfig(void);

		bool ParseContentsXml( const std::string strXmlFilePath );

		bool GetPartitionInfo( std::vector<S_PARTITION_INFO>& vPartitionInfo  ); 

		std::string GetDownloadFilePathToFileName( const char* szFileName );

		bool CheckAllDownLoadFileIsExist( std::vector<S_PARTITION_INFO>& vPartitionInfo );

	private:
		bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
		void PareseBuildNode( const TiXmlElement* pBuildNode );

	private:
		std::map<std::string,std::string>	mmapDownloadFiles;
		LPS_CONTENTS_FILES					mpSPartitionFileInfo;
		std::string							mstrFolder;
	};
}


