#pragma once
#include "HelpPackageUIInterface.h"
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    class CCJGW_HelpProjectDlg  : public CHelpPackageUIInterface
    {
    public:
        CCJGW_HelpProjectDlg(LPS_PACKAGE_UPDATE_CONFIG psPackageUpdateConfig);
        ~CCJGW_HelpProjectDlg(void);
    private:
        //! 初始化UI
        bool InitHelpPackageUI(CPaintManagerUI* pPaintManagerUI);
        //! 响应UI点击事件通知
        void OnNotifyHelpPackageUI(TNotifyUI& msg);
        //! 关闭UI事件回调
        void CloseHelpPackageUI();
        //! 处理客户事件消息
        void OnHandleEventCustomMessage(TEventUI* pTEventUI);
        //! 处理异步函数调用消息
        bool OnHandleAsyncFuncMsg(int nActionIndex);
    private:
        bool OnRspProjectUpdateBtnClickMsg();
        bool OnRspUploadProjectAddBtnClickMsg();
    private:
        MEMBER_VARIABLE_UI(CComboBoxUI,ProjectNameCombo);
        MEMBER_VARIABLE_UI(CEditUI,ProjectNameEdit);
        MEMBER_VARIABLE_UI(CEditUI,ProjectDesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,ProjectUpdateBtn);

        MEMBER_VARIABLE_UI(CEditUI,AddProjectNameEdit);
        MEMBER_VARIABLE_UI(CEditUI,AddProjectDesEdit);
        MEMBER_VARIABLE_UI(CButtonUI,UploadAddProjectPackageBtn);
    private:
        CPaintManagerUI* mpPaintManagerUI;
    };
}


