#pragma once
#include <UiLib/CUiLibWnd.h>
#include "CJGW_IPERF_TOOL_DEFINE.h"
#include "SCLLibHelper.h"
namespace JGW
{
    class CCJGW_TruntableSettingDlg : public CCUiLibWnd
    {
    public:
        CCJGW_TruntableSettingDlg(S_IperfToolConfig& sIperfToolConfig,SCLLibHelper& cSCLLibHelper,HWND hParentHwnd = NULL);
        ~CCJGW_TruntableSettingDlg(void);
    public:
        int	DoModal();

        static int FindTruntableComPort();
    private:
        UILIB_GetWindowClassName(CCJGW_TruntableSettingDlg);
        void OnInitWindow();
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
    private:
        void UpdateConfigParam();

        void OnStartPositionRotate();

        void OnStopPositionRotate();
    private:
        HWND mhParentHwnd;

        CEditUI* mpVelocityEdit;
        CEditUI* mpAccelerationEdit;
        CEditUI* mpDecelerationEdit;
        CEditUI* mpPositionEdit; //! 正数 CW  负数 CCW

        CButtonUI* mpStartBtn;
        CButtonUI* mpStopBtn;
        CButtonUI* mpApplyBtn;
        SCLLibHelper& mcSCLLibHelper;
        int mnPosition;
        S_IperfToolConfig& msIperfToolConfig;
    };
}