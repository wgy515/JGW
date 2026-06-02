#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include "CProductScreen_Define.h"
#include "CProductScreenThread.h"

namespace JGW
{
    class CCProductScreenView:public CCJGW_CommanTestPlnView
    {
    public:
        CCProductScreenView(void);
        ~CCProductScreenView(void);
    private:
        void OnRichEditAppendTestMsg(const WPARAM& wParam,const LPARAM& lParam);
        void CloseSubWndPlugin();
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
    private://!domainedt
        CRichEditUI* mpDoMainRichEditUi;
        CCProductScreenThread* mpTestThread;
        size_t m_nListItemHeight;
        S_PRODUCT_SCREEN_PARAM msProductScreenParam;
    };
}


