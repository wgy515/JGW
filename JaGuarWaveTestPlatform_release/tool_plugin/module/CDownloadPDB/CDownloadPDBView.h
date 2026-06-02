#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CDownloadPDB_Define.h"
#include "CDownloadPDBThread.h"

namespace JGW
{
    class CCDownloadPDBView:public CCJGW_CommanTestPlnView
    {
    public:
        CCDownloadPDBView(void);
        virtual ~CCDownloadPDBView(void);
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
        CCDownloadPDBThread* mpTestThread;
        size_t m_nListItemHeight;
        S_DOWNLOAD_PDB_PARAM msDownloadPDBParam;
    };
}


