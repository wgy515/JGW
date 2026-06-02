#pragma once
#include <map>
#include <string>
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
#define LOG4CPLUS_GLOBAL_FILE_PATH L"log\\log4cplus_global.properties"

namespace JGW
{
    class JGW_Log4cplusPropConfig
    {
    public:
        ~JGW_Log4cplusPropConfig(void);
        static JGW_Log4cplusPropConfig* GetInstance()
        {
            static JGW_Log4cplusPropConfig gLog4cplusPropConfig;
            return &gLog4cplusPropConfig;
        }
    public:
        //! 默认属性
        void WriteDefaultProp(const std::wstring& strLoggerName,const std::wstring& strLogFilePath);
        //! 修改log文件路径
        void WriteLog4cplusLogFilePath(const std::wstring& strLoggerName,const std::wstring& strLogFilePath);
        //!
        void BuildLog4cplusGlobalPropertiesFile();
        //!
        void WritePropValue(const std::wstring& strKeyName,const std::wstring& strValue);
        //!
        CCJGW_PropertyImpl* GetPropertyImpl();
    private:
        void LoadLog4cplusPropConfig();
    protected:
        JGW_Log4cplusPropConfig(void);
    private:
        std::map<std::wstring,std::wstring> mmapDefaultProp;
        CCJGW_PropertyImpl* mpPropertyImpl;
    };
}