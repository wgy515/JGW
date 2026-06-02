#pragma once
#include <UiLib/CUiLibWnd.h>
namespace JGW
{
    class CCJGW_AttenuatorSettingDlg : public CCUiLibWnd
    {
    public:
        CCJGW_AttenuatorSettingDlg(HWND hParentHwnd = NULL);
        ~CCJGW_AttenuatorSettingDlg(void);
    public:
        int	DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_AttenuatorSettingDlg);
        void OnInitWindow();
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
    private:
        HWND mhParentHwnd;
        CEditUI* mpAttenutorValueEdit;
    };
}