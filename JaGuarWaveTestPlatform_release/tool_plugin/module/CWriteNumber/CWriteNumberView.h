#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CWriteNumber_Define.h"
#include "CWriteNumberThread.h"

namespace JGW
{
    class CCWriteNumberView:public CCJGW_CommanTestPlnView
    {
    public:
        CCWriteNumberView(void);
        ~CCWriteNumberView(void);
    private:
        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        void CloseSubWndPlugin();
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
    private://!domainlist
        CListTestCtrlUI* mpDoMainList;
        CCWriteNumberThread* mpTestThread;
        size_t m_nListItemHeight;
        S_DOWNLOAD_PDB_PARAM msDownloadPDBParam;
    };

}

