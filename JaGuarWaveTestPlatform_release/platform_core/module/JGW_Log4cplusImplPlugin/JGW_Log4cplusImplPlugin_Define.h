#pragma once
#ifndef ZQT_LOG4CPLUS_DLL_DEFINE_H__
#define ZQT_LOG4CPLUS_DLL_DEFINE_H__
#include <string>
#include <map>

#define PropertiesXmlName L"logger.xml"

typedef struct  
{
	std::string mstrLocal;
	std::map<std::wstring,std::wstring> m_mRollingFileAppender;
	std::map<std::wstring,std::wstring> m_mPatternLayout;
	std::map<std::wstring,std::wstring> m_mLogLevelRangeFilter;
}S_LOGGER_PROPERTIES,*LPS_LOGGER_PROPERTIES;


#endif