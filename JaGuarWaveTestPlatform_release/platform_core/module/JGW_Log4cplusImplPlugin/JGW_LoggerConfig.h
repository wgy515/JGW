#pragma once

#include "JGW_Log4cplusImplPlugin_Define.h"
#include <tinyxml/tinyxml.h>

namespace JGW
{
	class CCJGW_LoggerConfig
	{
	public:
		CCJGW_LoggerConfig(void);
		~CCJGW_LoggerConfig(void);
		//! º”‘ÿLoggger≈‰÷√Œƒº˛
		bool LoadLoggerConfig( const char* strLoggerPath,S_LOGGER_PROPERTIES& sLoggerProperties );
	private:
		bool PareseLoggerNode( TiXmlElement* pLoggerNode,LPS_LOGGER_PROPERTIES psLoggerProperties );
	};
}


