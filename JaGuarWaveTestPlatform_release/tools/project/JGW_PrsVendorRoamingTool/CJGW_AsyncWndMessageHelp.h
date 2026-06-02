#pragma once
#include "CJGW_PrsVendorRoamingToolDlgController.h"

namespace JGW
{
    class CCJGW_AsyncWndMessageHelp
    {
    public:
        CCJGW_AsyncWndMessageHelp(CCJGW_AsyncWndMessage* pAsyncWndMessage);
        ~CCJGW_AsyncWndMessageHelp(void);
    public:
        //! 发送测试消息到窗口
        void SendWindowMsg(const std::wstring& strMessage);
        void SendWindowMsg(const wchar_t* szFormat,...);
        //! 异步字符串消息
        void PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const std::wstring& strMessage);
        //! 异步字符串消息
        void PutAsyncWParamStringMessage(UINT msgid,WPARAM wParam,const wchar_t* szFormat,...);
    private:
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
    };

}

