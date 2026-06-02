#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CWiFiTestThread.h"
#include "CWiFiTest_Define.h"
namespace JGW
{
    class CCWiFiTestView:public CCJGW_CommanTestPlnView
    {
    public:
        CCWiFiTestView(void);
        ~CCWiFiTestView(void);
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
        CCWiFiTestThread* mpTestThread;
        size_t m_nListItemHeight;
        S_WLAN_CONFIG m_sWiFiTestConfig;
        //! S_WIFITEST_PARAM msWiFiTestParam;
    };
}


