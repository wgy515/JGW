#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "UIWkeWindowlessBrowser.h"

namespace JGW
{
    class CCJGW_LogWindowsView:public CCAsyncMsgDialog
    {
    public:
        CCJGW_LogWindowsView(HWND hParentHwnd = NULL);
        ~CCJGW_LogWindowsView(void);

        int					DoModal();
    protected:
        CControlUI* CreateControl(LPCTSTR pstrClass);
    private:
        virtual LPCTSTR		GetWindowClassName() const;
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
    
        void				OnNotify(TNotifyUI& msg);
    private:
        CWkeWindowlessUI*   mpWkeBrowser;
        HWND				m_hParentHwnd;
        CRichEditUI*		m_plogedt;
        COptionUI*			m_pdebugOption;
        COptionUI*			m_perrorOption;
        std::wstring        m_strHtmlPath;
    };
}

