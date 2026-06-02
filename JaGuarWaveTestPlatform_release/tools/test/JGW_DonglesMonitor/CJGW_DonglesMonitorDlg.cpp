#include "StdAfx.h"
#include "CJGW_DonglesMonitorDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <string>
#include <vector>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "resource.h"
#include "CJGW_DonglesMCSStatisticsDlg.h"

namespace JGW
{
#define GB_UNIT_DEFINE (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024
    const LONGLONG gGBUint = GB_UNIT_DEFINE;
    CCJGW_DonglesMonitorDlg::CCJGW_DonglesMonitorDlg(std::string& strNetCardInfo):mstrNetCardInfo(strNetCardInfo),mbColseWindows(true),mIfTableTrafficMonitor(mstrNetCardInfo)
    {
        msDonglesMonitorParam.mbOpenDonglesPort = true;   
        msDonglesMonitorParam.mbPerasoSocImpSSH = true;
        msDonglesMonitorParam.mnSSHPort = 22;
        msDonglesMonitorParam.mstrBinPath = "/userdata/bin/prs_serial ";
        msDonglesMonitorParam.mstrUserName = "root";
        msDonglesMonitorParam.mstrUserPassword = "lion";
        msDonglesMonitorParam.mstrHost = "192.168.213.1";
        LoadMonitorConfig();
        for (int i = 0;i < 13;i ++) muMcsStatistics[i] = 0;
    }


    CCJGW_DonglesMonitorDlg::~CCJGW_DonglesMonitorDlg(void)
    {
        OnCloseWindow();
    }

    void CCJGW_DonglesMonitorDlg::OnCloseWindow()
    {
        if (!mbColseWindows)
        {
            KillTimer(m_hWnd,WM_TIMER_UPDATE_TIME);
            mcTrayIco.DeleteTrayIcon();
            mcDonglesTRxSectorThread.ExitDonglesPortThread();
            mcDonglesTRxSectorThread.TerminateMessageThread();
            if (mFileStream.is_open()) mFileStream.close();
            mbColseWindows = true;
            m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_DonglesMonitorDlg::OnEventCustomMessage,0);
        }
    }

    void CCJGW_DonglesMonitorDlg::OnNotify(TNotifyUI& msg)
    {
        if(msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender->GetName() == L"startBtn")
            {
                if (0 == mpTabMain->GetCurSel())
                {
                    this->ResizeClient(380, 630);
                    mpTabMain->SelectItem(1);
                    this->CenterWindow();
                }
                else
                {
                    this->ResizeClient(380, 240);
                    mpTabMain->SelectItem(0);
                    this->CenterWindow();
                }
            }
            else if (msg.pSender == mpEnableTxBeamBtn)
            {
                mcDonglesTRxSectorThread.SendCommandToDonglesPort("mib s 12-3-1-40 1\n");
            }
            else if (msg.pSender == mpEnableRxBeamBtn)
            {
                mcDonglesTRxSectorThread.SendCommandToDonglesPort("mib s 12-2-1-40 1\n");
            }
            else if (msg.pSender == mpDisableTxBeamBtn)
            {
                mcDonglesTRxSectorThread.SendCommandToDonglesPort("mib s 12-3-1-40 0\n");
            }
            else if (msg.pSender == mpDisableRxBeamBtn)
            {
                mcDonglesTRxSectorThread.SendCommandToDonglesPort("mib s 12-2-1-40 0\n");
            }
            else if (msg.pSender == mpWriteTxSectorBtn)
            {
                std::string strTemp;
                JGW_FormatString(strTemp,"mib s 12-3-1-41 %s00",mpTxSectorEdt->GetText().GetStringA().c_str());
                mcDonglesTRxSectorThread.SendCommandToDonglesPort(strTemp.c_str());
            }
            else if (msg.pSender == mpWriteRxSectorBtn)
            {
                std::string strTemp;
                JGW_FormatString(strTemp,"mib s 12-2-1-41 %s00",mpRxSectorEdt->GetText().GetStringA().c_str());
                mcDonglesTRxSectorThread.SendCommandToDonglesPort(strTemp.c_str());
            }
            else if (msg.pSender == mpAdvanceMCSBtn)
            {
                CCJGW_DonglesMCSStatisticsDlg mcsStatisticsDlg(muMcsStatistics,m_hWnd);
                mcsStatisticsDlg.DoModal();
            }
        }     
        else if (msg.sType == DUI_MSGTYPE_RETURN)
        {
            if (msg.pSender == mpWReadRichEdt)
                OnRspRichEditReturnKey();
        }
    }

    void CCJGW_DonglesMonitorDlg::OnRspRichEditReturnKey()
    {
        std::string strWrite = mpWReadRichEdt->GetText().GetStringA();
        size_t i = strWrite.rfind('>');
        if (std::wstring::npos == i) return ;
        std::string strCommand = strWrite.substr(i,strWrite.length());
        JGW_ReplaceStringA(strCommand,"\r","");
        JGW_ReplaceStringA(strCommand,"\n","");
        if (strCommand.empty()) return ;
        strCommand.erase(0,1);
        CHARRANGE ch = {i + 1,strWrite.length()};
        mpWReadRichEdt->SetSel(ch);
        mpWReadRichEdt->ReplaceSelW(L"");   
        strCommand += "\n";
        mcDonglesTRxSectorThread.SendCommandToDonglesPort(strCommand.c_str());
    }

    void CCJGW_DonglesMonitorDlg::OnInitWindow()
    {
        //! init view control
        {
            bool bSuccess = true;
            mpBpsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"bpsLab"));
            mpPpsCountLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"ppsCountLab"));
            mpTxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"txSectorLab"));
            mpRxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"rxSectorLab"));
            bSuccess &= (NULL != mpBpsLab);
            bSuccess &= (NULL != mpPpsCountLab);
            bSuccess &= (NULL != mpTxSectorLab);
            bSuccess &= (NULL != mpRxSectorLab);

            /*mpDonglesLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"donglesLayout"));*/
            mpTabMain = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"TabMain"));
            mpSerialNumberLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"serialNumberLab"));
            mpMacAddressLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"macAddressLab"));
            mpFirmwareVersionLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"firmwareVersionLab"));
            mpUsbVersionLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"usbVersion"));
            bSuccess &= (NULL != mpTabMain);
            bSuccess &= (NULL != mpSerialNumberLab);
            bSuccess &= (NULL != mpMacAddressLab);
            bSuccess &= (NULL != mpFirmwareVersionLab);
            bSuccess &= (NULL != mpUsbVersionLab);

            mpMcsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"mcsLab"));
            mpAdvanceMCSBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"advanceMCSBtn"));
            mpRssiLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"rssiLab"));
            mpLocalRssiLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"LocalRssiLabel"));
            mpBestRxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"bestRxSectorLab"));
            mpBestTxSectorLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"bestTxSectorLab"));
            mpEnableTxBeamBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"enableTxBeamBtn"));
            mpDisableTxBeamBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"disableTxBeamBtn"));
            mpEnableRxBeamBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"enableRxBeamBtn"));
            mpDisableRxBeamBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"disableRxBeamBtn"));
            mpTxSectorEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"txSectorEdit"));
            mpWriteTxSectorBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"wTxSecBtn"));
            mpRxSectorEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"rxSectorEdit"));
            mpWriteRxSectorBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"rRxSecBtn"));
            mpWReadRichEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"readEdt"));
            bSuccess &= (NULL != mpMcsLab);     
            bSuccess &= (NULL != mpAdvanceMCSBtn);
            bSuccess &= (NULL != mpRssiLab);
            bSuccess &= (NULL != mpLocalRssiLab);
            bSuccess &= (NULL != mpBestRxSectorLab);
            bSuccess &= (NULL != mpBestTxSectorLab);
            bSuccess &= (NULL != mpEnableTxBeamBtn);
            bSuccess &= (NULL != mpDisableTxBeamBtn);
            bSuccess &= (NULL != mpEnableRxBeamBtn);
            bSuccess &= (NULL != mpDisableRxBeamBtn);
            bSuccess &= (NULL != mpTxSectorEdt);
            bSuccess &= (NULL != mpWriteTxSectorBtn);
            bSuccess &= (NULL != mpRxSectorEdt);
            bSuccess &= (NULL != mpWriteRxSectorBtn);
            bSuccess &= (NULL != mpWReadRichEdt);

            if (!bSuccess)
            {
                MessageBox(m_PaintManager.GetPaintWindow(),_T("皮肤控件缺失"),_T("初始化失败"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
        }
        {
            mbColseWindows = false;
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_DonglesMonitorDlg::OnEventCustomMessage,0);
        }
        {
            msDonglesMonitorParam.mhMainWnd = m_PaintManager.GetPaintWindow();
            mcDonglesTRxSectorThread.CreateMessageThread(0,0,true);
            Sleep(250);
            mcDonglesTRxSectorThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(GetAsyncWndMessage()),LPARAM(&msDonglesMonitorParam));
        }
        {
            SetWindowPos(m_PaintManager.GetPaintWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
            CreateLogFile();
        }
        {
            mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"Dongels Monitor");
        }
        {
            mIfTableTrafficMonitor.GetNetCardInfo();
            mIfTableTrafficMonitor.SelectNetCardInfo();
            SetTimer(GetHWND(),WM_TIMER_UPDATE_TIME,1000,NULL);
            mtm.restart();
        }   
    }

    bool CCJGW_DonglesMonitorDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS:
            Close(IDCANCEL);
            break;
        case WM_HWND_TEST_LIST_MSG:
            OnRspTestInfoMsg((const wchar_t*)pTEventUI->wParam);
            break;
        case WM_HWND_TX_RX_SECTOR_MSG:
            OnRspTRxSectorMsg((const wchar_t*)pTEventUI->wParam);
            break;
        case WM_MENUCLICK:
            OnMenuElementClick(pTEventUI);
            break;
        case UIEVENT_TRAYICON:
            OnShowTrayMenuWnd(pTEventUI);
            break;
        case WM_HWND_UPDATE_DONGLES_INFO:
            OnResDonglesInfoMsg();
            break;
        case WM_HWND_UPDATE_RSSI_INFO:
            OnResRssiInfoMsg((int)pTEventUI->lParam,(int)pTEventUI->wParam);
            break;
        case WM_HWND_UPDATE_MCS_INFO:
            OnResMcsInfoMsg((int)pTEventUI->wParam);
            break;
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnRspAppendRichedt((const wchar_t*)pTEventUI->wParam);
            break;
        case WM_TIMER:
            if (WM_TIMER_UPDATE_TIME == (int)pTEventUI->wParam) OnUpdateBpsDataInfo();
        default:
            break;
        }
        return true;
    }

    void CCJGW_DonglesMonitorDlg::OnRspAppendRichedt(const wchar_t* strParam)
    {
        mpWReadRichEdt->AppendText(strParam);  
        mpWReadRichEdt->EndDown();
    }

    void CCJGW_DonglesMonitorDlg::OnShowTrayMenuWnd(TEventUI* pTEventUI)
    {
        if (WM_RBUTTONUP == pTEventUI->lParam)
        {
            ::SetForegroundWindow(this->m_hWnd);

            CMenuWnd* pMenu = new CMenuWnd();
            CPoint point(0,0);
            GetCursorPos(&point);
            pMenu->Init(NULL, _T("tray_menu.xml"), point, &m_PaintManager, &m_MenuCheckInfo,eMenuAlignment_Left | eMenuAlignment_Bottom );

            ::PostMessage(m_hWnd,WM_NULL,0,0);
        }  
    }

    void CCJGW_DonglesMonitorDlg::OnMenuElementClick(TEventUI* pTEventUI)
    {
        const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
        if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("exitApplicationMenu")))
        {
            Close(IDCANCEL);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("donglesPort")))
        {
            msDonglesMonitorParam.mbOpenDonglesPort = !msDonglesMonitorParam.mbOpenDonglesPort;
        }
    }

    void CCJGW_DonglesMonitorDlg::CreateLogFile()
    {
        std::wstring strLogFilePath;
        strLogFilePath = JGW_GetApplicationFolder(); 
        strLogFilePath += JGW_GetFileNameToCurrentTimeStamp();
        strLogFilePath += L"\\";
        JGW_CreateDirectory(strLogFilePath.c_str(),true);
        strLogFilePath += L"trafficmonitor.log";

        mFileStream.imbue( std::locale("chs") );
        mFileStream.open( strLogFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!mFileStream.is_open())
            MessageBox(m_PaintManager.GetPaintWindow(),L"无法保存LOG文件",L"警告",MB_ICONWARNING);
    }

    void CCJGW_DonglesMonitorDlg::GetBpsString(const ULONGLONG& uBps,std::wstring& strBps)
    {
        if (uBps < 1024)
        {
            JGW_FormatWString(strBps,L"%I64u b/s",uBps);
        }
        else if (uBps >= 1024 && uBps < 1024 * 1024)
        {
            double speed = (double)uBps / 1024.00;
            JGW_FormatWString(strBps,L"%.4lf Kb/s",speed/*uBps / 1024*/);
        }
        else if (uBps >= 1024 * 1024 && uBps < 1024 * 1024 * 1024)
        {
            double speed = (double)uBps / (1024.00 * 1024.00);
            JGW_FormatWString(strBps,L"%.4lf Mb/s",speed/*uBps / 1024*/);
        }
        else if (uBps >= 1024 * 1024 * 1024 && uBps < gGBUint)
        {
            double speed = (double)uBps / (1024.00 * 1024.00 * 1024.00);
            JGW_FormatWString(strBps,L"%.4lf Gb/s",speed/*uBps / 1024*/);
        }
        else
        {
            double speed = (double)uBps / (1024.00 * 1024.00 * 1024.00 * 1024.00);
            JGW_FormatWString(strBps,L"%.4lf Tb/s",speed/*uBps / 1024*/);
        }
    }

    void CCJGW_DonglesMonitorDlg::OnUpdateBpsDataInfo()
    {
        LONGLONG dwPps = 0;
        LONGLONG dwCounts = mIfTableTrafficMonitor.GetTotalSentRecvPps(dwPps);
        ULARGE_INTEGER Bps;

        double dwTime = mtm.elapsed();
        mtm.restart();
        u_int delay = dwTime * 1000000;
        /* 获取每秒的比特数b/s */
        Bps.QuadPart = (dwCounts * 8 * 1000000) / (delay);

        std::wstring strTemp; 
        SYSTEMTIME sys;
        GetLocalTime( &sys );
        JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%I64u,%I64u",sys.wHour, sys.wMinute, sys.wSecond,sys.wMilliseconds,Bps.QuadPart,dwPps);
        OnRspTestInfoMsg(strTemp.c_str());
    }

    void CCJGW_DonglesMonitorDlg::OnRspTestInfoMsg(const wchar_t* strParam)
    {
        static ULONGLONG gumxBps = 0;
        std::vector<std::wstring> vstrNetInfo;
        JGW_ParserStrW(strParam,L",",vstrNetInfo);
        if (3 != vstrNetInfo.size()) return;

        ULONGLONG uBps = _ttoi64(vstrNetInfo.at(1).c_str());
        std::wstring strTemp;
        GetBpsString(uBps,strTemp);

        mpBpsLab->SetText(strTemp.c_str());

        DWORD speed = mIfTableTrafficMonitor.GetNetCardSpeed();
        JGW_FormatWString(strTemp,L"speed : %u",speed);
        if (0 != JGW_WStrComparenoCaseWStr(strTemp.c_str(),mpAdvanceMCSBtn->GetToolTip().GetData()))
        {
            mpAdvanceMCSBtn->SetToolTip(strTemp.c_str());
        }
        
        mpPpsCountLab->SetText(vstrNetInfo.at(2).c_str());
        if (gumxBps < uBps)
        {
            mpBestTxSectorLab->SetText(mstrCurrentTxSector.c_str());
            mpBestRxSectorLab->SetText(mstrCurrentRxSector.c_str());
        }
        int mcs = mcDonglesTRxSectorThread.GetMcs();
        if (mcs >= 0&&mcs <= 12) muMcsStatistics[mcs]++;
        if (!mFileStream.is_open()) return ;
        mFileStream << vstrNetInfo.at(0) << L"\t";
        mFileStream << vstrNetInfo.at(1)  << L"\t";
        mFileStream << vstrNetInfo.at(2)  << L"\t";
        mFileStream << mstrCurrentTxSector  << L"\t";
        mFileStream << mstrCurrentRxSector  << L"\t";
        mFileStream << mpRssiLab->GetText().GetData() << L"\t";
        mFileStream << mpLocalRssiLab->GetText().GetData() << L"\t";
        mFileStream << mcs << std::endl;
    }

    void CCJGW_DonglesMonitorDlg::OnRspTRxSectorMsg(const wchar_t* strParam)
    {
        if (!strParam) 
        {
            mstrCurrentTxSector = L"";
            mstrCurrentRxSector = L"";        
            mpRssiLab->SetText(L"-128 dB");
            mpLocalRssiLab->SetText(L"-128 dB");
        }
        else
        {
            std::vector<std::wstring> vstrTRxSectorInfo;
            JGW_ParserStrW(strParam,L"-",vstrTRxSectorInfo);
            if (2 != vstrTRxSectorInfo.size()) return;
            mstrCurrentTxSector = vstrTRxSectorInfo.at(0);
            mstrCurrentRxSector = vstrTRxSectorInfo.at(1);
            JGW_ReplaceStringW(mstrCurrentTxSector,L"00 ",L"");
            JGW_ReplaceStringW(mstrCurrentRxSector,L"00 ",L"");
        }
        mpTxSectorLab->SetText(mstrCurrentTxSector.c_str());
        mpRxSectorLab->SetText(mstrCurrentRxSector.c_str());
    }

    void CCJGW_DonglesMonitorDlg::OnResDonglesInfoMsg()
    {
        mpSerialNumberLab->SetText(msDonglesMonitorParam.mstrSN.c_str());
        mpMacAddressLab->SetText(msDonglesMonitorParam.mstrMac.c_str());
        mpUsbVersionLab->SetText(msDonglesMonitorParam.mstrUsbSpeed.c_str());
        mpFirmwareVersionLab->SetText(msDonglesMonitorParam.mstrFW.c_str());
    }

    void CCJGW_DonglesMonitorDlg::OnResRssiInfoMsg(int rssi_type,int nRssi)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d dB",nRssi);

        switch (rssi_type)
        {
        case 0:
            mpRssiLab->SetText(strTemp.c_str());
        	break;
        case 1:
            mpLocalRssiLab->SetText(strTemp.c_str());
            break;
        default:
            mpRssiLab->SetText(strTemp.c_str());
            break;
        }
    }

    void CCJGW_DonglesMonitorDlg::OnResMcsInfoMsg(int mcs)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d",mcs);
        mpMcsLab->SetText(strTemp.c_str());
    }

    void CCJGW_DonglesMonitorDlg::LoadMonitorConfig()
    {
        std::wstring strConfigFilePath(JGW_GetApplicationFolder());
        strConfigFilePath += L"dongles_monitor.ini";
        CCJGW_ConfigIni monitorConfigIni;

        monitorConfigIni.InitIniFilePath(strConfigFilePath.c_str());
        msDonglesMonitorParam.mbPerasoSocImpSSH = monitorConfigIni.GetIniKeyBoolValue(L"MAIN",L"is_ssh");
        msDonglesMonitorParam.mnSSHPort = monitorConfigIni.GetIniKeyIntValue(L"SSH",L"port",22);
        msDonglesMonitorParam.mstrBinPath = monitorConfigIni.GetIniKeyValueA(L"SSH",L"peraso_bin",L"/userdata/bin/prs_serial") + " ";
        msDonglesMonitorParam.mstrUserName = monitorConfigIni.GetIniKeyValueA(L"SSH",L"username",L"root");
        msDonglesMonitorParam.mstrUserPassword = monitorConfigIni.GetIniKeyValueA(L"SSH",L"password",L"lion");
        msDonglesMonitorParam.mstrHost = monitorConfigIni.GetIniKeyValueA(L"SSH",L"host",L"192.168.213.1");
    }
}
/*
memset(&m_nid,0,sizeof(NOTIFYICONDATA));
m_nid.cbSize = sizeof(NOTIFYICONDATA);
m_nid.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SMALL));
m_nid.hWnd = m_hWnd;
lstrcpy(m_nid.szTip, _T("Duilib程序"));
m_nid.uCallbackMessage = WM_SHOWTASK;
m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
Shell_NotifyIcon(NIM_ADD, &m_nid);
ShowWindow(SW_HIDE);

//如果在图标中单击左键则还原
if( lParam == WM_LBUTTONDOWN )
{             
Shell_NotifyIcon(NIM_DELETE, &m_nid);//删除托盘图标
ShowWindow(SW_SHOWNORMAL);//显示主窗口
}
//如果在图标中单击右键则弹出声明式菜单
if( lParam == WM_RBUTTONDOWN )
{             
//获取鼠标坐标  
POINT pt; GetCursorPos(&pt);  
SetForegroundWindow(m_hWnd); //右击后点别地可以清除“右击出来的菜单”

HMENU hMenu; //托盘菜单    win32程序使用的是HMENU，如果是MFC程序可以使用CMenu
hMenu = CreatePopupMenu();//生成托盘菜单   
AppendMenu(hMenu, MF_STRING, WM_ONCLOSE, _T("退出"));  

int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd,  NULL);  
if(cmd == WM_ONCLOSE)   
{
//退出程序
m_nid.hIcon = NULL;
Shell_NotifyIcon(NIM_DELETE, &m_nid);
::PostQuitMessage(0);
}
}

*/
