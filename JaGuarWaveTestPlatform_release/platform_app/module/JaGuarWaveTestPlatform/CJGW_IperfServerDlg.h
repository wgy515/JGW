#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>

namespace JGW
{
    class CCJGW_IperfServerDlg : public CCUiLibWnd
    {
    public:
        CCJGW_IperfServerDlg(HWND hParentHwnd,LPS_MES_INFO psMesInfo);
        ~CCJGW_IperfServerDlg(void);
        UILIB_GetWindowClassName(CCJGW_IperfServerDlg);
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
        void OnClickOkBtn();
    private:
        HWND m_hParentHwnd;
        LPS_MES_INFO mpsMesInfo;
        CEditUI* mpIperfFilePath;
        CEditUI* mpIpAddressEdt;
        CEditUI* mpExtraParameterEdt;
    };
}