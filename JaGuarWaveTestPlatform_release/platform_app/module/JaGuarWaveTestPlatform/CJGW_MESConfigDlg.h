#pragma once
#include <UiLib/CUiLibWnd.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>

namespace JGW
{
    class CCJGW_MESConfigDlg : public CCUiLibWnd,public IMessageFilterUI
    {
    public:
        CCJGW_MESConfigDlg(HWND hParentHwnd = NULL);
        ~CCJGW_MESConfigDlg(void);

        int					DoModal(LPS_MES_INFO psMesInfo);
    private:
        UILIB_GetWindowClassName(CCJGW_MESConfigDlg);
        void				OnInitWindow();
        virtual UINT		GetClassStyle() const;
        void				OnNotify(TNotifyUI& msg);
		void				OnCloseWindow();
	private:
		void				InitMESInfoUI();
        void                CheckLoginIsExpired();
		LRESULT				MessageHandler(UINT uMsg, WPARAM wParam, LPARAM , bool& );
		void				GetTaskNumberToBarcode();
		void				OnResponseClickOKMsg();
    private:
        HWND				m_hParentHwnd;
        CButtonUI*          mpOkOptionUI;
        CButtonUI*          mpCancelOptionUI;
        
        CComboBoxUI*        mpMonoComboBoxUI;
		CEditUI*			mpUserNameEdt;
		CEditUI*			mpUserPasswordEdt;
		CEditUI*			mpBarcodeEdt;
#ifdef GET_INFO_WORK_ORDER_NUMBER
        CEditUI* mpWorkOrderNumberEdt;
#else
        CEditUI* mpTaskNumberEdt;
#endif
		CCheckBoxUI*		mpSaveAccountCheckBox;
		CComboBoxUI*		mpDurationTimeCombo;
		CComboBoxUI*		mpClassCombo;
        CComboBoxUI*        mpStationCombo;

        LPS_MES_INFO        mpsMesInfo;
    };
}