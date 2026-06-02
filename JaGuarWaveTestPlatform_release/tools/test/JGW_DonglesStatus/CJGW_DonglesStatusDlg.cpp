#include "StdAfx.h"
#include "CJGW_DonglesStatusDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <string>
#include <vector>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "resource.h"

namespace JGW
{
#define GB_UNIT_DEFINE (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024
    const LONGLONG gGBUint = GB_UNIT_DEFINE;
    CCJGW_DonglesStatusDlg::CCJGW_DonglesStatusDlg(std::wstring& strNetCardInfo):mstrNetCardInfo(strNetCardInfo),mcNetCardMonitorThread(strNetCardInfo),mbColseWindows(true)
    {
        msDonglesMonitorParam.mbOpenDonglesPort = true;
    }


    CCJGW_DonglesStatusDlg::~CCJGW_DonglesStatusDlg(void)
    {
        OnCloseWindow();
    }

    void CCJGW_DonglesStatusDlg::OnCloseWindow()
    {
        if (!mbColseWindows)
        {
            mcTrayIco.DeleteTrayIcon();
            mcNetCardMonitorThread.OnBreakLoop();
            mcNetCardMonitorThread.ExitThread(false);
            mcDonglesTRxSectorThread.ExitDonglesPortThread();
            mcNetCardMonitorThread.TerminateMessageThread();
            mcDonglesTRxSectorThread.TerminateMessageThread();
            mbColseWindows = true;
        }  
    }

    //! WM_NCLBUTTONDBLCLK
    void CCJGW_DonglesStatusDlg::OnNotify(TNotifyUI& msg)
    {
        if(msg.sType == DUI_MSGTYPE_CLICK)
        {
        }     
    }

    void CCJGW_DonglesStatusDlg::OnInitWindow()
    {
        //! init view control
        {
            bool bSuccess = true;
            mpBpsLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"bpsLab"));
            mpPpsCountLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"ppsCountLab"));
            bSuccess &= (NULL != mpBpsLab);
            bSuccess &= (NULL != mpPpsCountLab);

            /*mpDonglesLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"donglesLayout"));*/
            mpSerialNumberLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"serialNumberLab"));
            mpMacAddressLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"macAddressLab"));
            mpFirmwareVersionLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"firmwareVersionLab"));
            mpUsbVersionLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"usbVersion"));
            bSuccess &= (NULL != mpSerialNumberLab);
            bSuccess &= (NULL != mpMacAddressLab);
            bSuccess &= (NULL != mpFirmwareVersionLab);
            bSuccess &= (NULL != mpUsbVersionLab);

            if (!bSuccess)
            {
                MessageBox(m_PaintManager.GetPaintWindow(),_T("Æ¤·ô¿Ø¼þÈ±Ê§"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
                Close(IDCANCEL);
                return ;
            }
        }
        {
            mbColseWindows = false;
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_DonglesStatusDlg::OnEventCustomMessage,0);
        }
        {
            mcNetCardMonitorThread.CreateMessageThread(0,0,true);
            Sleep(250);
            mcNetCardMonitorThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(GetAsyncWndMessage()));

            msDonglesMonitorParam.mhMainWnd = m_PaintManager.GetPaintWindow();
            mcDonglesTRxSectorThread.CreateMessageThread(0,0,true);
            Sleep(250);
            mcDonglesTRxSectorThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(GetAsyncWndMessage()),LPARAM(&msDonglesMonitorParam));
        }
        {
            SetWindowPos(m_PaintManager.GetPaintWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
        }
        {
            mcTrayIco.CreateTrayIcon(m_PaintManager.GetPaintWindow(),IDI_SMALL,L"Dongels Status");
        }
    }

    bool CCJGW_DonglesStatusDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        //! ¹ýÂËµô±êÌâÀ¸Ë«»÷ÃüÁîÏìÓ¦
        if (WM_NCLBUTTONDBLCLK == pTEventUI->Type) return false;
        switch (pTEventUI->Type)
        {
        case WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS:
            Close(IDCANCEL);
            break;
        case WM_HWND_TEST_LIST_MSG:
            OnRspTestInfoMsg((const wchar_t*)pTEventUI->wParam);
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
        default:
            break;
        }
        return true;
    }

    void CCJGW_DonglesStatusDlg::OnShowTrayMenuWnd(TEventUI* pTEventUI)
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

    void CCJGW_DonglesStatusDlg::OnMenuElementClick(TEventUI* pTEventUI)
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

    void CCJGW_DonglesStatusDlg::GetBpsString(const ULONGLONG& uBps,std::wstring& strBps)
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

    void CCJGW_DonglesStatusDlg::OnRspTestInfoMsg(const wchar_t* strParam)
    {
        static ULONGLONG gumxBps = 0;
        std::vector<std::wstring> vstrNetInfo;
        JGW_ParserStrW(strParam,L",",vstrNetInfo);
        if (3 != vstrNetInfo.size()) return;

        ULONGLONG uBps = _ttoi64(vstrNetInfo.at(1).c_str());
        std::wstring strTemp;
        GetBpsString(uBps,strTemp);

        mpBpsLab->SetText(strTemp.c_str());
        mpPpsCountLab->SetText(vstrNetInfo.at(2).c_str());
    }

    void CCJGW_DonglesStatusDlg::OnResDonglesInfoMsg()
    {
        mpSerialNumberLab->SetText(msDonglesMonitorParam.mstrSN.c_str());
        mpMacAddressLab->SetText(msDonglesMonitorParam.mstrMac.c_str());
        mpUsbVersionLab->SetText(msDonglesMonitorParam.mstrUsbSpeed.c_str());
        mpFirmwareVersionLab->SetText(msDonglesMonitorParam.mstrFW.c_str());
    }
}