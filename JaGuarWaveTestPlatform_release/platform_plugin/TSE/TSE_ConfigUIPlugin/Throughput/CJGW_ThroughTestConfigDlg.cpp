#include "StdAfx.h"
#include "CJGW_ThroughTestConfigDlg.h"

namespace JGW
{
    CCJGW_ThroughTestConfigDlg::CCJGW_ThroughTestConfigDlg(HWND hParentHwnd,S_THROUGHPUT_CONFIG& sThroughputConfig) : m_hParentHwnd(hParentHwnd),msThroughputConfig(sThroughputConfig)
    {
    }


    CCJGW_ThroughTestConfigDlg::~CCJGW_ThroughTestConfigDlg(void)
    {
    }

    UINT CCJGW_ThroughTestConfigDlg::GetClassStyle() const
    {
        return UI_CLASSSTYLE_DIALOG;
    }

    int CCJGW_ThroughTestConfigDlg::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateNoBorderStyleWindow(_T(""),_T("tse_throughput_config.xml"),_T("JaGuarWave Throughput Test Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateNoBorderStyleWindow(_T(""),_T("tse_throughput_config.xml"),_T("JaGuarWave Throughput Test  Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#else
        CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("tse_throughput_config.xml"),_T("JaGuarWave Throughput Test Config"),m_hParentHwnd,UI_WNDSTYLE_DIALOG);
#endif
        CenterWindow();

        return ShowModal();
    }

    void CCJGW_ThroughTestConfigDlg::OnInitWindow()
    {
        mpWiFiServerAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"WiFiServerAddressEdt"));
        mpIperfIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"IperfIPAddressEdt"));
        mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"tsetTimeEdt"));
        mpTestRangeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"tsetRangeEdt"));

        if (!mpWiFiServerAddressEdt || !mpIperfIPAddressEdt || !mpTestTimeEdt || !mpTestRangeEdt)
        {
            Close(IDCANCEL);
            return ;
        }

        mpWiFiServerAddressEdt->SetText(msThroughputConfig.mstrWiFiScanIPAddress.c_str());
        mpIperfIPAddressEdt->SetText(msThroughputConfig.mstrIperfServerIPAddress.c_str());
        mpTestTimeEdt->SetText(msThroughputConfig.mstrTestTime.c_str());
        mpTestRangeEdt->SetText(msThroughputConfig.mstrTestRange.c_str());
    }

    void CCJGW_ThroughTestConfigDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"OkBtn")
            {
                msThroughputConfig.mstrWiFiScanIPAddress = mpWiFiServerAddressEdt->GetText().GetData();
                msThroughputConfig.mstrTestRange = mpTestRangeEdt->GetText().GetData();
                msThroughputConfig.mstrTestTime = mpTestTimeEdt->GetText().GetData();
                JGW_FormatWString(msThroughputConfig.mstrIperfCommandTimeOut,L"%d",_ttoi(msThroughputConfig.mstrTestTime.c_str()) + 10);
                msThroughputConfig.mstrIperfServerIPAddress = mpIperfIPAddressEdt->GetText().GetData();
                //! adb shell iperf -c 192.168.50.243 -i 1 -t 30 -P 8
                msThroughputConfig.mstrIperfCommand = JGW_GetFormatWString(L"adb shell iperf -c %s -i 1 -t %s -P 8 > %s",msThroughputConfig.mstrIperfServerIPAddress.c_str(),msThroughputConfig.mstrTestTime.c_str(),GetGlobalEnvironment()->GetString(L"TSE_IPER_LOG_FILE_PATH").c_str());

                Close(IDOK);
            }
            else if (msg.pSender->GetName() == L"CancelBtn")
            {
                Close(IDCANCEL);
            }
        }
    }

    void CCJGW_ThroughTestConfigDlg::OnCloseWindow()
    {

    }
}