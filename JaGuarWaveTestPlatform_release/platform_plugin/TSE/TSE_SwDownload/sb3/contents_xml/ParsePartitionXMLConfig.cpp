#include "stdafx.h"
#include "ParsePartitionXMLConfig.h"
#include <JGW_FoundationFunc/JGW_Exception.h>

namespace JGW
{
	CParsePartitionXMLConfig::CParsePartitionXMLConfig( std::vector<S_PARTITION_INFO>& vPartitionInfo ):mvPartitionInfo(vPartitionInfo)
	{
	}


	CParsePartitionXMLConfig::~CParsePartitionXMLConfig(void)
	{
	}

	bool CParsePartitionXMLConfig::ParesePartitionXmlConfig( const std::string strXmlFilePath )
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* xmlRoot = NULL;

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

	bool CParsePartitionXMLConfig::PareseAllTestProjectNode( const TiXmlElement* xmlRoot )
	{
		const TiXmlElement *xmlPartitions = NULL,*xmlPartition = NULL;
		S_PARTITION_INFO sPartitionInfo;
        std::wstring strTemp;

		xmlPartitions = xmlRoot->FirstChildElement("partitions");
		if ( !xmlPartitions ) return false;
		xmlPartition = xmlPartitions->FirstChildElement("partition");
		
		while ( xmlPartition )
		{
			GetElementTextValue( xmlPartition,"name",strTemp );
            sPartitionInfo.strPartitionName = JGW_W2A(strTemp,CP_UTF8);
			GetElementTextValue( xmlPartition,"img_name", strTemp);
            sPartitionInfo.strPartitionImageName = JGW_W2A(strTemp,CP_UTF8);
			xmlPartition = xmlPartition->NextSiblingElement();

			if ( sPartitionInfo.strPartitionName.empty() || sPartitionInfo.strPartitionImageName.empty() ) continue;
			mvPartitionInfo.push_back( sPartitionInfo );
		}

		return true;
	}
}

