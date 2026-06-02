#pragma once
#include <UiLib/CUiLibWnd.h>

namespace JGW
{
    class CCJGW_StartHelpServiceDlg : public CCUiLibWnd
    {
    public:
        CCJGW_StartHelpServiceDlg(HWND hParentHwnd = NULL);
        ~CCJGW_StartHelpServiceDlg(void);
        UILIB_GetWindowClassName(CCJGW_StartHelpServiceDlg);
    public:
        int	DoModal();
    private:
        void OnInitWindow();

        void OnNotify(TNotifyUI& msg);
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage( TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam );
        //!
        void OnCloseWindow();
    private:
        void StartMESServiceApplication();
    private:
        HWND m_hParentHwnd;
        CLabelUI* mpMESTipInfo;
    };
}