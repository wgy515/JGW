#include "StdAfx.h"
#include "ILog.h"
#include <iostream>

#define FORMAT_WSTRING(strLog,szFormat)\
    int nListCount = 0;\
{\
    va_list pArgList;\
    va_start(pArgList,szFormat);\
    int nLength = _vscwprintf(szFormat,pArgList) + 1;\
    strLog.resize(nLength);\
    nListCount +=_vsnwprintf_s(const_cast<wchar_t *>(strLog.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);\
    va_end(pArgList);\
    }\
    strLog.reserve(nListCount + 1);

#define FORMAT_STRING(strLog,szFormat)\
    int nListCount = 0;\
{\
    va_list pArgList;\
    va_start(pArgList,szFormat);\
    int nLength = _vscprintf(szFormat,pArgList) + 1;\
    strLog.resize(nLength);\
    nListCount +=_vsnprintf_s(const_cast<char *>(strLog.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);\
    va_end(pArgList);\
    }\
    strLog.reserve(nListCount + 1);

namespace JGW
{



    ILog::ILog(void)
    {
    }


    ILog::~ILog(void)
    {
    }

    void ILog::Verbose(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        std::wclog << strLog << std::endl;
    }

    void ILog::Verbose(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
        std::clog << strLog << std::endl;
    }

    void ILog::Debug(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
#ifdef _DEBUG
        std::wcout << strLog << std::endl;
#endif
    }

    void ILog::Debug(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
#ifdef _DEBUG
        std::cout << strLog << std::endl;
#endif
    }

    void ILog::Info(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        //std::wcout << strLog << std::endl;
    }

    void ILog::Info(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
        //std::cout << strLog << std::endl;
    }

    void ILog::Warning(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        //std::wcout << strLog << std::endl;
    }

    void ILog::Warning(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
        //std::cout << strLog << std::endl;
    }

    void ILog::Error(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        std::wcerr << strLog << std::endl;
    }

    void ILog::Error(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
        std::cerr << strLog << std::endl;
    }

    void ILog::Fatal(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        std::wcerr << strLog << std::endl;
    }

    void ILog::Fatal(const char* szFormat,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,szFormat);
        std::cerr << strLog << std::endl;
    }
}

