#ifndef JGW_LOG4CPLUSDLL_EXPORT_H__
#define JGW_LOG4CPLUSDLL_EXPORT_H__

#include <JGW_Log4cplusImplPlugin\JGW_LogDefine.h>
typedef void*  JGW_LogPtr;

#ifdef JGW_LOG4CPLUSDLL_API
#undef JGW_LOG4CPLUSDLL_API
#endif

#ifdef JGW_LOG4CPLUSDLL_EXPORTS
#define JGW_LOG4CPLUSDLL_API __declspec(dllexport)
#else
#define JGW_LOG4CPLUSDLL_IMPORTS
#define JGW_LOG4CPLUSDLL_API __declspec(dllimport)
#endif

#ifdef JGW_LOG4CPLUSDLL_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_Log4cplusImplPlugin_d.lib")
#   else
#       pragma comment(lib,"JGW_Log4cplusImplPlugin.lib")
#   endif
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
    //! Log File
	JGW_LOG4CPLUSDLL_API const JGW_LogPtr log4cplus_init(const char* tag);

	JGW_LOG4CPLUSDLL_API void log4cplus_write(const JGW_LogPtr ptrClass,Log4Level log_type,const void* log_buf);
    //!
    JGW_LOG4CPLUSDLL_API void jgw_clear_log_windows();

    JGW_LOG4CPLUSDLL_API void jgw_log_windows(JGWPrintLogType print_log_type,const wchar_t* log_buf);

    JGW_LOG4CPLUSDLL_API void jgw_log_print_line(JGWPrintLogType print_log_type);

    JGW_LOG4CPLUSDLL_API const wchar_t* jgw_get_log_windows(JGWPrintLogType print_log_type);

    JGW_LOG4CPLUSDLL_API const wchar_t* jgw_get_last_error_log();
#ifdef __cplusplus
};
#endif


#endif