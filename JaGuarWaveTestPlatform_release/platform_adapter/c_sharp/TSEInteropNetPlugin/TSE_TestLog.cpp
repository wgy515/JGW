#include "StdAfx.h"
#include "TSE_TestLog.h"

namespace JGW
{
    void CTSE_TestLog::OnDebugLog(const wchar_t* strDebugLog)
    {
        if (gobalLogCallBack)
        {
            gobalLogCallBack(E_INTEROP_DEBUG_TSE_LOG,0,strDebugLog);
        }
    }

    void CTSE_TestLog::OnInfoLog(const wchar_t* strInfoLog)
    {
        if (gobalLogCallBack)
        {
            gobalLogCallBack(E_INTEROP_INFO_TSE_LOG,0,strInfoLog);
        }
    }

    void CTSE_TestLog::OnErrorLog(const wchar_t* strErrorLog)
    {
        if (gobalLogCallBack)
        {
            gobalLogCallBack(E_INTEROP_ERROR_TSE_LOG,0,strErrorLog);
        }
    }

    void CTSE_TestLog::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        if (gobalLogCallBack)
        {
            gobalLogCallBack(E_INTEROP_USERDEFINE_TSE_LOG,id,strCustomLog);
        }
    }
}