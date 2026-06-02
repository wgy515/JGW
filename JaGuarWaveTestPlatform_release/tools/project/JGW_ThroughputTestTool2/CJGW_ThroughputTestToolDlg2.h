#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "UIWkeWindowlessBrowser.h"
#include "CJGW_ThroughputTestTool2Controller.h"
namespace JGW
{
    class CCJGW_ThroughputTestToolDlg2 : public CCAsyncMsgDialog
    {
    public:
        CCJGW_ThroughputTestToolDlg2(void);
        ~CCJGW_ThroughputTestToolDlg2(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //!
        UILIB_GetWindowClassName(CCJGW_ThroughputTestToolDlg2);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
        //! 创建外部控件
        virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    private:
        //! 获取UI控件指针
        bool GetUIControl();
    private:
        CWkeWindowlessUI* mpWkeBrowser;
        CEditUI* mpIperfCommandEdt;
        CComboUI* mpTerminalType;
        CComboUI* mpProtocolType;
        CComboUI* mpNetCardInfoComBo;
        CEditUI* mpIperfIPEdt;
        CEditUI* mpSshIpAddressEdt;
        CEditUI* mpCellphoneIPEdt;
        CEditUI* mpTestTimeEdt;
        CEditUI* mpLogPrefixEdt;
        CButtonUI* mpStartBtn;
        CButtonUI* mpStopBtn;
        CButtonUI* mpLogFolderBtn;
        CRichEditUI* mpLogEdt;
        CRichEditUI* mpExtraParameterEdt;
        COptionUI* mpUpdateConfigOption;
        CEditUI* mpThreadCountEdt;
        CCheckBoxUI* mpCheckUseCellphoneIP;
        CCheckBoxUI* mpSaveIPQLogCheckBox;
        CVerticalLayoutUI *mpClientConfigHorizontalLayout;
    private:
        friend class CCJGW_ThroughputTestTool2Controller;
        CCJGW_ThroughputTestTool2Controller mUIController;
    };
}