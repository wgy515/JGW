#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <vector>

namespace JGW
{
    class CCJGW_TestLogAnalyticalToolsDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_TestLogAnalyticalToolsDlg(void);
        ~CCJGW_TestLogAnalyticalToolsDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_HtmlTestLogParseDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    private:
        CLabelUI* mpTestStatus;
    };
}