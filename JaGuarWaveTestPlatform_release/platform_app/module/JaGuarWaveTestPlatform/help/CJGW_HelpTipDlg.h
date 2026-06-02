#pragma once
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include <UiLib/CAsyncMsgDialog.h>

namespace JGW
{
    class CCJGW_HelpTipDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_HelpTipDlg(S_JGW_HELP_CONFIG& sHelpConfig,HWND hParentHwnd = NULL);
        ~CCJGW_HelpTipDlg(void);
    public:
        int	DoModal(HWND hMainHwnd,E_JGW_HELP_AUTO_UPDATE_TYPE autoUpdateType,RECT rcClient);
    private:
        UILIB_GetWindowClassName(CCJGW_HelpTipDlg);
        void OnInitWindow();
        virtual UINT GetClassStyle() const;
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();
        //!
        void OnRspHelpDownloadMsg();
        //!
        void OnRspUpdateDownloadMsg();
    private:
        HWND m_hParentHwnd;
        HWND mhMainHwnd;
        E_JGW_HELP_AUTO_UPDATE_TYPE mAutoUpdateType;
        S_JGW_HELP_CONFIG& msHelpConfig;
    };
}