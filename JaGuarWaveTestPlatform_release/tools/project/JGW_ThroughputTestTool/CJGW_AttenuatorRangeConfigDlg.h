#pragma once
#include <UiLib/CUiLibWnd.h>
#include "CJGW_IPERF_TOOL_DEFINE.h"

namespace JGW
{
    class CCJGW_AttenuatorRangeConfigDlg : public CCUiLibWnd
    {
    public:
        CCJGW_AttenuatorRangeConfigDlg(S_ATTENUATOR_PARAM& sAttenuatorParam,HWND hParentHwnd = NULL);
        ~CCJGW_AttenuatorRangeConfigDlg(void);
    public:
        int	DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_TruntableSettingDlg);
        void OnInitWindow();
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
    private:
        void UpdateAttenuatorParamUI();
    private:
        HWND mhParentHwnd;
        CEditUI* mpStartEdt;
        CEditUI* mpEndEdt;
        CEditUI* mpRangeEdt;
        CLabelUI* mpTipslab;
        S_ATTENUATOR_PARAM& msAttenuatorParam;
    };
}