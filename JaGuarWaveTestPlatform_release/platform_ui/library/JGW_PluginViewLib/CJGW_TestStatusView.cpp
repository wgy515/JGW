#include "StdAfx.h"
#include <JGW_PluginViewLib/CJGW_TestStatusView.h>

namespace JGW
{
    CCJGW_TestStatusView::CCJGW_TestStatusView(HWND hParentHwnd /* = NULL */):m_hParentHwnd(hParentHwnd),mbTestStatus(true)
    {
    }


    CCJGW_TestStatusView::~CCJGW_TestStatusView(void)
    {
    }

    void CCJGW_TestStatusView::CloseWindow()
    {
        if (NULL == m_PaintManager.GetPaintWindow()) return ;
        if (!::IsWindow(m_PaintManager.GetPaintWindow())) return ;
        Close();
    }

    int CCJGW_TestStatusView::DoModal(bool bTestStatus,HWND hParentHwnd /* = NULL */)
    {
        //! m_hParentHwnd = hParentHwnd;
        mbTestStatus = bTestStatus;
        RECT rect = {0,0,500,200};
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindowToRect(_T(""),_T("test_status_view.xml"),_T("Log Windows"),rect,m_hParentHwnd,UI_WNDSTYLE_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindowToRect(_T(""),_T("test_status_view.xml"),_T("Log Windows"),rect,m_hParentHwnd,UI_WNDSTYLE_FRAME);
#else
        CreateNoBorderStyleWindowToRect(_T("skin\\JaGuarWave"),_T("test_status_view.xml"),_T("Log Windows"),rect,m_hParentHwnd,UI_WNDSTYLE_FRAME);
#endif
        CenterWindow();

        SetWindowPos(m_PaintManager.GetPaintWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

        return 1/*ShowModal()*/;
    }

    void CCJGW_TestStatusView::OnInitWindow()
    {
        mpTestStatus = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"testStatus"));
        if (!mpTestStatus) Close(IDCANCEL);

        if (mbTestStatus)
        {
            mpTestStatus->SetText(L"PASS");
            mpTestStatus->SetTextColor(0x00ff00);
        }
        else
        {
            mpTestStatus->SetText(L"FAIL");
            mpTestStatus->SetTextColor(0xffff0000);
        }
    }

    void CCJGW_TestStatusView::OnCloseWindow()
    {
    }

    void CCJGW_TestStatusView::OnNotify(TNotifyUI& msg)
    {

    }

    LRESULT CCJGW_TestStatusView::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (WM_MOUSEMOVE == uMsg || WM_KEYDOWN == uMsg || WM_LBUTTONDOWN == uMsg)
        {
            Close(IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
}