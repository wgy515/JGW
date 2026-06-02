#pragma once
#include "resource.h"
#include "HelpPackageUIInterface.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    class CCJaGuarWavePackageUpdateToolDlgV2 : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarWavePackageUpdateToolDlgV2(void);
        ~CCJaGuarWavePackageUpdateToolDlgV2(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJaGuarWavePackageUpdateToolDlgV2);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 创建模块UI容器
        void BuildModuleContainer(const std::wstring& strSkinXmlPath);
        void ClearModuleContainer();
    private:
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        void OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam);
        void OnCommitHelpDBClickMsg();
    public:
        //! 线程调用异步回调消息
        bool OnHandleAsyncFuncMsg(int nActionIndex);
    private:
        void LoadHelpConfig();
    private:
        COptionUI *mpConfigOption;
        COptionUI *mpOTAOption;
        COptionUI *mpTaskOption;
        COptionUI *mpProjectOption;
        //CLabelUI* mpModuleInfoLabel;
        CControlUI* mpModuleContainer;
        CHorizontalLayoutUI* mpModuleContentHorizontalLayout;
        CHorizontalLayoutUI* mpMainHorizontalLayout;
        CRichEditUI* mpLogRichEdit;
        CButtonUI *mpCommitHelpDBBtn;
        CHelpPackageUIInterface* mpHelpPackageUIInterface;
        CCMessageThread* mpMessageThread;
        S_PACKAGE_UPDATE_CONFIG msPackageUpdateConfig;
    };

}

