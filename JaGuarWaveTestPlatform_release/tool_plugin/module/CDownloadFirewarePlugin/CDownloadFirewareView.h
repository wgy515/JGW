#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CDownloadFireware_Define.h"
#include "CDownloadFirewareThread.h"

namespace JGW
{
    class CCDownloadFirewareView:public CCJGW_CommanTestPlnView
    {
    public:
        CCDownloadFirewareView(void);
        ~CCDownloadFirewareView(void);
    private:
        void OnRichEditAppendTestMsg(const WPARAM& wParam,const LPARAM& lParam);
        void CloseSubWndPlugin();
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
    private://!domainlist
        CRichEditUI* mpDoMainRichEditUi;
        CCDownloadFirewareThread* mpTestThread;
        S_DOWNLOAD_FIREWARE_PARAM msDownloadFirewareParam;
    };
}


