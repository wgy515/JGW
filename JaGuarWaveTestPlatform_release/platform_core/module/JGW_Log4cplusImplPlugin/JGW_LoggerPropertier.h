#pragma once
#include "JGW_Log4cplusImplPlugin_Define.h"

namespace JGW
{
	class CCJGW_LoggerPropertier
	{
	public:
		CCJGW_LoggerPropertier( const char* tag );
		~CCJGW_LoggerPropertier(void);
		//! 获取LOG配置属性根据TAG
		LPS_LOGGER_PROPERTIES	GetLoggerPropertiesToTag();
		//! 保存LOG属性配置文件
		void					SaveLog4cplusProperties();
	private:
		//! 填充默认LOG属性配置文件
		LPS_LOGGER_PROPERTIES	GetDefaultLoggerProerties();
		//! 获取路径文件
		std::string				GetLoggerPropertiesPath();
	private:
		std::string mstrLoggerPropertiesPath;
		std::wstring mwstrLoggerPropertiesPath;
		const char* mszTag;
		S_LOGGER_PROPERTIES msLoggerProperties;
	};
}


