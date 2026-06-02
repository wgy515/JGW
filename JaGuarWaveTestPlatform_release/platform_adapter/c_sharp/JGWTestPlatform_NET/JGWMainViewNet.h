#pragma once
#include <UiLib/CDuiDelegateDlg.h>

namespace JGW
{
    class JGWMainViewNet : public CDuiDelegateDlg
    {
    public:
        JGWMainViewNet();
        ~JGWMainViewNet(void);
    protected:
        virtual LPCTSTR GetWindowClassName() const;

        void Notify(TNotifyUI& msg);
    public:
        HWND DoModal(HWND hMaindWnd);

        UINT GetClassStyle() const;
        //void SubClassWindow(HWND hMaindWnd);
    private:
        HWND m_hParentHwnd;
    };
}

