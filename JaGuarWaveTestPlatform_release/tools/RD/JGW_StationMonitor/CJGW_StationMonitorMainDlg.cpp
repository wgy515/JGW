#include "StdAfx.h"
#include "CJGW_StationMonitorMainDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define GB_UNIT_DEFINE (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024
namespace JGW
{
    const LONGLONG gGBUint = GB_UNIT_DEFINE;
    CCJGW_StationMonitorMainDlg::CCJGW_StationMonitorMainDlg(void) : mpReportStationStatusThread(NULL),mstrStationServerUrl("http://%s/cgi-bin/leopard.fcgi"),mpStationServiceThread(NULL)
    {
        Log4WI(L"Start Station Monitor Tool");
        msStationMonitorUiInfo.mulDownloadSpeed = 0;
        msStationMonitorUiInfo.mfRxDropRate = 0.00;
        msStationMonitorUiInfo.mfTxDropRate = 0.00;
        msStationMonitorUiInfo.mulUploadSpeed = 0;
        msStationMonitorUiInfo.mbConnectPeraso = false;
    }


    CCJGW_StationMonitorMainDlg::~CCJGW_StationMonitorMainDlg(void)
    {
    }

    void CCJGW_StationMonitorMainDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_StationMonitorMainDlg::OnEventCustomMessage,0);
        KillTimer(m_hWnd,WM_TIMER_REPORT_STATION_STATUS);
        KillTimer(m_hWnd,WM_TIMER_REPORT_TRAFFIC_SPEED_INFO);
        if (NULL != mpReportStationStatusThread)
        {
            mpReportStationStatusThread->TerminateMessageThread();
            delete mpReportStationStatusThread;
            mpReportStationStatusThread = NULL;
        }
        if (NULL != mpStationServiceThread)
        {
            mpStationServiceThread->TerminateMessageThread();
            delete mpStationServiceThread;
            mpStationServiceThread = NULL;
        }
    }

    void CCJGW_StationMonitorMainDlg::OnInitWindow()
    {
        if (!OnInitUIControlClass())
        {
            Close();
            return ;
        }
        {
            SetTimer(GetHWND(),WM_TIMER_REPORT_STATION_STATUS,1000,NULL);
            SetTimer(GetHWND(),WM_TIMER_REPORT_TRAFFIC_SPEED_INFO,1000,NULL);
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_StationMonitorMainDlg::OnEventCustomMessage,0);
        }
        {
            //! 上报station状态线程，每隔一秒左右上报数据
            mpReportStationStatusThread = new CCJGW_ReportStationStatusThread(mstrStationServerUrl);
            mpReportStationStatusThread->CreateMessageThread(0,0,true);
            //! 开启station server线程，接收上级路由发送的指令消息
            try
            {
                mpStationServiceThread = new CCJGW_StationServiceThread();
            }
            catch (boost::system::system_error& se)
            {
                MessageBoxA(m_hWnd,se.what(),"Station Server Thread Start Fail",MB_ICONERROR);
                Close(IDCANCEL);
                return;
            }
            mpStationServiceThread->CreateMessageThread(0,0,true);
            mpStationServiceThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&msStationMonitorUiInfo));
        }
    }

    void CCJGW_StationMonitorMainDlg::OnNotify(TNotifyUI& msg)
    {

    }

    bool CCJGW_StationMonitorMainDlg::OnInitUIControlClass()
    {
        bool bSuccess = true;

        mpUploadSpeedLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"uploadLab"));
        mpDownloadSpeepLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"downloadLab"));
        bSuccess &= (NULL != mpUploadSpeedLab);
        bSuccess &= (NULL != mpDownloadSpeepLab);

        mpTxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"txSectorLab"));
        mpRxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"rxSectorLab"));
        bSuccess &= (NULL != mpTxSectorLab);
        bSuccess &= (NULL != mpRxSectorLab);

        mpTxMcsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"TxMcsLab"));
        mpRxMcsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"RxMcsLab"));
        bSuccess &= (NULL != mpTxMcsLab);
        bSuccess &= (NULL != mpRxMcsLab);

        mpIPAddressLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"IPAddressLab"));
        mpMcsAddressLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"macAddressLab"));
        mpRssiLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"rssiLab"));
        bSuccess &= (NULL != mpIPAddressLab);
        bSuccess &= (NULL != mpMcsAddressLab);
        bSuccess &= (NULL != mpRssiLab);

        mpTxDropRateLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"TxDropRateLab"));
        mpRxDropRateLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"RxDropRateLab"));
        bSuccess &= (NULL != mpTxDropRateLab);
        bSuccess &= (NULL != mpRxDropRateLab);

        return bSuccess;
    }

    bool CCJGW_StationMonitorMainDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_HWND_REPORT_INFO_MSG:
            OnUpdateUIDisplay();
            SetTimer(GetHWND(),WM_TIMER_REPORT_STATION_STATUS,1000,NULL);
            break;
        case WM_TIMER:
            OnTimerToID((int)pTEventUI->wParam);   
        default:
            break;
        }
        return true;
    }

    void CCJGW_StationMonitorMainDlg::OnTimerToID(int nIDEvent)
    {
        static S_NETWORK_TRAFFIC_INFO gsNetWorkTrafficInfo;

        if (WM_TIMER_REPORT_STATION_STATUS == nIDEvent) 
        {
            KillTimer(m_hWnd,WM_TIMER_REPORT_STATION_STATUS);
            mpReportStationStatusThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(&msStationMonitorUiInfo),LPARAM(&mAsyncWndMessage));
        }
        else if (WM_TIMER_REPORT_TRAFFIC_SPEED_INFO == nIDEvent)
        {
            double dwTime = mtm.elapsed();
            mtm.restart();
            //! 获取网卡信息
            u_int delay = static_cast<u_int>(dwTime * 1000000);
            /* 获取每秒的比特数b/s */
            mAdapterCommonInfo.GetNetWorkTrafficInfoToDescription(msStationMonitorUiInfo.mstrNetCardConnectName,gsNetWorkTrafficInfo);

            msStationMonitorUiInfo.mbConnectPeraso = gsNetWorkTrafficInfo.mbConnectNet;
            msStationMonitorUiInfo.mfRxDropRate = gsNetWorkTrafficInfo.mfRxDropRate;
            msStationMonitorUiInfo.mfTxDropRate = gsNetWorkTrafficInfo.mfTxDropRate;

            if (NULL != msStationMonitorUiInfo.mstrAnsiMacAddress.compare(gsNetWorkTrafficInfo.mstrMac))
            {
                CCJGW_CriticalSectionAutoLock lock(msStationMonitorUiInfo.mMacAddressCriticalSectionLock);
                msStationMonitorUiInfo.mstrAnsiMacAddress = gsNetWorkTrafficInfo.mstrMac;
                msStationMonitorUiInfo.mstrMacAddress = JGW_A2W(gsNetWorkTrafficInfo.mstrMac);
            }

            {
                CCJGW_CriticalSectionAutoLock lock(msStationMonitorUiInfo.mTrafficSpeedCriticalSectionLock);
                msStationMonitorUiInfo.mulUploadSpeed = (gsNetWorkTrafficInfo.mulOutOctets * 8 * 1000000) / (delay);
                msStationMonitorUiInfo.mulDownloadSpeed = (gsNetWorkTrafficInfo.mulInOctets * 8 * 1000000) / (delay);
                GetBpsString(msStationMonitorUiInfo.mulUploadSpeed,msStationMonitorUiInfo.mstrUploadSpeed);
                GetBpsString(msStationMonitorUiInfo.mulDownloadSpeed,msStationMonitorUiInfo.mstrDownloadSpeed);
            }

        }
    }

    void CCJGW_StationMonitorMainDlg::GetBpsString(const ULONGLONG& uBps,std::string& strBps)
    {
        if (uBps < 1024)
        {
            JGW_FormatString(strBps,"%I64u bps",uBps);
        }
        else if (uBps >= 1024 && uBps < 1024 * 1024)
        {
            double speed = (double)uBps / 1024.00;
            JGW_FormatString(strBps,"%.4lf Kbps",speed/*uBps / 1024*/);
        }
        else if (uBps >= 1024 * 1024 && uBps < 1024 * 1024 * 1024)
        {
            double speed = (double)uBps / (1024.00 * 1024.00);
            JGW_FormatString(strBps,"%.4lf Mbps",speed/*uBps / 1024*/);
        }
        else if (uBps >= 1024 * 1024 * 1024 && uBps < gGBUint)
        {
            double speed = (double)uBps / (1024.00 * 1024.00 * 1024.00);
            JGW_FormatString(strBps,"%.4lf Gbps",speed/*uBps / 1024*/);
        }
        else
        {
            double speed = (double)uBps / (1024.00 * 1024.00 * 1024.00 * 1024.00);
            JGW_FormatString(strBps,"%.4lf Tbps",speed/*uBps / 1024*/);
        }
    }

    void CCJGW_StationMonitorMainDlg::OnUpdateUIDisplay()
    {
        std::wstring strTemp;

        mpDownloadSpeepLab->SetText(JGW_A2W(msStationMonitorUiInfo.mstrDownloadSpeed).c_str());
        mpUploadSpeedLab->SetText(JGW_A2W(msStationMonitorUiInfo.mstrUploadSpeed).c_str());

        JGW_FormatWString(strTemp,L"%.02f",msStationMonitorUiInfo.mfTxDropRate);
        mpTxDropRateLab->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%.02f",msStationMonitorUiInfo.mfRxDropRate);
        mpRxDropRateLab->SetText(strTemp.c_str());

        mpTxSectorLab->SetText(msStationMonitorUiInfo.mstrTxSector.c_str());
        mpRxSectorLab->SetText(msStationMonitorUiInfo.mstrRxSector.c_str());
        mpTxMcsLab->SetText(msStationMonitorUiInfo.mstrTxMcs.c_str());
        mpRxMcsLab->SetText(msStationMonitorUiInfo.mstrRxMcs.c_str());

        {
            CCJGW_CriticalSectionAutoLock lock(msStationMonitorUiInfo.mIPAddressCriticalSectionLock);
            mpIPAddressLab->SetText(msStationMonitorUiInfo.mstrIPAddress.c_str());
        }

        mpMcsAddressLab->SetText(msStationMonitorUiInfo.mstrMacAddress.c_str());
        mpRssiLab->SetText(msStationMonitorUiInfo.mstrRssi.c_str());
    }
}