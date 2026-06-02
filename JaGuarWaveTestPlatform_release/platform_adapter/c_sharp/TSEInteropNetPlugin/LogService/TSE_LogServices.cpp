#include "StdAfx.h"
#include "TSE_LogServices.h"
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    void CTSE_LogServices::DebugLog(const wchar_t* strDebugLog)
    {
        AddAwaitListener();
        std::vector<CTSE_LogListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnDebugLog(strDebugLog);
        }
    }

    void CTSE_LogServices::DebugLogFormat(const wchar_t* strDebugLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strDebugLog);
        DebugLog(strLog.c_str());
    }

    void CTSE_LogServices::DebugLog(const char* strDebugLog)
    {
        DebugLog(JGW_A2W_A(strDebugLog).c_str());
    }

    void CTSE_LogServices::DebugLogFormat(const char* strDebugLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strDebugLog);
        DebugLog(strLog.c_str());
    }

    void CTSE_LogServices::ErrorLog(const wchar_t* strErrorLog)
    {
        AddAwaitListener();
        std::vector<CTSE_LogListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnErrorLog(strErrorLog);
        }
    }

    void CTSE_LogServices::ErrorLog(const char* strErrorLog)
    {
        ErrorLog(JGW_A2W_A(strErrorLog).c_str());
    }

    void CTSE_LogServices::ErrorLogFormat(const wchar_t* strErrorLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strErrorLog);
        ErrorLog(strLog.c_str());
    }

    void CTSE_LogServices::ErrorLogFormat(const char* strErrorLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strErrorLog);
        ErrorLog(strLog.c_str());
    }

    void CTSE_LogServices::InfoLog(const wchar_t* strInfoLog)
    {
        AddAwaitListener();
        std::vector<CTSE_LogListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnInfoLog(strInfoLog);
        }
    }

    void CTSE_LogServices::InfoLogFormat(const wchar_t* strInfoLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strInfoLog);
        InfoLog(strLog.c_str());
    }

    void CTSE_LogServices::InfoLog(const char* strInfoLog)
    {
        InfoLog(JGW_A2W_A(strInfoLog).c_str());
    }

    void CTSE_LogServices::InfoLogFormat(const char* strInfoLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strInfoLog);
        InfoLog(strLog.c_str());
    }

    void CTSE_LogServices::UserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        AddAwaitListener();
        std::vector<CTSE_LogListener*>::iterator it = mvListener.begin();
        for (;it != mvListener.end();it ++)
        {
            it[0]->OnUserDefinedLog(id,strCustomLog);
        }
    }

    void CTSE_LogServices::UserDefinedLogFormat(int id,const wchar_t* strCustomLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strCustomLog);
        UserDefinedLog(id,strLog.c_str());
    }
}