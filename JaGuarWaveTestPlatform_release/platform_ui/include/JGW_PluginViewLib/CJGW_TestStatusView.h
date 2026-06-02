#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <vector>

namespace JGW
{
    class CCJGW_TestStatusView : public CCAsyncMsgDialog
    {
    public:
        CCJGW_TestStatusView(HWND hParentHwnd  = NULL );
        ~CCJGW_TestStatusView(void);
    public:
        int DoModal(bool bTestStatus,HWND hParentHwnd = NULL);
        void CloseWindow();
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_TestStatusView);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    private:
        CLabelUI* mpTestStatus;
        HWND m_hParentHwnd;
        bool mbTestStatus;
    };
}