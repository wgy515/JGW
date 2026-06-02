#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include <poco/Zip/Decompress.h>
#include <poco/Zip/ZipLocalFileHeader.h>

namespace JGW
{
    typedef struct 
    {
        bool mbAutoUpdate;
        bool mbIsConfigVersion;
        std::string mstrUpdatePackFilePath;
        std::string mstrUpdateExecFolder;
        std::string mstrUpdateVersion;
    }S_UPDATE_HELP_CONFIG,*LPS_UPDATE_HELP_CONFIG;

    class CCJaGuarwaveUpdateHelpDlg : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarwaveUpdateHelpDlg(S_UPDATE_HELP_CONFIG& sUpdateHelpConfig);
        ~CCJaGuarwaveUpdateHelpDlg(void);
        UILIB_GetWindowClassName(CCJaGuarwaveUpdateHelpDlg);
    private:
        void OnInitWindow();

        void OnNotify(TNotifyUI& msg);

        void OnCloseWindow();

        void OnAppendRichEditTest(WPARAM wParam,LPARAM lParam);

        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //!
        void OnTimerMsg(int id);
    private:
        void OnRspSelectFolderBtnMsg();

        void OnDecompressOkCallback(const void* pSender,std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info);
        void OnDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
        void OnAutoUpdateTimer();

        void UpdateApplicationVersion();
    private:
        CEditUI* mpFolderPathEdt;
        CButtonUI* mpSelectFolderPath;
        CRichEditUI* mpUpdateLogRichEdt;
        DWORD _errCnt;
        S_UPDATE_HELP_CONFIG& msUpdateHelpConfig;
    };
}