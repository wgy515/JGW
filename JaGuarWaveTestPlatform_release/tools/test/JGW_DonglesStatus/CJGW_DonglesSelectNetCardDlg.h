#pragma once
#include <UiLib/CAsyncMsgDialog.h>

namespace JGW
{
    class CCJGW_DonglesSelectNetCardDlg:public CCAsyncMsgDialog
    {
    public:
        CCJGW_DonglesSelectNetCardDlg(std::wstring& strNetCardInfo);
        ~CCJGW_DonglesSelectNetCardDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_DonglesSelectNetCardDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        CComboUI* mpSelNetCardCombo;
        std::wstring& mstrNetCardInfo;
    };
}


