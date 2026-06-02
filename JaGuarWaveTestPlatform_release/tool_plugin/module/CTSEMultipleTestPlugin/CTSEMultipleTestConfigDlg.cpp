#include "StdAfx.h"
#include "CTSEMultipleTestConfigDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCTSEMultipleTestConfigDlg::CCTSEMultipleTestConfigDlg(HWND hParentHwnd,PS_MULTIPLE_CONFIG psMultipleConfig) : mhParentHwnd(hParentHwnd),mpsMultipleConfig(psMultipleConfig)
    {
    }


    CCTSEMultipleTestConfigDlg::~CCTSEMultipleTestConfigDlg(void)
    {
    }

    int CCTSEMultipleTestConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("multiple_upgrade_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("multiple_upgrade_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("multiple_upgrade_config.xml"),_T("JaGuarWave Multiple Test Config"),mhParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    UINT CCTSEMultipleTestConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    void CCTSEMultipleTestConfigDlg::OnInitWindow()
    {
        //! get ui ptr class
        {
            bool bSucceseful = true;

            mpIsNetcardDeviceCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsNetcardDeviceCheckBox"));
            mpIsSaveDownloadLogComCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsSaveDownloadLogComCheckBox"));
            mpIsDownloadUpgradeCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsDownloadUpgradeCheckBox"));
            mpIsFirewareFileCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"IsFirewareFileCheckBox"));
            mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeEdt"));
            mpTestTimeOutEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TestTimeOutEdt"));
            mpRowCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"RowCombo"));
            mpColumnCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"ColumnCombo"));
            mpUpgradeVersionEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"UpgradeVersionEdt"));
            mpLogPathEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"LogPathEdt"));
            mpOkBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"OkBtn"));
            mpCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"CancelBtn"));
            mpClearAllSortBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"ClearAllSortBtn"));

            bSucceseful &= (NULL != mpIsNetcardDeviceCheckBox);
            bSucceseful &= (NULL != mpIsSaveDownloadLogComCheckBox);
            bSucceseful &= (NULL != mpIsDownloadUpgradeCheckBox);
            bSucceseful &= (NULL != mpIsFirewareFileCheckBox);
            bSucceseful &= (NULL != mpTestTimeEdt);
            bSucceseful &= (NULL != mpTestTimeOutEdt);
            bSucceseful &= (NULL != mpRowCombo);
            bSucceseful &= (NULL != mpColumnCombo);
            bSucceseful &= (NULL != mpUpgradeVersionEdt);
            bSucceseful &= (NULL != mpLogPathEdt);
            bSucceseful &= (NULL != mpOkBtn);
            bSucceseful &= (NULL != mpCancelBtn);
            bSucceseful &= (NULL != mpClearAllSortBtn);

            if (!bSucceseful)
            {
                Close(IDCANCEL);
                return ;
            }
        }
        // init config 
        {
            mpIsNetcardDeviceCheckBox->SetCheck(!mpsMultipleConfig->mbIsUsbDevice);
            mpIsSaveDownloadLogComCheckBox->SetCheck(mpsMultipleConfig->mbSaveTestLog);
            mpIsDownloadUpgradeCheckBox->SetCheck(mpsMultipleConfig->mbIsDownloadUpgrade);
            mpIsFirewareFileCheckBox->SetCheck(mpsMultipleConfig->mbBinaryImageIsFolder);
            mpTestTimeEdt->SetText(JGW_GetFormatWString(L"%u",mpsMultipleConfig->mnDownloadTimeSec).c_str());
            mpTestTimeOutEdt->SetText(JGW_GetFormatWString(L"%u",mpsMultipleConfig->mnDownloadTimeOutSec).c_str());
            mpRowCombo->SelectItem(mpsMultipleConfig->mnMultipleTestRow - 1);
            mpColumnCombo->SelectItem(mpsMultipleConfig->mnMultipleTestCol - 1);
            mpUpgradeVersionEdt->SetText(mpsMultipleConfig->mstrUpgradeVersion.c_str());
            mpLogPathEdt->SetText(mpsMultipleConfig->mstrSaveTestLogFolder.c_str());
        }
    }

    void CCTSEMultipleTestConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpOkBtn == msg.pSender)
            {
                mpsMultipleConfig->mbIsUsbDevice = !mpIsNetcardDeviceCheckBox->GetCheck();
                mpsMultipleConfig->mbSaveTestLog = mpIsSaveDownloadLogComCheckBox->GetCheck();
                mpsMultipleConfig->mbIsDownloadUpgrade = mpIsDownloadUpgradeCheckBox->GetCheck();
                mpsMultipleConfig->mbBinaryImageIsFolder = mpIsFirewareFileCheckBox->GetCheck();
                mpsMultipleConfig->mnDownloadTimeSec = _ttoi(mpTestTimeEdt->GetText().GetData());
                mpsMultipleConfig->mnDownloadTimeOutSec = _ttoi(mpTestTimeOutEdt->GetText().GetData());
                mpsMultipleConfig->mnMultipleTestRow = mpRowCombo->GetCurSel() + 1;
                mpsMultipleConfig->mnMultipleTestCol = mpColumnCombo->GetCurSel() + 1;
                mpsMultipleConfig->mstrUpgradeVersion = mpUpgradeVersionEdt->GetText().GetData();
                mpsMultipleConfig->mstrSaveTestLogFolder = mpLogPathEdt->GetText().GetData();

                Close(IDOK);
            }
            else if(mpCancelBtn == msg.pSender)
            {
                Close(IDCANCEL);
            }
            else if(mpClearAllSortBtn == msg.pSender)
            {
                mpsMultipleConfig->mbClearSortIndex = true;
            }
        }
    }
}
