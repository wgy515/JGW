#ifndef JGW_LOGD_WRITE_H__
#define JGW_LOGD_WRITE_H__

#include "JGW_LogDefine.h"
#include <string>
#include <tchar.h>
//! 释放支持多线程
//#define JGW_HAVE_THREADS

#ifndef JGW_TAG
	#define JGW_TAG "main"
#else
    //#undef JGW_TAG
    //#define JGW_TAG "main"
#endif


// #ifdef _DEBUG
#define DEBUG_APPLICATION
// #endif

#if 0
#define JGW_PRINT_DEBUG_LOG_WINDOWS //!
#define JGW_GET_PRINT_DEBUG_LOG_WINDOWS //! 
#define JGW_PRINT_INFO_LOG_WINDOWS //!
#define JGW_GET_PRINT_INFO_LOG_WINDOWS //! 
#define JGW_PRINT_HTML_LOG_WINDOWS //!
#define JGW_GET_PRINT_HTML_LOG_WINDOWS //! 
#define JGW_CLEAR_PRINT_LOG_WINDOWS //! 

#define PRINTDLOGW //!
#define GETPRINGDLOGW //! 
#define PRINTILOGW //!
#define GETPRINGILOGW //! 
#define PRINTHLOGW //!
#define GETPRINGHLOGW //! 
#define CLEARPLOGW //! 


#else
#ifdef _DEBUG
//#define PRINTFUNCNAMELOGWW_F() JGW_Print_LogW_Format(JGWPrintLog_Debug,L"---%d--- ",__LINE__)
#define PRINTFUNCNAMELOGWW_F() JGW_Print_LogW_Format(JGWPrintLog_Debug,L"%s---%d---%s ",_T(__FILE__),__LINE__,_T(__FUNCTION__))
#else
#define PRINTFUNCNAMELOGWW_F //!
#endif

#define PDLOG4WW(log_buf) JGW_Print_LogW(JGWPrintLog_Debug,log_buf)
#define PDLOG4WW_L(log_buf) do {JGW_Print_LogW(JGWPrintLog_Debug,log_buf);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Debug);} while (0)

#define PDLOG4WW_F(log_buf,...) JGW_Print_LogW_Format(JGWPrintLog_Debug,log_buf,__VA_ARGS__)
#define PDLOG4WW_F_L(log_buf,...) do {JGW_Print_LogW_Format(JGWPrintLog_Debug,log_buf,__VA_ARGS__);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Debug);} while (0)

//#define PDLOG4WA(log_buf) JGW_Print_LogA(JGWPrintLog_Debug,log_buf)
//#define PDLOG4WA_L(log_buf) do {JGW_Print_LogA(JGWPrintLog_Debug,log_buf);\
//    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Debug);} while (0)

//#define PDLOG4WA_F(log_buf,...) JGW_Print_LogA_Format(JGWPrintLog_Debug,log_buf,__VA_ARGS__)
//#define PDLOG4WA_F_L(log_buf,...) do {JGW_Print_LogA_Format(JGWPrintLog_Debug,log_buf,__VA_ARGS__);\
//    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Debug);} while (0)

#define GETPDLOGW()  JGW_GetPrint_LogW(JGWPrintLog_Debug)

#ifdef _DEBUG
#define PELOG4WW(log_buf) do {JGW_Print_LogW(JGWPrintLog_Error,log_buf);PRINTFUNCNAMELOGWW_F();} while (0)
#define PELOG4WW_L(log_buf) do {PRINTFUNCNAMELOGWW_F(); JGW_Print_LogW(JGWPrintLog_Error,log_buf);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

#define PELOG4WW_F(log_buf,...) do {JGW_Print_LogW_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);PRINTFUNCNAMELOGWW_F();} while (0)
#define PELOG4WW_F_L(log_buf,...) do {PRINTFUNCNAMELOGWW_F();JGW_Print_LogW_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

//#define PELOG4WA(log_buf) do {JGW_Print_LogA(JGWPrintLog_Error,log_buf);PRINTFUNCNAMELOGWW_F();} while (0)
//#define PELOG4WA_L(log_buf) do {PRINTFUNCNAMELOGWW_F();JGW_Print_LogA(JGWPrintLog_Error,log_buf);\
//    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

//#define PELOG4WA_F(log_buf,...) do {JGW_Print_LogA_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);PRINTFUNCNAMELOGWW_F();} while (0)
//#define PELOG4WA_F_L(log_buf,...) do {PRINTFUNCNAMELOGWW_F();JGW_Print_LogA_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);\
//    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)


#define GETPELOG4W() JGW_GetPrint_LogW(JGWPrintLog_Error)
#else
#define PELOG4WW(log_buf) JGW_Print_LogW(JGWPrintLog_Error,log_buf)
#define PELOG4WW_L(log_buf) do {JGW_Print_LogW(JGWPrintLog_Error,log_buf);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

#define PELOG4WW_F(log_buf,...) JGW_Print_LogW_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__)
#define PELOG4WW_F_L(log_buf,...) do {JGW_Print_LogW_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

#define PELOG4WA(log_buf) JGW_Print_LogA(JGWPrintLog_Error,log_buf)
#define PELOG4WA_L(log_buf) do {JGW_Print_LogA(JGWPrintLog_Error,log_buf);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

#define PELOG4WA_F(log_buf,...) JGW_Print_LogA_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__)
#define PELOG4WA_F_L(log_buf,...) do {JGW_Print_LogA_Format(JGWPrintLog_Error,log_buf,__VA_ARGS__);\
    JGW_PrintLineAppendWindows_Log(JGWPrintLog_Error);} while (0)

#define GETPELOG4W() JGW_GetPrint_LogW(JGWPrintLog_Error)
#endif

#define PHLOG4WW(log_buf) JGW_Print_LogW(JGWPrintLog_Html,log_buf)
#define PHLOG4WW_F(log_buf,...) do {JGW_Print_LogW_Format(JGWPrintLog_Html,log_buf,__VA_ARGS__)
//#define PHLOG4WA(log_buf) JGW_Print_LogA(JGWPrintLog_Html,log_buf);} while (0)
//#define PHLOG4WA_F(log_buf,...) do {JGW_Print_LogA_Format(JGWPrintLog_Html,log_buf,__VA_ARGS__)
#define GETPHLOGW() JGW_GetPrint_LogW(JGWPrintLog_Html)

#define CLEARPLOGW() JGW_ClearPrintLog()

// #define JGW_GET_DEBUG_LOG JGW_GetLogToLogLevel(Log4Level_Debug)
// #define JGW_GET_ERROR_LOG JGW_GetLogToLogLevel(Log4Level_Error)
// #define JGW_GET_INFO_LOG JGW_GetLogToLogLevel(Log4Level_Info)
// #define JGW_GET_WARNING_LOG JGW_GetLogToLogLevel(Log4Level_Warning)
// #define JGW_CLEAR_LOG JGW_Clear_Log()

// #define JGW_PRINT_LOG_DEBUG 
// #define JGW_PRINT_LOG_ERROR 
// #define JGW_PRINT_LOG_INFO 
// #define JGW_PRINT_LOG_WARNING 
// #define JGW_PRINT_CLEAR_LOG
//! char 
//#define Log4A(log_level,log_buf) JGW_Write_LogA(log_level,JGW_TAG,log_buf);
//
//#define Log4A_F(log_level,log_buf,...) JGW_Write_LogA_Format(log_level,JGW_TAG,log_buf,__VA_ARGS__);
//
//#define Log4AV(log_buf) JGW_Write_LogA(Log4Level_Verbose,JGW_TAG,log_buf)
//
//#ifdef DEBUG_APPLICATION
//	#define Log4AD(log_buf) JGW_Write_LogA(Log4Level_Debug,JGW_TAG,log_buf)
//#else
//	#define Log4AD(log_buf) //
//#endif


//#define Log4AI(log_buf) JGW_Write_LogA(Log4Level_Info,JGW_TAG,log_buf)
//
//#define Log4AW(log_buf) JGW_Write_LogA(Log4Level_Warning,JGW_TAG,log_buf)
//
//#define Log4AE(log_buf) JGW_Write_LogA(Log4Level_Error,JGW_TAG,log_buf)
//
//#define Log4AF(log_buf) JGW_Write_LogA(Log4Level_Fatal,JGW_TAG,log_buf)

//#define Log4AV_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Verbose,JGW_TAG,log_buf,__VA_ARGS__)
//
//#ifdef DEBUG_APPLICATION
//	#define Log4AD_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Debug,JGW_TAG,log_buf,__VA_ARGS__)
//#else
//	#define Log4AD_F(log_buf,...) //
//#endif

//#define Log4AI_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Info,JGW_TAG,log_buf,__VA_ARGS__)
//
//#define Log4AW_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Warning,JGW_TAG,log_buf,__VA_ARGS__)
//
//#define Log4AE_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Error,JGW_TAG,log_buf,__VA_ARGS__)
//
//#define Log4AF_F(log_buf,...) JGW_Write_LogA_Format(Log4Level_Fatal,JGW_TAG,log_buf,__VA_ARGS__)

//! unicode
#define Log4W(log_level,log_buf) JGW_Write_LogW(log_level,JGW_TAG,log_buf);

#define Log4W_F(log_level,log_buf,...) JGW_Write_LogW_Format(log_level,JGW_TAG,log_buf,__VA_ARGS__);

#define Log4WV(log_buf) JGW_Write_LogW(Log4Level_Verbose,JGW_TAG,log_buf)

#ifdef DEBUG_APPLICATION
	#define Log4WD(log_buf) JGW_Write_LogW(Log4Level_Debug,JGW_TAG,log_buf)
#else
	#define Log4WD(log_buf) //
#endif

#define Log4WI(log_buf) JGW_Write_LogW(Log4Level_Info,JGW_TAG,log_buf)

#define Log4WW(log_buf) JGW_Write_LogW(Log4Level_Warning,JGW_TAG,log_buf)

#define Log4WE(log_buf) JGW_Write_LogW(Log4Level_Error,JGW_TAG,log_buf)

#define Log4WF(log_buf) JGW_Write_LogW(Log4Level_Fatal,JGW_TAG,log_buf)

#define Log4WV_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Verbose,JGW_TAG,log_buf,__VA_ARGS__)

#ifdef DEBUG_APPLICATION
	#define Log4WD_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Debug,JGW_TAG,log_buf,__VA_ARGS__)
#else
	#define Log4WD_F(log_buf,...) //
#endif

#define Log4WI_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Info,JGW_TAG,log_buf,__VA_ARGS__)

#define Log4WW_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Warning,JGW_TAG,log_buf,__VA_ARGS__)

#define Log4WE_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Error,JGW_TAG,log_buf,__VA_ARGS__)

#define Log4WF_F(log_buf,...) JGW_Write_LogW_Format(Log4Level_Fatal,JGW_TAG,log_buf,__VA_ARGS__)

#define JGW_GETLASTERRORINFO JGW_GetLastErrorMsg()

#define START_REMOTELOG_SOCKET(bOpen) JGW_StartRemoteLogSocket(bOpen)

// extern void JGW_Write_LogA(Log4Level log_type,const char* tag,const char* log_buf);
// 
// extern void JGW_Write_LogA_Format(Log4Level log_type,const char* tag,const char* log_buf,...);

extern void JGW_Write_LogW(Log4Level log_type,const char* tag,const wchar_t* log_buf);

extern void JGW_Write_LogW_Format(Log4Level log_type,const char* tag,const wchar_t* log_buf,...);

// extern void JGW_Print_LogA(JGWPrintLogType log_type,const char* log_buf);
// 
// extern void JGW_Print_LogA_Format(JGWPrintLogType log_type,const char* log_buf,...);

extern void JGW_Print_LogW(JGWPrintLogType log_type,const wchar_t* log_buf);

extern void JGW_Print_LogW_Format(JGWPrintLogType log_type,const wchar_t* log_buf,...);

extern std::wstring JGW_GetPrint_LogW(JGWPrintLogType print_log_type);

extern void JGW_PrintLineAppendWindows_Log(JGWPrintLogType log_type);

extern void JGW_ClearPrintLog();

extern std::wstring JGW_GetLastErrorMsg();

extern void JGW_StartRemoteLogSocket(bool bOpen,bool bDebug = false);
//extern const wchar_t* JGW_GetLogToLogLevel(Log4Level log_level);
//
//extern void JGW_Clear_Log();
//
//extern const wchar_t* JGW_GetLastLogInfo(const char* tag);
#endif

#ifndef JGW_LOG4CPLUSIMPL_LIB_EXPORTS
#   define JGW_LOG4CPLUSIMPL_LIB_IMPORTS
#endif

#ifdef JGW_LOG4CPLUSIMPL_LIB_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_Log4cplusImpl_d.lib")
#   else
#       pragma comment(lib,"JGW_Log4cplusImpl.lib")
#   endif
#endif

#endif