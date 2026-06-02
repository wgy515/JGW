// JGW_Log4cplusPropFileImplPlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <JGW_Log4cplusPropFileImplPlugin/JGW_Log4cplus.h>

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

#include "JGW_Log4cplusPropFile.h"

#include <JGW_FoundationFunc/JGW_AutoMutex.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

/*#include <intsafe.h>*/

boost::mutex gLog4cplusInitMutex;
//boost::mutex gLog4cplusWriteMutex;

// std::wstring gstrPrintLogWindows[JGWPrintLog_Html + 1];
// std::wstring gstrMainPrintLogWindows[JGWPrintLog_Html + 1];
// std::wstring gstrLastErrorMsg;

std::vector<std::shared_ptr<logger_reader>> gveclogger_reader;
std::map<std::wstring,std::auto_ptr<JGW::JGW_Log4cplusPropFile>> gmapLog4cplusPropFile;

JGW::JGW_Log4cplusPropFile* gpMainLog4cplus = NULL;

logger_log glogger_log[JGWPrintLog_Html + 1];

extern void do_write_log(logger_log *log, const char *buf, size_t count);
extern void fix_up_readers(logger_log *log, size_t len);
extern INT32 do_write_log_from_user(logger_log *log,const char *buf, size_t count);
extern logger_entry *get_entry_header(logger_log *log,
    size_t off, logger_entry *scratch);
extern size_t logger_offset(logger_log *log, size_t n);

void jgw_logger_windows(JGWPrintLogType print_log_type,const wchar_t* log_buf)
{
    {
        logger_log* log = &glogger_log[print_log_type];
        boost::mutex::scoped_lock lock(log->mutex);
        //! 
        size_t log_size = wcslen(log_buf);
        size_t prev_w_off = log->w_off;
        logger_entry header;
        //! 初始化log 参数
        header.pid = GetProcessId(GetCurrentProcess());
        header.tid = GetCurrentThreadId();//GetThreadId(GetCurrentThread());
        header.sec = GetTickCount();
        header.nsec = 0;
        header.hdr_size = LOGGER_ENTRY_STRUCT_SIZE;
        header.len = min(log_size * sizeof(wchar_t), LOGGER_ENTRY_MAX_PAYLOAD);

        fix_up_readers(log,LOGGER_ENTRY_STRUCT_SIZE + header.len);
        do_write_log(log, (char*)(&header), LOGGER_ENTRY_STRUCT_SIZE);
        do_write_log_from_user(log, (const char*)log_buf, header.len);
    }
    {
        switch (print_log_type)
        {
        case JGWPrintLog_Html:
            if (gpMainLog4cplus) gpMainLog4cplus->WriteLog4cplus(Log4Level_Debug,log_buf);
            break;
        case JGWPrintLog_Debug:
            if (gpMainLog4cplus) gpMainLog4cplus->WriteLog4cplus(Log4Level_Debug,log_buf);
            break;
        case JGWPrintLog_Error:
            if (gpMainLog4cplus) gpMainLog4cplus->WriteLog4cplus(Log4Level_Error,log_buf);
            break;
        }
    }  
}

void* jgw_logger_open(JGWPrintLogType print_log_type)
{
    std::shared_ptr<logger_reader> ptrlogger_reader(new logger_reader);
    ptrlogger_reader->log = &glogger_log[print_log_type];
    {
        boost::mutex::scoped_lock lock(ptrlogger_reader->log->mutex);
        ptrlogger_reader->r_off = ptrlogger_reader->log->head;
    }
    gveclogger_reader.push_back(ptrlogger_reader);
    return ptrlogger_reader.get();
}

size_t jgw_logger_read(void* plogger_reader,wchar_t* log_buf)
{
    logger_reader* reader = (logger_reader*)plogger_reader;
    logger_log* log = reader->log;
    boost::mutex::scoped_lock lock(log->mutex);
    if (log->w_off == reader->r_off) return 0;

    logger_entry scratch;
    size_t msg_start,len,count;
    logger_entry *entry = NULL;
    char* buf = (char*)log_buf;

    /*
    * First, copy the header to userspace, using the version of
    * the header requested
    */
    entry = get_entry_header(log, reader->r_off, &scratch);
    count = entry->len;

    msg_start = logger_offset(log,reader->r_off + LOGGER_ENTRY_STRUCT_SIZE);

    len = min(count, log->size - msg_start);
    if (!memcpy(buf, (void*)&(log->buffer[msg_start]), len))
        return 0;

    /*
    * Second, we read any remaining bytes, starting back at the head of
    * the log.
    */
    if (count != len)
        if (!memcpy(buf + len, log->buffer, count - len))
            return -EFAULT;

    reader->r_off = logger_offset(log, reader->r_off +
        LOGGER_ENTRY_STRUCT_SIZE + count);

    log_buf[len/2] = L'\0';

    return len/2;
}

void jgw_logger_close(void* plogger_reader)
{
    logger_reader* pLoggerReader = (logger_reader*)plogger_reader;
    boost::mutex::scoped_lock lock(pLoggerReader->log->mutex);

    std::vector<std::shared_ptr<logger_reader>>::iterator it = gveclogger_reader.begin();
    while (it != gveclogger_reader.end())
    {
        if (it[0].get() == plogger_reader)
        {
            it = gveclogger_reader.erase(it);
        }
        else
        {
            ++ it;
        }
    }
}

void jgw_logger_clear()
{
    boost::mutex::scoped_lock lock_debug(glogger_log[JGWPrintLog_Debug].mutex);
    glogger_log[JGWPrintLog_Debug].w_off = 0;
    glogger_log[JGWPrintLog_Debug].head = 0;

    boost::mutex::scoped_lock lock_error(glogger_log[JGWPrintLog_Error].mutex);
    glogger_log[JGWPrintLog_Error].w_off = 0;
    glogger_log[JGWPrintLog_Error].head = 0;

    boost::mutex::scoped_lock lock_html(glogger_log[JGWPrintLog_Html].mutex);
    glogger_log[JGWPrintLog_Html].w_off = 0;
    glogger_log[JGWPrintLog_Html].head = 0;

    for (std::vector<std::shared_ptr<logger_reader>>::iterator it = gveclogger_reader.begin();
        it != gveclogger_reader.end();
        ++ it)
    {
            it[0].get()->r_off = 0;
    }
}
/*
void jgw_clear_log_windows()
{
    boost::mutex::scoped_lock lock(gLog4cplusWriteMutex);
    gstrPrintLogWindows[JGWPrintLog_Debug] = L"";
    gstrPrintLogWindows[JGWPrintLog_Error] = L"";
    gstrPrintLogWindows[JGWPrintLog_Html] = L"";
}

void jgw_log_print_line(JGWPrintLogType print_log_type)
{
    boost::mutex::scoped_lock lock(gLog4cplusWriteMutex);

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
    boost::mutex::scoped_lock lock(gLog4cplusWriteMutex);

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
    boost::mutex::scoped_lock lock(gLog4cplusWriteMutex);
    gstrMainPrintLogWindows[print_log_type] = gstrPrintLogWindows[print_log_type].c_str();
    return gstrMainPrintLogWindows[print_log_type].c_str();
}

const wchar_t* jgw_get_last_error_log()
{
    return gstrLastErrorMsg.c_str();
}
*/

JGW_Log4cplusPtr jgw_log4cplus_init(const wchar_t* tag)
{
    boost::mutex::scoped_lock lock(gLog4cplusInitMutex);
    JGW::JGW_Log4cplusPropFile* pLog4cplusPropFile = gmapLog4cplusPropFile[tag].get();

    //! 当前的tag的log4cplus不存在则初始化
    if (!pLog4cplusPropFile)
    {
        pLog4cplusPropFile = new JGW::JGW_Log4cplusPropFile(tag);
        gmapLog4cplusPropFile[tag].reset(pLog4cplusPropFile);

        if (!pLog4cplusPropFile->InitLogger())
        {
            gmapLog4cplusPropFile.erase(tag);
            return NULL;
        }
    }

    if (NULL == gpMainLog4cplus/*NULL == JGW::JGW_WStrComparenoCaseWStr(tag,L"main")*/) gpMainLog4cplus = pLog4cplusPropFile;

    return pLog4cplusPropFile;
}

void jgw_log4cplus_write(const JGW_Log4cplusPtr ptrClass,Log4Level log_type,const wchar_t* log)
{
    JGW::JGW_Log4cplusPropFile* pLog4cplus = static_cast<JGW::JGW_Log4cplusPropFile*>(ptrClass);
    if (pLog4cplus) pLog4cplus->WriteLog4cplus(log_type,log);
}

void jgw_start_remote_log_socket(bool bOpen,bool bDebug/* = false*/)
{
    JGW_Log4cplusPtr mainLog4cplusPtr = jgw_log4cplus_init(L"main");
    JGW::JGW_Log4cplusPropFile* pLog4cplus = static_cast<JGW::JGW_Log4cplusPropFile*>(mainLog4cplusPtr);
    if (pLog4cplus) pLog4cplus->StartRemoteLogSocket(bOpen,bDebug);
}

//log4cplus::Logger mainLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("root_zq"));
// void jgw_log4cplus_write(const wchar_t* strLog)
// {
//     log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("C:/log4cplus_default.properties"));
//     LOG4CPLUS_ERROR_STR(mainLogger, L"This is a INFO message");
// }
