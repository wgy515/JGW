#include "StdAfx.h"
#include "DuiGpsConfig.h"

namespace JGW
{
    CDuiGpsConfig::CDuiGpsConfig(void)
    {
    }


    CDuiGpsConfig::~CDuiGpsConfig(void)
    {
    }

    int CDuiGpsConfig::DoModal( HWND hParentHwnd /* = NULL */ )
    {
        m_hParentHwnd = hParentHwnd;
#ifdef USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateDuiDialogWindow(m_hParentHwnd,_T(""),_T("GpsConfig.xml"),UI_WNDSTYLE_FRAME);
#else
        CreateDuiDialogWindow(m_hParentHwnd,_T("skin\\Login"),_T("GpsConfig.xml"),UI_WNDSTYLE_FRAME);
#endif

        SetFocus(m_hParentHwnd);
        CenterWindow();

        return ShowModal();
    }

    LPCTSTR CDuiGpsConfig::GetWindowClassName() const
    {
        return L"CDuiGpsConfig";
    }

    UINT CDuiGpsConfig::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    bool CDuiGpsConfig::InitWindow()
    {
        m_pCancelbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));

        if( !m_pCancelbtn )
        {
            Close(IDCANCEL);
            return false;
        }
        return true;
    }

    void CDuiGpsConfig::Notify(TNotifyUI& msg)
    {
        if ( msg.sType == DUI_MSGTYPE_CLICK )
        {
            if ( m_pCancelbtn == msg.pSender )
            {
                Close(IDCANCEL);//PostQuitMessage(0);		
                return ;
            }
        }
    }
}