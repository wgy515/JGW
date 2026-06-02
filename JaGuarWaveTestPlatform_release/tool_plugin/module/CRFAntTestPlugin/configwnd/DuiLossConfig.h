#pragma once
#include <UiLib/CDuiDialog.h>
#include <string>
#include "RfTestDefine.h"

namespace JGW
{
    class S_RF_CONFIG_INFO;
    class CConfigFile;

    class CDuiLossConfig:public CDuiDialog
    {
    public:
        CDuiLossConfig(CConfigFile* lpConfigFileClass);
        ~CDuiLossConfig(void);

        int		DoModal( HWND hParentHwnd = NULL );

    private:
        void				Notify(TNotifyUI& msg);
        virtual LPCTSTR		GetWindowClassName() const;
        //virtual UINT		GetClassStyle() const;
        bool				SaveConfig();
        bool				InitWindow();
        void				InitShowConfigContent();

        CEditUI*			GetListSubEditUi();
        void				ShowTestBandInfo(S_SYS_INFO& sysInfo,LPCTSTR strFristCol = L"LTE_B");
        void				GetVectortBandChannelInfo(std::vector<S_BAND_CHANNEL_INFO>& vBandChannelInfo,std::wstring& strChannel,std::wstring& strLoss);
        int					GetSys(LPCTSTR strBand);
        int					GetBandInfo(int nSys,LPCTSTR strBand);
    private:
        HWND				m_hParentHwnd;
        CButtonUI*			m_pCancelbtn;
        CButtonUI*			m_pSaveBtn;
        CListTestCtrlUI*	m_pListCtrl;
        S_RF_CONFIG_INFO*	m_pRfConfigInfo;
        CConfigFile*		m_lpConfigFileClass;
    };
}