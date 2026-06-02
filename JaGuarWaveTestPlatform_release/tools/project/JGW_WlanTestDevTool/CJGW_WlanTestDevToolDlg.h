#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_WlanTestDevThread.h"
#include "CJGW_WlanTestDevToolDefine.h"
namespace JGW
{
    class CCJGW_WlanTestDevToolDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_WlanTestDevToolDlg(void);
        ~CCJGW_WlanTestDevToolDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_WlanTestDevToolDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        void OnSelectWiFiTestLogPathBtnClick();
        void OnSelectSaveTemplateLogPathBtnClick();
        void OnSelectWiFiTemplatePathEdtBtnClick();
        void OnExecuteBtnClick();
    private:
        CEditUI* mpWiFiTestLogPathEdt;
        CButtonUI* mpSelectWiFiTestLogPathEdtBtn;

        CEditUI* mpWiFiTemplatePathEdt;
        CButtonUI* mpSelectWiFiTemplatePathEdtBtn;

        CEditUI* mpSaveTemplatePathEdt;
        CButtonUI* mpSelectSaveTemplatePathEdtBtn;

        CComboUI* mpTemperatureComBo;
        CButtonUI* mpExecuteBtn;
        CCJGW_WlanTestDevThread* mpWlanTestDevThread;
        S_WIFI_TEST_TEMPLATE_PARAM msWiFiTestTemplateParam;
    };

}

