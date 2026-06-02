#pragma once
#include <UiLib/CUiLibWnd.h>
#include "CJGW_IPERF_TOOL_DEFINE.h"
namespace JGW
{
    class CCJGW_TRxSectorDlg : public CCUiLibWnd
    {
    public:
        CCJGW_TRxSectorDlg(S_IperfToolConfig& sIperfToolConfig,HWND hParentHwnd = NULL);
        ~CCJGW_TRxSectorDlg(void);
    public:
        int	DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_TRxSectorDlg);
        void OnInitWindow();
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
    private:
        HWND mhParentHwnd;
        CRichEditUI* mpRxSecortRichEditUI;
        CRichEditUI* mpTxSecortRichEditUI;
        S_IperfToolConfig& msIperfToolConfig;
    };
}