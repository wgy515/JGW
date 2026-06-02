#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "UIWkeWindowlessBrowser.h"
#include "CJGW_PrsVendorRoamingToolDlgController.h"
#include "CJGWPrsStatusBrowserInterface.h"

#define MAX_AP_NUM 10
#define MAX_STA_NUM 2
namespace JGW
{
    class CCJGW_PrsVendorRoamingToolDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_PrsVendorRoamingToolDlg(void);
        ~CCJGW_PrsVendorRoamingToolDlg(void);
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
        CCJGWPrsStatusBrowserInterface* mpWkeBrowser;
        CEditUI* mpIperfCommandEdt;
        CComboUI* mpTerminalType;
        CComboUI* mpProtocolType;
        //CComboUI* mpNetCardInfoComBo;
        CEditUI* mpIperfIPEdt;
        CEditUI* mpSshIpAddressEdt;
        //CEditUI* mpCellphoneIPEdt;
        CEditUI* mpTestTimeEdt;
        CEditUI* mpLogPrefixEdt;
        CButtonUI* mpStartBtn;
        CButtonUI* mpStopBtn;
        CButtonUI* mpLogFolderBtn;
		CButtonUI* mpCloseBtn;
        
        CRichEditUI* mpLogEdt;
        CRichEditUI* mpExtraParameterEdt;
        COptionUI* mpUpdateConfigOption;
        CEditUI* mpThreadCountEdt;


        CCheckBoxUI* mpLogDebugTestCheckBox;
        CHorizontalLayoutUI* mpLogDebugTestHorizontalLayout;
        CButtonUI* mpLogDebugBtn;
        CEditUI* mpLogDebugFilePathEdt;

		CComboUI* mpApMacCombo;
		CRichEditUI* mpApPrsRoamingInfoRichEdit;

		CComboUI* mpStaMacCombo;
		CEditUI* mpConnectApMacEdit;
		CRichEditUI* mpStaPrsRoamingInfoRichEdit;
		CVerticalLayoutUI* mpStaPrsRoamingInfoVerticalLayout;

        CCheckBoxUI* mpSaveIPQLogCheckBox;
        CVerticalLayoutUI *mpClientConfigHorizontalLayout;
    private:
        friend class CCJGW_PrsVendorRoamingToolDlgController;
        CCJGW_PrsVendorRoamingToolDlgController mUIController;
    };
}
