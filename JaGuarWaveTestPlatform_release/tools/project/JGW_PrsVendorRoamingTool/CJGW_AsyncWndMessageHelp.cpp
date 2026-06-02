#include "StdAfx.h"
#include "CJGW_AsyncWndMessageHelp.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_AsyncWndMessageHelp::CCJGW_AsyncWndMessageHelp(CCJGW_AsyncWndMessage* pAsyncWndMessage) : mpAsyncWndMessage(pAsyncWndMessage)
    {
    }


    CCJGW_AsyncWndMessageHelp::~CCJGW_AsyncWndMessageHelp(void)
    {
    }

    void CCJGW_AsyncWndMessageHelp::SendWindowMsg(const std::wstring& strMessage)
    {
        Log4WI(strMessage.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
    }

    void CCJGW_AsyncWndMessageHelp::SendWindowMsg(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        Log4WI(strLog.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
    }

    void CCJGW_AsyncWndMessageHelp::PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const std::wstring& strMessage)
    {
        mpAsyncWndMessage->PutAsyncWParamStringMessage(msgid,wParam,strMessage.c_str());
    }

    void CCJGW_AsyncWndMessageHelp::PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        Log4WI(strLog.c_str());
        PutAsyncWParamStringMessage(msgid,wParam,strLog);
    }
}
