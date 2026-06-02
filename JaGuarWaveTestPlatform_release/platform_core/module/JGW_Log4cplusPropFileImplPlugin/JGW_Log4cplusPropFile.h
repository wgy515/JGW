#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/socketappender.h>

#include <JGW_Log4cplusImplPlugin/JGW_LogDefine.h>
namespace JGW
{
    class JGW_Log4cplusPropFile
    {
    public:
        JGW_Log4cplusPropFile(const std::wstring& strLoggerName);
        ~JGW_Log4cplusPropFile(void);
    public:
        bool InitLogger();

        void WriteLog4cplus(Log4Level log_type,const wchar_t* log);

        void StartRemoteLogSocket(bool bOpen = true,bool bDebug = false);
    private:
        bool InitLog4cplusFilePath( void );

        void InitLog4cplusProperties( void );
    private:
        std::wstring mstrFilePath;
        std::wstring mstrLoggerName;
        log4cplus::Logger mLogger;
    };
}