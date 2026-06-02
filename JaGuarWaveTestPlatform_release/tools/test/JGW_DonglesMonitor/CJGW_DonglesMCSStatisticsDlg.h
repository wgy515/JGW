#pragma once
#include <UiLib/CUiLibWnd.h>

namespace JGW
{
    class CCJGW_DonglesMCSStatisticsDlg : public CCUiLibWnd
    {
    public:
        CCJGW_DonglesMCSStatisticsDlg(ULONGLONG (&uMcsStatistics)[13],HWND hParentHwnd = NULL);
        ~CCJGW_DonglesMCSStatisticsDlg(void);
    public:
         int DoModal();
    private:
        void OnCloseWindow();
        void OnInitWindow();
        UILIB_GetWindowClassName(CCJGW_DonglesMCSStatisticsDlg);
        void OnNotify(TNotifyUI& msg);
        void OnUpdateMCS();
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
    private:
        HWND m_hParentHwnd;

        CLabelUI* mpMcsLab[13];
        CButtonUI* mpResetStatisticsBtn;

        ULONGLONG (&muMcsStatistics)[13];
    };
}