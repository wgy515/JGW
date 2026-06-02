#include "StdAfx.h"
#include "CJGW_AttenuatorRangeConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_AttenuatorRangeConfigDlg::CCJGW_AttenuatorRangeConfigDlg(S_ATTENUATOR_PARAM& sAttenuatorParam,HWND hParentHwnd /* = NULL */) : msAttenuatorParam(sAttenuatorParam),mhParentHwnd(hParentHwnd)
    {
    }


    CCJGW_AttenuatorRangeConfigDlg::~CCJGW_AttenuatorRangeConfigDlg(void)
    {
    }

    int CCJGW_AttenuatorRangeConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\IperfTool.zip");
        CreateNoBorderStyleWindow(_T(""),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\IperfTool"),_T("range_config.xml"),_T("Turntable Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_AttenuatorRangeConfigDlg::OnInitWindow()
    {
        //! init control ptr
        {
            bool bSuccess = true;
            mpStartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"startEdit"));
            mpEndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"endEdit"));
            mpRangeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"stepEdit"));
            mpTipslab = static_cast<CEditUI*>(m_PaintManager.FindControl(L"tipslab"));
            bSuccess &= (NULL != mpStartEdt);
            bSuccess &= (NULL != mpEndEdt);
            bSuccess &= (NULL != mpRangeEdt);
            bSuccess &= (NULL != mpTipslab);
            if (!bSuccess) { Close(); return ; }
            mpTipslab->SetText(L" Attenuator Config");
        }

        UpdateAttenuatorParamUI();
    }

    void CCJGW_AttenuatorRangeConfigDlg::OnCloseWindow()
    {

    }

    void CCJGW_AttenuatorRangeConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"applyBtn")
            {
                msAttenuatorParam.mfAttenutorStart = static_cast<float>(_ttof(mpStartEdt->GetText().GetData()));
                msAttenuatorParam.mfAttenutorEnd = static_cast<float>(_ttof(mpEndEdt->GetText().GetData()));
                msAttenuatorParam.mfAttenutorStep = static_cast<float>(_ttof(mpRangeEdt->GetText().GetData()));
                if (msAttenuatorParam.mfAttenutorStep <= 0) msAttenuatorParam.mfAttenutorStep = 5;
                if (msAttenuatorParam.mfAttenutorStart <= 0) msAttenuatorParam.mfAttenutorStart = 0;
                if (msAttenuatorParam.mfAttenutorEnd <= 0) msAttenuatorParam.mfAttenutorEnd = 0;
                if (msAttenuatorParam.mfAttenutorStart > msAttenuatorParam.mfAttenutorEnd) msAttenuatorParam.mfAttenutorStart = msAttenuatorParam.mfAttenutorEnd;
                Close(IDOK);
            }
            else if (msg.pSender->GetName() == L"closebtn")
            {
                Close(IDOK);
            }
        }
    }

    void CCJGW_AttenuatorRangeConfigDlg::UpdateAttenuatorParamUI()
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%.2f",msAttenuatorParam.mfAttenutorStart);
        mpStartEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%.2f",msAttenuatorParam.mfAttenutorEnd);
        mpEndEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%.2f",msAttenuatorParam.mfAttenutorStep);
        mpRangeEdt->SetText(strTemp.c_str());
    }
}