#include "StdAfx.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include <log4cplus/fileappender.h> 
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

#include "JGW_Log4cplus.h"

namespace JGW
{
	JGW_Log4cplus::JGW_Log4cplus( const char* tag )
		:mInited(false)
		,mLoggerPropertiesClass( tag)
		,mstr_Tag( tag )
	{
		mpsLoggerProperties = mLoggerPropertiesClass.GetLoggerPropertiesToTag();
		_ASSERT(mpsLoggerProperties);
		//log4cplus::helpers::setLocal( std::locale(mpsLoggerProperties->mstrLocal.c_str()) );
		mstrLoggerName = JGW_A2W(mstr_Tag);
	}


	JGW_Log4cplus::~JGW_Log4cplus(void)
	{
	}

	bool JGW_Log4cplus::InitLog4cplus( )
	{
		//! 如果存在当前模块则直接返回true
		if ( mInited ) return true;
		mLogger = log4cplus::Logger::getInstance( mstrLoggerName.c_str() ); 
// 		if( mLogger.getAllAppenders().size() >= 1 )
// 		{
// 			mInited = true;
// 			return true;
// 		}

		if( !InitLog4cplusFilePath() )
		{
			return false;
		}

		InitLog4cplusProperties();

		mLoggerPropertiesClass.SaveLog4cplusProperties();
#if 0
		//log4cplus::RollingFileAppender
		log4cplus::SharedAppenderPtr pFileAppender(new log4cplus::RollingFileAppender(mstr_Path.c_str(), 10*1024*1024, 30, true));
		//! Layout
		std::auto_ptr<log4cplus::Layout> pPatternLayout(new log4cplus::PatternLayout(LOG4CPLUS_TEXT("%D [%5t] %-5p - %m%n"))); 
		pFileAppender->setLayout(pPatternLayout); 
		//! filter
		log4cplus::helpers::Properties pro;
		pro.setProperty(LOG4CPLUS_TEXT("LogLevelMin"),LOG4CPLUS_TEXT("DEBUG"));
		pro.setProperty(LOG4CPLUS_TEXT("LogLevelMax"),LOG4CPLUS_TEXT("FATAL"));
		pro.setProperty(LOG4CPLUS_TEXT("AcceptOnMatch"),LOG4CPLUS_TEXT("true"));
		log4cplus::spi::FilterPtr pFilter(new log4cplus::spi::LogLevelRangeFilter(pro));
		pFileAppender->setFilter(pFilter);
		//appender
		//Logger::get
		mLogger.addAppender(pFileAppender);
		//mLogger.setLogLevel(INFO_LOG_LEVEL);
#endif
		
		return mInited = true;
	}

	void JGW_Log4cplus::InitLog4cplusProperties()
	{
		//! local
		std::locale loc( mpsLoggerProperties->mstrLocal.c_str() );
		log4cplus::helpers::Properties proRollingFileAppender,proPatternLayout,proLogLevelRangeFilter;

		//! RollingFileAppender
		mpsLoggerProperties->m_mRollingFileAppender[L"File"] = mstr_Path;
		for ( auto it = mpsLoggerProperties->m_mRollingFileAppender.begin();
			it != mpsLoggerProperties->m_mRollingFileAppender.end();
			++it )
		{
			proRollingFileAppender.setProperty( it->first.c_str(),it->second.c_str() );
		}

		log4cplus::SharedAppenderPtr pFileAppender( new log4cplus::RollingFileAppender(proRollingFileAppender) );

		//! PatternLayout
		for ( auto it = mpsLoggerProperties->m_mPatternLayout.begin();
			it != mpsLoggerProperties->m_mPatternLayout.end();
			++it )
		{
			proPatternLayout.setProperty( it->first.c_str(),it->second.c_str() );
		}
        proPatternLayout.setProperty(L"Locale",JGW_A2W(mpsLoggerProperties->mstrLocal));
		std::auto_ptr<log4cplus::Layout> pPatternLayout( new log4cplus::PatternLayout(proPatternLayout) ); 
		pFileAppender->setLayout(pPatternLayout); 

		//! LogLevelRangeFilter
		for ( auto it = mpsLoggerProperties->m_mLogLevelRangeFilter.begin();
			it != mpsLoggerProperties->m_mLogLevelRangeFilter.end();
			++it )
		{
			proLogLevelRangeFilter.setProperty( it->first.c_str(),it->second.c_str() );
		}
		log4cplus::spi::FilterPtr pFilter( new log4cplus::spi::LogLevelRangeFilter(proLogLevelRangeFilter) );

		
		//pFileAppender->imbue(loc);
		pFileAppender->setFilter( pFilter );

		mLogger.addAppender(pFileAppender);
	}

	bool JGW_Log4cplus::InitLog4cplusFilePath()
	{
		time_t timeCur;
		struct tm tmTmp ;
		time(&timeCur);
		localtime_s(&tmTmp,&timeCur);

		JGW_GetExecSubFolder(mstr_Path,L"log",NULL);
		JGW_FormatWString(mstr_Path,L"%s\\%s\\%4d-%02d-%02d\\%s.log",mstr_Path.c_str(),mstrLoggerName.c_str(),tmTmp.tm_year + 1900,tmTmp.tm_mon+1,tmTmp.tm_mday ,mstrLoggerName.c_str());

		if( !JGW_CreateDirectory( mstr_Path.c_str(),false ) )
		{
			return false;
		}

		return true;
	}

	void JGW_Log4cplus::WriteLog4cplus(Log4Level log_type,const wchar_t* log_buf)
	{
		if( !mInited || !log_buf ) return;

		switch(log_type)
		{
		case Log4Level_Verbose:
			LOG4CPLUS_TRACE_STR(mLogger, log_buf);
			break;
		case Log4Level_Debug:
			LOG4CPLUS_DEBUG_STR(mLogger, log_buf);
			break;
		case Log4Level_Info:
			LOG4CPLUS_INFO_STR(mLogger, log_buf);
			break;
		case Log4Level_Warning:
			LOG4CPLUS_WARN_STR(mLogger, log_buf);
			break;
		case Log4Level_Error:
			LOG4CPLUS_ERROR_STR(mLogger, log_buf);
			break;
		case Log4Level_Fatal:
			LOG4CPLUS_FATAL_STR(mLogger,log_buf);
		}
	}
}

