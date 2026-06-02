#pragma once
/*#include "CJGW_UpdateClientThread.h"*/
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
namespace JGW
{
    class CCJGW_HelpDlg : public CCUiLibWnd
    {
    public:
        CCJGW_HelpDlg(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_HelpDlg(void);

        UILIB_GetWindowClassName(CCJGW_HelpDlg);
    public:
        int	DoModal();
    private:
        void OnInitWindow();

        void OnNotify(TNotifyUI& msg);
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //!
        void OnCloseWindow();
        //!
        void OnRspUpdateProgressTimer();

        void OnCloseBtn(TNotifyUI& msg);
        //!
        void OnRspHelpDownloadMsg();
        //!
        void OnRspUpdateDownloadMsg();
    private:
        HWND m_hParentHwnd;
        CProgressUI* mpUpdateProgressUI;
        CCMessageThread* mpHelpModuleThread;
        //CCJGW_UpdateClientThread& mUpdateClientThread;
        S_JGW_HELP_CONFIG& msHelpConfig;
    };
}