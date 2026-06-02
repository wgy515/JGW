#include "StdAfx.h"
#include "CJGW_AttenuatorSettingDlg.h"

namespace JGW
{
    CCJGW_AttenuatorSettingDlg::CCJGW_AttenuatorSettingDlg(HWND hParentHwnd /* = NULL */):mhParentHwnd(hParentHwnd)
    {
    }


    CCJGW_AttenuatorSettingDlg::~CCJGW_AttenuatorSettingDlg(void)
    {
    }

    int CCJGW_AttenuatorSettingDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("attenutor_setting.xml"),_T("Attenuator Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\IperfTool.zip");
        CreateNoBorderStyleWindow(_T(""),_T("attenutor_setting.xml"),_T("Turntable Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\IperfTool"),_T("attenutor_setting.xml"),_T("Attenuator Setting"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_AttenuatorSettingDlg::OnInitWindow()
    {
        mpAttenutorValueEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"attenutorValueEdit"));
    }

    void CCJGW_AttenuatorSettingDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"SettingBtn")
            {
                float fValue = 50.00f;
                if (mpAttenutorValueEdit) fValue = static_cast<float>(_ttof(mpAttenutorValueEdit->GetText().GetData()));
                GetAttenuatorEquipment().SetAuttenuatorValue(fValue);
            }
        }
    }

    void CCJGW_AttenuatorSettingDlg::OnCloseWindow()
    {
    }
}