#pragma once
#include <JGW_Log4cplusImplPlugin/JGW_LogDefine.h>
#ifdef JGW_LOG4CPLUS_PROP_FILE_API
#undef JGW_LOG4CPLUS_PROP_FILE_API
#endif

#ifdef JGW_LOG4CPLUSPROPFILEIMPLPLUGIN_EXPORTS
#define JGW_LOG4CPLUS_PROP_FILE_API __declspec(dllexport)
#else
#define JGW_LOG4CPLUSPROPFILEIMPLPLUGIN_IMPORTS
#define JGW_LOG4CPLUS_PROP_FILE_API __declspec(dllimport)
#endif

#ifdef JGW_LOG4CPLUSPROPFILEIMPLPLUGIN_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_Log4cplusPropFileImplPlugin_d.lib")
#   else
#       pragma comment(lib,"JGW_Log4cplusPropFileImplPlugin.lib")
#   endif
#endif

typedef void*  JGW_Log4cplusPtr;

#ifdef __cplusplus
extern "C" 
{
#endif
    JGW_LOG4CPLUS_PROP_FILE_API JGW_Log4cplusPtr jgw_log4cplus_init(const wchar_t* tag);

    //!JGW_LOG4CPLUS_PROP_FILE_API void jgw_log4cplus_write(const wchar_t* strLog);
    JGW_LOG4CPLUS_PROP_FILE_API void jgw_log4cplus_write(const JGW_Log4cplusPtr ptrClass,Log4Level log_type,const wchar_t* log);

    JGW_LOG4CPLUS_PROP_FILE_API void jgw_start_remote_log_socket(bool bOpen,bool bDebug = false);
    //!
//     JGW_LOG4CPLUS_PROP_FILE_API void jgw_clear_log_windows();
// 
//     JGW_LOG4CPLUS_PROP_FILE_API void jgw_log_windows(JGWPrintLogType print_log_type,const wchar_t* log_buf);
// 
//     JGW_LOG4CPLUS_PROP_FILE_API void jgw_log_print_line(JGWPrintLogType print_log_type);
// 
//     JGW_LOG4CPLUS_PROP_FILE_API const wchar_t* jgw_get_log_windows(JGWPrintLogType print_log_type);
// 
//     JGW_LOG4CPLUS_PROP_FILE_API const wchar_t* jgw_get_last_error_log();


    JGW_LOG4CPLUS_PROP_FILE_API void jgw_logger_windows(JGWPrintLogType print_log_type,const wchar_t* log_buf);

    JGW_LOG4CPLUS_PROP_FILE_API void* jgw_logger_open(JGWPrintLogType print_log_type);

    JGW_LOG4CPLUS_PROP_FILE_API size_t jgw_logger_read(void* plogger_reader,wchar_t* log_buf/*,size_t count*/);

    JGW_LOG4CPLUS_PROP_FILE_API void jgw_logger_close(void* plogger_reader);

    JGW_LOG4CPLUS_PROP_FILE_API void jgw_logger_clear();
#ifdef __cplusplus
};
#endif