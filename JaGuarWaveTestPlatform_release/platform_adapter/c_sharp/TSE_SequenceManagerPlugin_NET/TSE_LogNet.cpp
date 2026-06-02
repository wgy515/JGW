#include "StdAfx.h"
#include "TSE_LogNet.h"
#include "TSEExportInterface.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

extern LogCallBackDelegate gLogCallBack;

namespace JGW
{
    enum LogTypePluginDomain
    {
        Debug = 0,
        Info = 1,
        Warn = 2,
        Error = 3,
        UserDefine = 4
    };
    /// <summary>
    /// USER DEFINE ID TYPE
    /// </summary>
    enum UserDefineIDType
    {
        E_USER_DEFINE_HTMLFILEPATH = 0,// html log 文件路径
        E_TSE_SHOW_CONTENT_INFO = 1,// 测试list内容修改
        E_TSE_SHOW_TEST_INFO_TO_RICHEDIT = 2
    };

    void CTSE_LogNet::DebugLog(const wchar_t* strDebugLog)
    {
        if (gLogCallBack)
        {
            gLogCallBack(Debug,0,strDebugLog);
        }
    }

    void CTSE_LogNet::DebugLogFormat(const wchar_t* strDebugLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strDebugLog);
        DebugLog(strLog.c_str());
    }

    void CTSE_LogNet::DebugLog(const char* strDebugLog)
    {
        DebugLog(JGW_A2W_A(strDebugLog).c_str());
    }

    void CTSE_LogNet::DebugLogFormat(const char* strDebugLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strDebugLog);
        DebugLog(strLog.c_str());
    }

    void CTSE_LogNet::ErrorLog(const wchar_t* strErrorLog)
    {
        if (gLogCallBack)
        {
            gLogCallBack(Error,0,strErrorLog);
        }
    }

    void CTSE_LogNet::ErrorLog(const char* strErrorLog)
    {
        ErrorLog(JGW_A2W_A(strErrorLog).c_str());
    }

    void CTSE_LogNet::ErrorLogFormat(const wchar_t* strErrorLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strErrorLog);
        ErrorLog(strLog.c_str());
    }

    void CTSE_LogNet::ErrorLogFormat(const char* strErrorLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strErrorLog);
        ErrorLog(strLog.c_str());
    }

    void CTSE_LogNet::InfoLog(const wchar_t* strInfoLog)
    {
        if (gLogCallBack)
        {
            gLogCallBack(Info,0,strInfoLog);
        }
    }

    void CTSE_LogNet::InfoLogFormat(const wchar_t* strInfoLog,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,strInfoLog);
        InfoLog(strLog.c_str());
    }

    void CTSE_LogNet::InfoLog(const char* strInfoLog)
    {
        InfoLog(JGW_A2W_A(strInfoLog).c_str());
    }

    void CTSE_LogNet::InfoLogFormat(const char* strInfoLog,...)
    {
        std::string strLog;
        FORMAT_STRING(strLog,strInfoLog);
        InfoLog(strLog.c_str());
    }

    void CTSE_LogNet::UserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        if (gLogCallBack)
        {
            gLogCallBack(UserDefine,id,strCustomLog);
        }
    }

	void CTSE_LogNet::UserDefinedLogFormat(int id,const wchar_t* strCustomLog,...)
	{
		std::wstring strLog;
		FORMAT_WSTRING(strLog,strCustomLog);
		if (gLogCallBack)
		{
			gLogCallBack(UserDefine,id,strLog.c_str());
		}
	}
}

