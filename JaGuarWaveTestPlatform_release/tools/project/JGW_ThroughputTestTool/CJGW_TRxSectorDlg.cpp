#include "StdAfx.h"
#include "CJGW_TRxSectorDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{ 
    CCJGW_TRxSectorDlg::CCJGW_TRxSectorDlg(S_IperfToolConfig& sIperfToolConfig,HWND hParentHwnd /* = NULL */) : mhParentHwnd(hParentHwnd),msIperfToolConfig(sIperfToolConfig)
    {
    }


    CCJGW_TRxSectorDlg::~CCJGW_TRxSectorDlg(void)
    {
    }

    int CCJGW_TRxSectorDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("sector_config.xml"),_T("Sector Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\IperfTool.zip");
        CreateNoBorderStyleWindow(_T(""),_T("sector_config.xml"),_T("Sector Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\IperfTool"),_T("sector_config.xml"),_T("Sector Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_TRxSectorDlg::OnInitWindow()
    {
        mpRxSecortRichEditUI = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"RxSectorEdt"));
        mpTxSecortRichEditUI = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"TxSectorEdt"));
        if (mpTxSecortRichEditUI) mpTxSecortRichEditUI->SetText(msIperfToolConfig.mstrConfigTxSectors.c_str());
        if (mpRxSecortRichEditUI) mpRxSecortRichEditUI->SetText(msIperfToolConfig.mstrConfigRxSectors.c_str());
    }

    void CCJGW_TRxSectorDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"applyBtn")
            {
                if (mpRxSecortRichEditUI)
                {
                    msIperfToolConfig.mvRxSector.clear();
                    msIperfToolConfig.mstrConfigRxSectors = mpRxSecortRichEditUI->GetText().GetData();
                    JGW_ParserStrW(mpRxSecortRichEditUI->GetText().GetData(),L",",msIperfToolConfig.mvRxSector);
                }
                if (mpTxSecortRichEditUI)
                {
                    msIperfToolConfig.mvTxSector.clear();
                    msIperfToolConfig.mstrConfigTxSectors = mpTxSecortRichEditUI->GetText().GetData();
                    JGW_ParserStrW(mpTxSecortRichEditUI->GetText().GetData(),L",",msIperfToolConfig.mvTxSector);
                }
                Close(IDOK);
            }
            else if (msg.pSender->GetName() == L"closebtn")
            {
                Close(IDCANCEL);
            }
        }
    }

    void CCJGW_TRxSectorDlg::OnCloseWindow()
    {
    }
}