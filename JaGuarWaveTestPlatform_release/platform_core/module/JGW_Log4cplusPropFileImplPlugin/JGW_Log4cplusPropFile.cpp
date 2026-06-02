#include "StdAfx.h"
#include "JGW_Log4cplusPropFile.h"
#include "JGW_Log4cplusPropConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <fstream>
#include <log4cplus/fstreams.h>
#include <log4cplus/spi/factory.h>
#include "ChineseUtf8LocaleFactory.h"
#include <memory>

namespace JGW
{
    JGW_Log4cplusPropFile::JGW_Log4cplusPropFile(const std::wstring& strLoggerName) : mstrLoggerName(strLoggerName)
    {
        //log4cplus::Initializer initializer;
        log4cplus::spi::LocaleFactoryRegistry& localeFactoryRegistry = log4cplus::spi::getLocaleFactoryRegistry();
        localeFactoryRegistry.put(std::auto_ptr<ChineseUtf8LocaleFactory>(new ChineseUtf8LocaleFactory()));

        //PropertyConfigurator pc(LOG4CPLUS_TEXT("log4cplus.properties"));
        //pc.configure();
    }


    JGW_Log4cplusPropFile::~JGW_Log4cplusPropFile(void)
    {
    }

    bool JGW_Log4cplusPropFile::InitLogger()
    {
        mLogger = log4cplus::Logger::getInstance(mstrLoggerName.c_str());

        if( !InitLog4cplusFilePath() ) return false;
        InitLog4cplusProperties();
        //!log4cplus::PropertyConfigurator::doConfigure(JGW_RealativePathToAbsPath(LOG4CPLUS_GLOBAL_FILE_PATH).c_str(),log4cplus::Logger::getDefaultHierarchy(),log4cplus::helpers::Properties::fUTF8);
        //! 解决中文目录或者中文路径问题
        log4cplus::tifstream propertyStream;
        propertyStream.imbue(std::locale(""));
        propertyStream.open(JGW_RealativePathToAbsPath(LOG4CPLUS_GLOBAL_FILE_PATH).c_str(),
            std::ios::binary);
        if (!propertyStream.good ()) return false;

        log4cplus::PropertyConfigurator tmp(propertyStream);
        tmp.configure();

        return true;
    }

    void JGW_Log4cplusPropFile::StartRemoteLogSocket(bool bOpen /* = true */,bool bDebug /* = false */)
    {
        if (bOpen)
        {
            mLogger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
            log4cplus::SharedAppenderPtr append_1(new log4cplus::SocketAppender(JGW_Log4cplusPropConfig::GetInstance()->GetPropertyImpl()->GetString(L"log4cplus.appender.global.host"),JGW_Log4cplusPropConfig::GetInstance()->GetPropertyImpl()->GetInt(L"log4cplus.appender.global.port")));
            append_1->setName(LOG4CPLUS_TEXT("remote_log_socket"));
            mLogger.addAppender(append_1);
        }
        else
        {
            if (mLogger.getAppender(LOG4CPLUS_TEXT("remote_log_socket")))
            {
                mLogger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
                mLogger.removeAppender(L"remote_log_socket");
            }     
        }
        if (bDebug)
        {
            mLogger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
        }
#if 0
        //! JGW_Log4cplusPropConfig::GetInstance()->WritePropValue(L"log4cplus.appender.global",bOpen?L"log4cplus::SocketAppender":L"log4cplus::RollingFileAppender");
        //log4cplus::PropertyConfigurator::doConfigure(JGW_RealativePathToAbsPath(L"log\\log4cplus_remote_global.properties").c_str());
        //! log4cplus::PropertyConfigurator::doConfigure(JGW_RealativePathToAbsPath(LOG4CPLUS_GLOBAL_FILE_PATH).c_str());
#endif
    }

    bool JGW_Log4cplusPropFile::InitLog4cplusFilePath()
    {
        time_t timeCur;
        struct tm tmTmp ;
        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        JGW_GetExecSubFolder(mstrFilePath,L"log",NULL);
        JGW_FormatWString(mstrFilePath,L"%s\\%s\\%4d-%02d-%02d\\%s.log",mstrFilePath.c_str(),mstrLoggerName.c_str(),tmTmp.tm_year + 1900,tmTmp.tm_mon+1,tmTmp.tm_mday ,mstrLoggerName.c_str());

        if(!JGW_CreateDirectory(mstrFilePath.c_str(),false))
        {
            return false;
        }

        return true;
    }

    void JGW_Log4cplusPropFile::InitLog4cplusProperties()
    {
        JGW_Log4cplusPropConfig::GetInstance()->WriteDefaultProp(mstrLoggerName,mstrFilePath);
    }

    void JGW_Log4cplusPropFile::WriteLog4cplus(Log4Level log_type,const wchar_t* log)
    {
        switch(log_type)
        {
        case Log4Level_Verbose:
            LOG4CPLUS_TRACE_STR(mLogger, log);
            break;
        case Log4Level_Debug:
            LOG4CPLUS_DEBUG_STR(mLogger, log);
            break;
        case Log4Level_Info:
            LOG4CPLUS_INFO_STR(mLogger, log);
            break;
        case Log4Level_Warning:
            LOG4CPLUS_WARN_STR(mLogger, log);
            break;
        case Log4Level_Error:
            LOG4CPLUS_ERROR_STR(mLogger, log);
            break;
        case Log4Level_Fatal:
            LOG4CPLUS_FATAL_STR(mLogger,log);
        }
    }
}