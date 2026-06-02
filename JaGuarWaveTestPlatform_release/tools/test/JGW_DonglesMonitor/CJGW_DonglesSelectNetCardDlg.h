#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_IfTableTrafficMonitor.h"

namespace JGW
{
    class CCJGW_DonglesSelectNetCardDlg:public CCAsyncMsgDialog
    {
    public:
        CCJGW_DonglesSelectNetCardDlg(std::string& strNetCardInfo);
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
        std::string& mstrNetCardInfo;
        CCJGW_IfTableTrafficMonitor mIfTableTrafficMonitor;
    };
}


