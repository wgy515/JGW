#include "StdAfx.h"
#include <string>
#include <fstream>

#include "JGW_LoggerPropertier.h"
#include "JGW_LoggerConfig.h"

#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

//extern std::map<std::string,S_LOGGER_PROPERTIES*> g_sLoggerPropertiesMap;
namespace JGW
{
	CCJGW_LoggerPropertier::CCJGW_LoggerPropertier( const char* tag ):mszTag(tag)
	{
	}


	CCJGW_LoggerPropertier::~CCJGW_LoggerPropertier(void)
	{
	}

	LPS_LOGGER_PROPERTIES CCJGW_LoggerPropertier::GetLoggerPropertiesToTag()
	{
		CCJGW_LoggerConfig loggerConfig;
		std::string strLoggerPath = GetLoggerPropertiesPath();

		if ( !loggerConfig.LoadLoggerConfig( strLoggerPath.c_str(),msLoggerProperties ) )
		{
			GetDefaultLoggerProerties();
		}
		return &msLoggerProperties;
	}

	std::string CCJGW_LoggerPropertier::GetLoggerPropertiesPath()
	{
		std::wstring strLoggerName;

		JGW::JGW_GetExecSubFolder(mwstrLoggerPropertiesPath,L"log",NULL);
		strLoggerName = JGW::JGW_A2W_A_C( mszTag );
		mwstrLoggerPropertiesPath += strLoggerName;
		mwstrLoggerPropertiesPath += L"\\";
		mwstrLoggerPropertiesPath += PropertiesXmlName;
		mstrLoggerPropertiesPath = JGW::JGW_W2A(mwstrLoggerPropertiesPath);

		return mstrLoggerPropertiesPath;
	}

	LPS_LOGGER_PROPERTIES CCJGW_LoggerPropertier::GetDefaultLoggerProerties()
	{
		msLoggerProperties.m_mLogLevelRangeFilter.clear();
		msLoggerProperties.m_mPatternLayout.clear();
		msLoggerProperties.m_mRollingFileAppender.clear();
		//! LogLevelRangeFilter
		msLoggerProperties.m_mLogLevelRangeFilter[L"LogLevelMin"] = L"INFO";
		msLoggerProperties.m_mLogLevelRangeFilter[L"LogLevelMax"] = L"FATAL";
		//! PatternLayout
		msLoggerProperties.m_mPatternLayout[L"ConversionPattern"] = L"%D [%5t] %-5p - %m%n";
		//! RollingFileAppender
		msLoggerProperties.m_mRollingFileAppender[L"MaxFileSize"] = L"10MB";
		msLoggerProperties.m_mRollingFileAppender[L"MaxBackupIndex"] = L"10";
		msLoggerProperties.m_mRollingFileAppender[L"File"] = L"";
		//! local
		msLoggerProperties.mstrLocal = "";

		return &msLoggerProperties;
	}
	//! UniFile<<UniFile.narrow(0xFF);               //设置Unicode文本文件标志.这两个是窄字符
	//! UniFile<<UniFile.narrow(0xFE);
	void CCJGW_LoggerPropertier::SaveLog4cplusProperties()
	{
		std::wstring strLog4PropertiesPath,strLoggerName;

		JGW::JGW_GetExecSubFolder(strLog4PropertiesPath,L"log",NULL);
		strLoggerName = JGW::JGW_A2W_A(mszTag);
		strLog4PropertiesPath += strLoggerName;
		strLog4PropertiesPath += L"\\";
		strLog4PropertiesPath += PropertiesXmlName;

		std::wofstream properties;

		//properties.open( strLog4PropertiesPath.c_str(),std::ios_base::out | std::ios_base::_Noreplace );
		properties.open( strLog4PropertiesPath.c_str(),std::ios_base::out | std::ios_base::trunc );
		if ( properties.is_open() )
		{
			properties.imbue(std::locale( "", std::locale::all ^ std::locale::numeric));
			properties << L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
			properties << L"<!--" << std::endl;
			properties << L"MaxFileSize:MB KB" << std::endl;
			properties << L"LogLevel:ALL(0),TRACE(0),DEBUG(10000),INFO(20000),WARN(30000),ERROR(40000),FATAL(50000),OFF(60000),NOTSET,UNKNOWN" << std::endl;
			properties << L"-->" << std::endl 
				<< L"<zqt>" << std::endl 
				<< L"	<logger local=\"" << msLoggerProperties.mstrLocal.c_str() << L"\">" << std::endl;

			//! RollingFileAppender
			{
				properties << L"		<RollingFileAppender>" << std::endl;
				for ( auto it = msLoggerProperties.m_mRollingFileAppender.begin();
					it != msLoggerProperties.m_mRollingFileAppender.end();
					++it )
				{
					properties << L"			<properties name=\"" << it->first.c_str() << L"\" value=\"" << it->second.c_str() 
						<< L"\" />" << std::endl;
				}
				properties << L"		</RollingFileAppender>" << std::endl;
			}

			//! PatternLayout
			{
				properties << L"		<PatternLayout>" << std::endl;
				for ( auto it = msLoggerProperties.m_mPatternLayout.begin();
					it != msLoggerProperties.m_mPatternLayout.end();
					++it )
				{
					properties << L"			<properties name=\"" << it->first.c_str() << L"\" value=\"" << it->second.c_str() 
						<< L"\" />" << std::endl;
				}
				properties << L"		</PatternLayout>" << std::endl;
			}

			//! LogLevelRangeFilter
			{
				properties << L"		<LogLevelRangeFilter>" << std::endl;
				for ( auto it = msLoggerProperties.m_mLogLevelRangeFilter.begin();
					it != msLoggerProperties.m_mLogLevelRangeFilter.end();
					++it )
				{
					properties << L"			<properties name=\"" << it->first.c_str() << L"\" value=\"" << it->second.c_str() 
						<< L"\" />" << std::endl;
				}
				properties << L"		</LogLevelRangeFilter>" << std::endl;
			}

			properties << L"	</logger>" << std::endl;
			properties << L"</zqt>" << std::endl;
			properties.close();
		}
	}
}
