#pragma once
#include <UiLib/CDuiDialog.h>

namespace JGW
{
    class CDuiGpsConfig:public CDuiDialog
    {
    public:
        CDuiGpsConfig(void);
        ~CDuiGpsConfig(void);

        int		DoModal( HWND hParentHwnd = NULL );

    private:
        void				Notify(TNotifyUI& msg);
        virtual LPCTSTR		GetWindowClassName() const;
        virtual UINT		GetClassStyle() const;

        bool				InitWindow();
    private:
        HWND				m_hParentHwnd;
        CButtonUI*			m_pCancelbtn;
        CButtonUI*			m_pSaveBtn;
    };
}