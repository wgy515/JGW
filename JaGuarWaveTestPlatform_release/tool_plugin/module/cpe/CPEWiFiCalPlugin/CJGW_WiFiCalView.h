#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CWiFiCalTestThread.h"
//#include "CWiFiTest_Define.h"

namespace JGW
{
    class CCJGW_WiFiCalView : public CCJGW_CommanTestPlnView
    {
    public:
        CCJGW_WiFiCalView(void);
        ~CCJGW_WiFiCalView(void);
    private:
        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        void CloseSubWndPlugin();
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
        void OnSaveTest(bool bTestStatus);
    private://!domainlist
        CListTestCtrlUI* mpDoMainList;
        CCWiFiCalTestThread* mpTestThread;

        size_t m_nListItemHeight;

        S_WIFI_CAL_CONFIG msWiFiCalConfig;
    };
}