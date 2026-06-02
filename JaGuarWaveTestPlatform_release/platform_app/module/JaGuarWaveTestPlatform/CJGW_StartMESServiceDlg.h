#pragma once
#include <UiLib/CUiLibWnd.h>

namespace JGW
{
    class CCJGW_StartMESServiceDlg : public CCUiLibWnd
    {
    public:
        CCJGW_StartMESServiceDlg(HWND hParentHwnd = NULL);
        ~CCJGW_StartMESServiceDlg(void);
        UILIB_GetWindowClassName(CCJGW_StartMESServiceDlg);
    public:
        int	DoModal();
    private:
        void OnInitWindow();

        virtual UINT GetClassStyle() const;

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