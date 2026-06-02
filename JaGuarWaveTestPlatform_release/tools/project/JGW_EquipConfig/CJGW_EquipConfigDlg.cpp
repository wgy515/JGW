#include "StdAfx.h"
#include "CJGW_EquipConfigDlg.h"
#include "CJGW_EquipToolConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_EquipConfigQcommCalXml.h"
#include "CJGW_QSEQConfigXml.h"
#include "CJGW_EquipConfigCustomIni.h"
#include "CJGW_QCOMMWlanXml.h"

namespace JGW
{
    CCJGW_EquipConfigDlg::CCJGW_EquipConfigDlg(void) : mpsEquipPrjectConfig(NULL)
    {
        //S_EQUIP_PROJECT_CONFIG sQuipProjectConfig;
        //sQuipProjectConfig.mstrGroupName = L"";
        //sQuipProjectConfig.mstrModuleFolder = L"..\\Databases";
        //sQuipProjectConfig.mstrConfigFileName = L"equipconfig.xml";
        //sQuipProjectConfig.mstrTestName = L"CalDB";
        //sQuipProjectConfig.mcEquipType = E_QCOMM_CAL;
        //mvsEquipProjectConfig.push_back(sQuipProjectConfig);

        //sQuipProjectConfig.mstrGroupName = L"";
        //sQuipProjectConfig.mstrModuleFolder = L"..\\Databases";
        //sQuipProjectConfig.mstrConfigFileName = L"QSEQ_DeviceConfigSample_NSVFS_CMW500.xml";
        //sQuipProjectConfig.mstrTestName = L"QSEQ";
        //sQuipProjectConfig.mcEquipType = E_QSEQ;
        //mvsEquipProjectConfig.push_back(sQuipProjectConfig);

        msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
        msMonitorInfo.m_strModelName = L"CMW500";
        msMonitorInfo.mnDeviceID = 3;
        msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = NI_GPIB_TYPE;
        msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
        msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 20;
        msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument = 0;
        msMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort = 5026;
    }

    CCJGW_EquipConfigDlg::~CCJGW_EquipConfigDlg(void)
    {
    }

    void CCJGW_EquipConfigDlg::OnInitWindow()
    {
        if (!LoadControlUiClassPtr()) return;
        LoadTargetProjectView();
    }

    void CCJGW_EquipConfigDlg::OnCloseWindow()
    {

    }

    void CCJGW_EquipConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (mpTargetCombo == msg.pSender)
            {
                OnChangeTargetProject();
            }
            else if (m_pRemoteInstrumentCombo == msg.pSender)
            {
                ChangeRemoteInstrumentView();
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpSaveBtn == msg.pSender)
            {
                OnSaveEquipConfigBtn();
            }        
        }
    }

    bool CCJGW_EquipConfigDlg::LoadControlUiClassPtr()
    {
        bool bFindControlOk = true;

        mpTargetCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"targetCombo"));
        mpSaveBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"saveBtn"));
        m_pInstrumentCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("instrumentCombo")));
        m_pRemoteInstrumentCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("remoteCombo")));
        bFindControlOk &= (NULL != mpTargetCombo);
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

        m_pSourceAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("sourceAddressEdt")));
        m_pSourceLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("sourceIPLab")));

        m_pSocketPortEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("portEdt")));
        m_pSocketPortLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("portLab")));

        m_pSubInstrumentEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("subInstrumentEdt")));
        m_pSubInstrumentLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("subInstrumentLab")));
        bFindControlOk &= (NULL != m_pIPAddressEdt);
        bFindControlOk &= (NULL != m_pIPLab);
        bFindControlOk &= (NULL != m_pSubInstrumentEdt);
        bFindControlOk &= (NULL != m_pSubInstrumentLab);
        bFindControlOk &= (NULL != m_pSourceAddressEdt);
        bFindControlOk &= (NULL != m_pSourceLab);
        bFindControlOk &= (NULL != m_pSocketPortEdt);
        bFindControlOk &= (NULL != m_pSocketPortLab);

        m_pVisaLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("visaLab")));
        m_pVisaEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("visaStringEdt")));
        bFindControlOk &= (NULL != m_pVisaLab);
        bFindControlOk &= (NULL != m_pVisaEdt);

        if( !bFindControlOk ) Close(IDCANCEL);

        return bFindControlOk;
    }

    void CCJGW_EquipConfigDlg::LoadTargetProjectView()
    {
        CCJGW_EquipToolConfig equipToolConfig;
        std::wstring strTemp = JGW_GetApplicationFolder();
        strTemp += L"equiptool.ini";
        equipToolConfig.LoadEquipConfig(strTemp,mvsEquipProjectConfig);

        for (std::vector<S_EQUIP_PROJECT_CONFIG>::iterator it = mvsEquipProjectConfig.begin();it != mvsEquipProjectConfig.end(); ++it)
        {
            CListLabelElementUI* pListLabelEUi = new CListLabelElementUI();
            strTemp = L"";
            JGW_FormatWString(strTemp,L"%s_%s_Instrument Settings(%s)",it->mstrGroupName.c_str(),it->mstrTestName.c_str(),it->mstrConfigFileName.c_str());
            pListLabelEUi->SetText(strTemp.c_str());
            pListLabelEUi->SetDate(&it[0]);
            mpTargetCombo->Add(pListLabelEUi);
        }
        if (mpTargetCombo->GetCount() >= 1) mpTargetCombo->SelectItem(0);
    }

    void CCJGW_EquipConfigDlg::ChangeRemoteInstrumentView()
    {
        m_pInstrumentCombo->SetVisible(true);
        m_pboardidLab->SetVisible(false);
        m_pboardidEdt->SetVisible(false);
        m_pPadLab->SetVisible(false);
        m_pPadEdt->SetVisible(false);

        m_pVisaLab->SetVisible(false);
        m_pVisaEdt->SetVisible(false);

        m_pSourceAddressEdt->SetVisible(false);
        m_pSourceLab->SetVisible(false);
        m_pSocketPortEdt->SetVisible(false);
        m_pSocketPortLab->SetVisible(false);
        m_pIPLab->SetVisible(false);
        m_pIPAddressEdt->SetVisible(false);
        m_pSubInstrumentLab->SetVisible(false);
        m_pSubInstrumentEdt->SetVisible(false);
        m_pGpibTypeCombo->SetVisible(false);

        if (E_TCP_IP_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            m_pInstrumentCombo->SetVisible(false);
            m_pIPLab->SetVisible();
            m_pIPAddressEdt->SetVisible();
            m_pSubInstrumentLab->SetVisible();
            m_pSubInstrumentEdt->SetVisible();          
            m_pSourceAddressEdt->SetVisible();
            m_pSourceLab->SetVisible();
            m_pSocketPortEdt->SetVisible();
            m_pSocketPortLab->SetVisible();
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
        msMonitorInfo.m_cInterfaceType = m_pRemoteInstrumentCombo->GetCurSel();
        UpdateMonitorInfoView();
    }

    void CCJGW_EquipConfigDlg::UpdateMonitorInfoView()
    {
        std::wstring strTemp;
        if (E_VISA_HIVE == msMonitorInfo.m_cInterfaceType)
        {
            m_pRemoteInstrumentCombo->SelectItem(E_VISA_HIVE);
            strTemp = JGW_A2W(msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress);
            m_pVisaEdt->SetText(strTemp.c_str());
        }
        else if (E_TCP_IP_HIVE == msMonitorInfo.m_cInterfaceType)
        {
            m_pRemoteInstrumentCombo->SelectItem(E_TCP_IP_HIVE);
            strTemp = JGW_A2W(msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress);
            m_pIPAddressEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"%d",msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
            m_pSubInstrumentEdt->SetText(strTemp.c_str());

            m_pSourceAddressEdt->SetText(JGW_A2W(msMonitorInfo.msTCPIPInstrumentInfo.mstrSourceAddress).c_str());

            JGW_FormatWString(strTemp,L"%d",msMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort);
            m_pSocketPortEdt->SetText(strTemp.c_str());
        }
        else
        {       
            m_pRemoteInstrumentCombo->SelectItem(E_GPIB_HIVE);
            JGW_FormatWString(strTemp,L"%d",msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface);
            m_pboardidEdt->SetText(strTemp.c_str());
            m_pGpibTypeCombo->SelectItem(msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType);
            JGW_FormatWString(strTemp,L"%d",msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress);
            m_pPadEdt->SetText(strTemp.c_str());
        }
    }

    void CCJGW_EquipConfigDlg::OnChangeTargetProject()
    {
        mpsEquipPrjectConfig = (LPS_EQUIP_PROJECT_CONFIG)mpTargetCombo->GetItemAt(mpTargetCombo->GetCurSel())->GetDate(); 
        msMonitorInfo.mnDeviceID = mpsEquipPrjectConfig->mnDeviceID;
        if (E_QCOMM_CAL == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_EquipConfigQcommCalXml equipConfigXml;
            equipConfigXml.LoadEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else if (E_QSEQ == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_QSEQConfigXml qseqConfigXml;
            qseqConfigXml.LoadEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else if (E_QCOMM_WLAN == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_QCOMMWlanXml qcommWlanXml;
            qcommWlanXml.LoadEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else
        {
            CCJGW_EquipConfigCustomIni customIni;
            customIni.LoadCustomIni(mpsEquipPrjectConfig,msMonitorInfo);
        }
        UpdateMonitorInfoView();
    }

    void CCJGW_EquipConfigDlg::GetEquipControlUiInfo()
    {
        msMonitorInfo.m_strModelName = m_pInstrumentCombo->GetText();

        if (E_GPIB_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
            msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = m_pGpibTypeCombo->GetCurSel();
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = _ttoi(m_pboardidEdt->GetText().GetData());
            msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = _ttoi(m_pPadEdt->GetText().GetData());
        }
        else if (E_VISA_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            msMonitorInfo.m_cInterfaceType = E_VISA_HIVE;
            msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress = JGW_W2A_W(m_pVisaEdt->GetText().GetData());
        }
        else if (E_TCP_IP_HIVE == m_pRemoteInstrumentCombo->GetCurSel())
        {
            msMonitorInfo.m_cInterfaceType = E_TCP_IP_HIVE;
            msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress = JGW_W2A_W(m_pIPAddressEdt->GetText().GetData());
            msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument =_ttoi(m_pSubInstrumentEdt->GetText().GetData());
            msMonitorInfo.msTCPIPInstrumentInfo.mstrSourceAddress = JGW_W2A_W(m_pSourceAddressEdt->GetText().GetData());
            msMonitorInfo.msTCPIPInstrumentInfo.mnSocketPort =_ttoi(m_pSocketPortEdt->GetText().GetData());
        }
    }

    void CCJGW_EquipConfigDlg::OnSaveEquipConfigBtn()
    {
        GetEquipControlUiInfo();
        if (E_QCOMM_CAL == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_EquipConfigQcommCalXml equipConfigXml;
            equipConfigXml.SaveEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else if (E_QSEQ == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_QSEQConfigXml qseqConfigXml;
            qseqConfigXml.SaveEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else if (E_QCOMM_WLAN == mpsEquipPrjectConfig->mcEquipType)
        {
            CCJGW_QCOMMWlanXml qcommWlanXml;
            qcommWlanXml.SaveEquipConfigXml(mpsEquipPrjectConfig,msMonitorInfo);
        }
        else
        {
            CCJGW_EquipConfigCustomIni customIni;
            if (!customIni.SaveCustomIni(mpsEquipPrjectConfig,msMonitorInfo))
            {
                MessageBox(m_PaintManager.GetPaintWindow(),L"Failed to save instrument configuration file!!!",L"Save Config",MB_ICONERROR);
            }
        }
        UpdateMonitorInfoView();
    }
}