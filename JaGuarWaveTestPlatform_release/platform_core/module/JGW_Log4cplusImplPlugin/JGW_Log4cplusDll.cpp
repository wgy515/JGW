// JGW_Log4cplusDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#define JGW_ASSEMBLY_EXPORTS

#include <map>
#include <string>

#include <JGW_Log4cplusImplPlugin\JGW_LogDLL_Export.h>
#include <JGW_FoundationFunc/JGW_AutoMutex.h>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include "JGW_Log4cplus.h"
#include "JGW_Log4cplusImplPlugin_Define.h"
#include "JGW_LoggerPropertier.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <sstream>

boost::mutex io_mutex;
JGW::JGW_Log4cplus* gpMainLog4cplus = NULL;
std::wstring gstrPrintLogWindows[JGWPrintLog_Html + 1];
std::wstring gstrMainPrintLogWindows[JGWPrintLog_Html + 1];
std::wstring gstrLastErrorMsg;

std::map<std::string,std::auto_ptr<JGW::JGW_Log4cplus>> gstr_log4_map;

const JGW_LogPtr log4cplus_init( const char* tag )
{
    //! 多线程保护
    JGW::JGW_AutoMutex auto_mutex;
    JGW::JGW_Log4cplus* pLog4cplus = gstr_log4_map[tag].get();

    //! 当前的tag的log4cplus不存在则初始化
    if ( !pLog4cplus )
    {
        pLog4cplus = new JGW::JGW_Log4cplus( tag );
        gstr_log4_map[tag].reset( pLog4cplus );
    }

    if ( !pLog4cplus->InitLog4cplus() )
    {
        gstr_log4_map.erase(tag);
        return NULL;
    }

    if (NULL == JGW::JGW_StrCaseStr(tag,"main")) gpMainLog4cplus = pLog4cplus;

    return pLog4cplus;
}

//! 对log4cpluslib进行处理
void log4cplus_write(const JGW_LogPtr ptrClass,Log4Level log_type,const void* log_buf)
{
    JGW::JGW_Log4cplus* pLog4cplus = static_cast<JGW::JGW_Log4cplus*>(ptrClass);
    if (pLog4cplus) pLog4cplus->WriteLog4cplus(log_type,(const wchar_t*)log_buf);

    if (Log4Level_Error <= log_type) 
    {
        boost::mutex::scoped_lock lock(io_mutex);
        gstrLastErrorMsg = (const wchar_t*)log_buf;    
    }
}

void jgw_clear_log_windows()
{
    boost::mutex::scoped_lock lock(io_mutex);
    gstrPrintLogWindows[JGWPrintLog_Debug] = L"";
    gstrPrintLogWindows[JGWPrintLog_Error] = L"";
    gstrPrintLogWindows[JGWPrintLog_Html] = L"";
}

void jgw_log_print_line(JGWPrintLogType print_log_type)
{
    boost::mutex::scoped_lock lock(io_mutex);

    switch (print_log_type)
    {
    case JGWPrintLog_Html:
        if (L'\n' != gstrPrintLogWindows[print_log_type][gstrPrintLogWindows[print_log_type].length() - 1]) gstrPrintLogWindows[print_log_type] += L"\n";
        break;
    case JGWPrintLog_Error:
        if (L'\n' != gstrPrintLogWindows[print_log_type][gstrPrintLogWindows[print_log_type].length() - 1]) gstrPrintLogWindows[print_log_type] += L"\n";
        if (L'\n' != gstrPrintLogWindows[JGWPrintLog_Debug][gstrPrintLogWindows[JGWPrintLog_Debug].length() - 1]) gstrPrintLogWindows[JGWPrintLog_Debug] += L"\n";
        break;
    case JGWPrintLog_Debug:
        if (L'\n' != gstrPrintLogWindows[JGWPrintLog_Debug][gstrPrintLogWindows[JGWPrintLog_Debug].length() - 1]) gstrPrintLogWindows[JGWPrintLog_Debug] += L"\n";
        break;
    
    }
}

void jgw_log_windows(JGWPrintLogType print_log_type,const wchar_t* log_buf)
{
    boost::mutex::scoped_lock lock(io_mutex);

    switch (print_log_type)
    {
    case JGWPrintLog_Html:
        gstrPrintLogWindows[print_log_type] = log_buf;
        break;
    case JGWPrintLog_Debug:
        gstrPrintLogWindows[print_log_type] += log_buf;
        if (gpMainLog4cplus) gpMainLog4cplus->WriteLog4cplus(Log4Level_Info,log_buf);
        break;
    case JGWPrintLog_Error:
        gstrPrintLogWindows[print_log_type] += log_buf;
        gstrPrintLogWindows[JGWPrintLog_Debug] += log_buf;
        if (gpMainLog4cplus) gpMainLog4cplus->WriteLog4cplus(Log4Level_Error,log_buf);
        break;
    }
}

const wchar_t* jgw_get_log_windows(JGWPrintLogType print_log_type)
{
    boost::mutex::scoped_lock lock(io_mutex);
    gstrMainPrintLogWindows[print_log_type] = gstrPrintLogWindows[print_log_type].c_str();
    return gstrMainPrintLogWindows[print_log_type].c_str();
}

const wchar_t* jgw_get_last_error_log()
{
    return gstrLastErrorMsg.c_str();
}
