#include "StdAfx.h"
#include "JGW_Log4cplusPropConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    JGW_Log4cplusPropConfig::JGW_Log4cplusPropConfig(void) : mpPropertyImpl(NULL)
    {
        mpPropertyImpl = GetFilePropertyToFileName(LOG4CPLUS_GLOBAL_FILE_PATH);
        mpPropertyImpl->SetPropertyAppName(L"log4cplus");
        LoadLog4cplusPropConfig();
    }


    JGW_Log4cplusPropConfig::~JGW_Log4cplusPropConfig(void)
    {
        DestroyFilePropertyToPropertyImplPtr(mpPropertyImpl);
    }

    void JGW_Log4cplusPropConfig::BuildLog4cplusGlobalPropertiesFile()
    {

    }

    CCJGW_PropertyImpl* JGW_Log4cplusPropConfig::GetPropertyImpl()
    {
        return mpPropertyImpl;
    }

    void JGW_Log4cplusPropConfig::WritePropValue(const std::wstring& strKeyName,const std::wstring& strValue)
    {
        mpPropertyImpl->PutString(strKeyName,strValue);
    }

    void JGW_Log4cplusPropConfig::LoadLog4cplusPropConfig()
    {
        if (!mpPropertyImpl) return ;

        bool bBuildGlobalLog = false;
        for (std::unordered_map<std::wstring,std::wstring>::iterator it = mpPropertyImpl->Begin();
        it != mpPropertyImpl->End();
        ++ it)
        {
            if (std::wstring::npos != it->first.find(L"default"))
            {
                mmapDefaultProp[it->first] = it->second;
            }
            if (NULL == it->first.compare(L"log4cplus.appender.global.File"))
            {
                bBuildGlobalLog = true;
            }
        }

        if (bBuildGlobalLog)
        {
            mpPropertyImpl->PutString(L"log4cplus.appender.global.File",JGW_BuildLog4cplusLogFilePath(L"global").c_str());
        }

        if (mmapDefaultProp.empty())
        {
            mpPropertyImpl->PutString(L"log4cplus.logger.default",L"ALL,default");
            mpPropertyImpl->PutString(L"log4cplus.appender.default",L"log4cplus::RollingFileAppender");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.File",L"default.log");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.MaxFileSize",L"10MB ");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.MaxBackupIndex",L"30");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.layout",L"log4cplus::PatternLayout");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.filters.1",L"log4cplus::spi::LogLevelRangeFilter");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.filters.1.LogLevelMin",L"INFO");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.filters.1.LogLevelMax",L"FATAL");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.filters.1.AcceptOnMatch",L"true");
            mpPropertyImpl->PutString(L"log4cplus.appender.default.layout.ConversionPattern",L"%D %-5p - %m%n");
            LoadLog4cplusPropConfig();
        }
    }

    void JGW_Log4cplusPropConfig::WriteDefaultProp(const std::wstring& strLoggerName,const std::wstring& strLogFilePath)
    {
        std::wstring strKey,strValue,strTemp;

        JGW_FormatWString(strKey,L"log4cplus.appender.%s",strLoggerName.c_str());
        if (mpPropertyImpl->GetString(strKey).empty())
        {
            for (std::map<std::wstring,std::wstring>::iterator it = mmapDefaultProp.begin();
                it != mmapDefaultProp.end();
                ++ it)
            {
                strKey = it->first;
                strValue = it->second;
                JGW_ReplaceStringW(strKey,L"default",strLoggerName);
                JGW_ReplaceStringW(strValue,L"default",strLoggerName);
                mpPropertyImpl->PutString(strKey,strValue);
            }
        }
        
        WriteLog4cplusLogFilePath(strLoggerName,strLogFilePath);
    }

    void JGW_Log4cplusPropConfig::WriteLog4cplusLogFilePath(const std::wstring& strLoggerName,const std::wstring& strLogFilePath)
    {
        std::wstring strKey;
        JGW_FormatWString(strKey,L"log4cplus.appender.%s.File",strLoggerName.c_str());
        mpPropertyImpl->PutString(strKey,strLogFilePath);
    }
}