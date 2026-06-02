#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "TSE_ThroughputConfig.h"
namespace JGW
{
    class CCJGW_ThroughTestConfigDlg : public CCUiLibWnd
    {
    public:
        CCJGW_ThroughTestConfigDlg(HWND hParentHwnd,S_THROUGHPUT_CONFIG& sThroughputConfig);
        ~CCJGW_ThroughTestConfigDlg(void);
        int					DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_ThroughTestConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
        void				OnCloseWindow();
    private:
        HWND m_hParentHwnd;
        CEditUI* mpWiFiServerAddressEdt;
        CEditUI* mpIperfIPAddressEdt;
        CEditUI* mpTestTimeEdt;
        CEditUI* mpTestRangeEdt;
        S_THROUGHPUT_CONFIG& msThroughputConfig;
    };
}