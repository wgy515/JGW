#include "StdAfx.h"
#include "CJGW_TestLogAnalyticalTools.h"

namespace JGW
{
    CCJGW_TestLogAnalyticalToolsDlg::CCJGW_TestLogAnalyticalToolsDlg(void)
    {
    }


    CCJGW_TestLogAnalyticalToolsDlg::~CCJGW_TestLogAnalyticalToolsDlg(void)
    {
    }

    void CCJGW_TestLogAnalyticalToolsDlg::OnInitWindow()
    {
        mpTestStatus = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"testStatus"));
        if (!mpTestStatus) Close(IDCANCEL);
    }

    void CCJGW_TestLogAnalyticalToolsDlg::OnCloseWindow()
    {
    }

    void CCJGW_TestLogAnalyticalToolsDlg::OnNotify(TNotifyUI& msg)
    {

    }

    LRESULT CCJGW_TestLogAnalyticalToolsDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (WM_MOUSEMOVE == uMsg || WM_KEYDOWN == uMsg || WM_LBUTTONDOWN == uMsg)
        {
            Close(IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
}