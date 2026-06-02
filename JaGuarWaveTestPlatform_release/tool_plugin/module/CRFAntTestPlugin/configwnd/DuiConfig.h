#pragma once
#include <UiLib/CDuiDialog.h>
#include <string>
#include "RfTestDefine.h"


namespace JGW
{
    class S_RF_CONFIG_INFO;
    class CConfigFile;
    class CDuiConfig:public CDuiDialog
    {
    public:
        CDuiConfig(CConfigFile* lpConfigFileClass);
        ~CDuiConfig(void);

        int		DoModal( HWND hParentHwnd = NULL );

    private:
        void				Notify(TNotifyUI& msg);
        virtual LPCTSTR		GetWindowClassName() const;
        virtual UINT		GetClassStyle() const;

        bool				InitWindow();
        void				InitConfigShow();
        void				GetBandInfo(S_SYS_INFO& m_sTestBandInfo,std::wstring& strBand);
        bool				SaveConfig();
    private:
        void                ChangeRemoteInstrumentView();
    private:
        HWND				m_hParentHwnd;
        S_RF_CONFIG_INFO*	m_pRfConfigInfo;
        CConfigFile*		m_lpConfigFileClass;
        CButtonUI*			m_pCancelbtn;
        CButtonUI*			m_pSaveBtn;
        CButtonUI*			m_pGpsdetaileconfigbtn;
        CButtonUI*			m_pDetailedConfigBtn;

        CCheckBoxUI*		m_pTestGpsCbox;
        CCheckBoxUI*		m_pTestLteCbox;
        CCheckBoxUI*		m_pTestWcdmaCbox;
        CCheckBoxUI*		m_pTestTdscdmaCbox;
        CCheckBoxUI*		m_pTestCdmaCbox;
        CCheckBoxUI*		m_pTestGsmCbox;
        CCheckBoxUI*		m_pTestRxCbox;
        CCheckBoxUI*        m_pTestWlanCbox;
        CCheckBoxUI*		m_pExportLogCbox;

        CEditUI*			m_pLteBandEdt;
        CEditUI*			m_pWcdmaBandEdt;
        CEditUI*			m_pTdscdmaBandEdt;
        CEditUI*			m_pCdmaBandEdt;
        CEditUI*			m_pGsmBandEdt;
        CEditUI*            m_pWlanBandEdt;
        CEditUI*			m_pReTestCountEdt;

        CComboUI*			m_pInstrumentCombo;
        CComboUI*           m_pRemoteInstrumentCombo;
        CLabelUI*           m_pboardidLab;
        CEditUI*			m_pboardidEdt;
        CLabelUI*           m_pPadLab;
        CEditUI*			m_pPadEdt;//! ÒÇÆ÷µØÖ·
        CComboUI*           m_pGpibTypeCombo;

        CLabelUI*           m_pVisaLab;
        CEditUI*            m_pVisaEdt;
        
        CLabelUI*           m_pIPLab;
        CEditUI*            m_pIPAddressEdt;
        CLabelUI*           m_pSubInstrumentLab;
        CEditUI*            m_pSubInstrumentEdt;
    };
}