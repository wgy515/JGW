#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CRFAntTestThread.h"
#include "ConfigFile.h"
namespace JGW
{
    class CCRFAntTestView:public CCJGW_CommanTestPlnView
    {
    public:
        CCRFAntTestView(void);
        ~CCRFAntTestView(void);
    private:
        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        void CloseSubWndPlugin();
        
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        virtual void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        bool OnFailTestStatus();
        bool LoadConfig();
		void OnSaveTest(bool bTestStatus);
    private://!domainlist
        bool m_bTestThreadStatus;
        CListTestCtrlUI* mpDoMainList;
        CCRFAntTestThread* mpTestThread;
        size_t m_nListItemHeight;
        CConfigFile	m_configFile;
    };
}


