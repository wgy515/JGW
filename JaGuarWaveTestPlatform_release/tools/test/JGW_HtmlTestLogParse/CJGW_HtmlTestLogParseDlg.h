#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <vector>

namespace JGW
{
    class CCJGW_HtmlTestLogParseDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_HtmlTestLogParseDlg(void);
        ~CCJGW_HtmlTestLogParseDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_HtmlTestLogParseDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    };
}