#include "StdAfx.h"
#include "DuiConfig.h"
#include "DuiGpsConfig.h"
#include "DuiLossConfig.h"
#include "../ConfigFile.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CDuiConfig::CDuiConfig(CConfigFile* lpConfigFileClass):m_lpConfigFileClass(lpConfigFileClass)
        ,m_hParentHwnd(NULL)
        ,m_pRfConfigInfo(NULL)
        ,m_pPadEdt(NULL)
    {
        if( m_lpConfigFileClass ) m_pRfConfigInfo = &m_lpConfigFileClass->m_sRfConfigInfo;
    }


    CDuiConfig::~CDuiConfig(void)
    {
    }

    int CDuiConfig::DoModal( HWND hParentHwnd /* = NULL */ )
    {
        m_hParentHwnd = hParentHwnd;
#ifdef USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateDuiDialogWindow(m_hParentHwnd,_T(""),_T("Config.xml"),UI_WNDSTYLE_DIALOG,NULL);
#else
        CreateDuiDialogWindow(m_hParentHwnd,_T("skin\\Login"),_T("Config.xml"),UI_WNDSTYLE_DIALOG ,/*WS_EX_STATICEDGE | */NULL ,_T("CDuiConfig"));
#endif

        SetFocus(m_hParentHwnd);
        CenterWindow();

        return ShowModal();
    }

    LPCTSTR CDuiConfig::GetWindowClassName() const
    {
        return L"CDuiConfig";
    }

    UINT CDuiConfig::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    bool CDuiConfig::InitWindow()
    {
        bool bFindControlOk = true;

        bFindControlOk &= (NULL != m_pRfConfigInfo);

        m_pCancelbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
        m_pSaveBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("savebtn")));
        m_pDetailedConfigBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("detailedConfigBtn")));
        m_pGpsdetaileconfigbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("gpsdetaileconfigbtn")));

        bFindControlOk &= (NULL != m_pCancelbtn);
        bFindControlOk &= (NULL != m_pSaveBtn);
        bFindControlOk &= (NULL != m_pDetailedConfigBtn);
        bFindControlOk &= (NULL != m_pGpsdetaileconfigbtn);

        m_pTestGpsCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bGpsbox")));
        m_pTestLteCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bLtebox")));
        m_pTestWcdmaCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bWcdmabox")));
        m_pTestTdscdmaCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bTdscdmabox")));
        m_pTestCdmaCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bCdmabox")));
        m_pTestGsmCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bGsmbox")));
        m_pTestRxCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bRx")));
        m_pExportLogCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bExportLog")));
        m_pTestWlanCbox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("bWlanbox")));

        bFindControlOk &= (NULL != m_pTestGpsCbox);
        bFindControlOk &= (NULL != m_pTestLteCbox);
        bFindControlOk &= (NULL != m_pTestWcdmaCbox);
        bFindControlOk &= (NULL != m_pTestTdscdmaCbox);
        bFindControlOk &= (NULL != m_pTestCdmaCbox);
        bFindControlOk &= (NULL != m_pTestGsmCbox);
        bFindControlOk &= (NULL != m_pTestRxCbox);
        bFindControlOk &= (NULL != m_pTestWlanCbox);
        bFindControlOk &= (NULL != m_pExportLogCbox);

        m_pLteBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ltebandedt")));
        m_pWcdmaBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("wcdmabandedt")));
        m_pTdscdmaBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("tdscdmabandedt")));
        m_pCdmaBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("cdmabandedt")));
        m_pGsmBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("gsmbandedt")));
        m_pWlanBandEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("wlanbandedt")));

        bFindControlOk &= (NULL != m_pLteBandEdt);
        bFindControlOk &= (NULL != m_pWcdmaBandEdt);
        bFindControlOk &= (NULL != m_pTdscdmaBandEdt);
        bFindControlOk &= (NULL != m_pCdmaBandEdt);
        bFindControlOk &= (NULL != m_pGsmBandEdt);
        bFindControlOk &= (NULL != m_pWlanBandEdt);

        m_pReTestCountEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("rxcountedt")));
        m_pInstrumentCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("instrumentCombo")));
        m_pRemoteInstrumentCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("remoteCombo")));
        bFindControlOk &= (NULL != m_pReTestCountEdt);
        bFindControlOk &= (NULL != m_pInstrumentCombo);
        bFindControlOk &= (NULL != m_pRemoteInstrumentCombo);

        m_pboardidEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("boardidEdt")));
        m_pboardidLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("boardidLab")));
        m_pPadEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("padEdt")));
        m_pPadLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("padLab")));
        m_pGpibTypeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("gpibTypeCombo")));
        bFindControlOk &= (NULL != m_pboardidEdt);
        bFindControlOk &= (NULL != m_pPadEdt);
        bFindControlOk &= (NULL != m_pPadLab);
        bFindControlOk &= (NULL != m_pboardidLab);
        bFindControlOk &= (NULL != m_pGpibTypeCombo);

        m_pIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ipAddressEdt")));
        m_pIPLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("ipLab")));
        m_pSubInstrumentEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("subInstrumentEdt")));
        m_pSubInstrumentLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("subInstrumentLab")));
        bFindControlOk &= (NULL != m_pIPAddressEdt);
        bFindControlOk &= (NULL != m_pIPLab);
        bFindControlOk &= (NULL != m_pSubInstrumentEdt);
        bFindControlOk &= (NULL != m_pSubInstrumentLab);

        m_pVisaLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("visaLab")));
        m_pVisaEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("visaStringEdt")));
        bFindControlOk &= (NULL != m_pVisaLab);
        bFindControlOk &= (NULL != m_pVisaEdt);

        if( !bFindControlOk )
        {
            Close(IDCANCEL);
            return false;
        }

        InitConfigShow();

        return true;
    }

    void CDuiConfig::ChangeRemoteInstrumentView()
    {
        m_pboardidLab->SetVisible(false);
        m_pboardidEdt->SetVisible(false);
        m_pPadLab->SetVisible(false);
        m_pPadEdt->SetVisible(false);

        m_pVisaLab->SetVisible(false);
        m_pVisaEdt->SetVisible(false);

        m_pIPLab->SetVisible(false);
        m_pIPAddressEdt->SetVisible(false);
        m_pSubInstrumentLab->SetVisible(false);
        m_pSubInstrumentEdt->SetVisible(false);
        m_pGpibTypeCombo->SetVisible(false);

        if (E_TCP_IP_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pIPLab->SetVisible();
            m_pIPAddressEdt->SetVisible();
            m_pSubInstrumentLab->SetVisible();
            m_pSubInstrumentEdt->SetVisible();          
        }
        else if (E_VISA_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pVisaLab->SetVisible();
            m_pVisaEdt->SetVisible();
        }
        else
        {
            m_pboardidLab->SetVisible();
            m_pboardidEdt->SetVisible();
            m_pPadLab->SetVisible();
            m_pPadEdt->SetVisible();
            m_pGpibTypeCombo->SetVisible();
        }
    }

    void CDuiConfig::Notify(TNotifyUI& msg)
    {
        if ( msg.sType == DUI_MSGTYPE_CLICK )
        {
            if ( m_pCancelbtn == msg.pSender )
            {
                Close(IDCANCEL);//PostQuitMessage(0);		
                return ;
            }
            if ( m_pSaveBtn == msg.pSender )
            {
                if( SaveConfig() )
                {
                    ::MessageBox(GetHWND(),L"保存配置文件成功",L"保存配置文件",MB_OK);
                    Close(IDCANCEL);
                }
                return;
            }
            if ( m_pDetailedConfigBtn == msg.pSender )
            {
                CDuiLossConfig duiloss(m_lpConfigFileClass);
                duiloss.DoModal(GetHWND());
                return ;
            }
            if ( m_pGpsdetaileconfigbtn == msg.pSender )
            {
                CDuiGpsConfig duigsp;
                duigsp.DoModal(GetHWND());
                return ;
            }
        }
        else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (m_pRemoteInstrumentCombo == msg.pSender)
            {
                ChangeRemoteInstrumentView();
            }
        }
    }

    void CDuiConfig::InitConfigShow()
    {
        std::wstring strTemp(L"");

        m_pTestRxCbox->SetCheck( m_pRfConfigInfo->m_bIsTestRx );
        m_pTestGpsCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.gps );
        m_pTestLteCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.lte );
        m_pTestWcdmaCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.wcdma );
        m_pTestTdscdmaCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.tds );
        m_pTestCdmaCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.cdma );
        m_pTestGsmCbox->SetCheck( m_pRfConfigInfo->m_sModeSelecter.gsm );
        m_pTestWlanCbox->SetCheck(m_pRfConfigInfo->m_sModeSelecter.wlan);
        m_pExportLogCbox->SetCheck( m_pRfConfigInfo->m_bIsExportTestInfo );

        JGW_FormatWString(strTemp,L"%d",m_pRfConfigInfo->m_nErrorReTestCount);
        m_pReTestCountEdt->SetText(strTemp.c_str());

 
        if (E_VISA_HIVE == m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType)
        {
            strTemp = JGW_A2W(m_pRfConfigInfo->m_sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress);
            m_pVisaEdt->SetText(strTemp.c_str());
        }
        else if (E_TCP_IP_HIVE == m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType)
        {
            strTemp = JGW_A2W(m_pRfConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress);
            m_pIPAddressEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"%d",m_pRfConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
            m_pSubInstrumentEdt->SetText(strTemp.c_str());
        }
        else
        {       
            JGW_FormatWString(strTemp,L"%d",m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
            m_pboardidEdt->SetText(strTemp.c_str());
            m_pGpibTypeCombo->SelectItem(m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType);
            JGW_FormatWString(strTemp,L"%d",m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
            m_pPadEdt->SetText(strTemp.c_str());
        }

        for (int i = 0;i < m_pInstrumentCombo->GetCount();i ++)
        {
            if ( NULL == JGW_WStrComparenoCaseWStr(m_pInstrumentCombo->GetItemAt(i)->GetText(),m_pRfConfigInfo->m_sMonitorInfo.m_strModelName.c_str()) )
            {
                m_pInstrumentCombo->SelectItem(i);
            }
        }

        m_pRemoteInstrumentCombo->SelectItem(m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType);
        ChangeRemoteInstrumentView();

        GetBandInfo(m_pRfConfigInfo->m_sLteInfo.m_sTestBandInfo,strTemp);
        m_pLteBandEdt->SetText(strTemp.c_str());

        GetBandInfo(m_pRfConfigInfo->m_sWcdmaInfo.m_sTestBandInfo,strTemp);
        m_pWcdmaBandEdt->SetText(strTemp.c_str());

        GetBandInfo(m_pRfConfigInfo->m_sTdscdmaInfo.m_sTestBandInfo,strTemp);
        m_pTdscdmaBandEdt->SetText(strTemp.c_str());

        GetBandInfo(m_pRfConfigInfo->m_sCdmaInfo.m_sTestBandInfo,strTemp);
        m_pCdmaBandEdt->SetText(strTemp.c_str());

        GetBandInfo(m_pRfConfigInfo->m_sGsmInfo.m_sTestBandInfo,strTemp);
        m_pGsmBandEdt->SetText(strTemp.c_str());

        GetBandInfo(m_pRfConfigInfo->m_sWlanInfo.m_sTestBandInfo,strTemp);
        m_pWlanBandEdt->SetText(strTemp.c_str());
    }

    bool CDuiConfig::SaveConfig()
    {
        m_pRfConfigInfo->m_bIsTestRx = m_pTestRxCbox->GetCheck();
        m_pRfConfigInfo->m_nErrorReTestCount = _ttoi( m_pReTestCountEdt->GetText() );
        m_pRfConfigInfo->m_sModeSelecter.lte = m_pTestLteCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.wcdma = m_pTestWcdmaCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.tds = m_pTestTdscdmaCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.cdma = m_pTestCdmaCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.gsm = m_pTestGsmCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.gps = m_pTestGpsCbox->GetCheck();
        m_pRfConfigInfo->m_sModeSelecter.wlan = m_pTestWlanCbox->GetCheck();

        m_pRfConfigInfo->m_bIsExportTestInfo = m_pExportLogCbox->GetCheck();
        m_pRfConfigInfo->m_sMonitorInfo.m_strModelName = m_pInstrumentCombo->GetText();

        if (E_GPIB_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = m_pGpibTypeCombo->GetCurSel();
            m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = _ttoi(m_pboardidEdt->GetText().GetData());
            m_pRfConfigInfo->m_sMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = _ttoi(m_pPadEdt->GetText().GetData());
        }
        else if (E_VISA_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            m_pRfConfigInfo->m_sMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A_W(m_pVisaEdt->GetText().GetData());
        }
        else if (E_TCP_IP_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pRfConfigInfo->m_sMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
            m_pRfConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A_W(m_pIPAddressEdt->GetText().GetData());
            m_pRfConfigInfo->m_sMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument =_ttoi(m_pSubInstrumentEdt->GetText().GetData());
        }


        S_CHANGE_BAND_INFO sChangeBandInfo;
        sChangeBandInfo.m_strLteBand = m_pLteBandEdt->GetText();
        sChangeBandInfo.m_strTdscdmaBand = m_pTdscdmaBandEdt->GetText();
        sChangeBandInfo.m_strWcdmaBand = m_pWcdmaBandEdt->GetText();
        sChangeBandInfo.m_strcdmaBand = m_pCdmaBandEdt->GetText();
        sChangeBandInfo.m_strGsmBand = m_pGsmBandEdt->GetText();
        sChangeBandInfo.m_strWlanBand = m_pWlanBandEdt->GetText();

        if(m_lpConfigFileClass)  return m_lpConfigFileClass->ChangesModeSelecter(sChangeBandInfo);

        return true;
    }

    void CDuiConfig::GetBandInfo(S_SYS_INFO& m_sTestBandInfo,std::wstring& strBand)
    {
        strBand = L"";
        TCHAR szBuffer[260] = {0};

        for ( std::vector<S_BAND_INFO>::iterator it = m_sTestBandInfo.m_vBandInfo.begin();
            it != m_sTestBandInfo.m_vBandInfo.end();
            it ++ )
        {
            JGW_FormatWString(strBand,L"%d,",it->m_nBand);
            _tcscat_s(szBuffer,strBand.c_str());
        }
        strBand = szBuffer;
        if ( -1 != strBand.find(L',') )
        {
            strBand.erase(strBand.find_last_of(L','));
        }
    }
}