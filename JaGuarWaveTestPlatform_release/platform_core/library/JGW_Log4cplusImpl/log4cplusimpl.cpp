#include "stdafx.h"

#define USE_LOG4_PROP_FILE

#include <JGW_Log4cplusImplPlugin\log4cplusimpl.h>

#ifdef USE_LOG4_PROP_FILE
#include <JGW_Log4cplusPropFileImplPlugin\JGW_Log4cplus.h>
#else
#include <JGW_Log4cplusImplPlugin\JGW_LogDLL_Export.h>
#endif

#include <JGW_FoundationFunc\JGW_StringFunc.h>

#include <Windows.h>
#include <string>
#include <map>

#ifdef USE_LOG4_PROP_FILE
JGW_Log4cplusPtr gPtrClass;
#else
JGW_LogPtr gPtrClass;
#endif

typedef void (*write_to_logfunc)( Log4Level log_type,const char* tag,const void* log_buf );
//! ≥ı ºªØLOG
static void _write_log_init( Log4Level log_type,const char* tag,const void* log_buf );

write_to_logfunc write_to_log = _write_log_init;

static void _write_to_log(Log4Level log_type,const char* tag,const void* log_buf)
{
    if( !log_buf ) return;
    if (Log4Level_Error == log_type || Log4Level_Fatal == log_type) JGW_Print_LogW(JGWPrintLog_Error,(const wchar_t*)log_buf);
#ifdef USE_LOG4_PROP_FILE
    jgw_log4cplus_write(gPtrClass,log_type,(const wchar_t*)log_buf);
#else
    log4cplus_write(gPtrClass,log_type,log_buf);
#endif
}

static void _write_to_log_null(Log4Level log_type,const char* tag,const void* log_buf)
{

}

static void _write_log_init(Log4Level log_type,const char* tag,const void* log_buf)
{
    {
#ifdef JGW_HAVE_THREADS
        JGW::JGW_AutoMutex log_auto_mutex;
#endif
        if( write_to_log == _write_log_init )
        {
#ifdef USE_LOG4_PROP_FILE
            gPtrClass = jgw_log4cplus_init(JGW::JGW_A2W_A(tag).c_str());
#else
            gPtrClass = log4cplus_init(tag);
#endif
            write_to_log = gPtrClass?_write_to_log:_write_to_log_null;
        }
    }
    write_to_log(log_type,tag,log_buf);
}



void JGW_Write_LogA(Log4Level log_type,const char* tag,const char* log_buf)
{
    std::wstring strTemp = JGW::JGW_A2W_A(log_buf);
    JGW_Write_LogW(log_type,tag,strTemp.c_str());
}

void JGW_Write_LogA_Format(Log4Level log_type,const char* tag,const char* log_buf,...)
{
    std::string strLog("");
    FORMAT_STRING(strLog,log_buf);
    std::wstring strTemp = JGW::JGW_A2W(strLog);
    //     JGW::JGW_ReplaceCharW(strTemp,L'\r',L' ');
    //     JGW::JGW_ReplaceCharW(strTemp,L'\n',L' ');
    write_to_log(log_type,tag,strTemp.c_str());
}

void JGW_Write_LogW(Log4Level log_type,const char* tag,const wchar_t* log_buf)
{
    std::wstring strLog(log_buf);
    //     JGW::JGW_ReplaceCharW(strLog,L'\r',L' ');
    //     JGW::JGW_ReplaceCharW(strLog,L'\n',L' ');
    write_to_log(log_type,tag,strLog.c_str());
}

void JGW_Write_LogW_Format(Log4Level log_type,const char* tag,const wchar_t* log_buf,...)
{
    std::wstring strLog(L"");
    FORMAT_WSTRING(strLog,log_buf);
    //     JGW::JGW_ReplaceCharW(strLog,L'\r',L' ');
    //     JGW::JGW_ReplaceCharW(strLog,L'\n',L' ');
    write_to_log(log_type,tag,strLog.c_str());
}


#if 1
// class JGW_Logger
// {
// public:
//     JGW_Logger() : mptrLogger(NULL),mPrintLogType(JGWPrintLog_Html)
//     {
// 
//     }
// 
//     ~JGW_Logger()
//     {
//         if (mptrLogger)
//         {
//             jgw_logger_close(mptrLogger);
//             mptrLogger = NULL;
//         }
//     }
// 
//     void ReOpenLogger(JGWPrintLogType log_type)
//     {
//         if (mptrLogger)
//         {
//             jgw_logger_close(mptrLogger);
//             mptrLogger = NULL;
//         }
//         mptrLogger = jgw_logger_open(log_type);
//     }
// 
//     void LoggerWrite(JGWPrintLogType log_type,const wchar_t* log_buf)
//     {
//         jgw_logger_windows(log_type,log_buf);
//     }
// 
//     const wchar_t* LoggerRead(JGWPrintLogType log_typ)
//     {
//         /*if (!mptrLogger && mPrintLogType != log_typ)*/ ReOpenLogger(log_typ);
//         wchar_t szBuf[4076] = {0};
//         size_t ret = 0;
//         mstrLog = L"";
// 
//         while (0 != (ret = jgw_logger_read(mptrLogger,szBuf)))
//         {
//             mstrLog += szBuf;
//             if (L'\n' != szBuf[ret - 1]) mstrLog += L"\n";
//         }
//         return mstrLog.c_str();
//     }
// 
//     const wchar_t* LoggerReadLastError(JGWPrintLogType log_typ)
//     {
//         ReOpenLogger(log_typ);
//         wchar_t szBuf[4076] = {0};
// 
//         while (0 != jgw_logger_read(mptrLogger,szBuf))
//         {
//             mstrLog = szBuf;
//         }
//         return mstrLog.c_str();
//     }
// 
// private:
//     JGWPrintLogType mPrintLogType;
//     void* mptrLogger;
//     std::wstring mstrLog;
// };
// 
// JGW_Logger gLogger;

void JGW_Print_LogA(JGWPrintLogType log_type,const char* log_buf)
{
    std::wstring strTemp = JGW::JGW_A2W_A(log_buf);
    jgw_logger_windows(log_type,strTemp.c_str());
}

void JGW_Print_LogA_Format(JGWPrintLogType log_type,const char* log_buf,...)
{
    std::string strLog("");
    FORMAT_STRING(strLog,log_buf);
    std::wstring strTemp = JGW::JGW_A2W(strLog);
    jgw_logger_windows(log_type,strTemp.c_str());
}

void JGW_PrintLineAppendWindows_Log(JGWPrintLogType log_type)
{
    
}

void JGW_Print_LogW(JGWPrintLogType log_type,const wchar_t* log_buf)
{
    jgw_logger_windows(log_type,log_buf);
}

void JGW_Print_LogW_Format(JGWPrintLogType log_type,const wchar_t* log_buf,...)
{
    std::wstring strLog(L"");
    FORMAT_WSTRING(strLog,log_buf);
    jgw_logger_windows(log_type,strLog.c_str());
}

std::wstring JGW_GetPrint_LogW(JGWPrintLogType print_log_type)
{  
    std::wstring strLog;
    size_t ret = 0;
    wchar_t szBuf[4076] = {0};
    
    void* ptrLogger = jgw_logger_open(print_log_type);
    if (JGWPrintLog_Html != print_log_type)
    {
        while (0 != (ret = jgw_logger_read(ptrLogger,szBuf)))
        {
            strLog += szBuf;
            if (L'\n' != szBuf[ret - 1]) strLog += L"\n";
        }
    }
    else
    {
        while (0 != jgw_logger_read(ptrLogger,szBuf)) strLog = szBuf;
    }
    jgw_logger_close(ptrLogger);

    return strLog;
}

void JGW_ClearPrintLog()
{
    jgw_logger_clear();
}

std::wstring JGW_GetLastErrorMsg()
{
    wchar_t szBuf[4076] = {0};
    std::wstring strLog;

    void* ptrLogger = jgw_logger_open(JGWPrintLog_Error);
    while (0 != jgw_logger_read(ptrLogger,szBuf))
        strLog = szBuf;
    jgw_logger_close(ptrLogger);

    return strLog;
}

void JGW_StartRemoteLogSocket(bool bOpen,bool bDebug /*= false*/)
{
    jgw_start_remote_log_socket(bOpen,bDebug);
}
#else
void JGW_Print_LogA(JGWPrintLogType log_type,const char* log_buf)
{
    std::wstring strTemp = JGW::JGW_A2W_A(log_buf);
    JGW_Print_LogW(log_type,strTemp.c_str());
}

void JGW_Print_LogA_Format(JGWPrintLogType log_type,const char* log_buf,...)
{
    std::string strLog("");
    FORMAT_STRING(strLog,log_buf);
    std::wstring strTemp = JGW::JGW_A2W(strLog);
    JGW_Print_LogW(log_type,strTemp.c_str());
}

void JGW_PrintLineAppendWindows_Log(JGWPrintLogType log_type)
{
    jgw_log_print_line(log_type);
}


void JGW_Print_LogW(JGWPrintLogType log_type,const wchar_t* log_buf)
{
    jgw_log_windows(log_type,log_buf);
}

void JGW_Print_LogW_Format(JGWPrintLogType log_type,const wchar_t* log_buf,...)
{
    std::wstring strLog(L"");
    FORMAT_WSTRING(strLog,log_buf);
    JGW_Print_LogW(log_type,strLog.c_str());
}

const wchar_t* JGW_GetPrint_LogW(JGWPrintLogType print_log_type)
{
    return jgw_get_log_windows(print_log_type);
}

void JGW_ClearPrintLog()
{
    jgw_clear_log_windows();
}

const wchar_t* JGW_GetLastErrorMsg()
{
    return jgw_get_last_error_log();
}
#endif

#if 0
const wchar_t* JGW_GetLastLogInfo(const char* tag)
{
    return g_mapLastErrorLog[tag].c_str();
}

const wchar_t* JGW_GetLogToLogLevel(Log4Level log_level)
{
    return Log4_GetLogToLogLevel(log_level);
}

void JGW_Clear_Log()
{
    Log4_ClearLog(Log4Level_Debug);
    Log4_ClearLog(Log4Level_Error);
    Log4_ClearLog(Log4Level_Info);
    Log4_ClearLog(Log4Level_Warning);
}
#endif