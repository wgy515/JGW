#pragma once
#include <UiLib/CAsyncMsgDialog.h>

namespace JGW
{
    class CCJGW_LossEdtDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_LossEdtDlg(void);
        ~CCJGW_LossEdtDlg(void);

    public:     
        std::wstring GetInputLoss();

        int	DoModal(HWND hParentHwnd);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_LossEdtDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        CEditUI* mpInputEdit;
        CButtonUI* mpApplyBtn;
        CButtonUI* mpCancelBtn;
        std::wstring mstrInputLoss;
    };
}