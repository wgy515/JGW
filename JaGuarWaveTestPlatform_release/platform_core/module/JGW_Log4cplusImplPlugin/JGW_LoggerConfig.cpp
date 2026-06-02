#include "StdAfx.h"
#include "JGW_LoggerConfig.h"

namespace JGW
{
	CCJGW_LoggerConfig::CCJGW_LoggerConfig(void)
	{
	}


	CCJGW_LoggerConfig::~CCJGW_LoggerConfig(void)
	{
	}

	bool CCJGW_LoggerConfig::LoadLoggerConfig( const char* strLoggerPath,S_LOGGER_PROPERTIES& sLoggerProperties )
	{
		std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
		TiXmlElement* pRoot,*pLoggerNode,*pPluginNode = NULL;
		sLoggerProperties.m_mLogLevelRangeFilter.clear();
		sLoggerProperties.m_mPatternLayout.clear();
		sLoggerProperties.m_mRollingFileAppender.clear();

		if ( !ptrXmlDoc->LoadFile(strLoggerPath) )
		{
			return false;
		}
		pRoot = ptrXmlDoc->RootElement();
		if ( !pRoot || ( pLoggerNode = pRoot->FirstChildElement("logger") , !PareseLoggerNode(pLoggerNode,&sLoggerProperties)) )
		{
			return false;
		}
		return true;
	}

	bool CCJGW_LoggerConfig::PareseLoggerNode( TiXmlElement* pLoggerNode,LPS_LOGGER_PROPERTIES psLoggerProperties )
	{
		TiXmlElement* pRollingFileAppenderNode = NULL,*pPatternLayout = NULL,*pLogLevelRangeFilterNode = NULL,*pPropertiesNode = NULL;
		std::wstring name,value;
		const char* pszTemp = NULL;

		//! local
		pszTemp = pLoggerNode->Attribute( "local" );
		psLoggerProperties->mstrLocal = pszTemp?pszTemp:"";
		
		//! RollingFileAppender
		{
			pRollingFileAppenderNode = pLoggerNode->FirstChildElement( "RollingFileAppender" );
			if ( !pRollingFileAppenderNode ) return false;

			pPropertiesNode = pRollingFileAppenderNode->FirstChildElement( "properties" );
			while ( pPropertiesNode )
			{
				GetElementAttributeValue(pPropertiesNode,"name",name);
				GetElementAttributeValue(pPropertiesNode,"value",value);
				psLoggerProperties->m_mRollingFileAppender[name] = value;
				pPropertiesNode = pPropertiesNode->NextSiblingElement();
			}
		}

		//! PatternLayout
		{
			pPatternLayout = pLoggerNode->FirstChildElement( "PatternLayout" );
			if ( !pPatternLayout ) return false;

			pPropertiesNode = pPatternLayout->FirstChildElement( "properties" );
			while ( pPropertiesNode )
			{
				GetElementAttributeValue(pPropertiesNode,"name",name);
				GetElementAttributeValue(pPropertiesNode,"value",value);
				psLoggerProperties->m_mPatternLayout[name] = value;
				pPropertiesNode = pPropertiesNode->NextSiblingElement();
			}
		}

		//! LogLevelRangeFilter
		{
			pLogLevelRangeFilterNode = pLoggerNode->FirstChildElement( "LogLevelRangeFilter" );
			if ( !pLogLevelRangeFilterNode ) return false;

			pPropertiesNode = pLogLevelRangeFilterNode->FirstChildElement( "properties" );
			while ( pPropertiesNode )
			{
				GetElementAttributeValue(pPropertiesNode,"name",name);
				GetElementAttributeValue(pPropertiesNode,"value",value);
				psLoggerProperties->m_mLogLevelRangeFilter[name] = value;
				pPropertiesNode = pPropertiesNode->NextSiblingElement();
			}
		}

		return true;
	}
}

