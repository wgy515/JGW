// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <JGW_Log4cplusPropFileImplPlugin/JGW_Log4cplus.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
extern logger_log glogger_log[JGWPrintLog_Html + 1];

#define LOGGER_DEBUG_BUFFER_SIZE 128 * 1024
#define LOGGER_ERROR_BUFFER_SIZE 8 * 1024
#define LOGGER_HTML_BUFFER_SIZE 8 * 1024

void jgw_logger_init(logger_log& logger_log,size_t logger_buf_size)
{
    boost::mutex::scoped_lock lock(logger_log.mutex);
    logger_log.head = 0;
    logger_log.w_off = 0;
    //logger_log.prev_w_off = 0;
    logger_log.size = logger_buf_size;
    logger_log.buffer = (char*)calloc(1,logger_log.size/*sizeof(wchar_t)*/);
}

void jgw_logger_exit(logger_log& logger_log)
{
    boost::mutex::scoped_lock lock(logger_log.mutex);
    logger_log.head = 0;
    logger_log.w_off = 0;
    logger_log.size = 0;
    if (logger_log.buffer) free((void*)logger_log.buffer);
    logger_log.buffer = NULL;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        //! 128k
        jgw_logger_init(glogger_log[JGWPrintLog_Debug],LOGGER_DEBUG_BUFFER_SIZE);
        //! 8k
        jgw_logger_init(glogger_log[JGWPrintLog_Error],LOGGER_ERROR_BUFFER_SIZE);
        //! 8k
        jgw_logger_init(glogger_log[JGWPrintLog_Html],LOGGER_HTML_BUFFER_SIZE);
#ifdef IS_ENCRYPT_MODULE
        return JGW::EncryptModuleIsLoaded(hModule) ? TRUE : FALSE;
#endif  
	case DLL_THREAD_ATTACH:
        
        break;
	case DLL_THREAD_DETACH:
        
        break;
	case DLL_PROCESS_DETACH:
        jgw_logger_exit(glogger_log[JGWPrintLog_Debug]);
        jgw_logger_exit(glogger_log[JGWPrintLog_Error]);
        jgw_logger_exit(glogger_log[JGWPrintLog_Html]);
		break;
	}
	return TRUE;
}

