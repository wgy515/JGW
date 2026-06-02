#include "StdAfx.h"
#include "CSTSE_LogListenerEmpty.h"

namespace JGW
{
    CCSTSE_LogListenerEmpty::CCSTSE_LogListenerEmpty(void)
    {
    }


    CCSTSE_LogListenerEmpty::~CCSTSE_LogListenerEmpty(void)
    {
    }

    CCSTSE_LogListenerEmpty* CCSTSE_LogListenerEmpty::GetLogListenerEmpty()
    {
        static CCSTSE_LogListenerEmpty logListenerEmpty;
        return & logListenerEmpty;
    }

    void CCSTSE_LogListenerEmpty::OnDebugLog(const wchar_t* strDebugLog)
    {

    }

    void CCSTSE_LogListenerEmpty::OnErrorLog(const wchar_t* strErrorLog)
    {

    }

    void CCSTSE_LogListenerEmpty::OnInfoLog(const wchar_t* strInfoLog)
    {

    }

    void CCSTSE_LogListenerEmpty::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {

    }
}
