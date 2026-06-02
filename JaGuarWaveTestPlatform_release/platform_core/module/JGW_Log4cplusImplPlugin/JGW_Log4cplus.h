#pragma once
#include <log4cplus/logger.h> 
#include <JGW_Log4cplusImplPlugin\JGW_LogDefine.h>
#include "JGW_Log4cplusImplPlugin_Define.h"
#include "JGW_LoggerPropertier.h"

namespace JGW
{
	class JGW_Log4cplus
	{
	public:
		JGW_Log4cplus( const char* tag );
		~JGW_Log4cplus(void);
		//! ≥ı ºªØ
		bool InitLog4cplus( void );
		//! –¥»Î
		void WriteLog4cplus(Log4Level log_type,const wchar_t* log_buf);
	private:
		bool InitLog4cplusFilePath( void );

		void InitLog4cplusProperties( void );
	private:
		//CCJGW_LoggerPropertier* mpLoggerPropertiesClass;
		LPS_LOGGER_PROPERTIES mpsLoggerProperties;
		log4cplus::Logger	mLogger;
		std::string			mstr_Tag;
		std::wstring		mstr_Path;
		std::wstring		mstrLoggerName;
		bool				mInited;
	private:
		CCJGW_LoggerPropertier mLoggerPropertiesClass;
	};
}

