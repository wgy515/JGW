#include "StdAfx.h"
#include "CJGW_LossEdtDlg.h"

namespace JGW
{
    CCJGW_LossEdtDlg::CCJGW_LossEdtDlg(void)
    {
    }


    CCJGW_LossEdtDlg::~CCJGW_LossEdtDlg(void)
    {

    }

    std::wstring CCJGW_LossEdtDlg::GetInputLoss()
    {
        return mstrInputLoss;
    }

    void CCJGW_LossEdtDlg::OnInitWindow()
    {
        mpInputEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"inputEdit"));
        mpApplyBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"applyBtn"));
        mpCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"cancelBtn"));

        if (!mpInputEdit || !mpApplyBtn || !mpCancelBtn) Close(IDCANCEL);
    }

    void CCJGW_LossEdtDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpApplyBtn == msg.pSender)
            {
                mstrInputLoss = mpInputEdit->GetText();
                Close(IDOK);
            }
            else if (mpCancelBtn == msg.pSender)
            {
                Close(IDCANCEL);
            }
        }
    }

    void CCJGW_LossEdtDlg::OnCloseWindow()
    {

    }

    int CCJGW_LossEdtDlg::DoModal(HWND hParentHwnd)
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("edit_loss.xml"),_T("Loss Windows"),hParentHwnd,UI_WNDSTYLE_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\LossConfigTools.zip");
        CreateNoBorderStyleWindow(_T(""),_T("edit_loss.xml"),_T("Loss Windows"),hParentHwnd,UI_WNDSTYLE_FRAME);
#else
        CreateNoBorderStyleWindow(_T("skin\\LossConfigTools"),_T("edit_loss.xml"),_T("Loss Windows"),hParentHwnd,UI_WNDSTYLE_FRAME);
#endif
        CenterWindow();

        return ShowModal();
    }
}