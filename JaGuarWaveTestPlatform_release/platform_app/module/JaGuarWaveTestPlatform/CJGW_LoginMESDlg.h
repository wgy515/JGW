#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include "CJGW_StartMESServiceDlg.h"

namespace JGW
{
    class CCJGW_LoginMESDlg : public CCUiLibWnd,public IMessageFilterUI
    {
    public:
        CCJGW_LoginMESDlg(S_MES_INFO& sMesInfo);
        ~CCJGW_LoginMESDlg(void);
    public:
        int DoModal();
    private:
        UILIB_GetWindowClassName(CCJGW_LoginMESDlg);
        void OnInitWindow();
        virtual UINT GetClassStyle() const;
        void OnNotify(TNotifyUI& msg);
        void OnCloseWindow();

    private:
        void InitMESInfoUI();
        void CheckLoginIsExpired();
        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& );
        void GetTaskNumberToBarcode();
        void OnResponseClickOKMsg();
    private:
        HWND m_hParentHwnd;
        S_MES_INFO& msMesInfo;
        LPS_MES_INFO mpsMesInfo;
        CButtonUI* mpOkOptionUI;
        CButtonUI* mpCancelOptionUI;

        CComboBoxUI* mpMonoComboBoxUI;

        CEditUI* mpUserNameEdt;
        CEditUI* mpUserPasswordEdt;
        CEditUI* mpBarcodeEdt;
#ifdef GET_INFO_WORK_ORDER_NUMBER
        CEditUI* mpWorkOrderNumberEdt;
#else
        CEditUI* mpTaskNumberEdt;
#endif
        CCheckBoxUI* mpSaveAccountCheckBox;
        CComboBoxUI* mpDurationTimeCombo;
        CComboBoxUI* mpClassCombo;
        CComboBoxUI* mpStationCombo;

        CCJGW_StartMESServiceDlg* mpStartMESServices;
    };
}