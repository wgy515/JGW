#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "CTSEMultipleTestDefine.h"
namespace JGW
{
    class CCTSEMultipleTestConfigDlg : public CCUiLibWnd
    {
    public:
        CCTSEMultipleTestConfigDlg(HWND hParentHwnd,PS_MULTIPLE_CONFIG psMultipleConfig);
        ~CCTSEMultipleTestConfigDlg(void);
    public:
        int DoModal();
    private:
        UILIB_GetWindowClassName(CCTSEMultipleTestConfigDlg);
        void OnInitWindow();
        virtual UINT GetClassStyle() const;
        void OnNotify(TNotifyUI& msg);
    private:
        HWND mhParentHwnd;
        PS_MULTIPLE_CONFIG mpsMultipleConfig;

        CCheckBoxUI* mpIsNetcardDeviceCheckBox;
        CCheckBoxUI* mpIsSaveDownloadLogComCheckBox;
        CCheckBoxUI* mpIsDownloadUpgradeCheckBox;
        CCheckBoxUI* mpIsFirewareFileCheckBox;
        CEditUI* mpTestTimeEdt;
        CEditUI* mpTestTimeOutEdt;
        CComboUI* mpRowCombo;
        CComboUI* mpColumnCombo;
        CEditUI* mpUpgradeVersionEdt;
        CEditUI* mpLogPathEdt;
        CButtonUI* mpOkBtn;
        CButtonUI* mpCancelBtn;
        CButtonUI* mpClearAllSortBtn;
    };
}