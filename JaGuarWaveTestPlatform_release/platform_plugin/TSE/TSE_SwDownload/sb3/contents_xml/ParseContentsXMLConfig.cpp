#include "stdafx.h"
#include "ParseContentsXMLConfig.h"
#include "ParsePartitionXMLConfig.h"
#include <JGW_FoundationFunc/JGW_Exception.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
#define IsNull(x,result)\
	if( !(x) )\
	return result;

	CParseContentsXMLConfig::CParseContentsXMLConfig(void):mpSPartitionFileInfo(NULL)
	{
	}


	CParseContentsXMLConfig::~CParseContentsXMLConfig(void)
	{
		if ( mpSPartitionFileInfo )
		{
			delete mpSPartitionFileInfo;
			mpSPartitionFileInfo = NULL;
		}
	}

	std::string CParseContentsXMLConfig::GetDownloadFilePathToFileName( const char* szFileName )
	{
		for ( auto itdownload = mmapDownloadFiles.begin();
			itdownload != mmapDownloadFiles.end();
			++ itdownload )
		{
			if ( NULL ==  JGW_StrComparenoCaseStr(itdownload->first.c_str(),szFileName) )
			{
				return itdownload->second;
			}
		}

		return "";
	}

	bool CParseContentsXMLConfig::ParseContentsXml( const std::string strXmlFilePath )
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* xmlRoot = NULL;
		mstrFolder = JGW_GetFileFolderToFilePathA ( strXmlFilePath.c_str() );


		if ( !ptrXmlDoc->LoadFile( strXmlFilePath.c_str() ) )
		{
			throw CJGW_Exception( L"没有找到配置XML文件\n" );
		}

		if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseAllTestProjectNode(xmlRoot) )
		{
			throw CJGW_Exception( L"解析XML文件失败\n" );
		}	

		return true;
	}

	bool CParseContentsXMLConfig::PareseAllTestProjectNode(const TiXmlElement* xmlRoot)
	{
		const TiXmlElement*	pTempXmlNode = NULL;
		mmapDownloadFiles.clear();
		if ( mpSPartitionFileInfo ) { delete mpSPartitionFileInfo;mpSPartitionFileInfo = NULL; }
		pTempXmlNode = xmlRoot->FirstChildElement("builds_flat");
		IsNull(pTempXmlNode,false);
		pTempXmlNode = pTempXmlNode->FirstChildElement("build");
		IsNull(pTempXmlNode,false);

		while ( pTempXmlNode )
		{
			PareseBuildNode(pTempXmlNode);
			pTempXmlNode = pTempXmlNode->NextSiblingElement();
		}

		return true;
	}

	void CParseContentsXMLConfig::PareseBuildNode( const TiXmlElement* pBuildNode )
	{
		const TiXmlElement*	pTempXmlNode = NULL;

		{
			std::string strFileName,strFilePath;
            std::wstring strTemp;
			pTempXmlNode = pBuildNode->FirstChildElement("download_file");
			while ( pTempXmlNode )
			{
				GetElementTextValue( pTempXmlNode,"file_name",strTemp );
                strFileName = JGW_W2A(strTemp,CP_UTF8);
				GetElementTextValue( pTempXmlNode,"file_path",strTemp );
                strFilePath = JGW_W2A(strTemp,CP_UTF8);
				mmapDownloadFiles.insert( std::map<std::string,std::string>::value_type( strFileName,strFilePath) );
				pTempXmlNode = pTempXmlNode->NextSiblingElement();
			}
		}
		if ( mpSPartitionFileInfo ) return ;

		{
			const TiXmlElement*	pPartition_fileNode = NULL;
            std::wstring strTemp;
			pPartition_fileNode = pBuildNode->FirstChildElement("partition_file");
			if ( pPartition_fileNode )
			{
				mpSPartitionFileInfo = new S_CONTENTS_FILES;
				GetElementTextValue(pPartition_fileNode,"file_name",strTemp );
                mpSPartitionFileInfo->mstrFileName = JGW_W2A(strTemp,CP_UTF8);
				GetElementTextValue(pPartition_fileNode,"file_path",strTemp);
                mpSPartitionFileInfo->mstrFilePath = JGW_W2A(strTemp,CP_UTF8);
			}
		}
	}

	bool CParseContentsXMLConfig::GetPartitionInfo( std::vector<S_PARTITION_INFO>& vPartitionInfo )
	{
		if ( !mpSPartitionFileInfo ) return false;

		std::string strImagePath;
		std::string strPartitionFilePath = mstrFolder;
		bool bOk = true;
		strPartitionFilePath += mpSPartitionFileInfo->mstrFilePath;
		strPartitionFilePath += mpSPartitionFileInfo->mstrFileName;

		CParsePartitionXMLConfig parse_partitionxml ( vPartitionInfo );

		parse_partitionxml.ParesePartitionXmlConfig( strPartitionFilePath );

		for ( auto it = vPartitionInfo.begin();
			it != vPartitionInfo.end();
			++it )
		{
			bOk = false;
			for ( auto itdownload = mmapDownloadFiles.begin();
				itdownload != mmapDownloadFiles.end();
				++ itdownload )
			{
				if ( NULL == it->strPartitionImageName.compare( itdownload->first ) )
				{
					strImagePath = mstrFolder;
					strImagePath += itdownload->second;
					strImagePath += itdownload->first;
					JGW_ReplaceCharA( strImagePath,'/','\\');
					it->strPartitionImageName = strImagePath;
					bOk = true;
					break;
				}
			}
			if ( !bOk ) return false;
		}

		return true;
	}
}
