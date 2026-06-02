#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "UIWkeWindowlessBrowser.h"
#include "CJGW_IPERF_TOOL_DEFINE.h"
#include "CJGW_IperfToolController.h"
#include "CJGW_IperfTestThread.h"

namespace JGW
{
    class CCJGW_IperfToolDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_IperfToolDlg(void);
        ~CCJGW_IperfToolDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_DonglesThroughputDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
        //! 创建外部控件
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    private:
        void InitPropertyConfig();
        //!
        void UpdatePropertyConfig();
        //! 获取UI控件指针
        bool GetUIControl();
        //!
        void UpdateIperfUIInfo();
        //! 
        void UpdateIperfToolConfigParam();
        //! 
        void UpdateIperfCommand();
        //!
        void OnUpdateConfigInfoUI();
    private:
        void BuildIperfOutputLogFilePath();
        void EnableTestUI(bool bEnable = false);
    private:
        CComboUI* mpTerminalType;
        CComboUI* mpProtocolType;

        CVerticalLayoutUI* mpClientConfigHorizontalLayout;
        CEditUI* mpIpAddressEdt;
        CEditUI* mpTestTimeEdt;
        CEditUI* mpBandWidthEdt;
        CEditUI* mpThreadEdt;

        CRichEditUI* mpExtraParameterEdt; //! 附加参数
        CRichEditUI* mpLogEdt;
        CRichEditUI* mpConfigInfoEdt;
        CWkeWindowlessUI* mpWkeBrowser;

        CButtonUI* mpStartBtn;
        CButtonUI* mpStopBtn;

        S_IperfToolConfig msIperfToolConfig;
        friend class CCJGW_IperfToolController;
        CCJGW_IperfToolController mUIController;
        CCJGW_IperfTestThread mTestThread;
    };
}