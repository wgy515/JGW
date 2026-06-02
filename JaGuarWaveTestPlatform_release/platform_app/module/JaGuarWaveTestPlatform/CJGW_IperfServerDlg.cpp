#include "StdAfx.h"
#include "CJGW_IperfServerDlg.h"
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>

namespace JGW
{
    CCJGW_IperfServerDlg::CCJGW_IperfServerDlg(HWND hParentHwnd,LPS_MES_INFO psMesInfo) : m_hParentHwnd(hParentHwnd),mpsMesInfo(psMesInfo)
    {
    }


    CCJGW_IperfServerDlg::~CCJGW_IperfServerDlg(void)
    {
    }
    //! iperf.exe -B 192.168.50.243 -s
    int CCJGW_IperfServerDlg::DoModal()
    {
        //! m_hParentHwnd = hParentHwnd; 
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("iperf_server_config.xml"),_T("Iperf Server Config Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("iperf_server_config.xml"),_T("Iperf Server Config Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("iperf_server_config.xml"),_T("Iperf Server Config Windows"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();
        //  SetWindowPos(m_PaintManager.GetPaintWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
        return ShowModal();
    }

    void CCJGW_IperfServerDlg::OnInitWindow()
    {
        mpIperfFilePath = static_cast<CEditUI*>(m_PaintManager.FindControl(L"iperfFilePathEdt"));
        mpIpAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ipAddressEdt"));
        mpExtraParameterEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"extraParameterEdt"));
        if (!mpIperfFilePath || !mpIpAddressEdt || !mpExtraParameterEdt) {Close(IDCANCEL);return;}

        mpIpAddressEdt->SetText(GetFileProperty()->GetString(L"IPERF.ip").c_str());
        mpIperfFilePath->SetText(JGW_RealativePathToAbsPath(GetFileProperty()->GetString(L"IPERF.FilePath").c_str()).c_str());//! 
        mpExtraParameterEdt->SetText(GetFileProperty()->GetString(L"IPERF.ExtraParameter").c_str());

        if (-1 == JGW_GetProcessID2Name(WLAN_SERVICE_APPLICATION_NAME))
        {
            std::wstring strTemp = JGW_GetApplicationFolder();
            strTemp += WLAN_SERVICE_APPLICATION_NAME;
            JGW_CreateProcess(strTemp.c_str());
        }
    }

    void CCJGW_IperfServerDlg::OnCloseWindow()
    {

    }

    void CCJGW_IperfServerDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"StartBtn")
            {
                OnClickOkBtn();
            }
        }
    }

    bool CCJGW_IperfServerDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        return true;
    }

    void CCJGW_IperfServerDlg::OnClickOkBtn()
    {
        std::wstring strFilePath (mpIperfFilePath->GetText().GetData()),strIPAddress(mpIpAddressEdt->GetText().GetData()),strParam(L" -s"),strExtralParam(mpExtraParameterEdt->GetText().GetData());
        if (!strIPAddress.empty())
        {
            strParam += L" -B ";
            strParam += strIPAddress;
        }

        if (!strExtralParam.empty())
        {
            strParam += L" ";
            strParam += strExtralParam;
        }
        
        ShellExecute(NULL,L"open",strFilePath.c_str(),strParam.c_str(),JGW::JGW_GetApplicationFolder(),SW_NORMAL);

        JGW_ReplaceStringW(strFilePath,JGW_GetApplicationFolder(),L"");
        GetFileProperty()->PutString(L"IPERF.FilePath",strFilePath);
        GetFileProperty()->PutString(L"IPERF.ip",mpIpAddressEdt->GetText().GetData());
        GetFileProperty()->PutString(L"IPERF.ExtraParameter",mpExtraParameterEdt->GetText().GetData());

        Close(IDOK);
    }
}

