#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "CMultipleTestPluginDefine.h"

namespace JGW
{
    class CCMultipleTestConfigDlg : public CCUiLibWnd
    {
    public:
        CCMultipleTestConfigDlg(HWND hParentHwnd,PS_MULTIPLE_CONFIG psMultipleConfig);
        ~CCMultipleTestConfigDlg(void);
    public:
        int DoModal();
    private:
        UILIB_GetWindowClassName(CCMultipleTestConfigDlg);
        void OnInitWindow();
        virtual UINT GetClassStyle() const;
        void OnNotify(TNotifyUI& msg);
    private:
        void SetDeviceModeCombo(E_DEVICE_MODE& eDeviceMode,CComboUI* pDeviceModeCombo);
        E_DEVICE_MODE GetDeviceModeCombo(CComboUI* pDeviceModeCombo);
        void OnChangeDeviceQueueComboUI();
    private:
        HWND mhParentHwnd;
        PS_MULTIPLE_CONFIG mpsMultipleConfig;

        CCheckBoxUI* mpIsSaveDownloadLogComCheckBox;
        CCheckBoxUI* mpIsFileCheckBox;

        CEditUI* mpTestTimeEdt;
        CEditUI* mpTestTimeOutEdt;
        CComboUI* mpRowCombo;
        CComboUI* mpColumnCombo;

        CComboUI* mpDeviceQueueCombo;
        CEditUI* mpHostIPAddrEdt;

        CLabelUI* mpFristDeviceModeLabel;
        CLabelUI* mpSecondDeviceModeLabel;
        CComboUI* mpFristDeviceModeCombo;
        CComboUI* mpSecondDeviceModeCombo;

        CEditUI* mpUpgradeVersionEdt;
        CEditUI* mpLogPathEdt;

        CButtonUI* mpOkBtn;
        CButtonUI* mpCancelBtn;
        CButtonUI* mpDeviceQueueConfigBtn;
        CButtonUI* mpClearDeviceQueueSortConfigBtn;

        std::wstring mstrDownloadFileConfig;
        std::wstring mstrTFTPDIPAddr;
    };
}
