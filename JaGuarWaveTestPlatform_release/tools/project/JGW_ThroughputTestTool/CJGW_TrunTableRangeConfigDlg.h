#pragma once
#include <UiLib/CUiLibWnd.h>
#include "CJGW_IPERF_TOOL_DEFINE.h"
namespace JGW
{
    class CCJGW_TrunTableRangeConfigDlg : public CCUiLibWnd
    {
    public:
        CCJGW_TrunTableRangeConfigDlg(S_TRUNTABLE_PARAM& sTruntableParam,HWND hParentHwnd = NULL);
        ~CCJGW_TrunTableRangeConfigDlg(void);
    public:
        int	DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_TruntableSettingDlg);
        void OnInitWindow();
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
    private:
        void UpdateTrunTableParamUI();
    private:
        HWND mhParentHwnd;
        CEditUI* mpStartEdt;
        CEditUI* mpEndEdt;
        CEditUI* mpRangeEdt;
        CLabelUI* mpTipslab;
        S_TRUNTABLE_PARAM& msTruntableParam;
    };
}