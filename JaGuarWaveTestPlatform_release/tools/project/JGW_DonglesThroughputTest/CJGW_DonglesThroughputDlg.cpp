#include "StdAfx.h"
#include "CJGW_DonglesThroughputDlg.h"
#include "CJGW_TcpServerThread.h"
#include "CJGW_TcpClientThread.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include "Common/Src/SysHelper.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "CJGW_ThroughputTestConfig.h"
#include "CJGW_TruntableConfigDlg.h"

namespace JGW
{
#define GB_UNIT_DEFINE (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024
    const LONGLONG gGBUint = GB_UNIT_DEFINE;

    CCJGW_DonglesThroughputDlg::CCJGW_DonglesThroughputDlg(void) : mpTcpStatsImplThread(NULL),mnTestTimeIndex(0),mIfTableTrafficMonitor(mstrConnectionName),mbSelectIfTableTrafficMonitor(false),mnCurrentTurnTableAngle(0),mDonglesComPort(mcSerialComPort)
    {
        msThroughputTestConfig.meSocketTerimalType = E_CLIENT_TYPE;
        msThroughputTestConfig.mstrIPAddress = L"0.0.0.0";
        msThroughputTestConfig.mnSocketPort = 5555;
        msThroughputTestConfig.meTranservicesType = E_SEND_TYPE;
        msThroughputTestConfig.mnTestTimes = 100;
        msThroughputTestConfig.mnIntervTime = 1;
        msThroughputTestConfig.mnSocketCount = 100;
        msThroughputTestConfig.mnDataLength = 5000;
        msThroughputTestConfig.meSendPolicy = SP_PACK;
        msThroughputTestConfig.mnServerThreadCount = min((::SysGetNumberOfProcessors() * 2 + 2), 500);
        msThroughputTestConfig.mnServerMaxConnSocketCount = 1000;
        msThroughputTestConfig.mpAsyncWndMessage = &mAsyncWndMessage;
        msThroughputTestConfig.mbEnableTurnTable = false;
        msThroughputTestConfig.mTurnTableStep = 2;
        msThroughputTestConfig.mfVelocity = 5;
        msThroughputTestConfig.mfAcceleration = 100;
        msThroughputTestConfig.mfDeceleration = 100;
        msThroughputTestConfig.mTurnTableAngleStart = 0;
        msThroughputTestConfig.mTurnTableAngleEnd = 360;
        msThroughputTestConfig.mbReadDonglesSN = false;
        msThroughputTestConfig.mstrHtmlFile = L"html\\current_echarts_qt.html";
    }


    CCJGW_DonglesThroughputDlg::~CCJGW_DonglesThroughputDlg(void)
    {
        //! OnTestStatusMsg(E_TEST_FAIL);
    }

    bool CCJGW_DonglesThroughputDlg::GetUIControl()
    {
        bool bSuccess = true;

        mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        mpTerminalType = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"terminalType"));
        mpIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ipAddressEdt"));
        mpSocketPortEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"socketPortEdt"));
        mpDataTransceivers = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"dataTransceivers"));
        mpTestIntervTimeLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"testIntervTimeLab"));
        mpTestIntervTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"testIntervTimeEdt"));
        bSuccess &= (NULL != mpWkeBrowser);
        bSuccess &= (NULL != mpTerminalType);
        bSuccess &= (NULL != mpIPAddressEdt);
        bSuccess &= (NULL != mpSocketPortEdt);
        bSuccess &= (NULL != mpDataTransceivers);
        bSuccess &= (NULL != mpTestIntervTimeLab);
        bSuccess &= (NULL != mpTestIntervTimeEdt);

        //! client config
        mpClientConfigHor = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"clientConfigHorizontalLayout"));
        mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"testTimeEdt"));
        mpSocketCount = static_cast<CEditUI*>(m_PaintManager.FindControl(L"socketCount"));
        mpDataLengthEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"dataLengthEdt"));
        bSuccess &= (NULL != mpClientConfigHor);
        bSuccess &= (NULL != mpTestTimeEdt);
        bSuccess &= (NULL != mpSocketCount);
        bSuccess &= (NULL != mpDataLengthEdt);
        //! services config
        mpServicesConfigHor = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"servicesConfigHorizontalLayout"));
        mpSendPolicy = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(L"sendPolicy"));
        mpThreadCountEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"threadCountEdt"));
        mpMaxSocketCount = static_cast<CEditUI*>(m_PaintManager.FindControl(L"maxSocketCount"));
        mpEditFuncMenu = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"editMenu"));
        bSuccess &= (NULL != mpSendPolicy);
        bSuccess &= (NULL != mpThreadCountEdt);
        bSuccess &= (NULL != mpMaxSocketCount);
        bSuccess &= (NULL != mpServicesConfigHor);
        bSuccess &= (NULL != mpEditFuncMenu);

        mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"startBtn"));
        mpStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"stopBtn"));
        //mpSaveXmlConfigBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"saveConfigBtn"));
        mpDataCollectionModeMenu = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"selDataCollectionModeBtn"));
        mpLogEdt  = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logEdt"));
        mpTurnTableStep  = static_cast<CEditUI*>(m_PaintManager.FindControl(L"turntableStepEdt"));
        mpEnableTurnTable  = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"enableTurnTableCheckBox"));
        mpTurntableStartAngle = static_cast<CEditUI*>(m_PaintManager.FindControl(L"turntableStar"));
        mpTurntableEndAngle = static_cast<CEditUI*>(m_PaintManager.FindControl(L"turntableEnd"));
        bSuccess &= (NULL != mpLogEdt);
        bSuccess &= (NULL != mpStartBtn);
        bSuccess &= (NULL != mpStopBtn);
        bSuccess &= (NULL != mpDataCollectionModeMenu);
        bSuccess &= (NULL != mpTurnTableStep);
        bSuccess &= (NULL != mpEnableTurnTable);
        bSuccess &= (NULL != mpTurntableStartAngle);
        bSuccess &= (NULL != mpTurntableEndAngle);
        // bSuccess &= (NULL != mpSaveXmlConfigBtn);

        if (!bSuccess)
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("控件加载失败"),_T("视图初始化错误"),MB_ICONERROR);
        }

        return bSuccess;
    }

    void CCJGW_DonglesThroughputDlg::OnInitWindow()
    {
        if (!GetUIControl()) return ;
        //! config
        {
            CCJGW_ThroughputTestConfig throughputTestConfig(msThroughputTestConfig);
            std::string strFilePath = JGW_W2A_W(JGW_GetApplicationFolder());
            strFilePath += DonglesThroughputTestConfigName;
            throughputTestConfig.LoadThroughputTestXMLConfig(strFilePath.c_str());
            if (msThroughputTestConfig.mbEnableTurnTable && !mSCLLibHelper.LoadSCLLibHelperLibrary())
            {
                Close();
                return ;
            }
            UpdateThroughputConfigToXMLConfig();
        }
        {
            mpWkeBrowser->InitBrowser(WM_TIMER_WKE_BROWSER);
            std::wstring strHtmlFile = JGW_RealativePathToAbsPath(msThroughputTestConfig.mstrHtmlFile.c_str());
            std::wstring strEncodeHtmlFilePath,strDecodeHtmlFilePath;
            JGW_URLEncodeW(strHtmlFile,strEncodeHtmlFilePath);
            JGW_URLDecodeW(strEncodeHtmlFilePath,strDecodeHtmlFilePath);
            mpWkeBrowser->LoadFile(strEncodeHtmlFilePath.c_str());
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGW_DonglesThroughputDlg::OnEventCustomMessage,0);
        }    
    }

    void CCJGW_DonglesThroughputDlg::SwitchTerminalTypeUI(int selPos)
    {
        if (E_CLIENT_TYPE == selPos)
        {
            mpClientConfigHor->SetVisible(true);
            mpServicesConfigHor->SetVisible(false);
            mpDataTransceivers->SelectItem(E_SEND_TYPE);
        }
        else
        {
            mpClientConfigHor->SetVisible(false);
            mpServicesConfigHor->SetVisible(true);    
            mpSendPolicy->SetEnabled(E_SEND_TYPE == mpDataTransceivers->GetCurSel());
            mpDataTransceivers->SelectItem(E_RECV_TYPE);
        }
    }

    void CCJGW_DonglesThroughputDlg::SwitchDataTransceiversTypeUI(int selPos)
    {
        if (E_RECV_TYPE == selPos)
        {
            mpTestIntervTimeLab->SetVisible(false);
            mpTestIntervTimeEdt->SetVisible(false);
            mpSendPolicy->SetEnabled(false);
        }
        else
        {
            mpTestIntervTimeLab->SetVisible(true);
            mpTestIntervTimeEdt->SetVisible(true);
            mpSendPolicy->SetEnabled(true);
        }
    }

    void CCJGW_DonglesThroughputDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (mpTerminalType == msg.pSender)
            {
                SwitchTerminalTypeUI(mpTerminalType->GetCurSel());
            }
            else if (mpDataTransceivers == msg.pSender)
            {
                SwitchDataTransceiversTypeUI(mpDataTransceivers->GetCurSel());
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpStartBtn)
            {
                if (!msThroughputTestConfig.mbReadDonglesSN)
                {
                    OnClickStartBtn();
                }
                else 
                {
                    mpStartBtn->SetEnabled(false);
                    if (!mUIDelegateThread.IsThreadRun()) mUIDelegateThread.CreateMessageThread(0,0,true);
                    mUIDelegateThread.BeginInvoke(this,static_cast< void (CWindowWnd::*)(WPARAM& wParam,LPARAM& lParam)>(&CCJGW_DonglesThroughputDlg::OnStartDonglesthroughputTest),WM_THREAD_TEST_START_MSG,NULL,NULL);
                }
            }
            else if (msg.pSender == mpStopBtn)
            {
                while (!mvTurnTableAngleQueue.empty()) mvTurnTableAngleQueue.pop();
                OnClickStopBtn();
            }
            else if (msg.pSender == mpDataCollectionModeMenu)
            {
                mpSelMenuBtn = msg.pSender;
                OnDataCollectionModeMenuClick(msg);
            }
            else if (msg.pSender == mpEditFuncMenu)
            {
                mpSelMenuBtn = msg.pSender;
                OnEditMenuClick(msg);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (msg.pSender == mpEnableTurnTable)
            {
                msThroughputTestConfig.mbEnableTurnTable = mpEnableTurnTable->GetCheck();
                if (msThroughputTestConfig.mbEnableTurnTable) 
                {
                    OnEnableTurnTable();
                }
                else 
                {
                    mpTurnTableStep->SetEnabled(false);
                    mpTurntableStartAngle->SetEnabled(false);
                    mpTurntableEndAngle->SetEnabled(false);
                    mSCLLibHelper.Close();
                }
            }
        }
    }

    void CCJGW_DonglesThroughputDlg::OnEnableTurnTable()
    {
        if (!OpenTurnTableComPort())
        {
            mpEnableTurnTable->SetCheck(false);
            return ;
        }
        mpTurnTableStep->SetEnabled(true);
        mpTurntableStartAngle->SetEnabled(true);
        mpTurntableEndAngle->SetEnabled(true);
    }

    void CCJGW_DonglesThroughputDlg::OnDataCollectionModeMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_PaintManager.GetPaintWindow(), &point);
        pMenu->Init(NULL,_T("data_menu.xml"),point,&m_PaintManager,&m_MenuCheckInfo);
        // CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo);
        CMenuUI* rootMenu = pMenu->GetMenuUI();
        CMenuElementUI* pDataPacketMenu = new CMenuElementUI;
        pDataPacketMenu->SetName(_T("dataPacketMenu"));
        pDataPacketMenu->SetText(_T("Data Packet"));
        pDataPacketMenu->SetShowExplandIcon(true);
        pDataPacketMenu->SetIcon(_T("right.png"));
        pDataPacketMenu->SetIconSize(9,9);
        pDataPacketMenu->SetCheckItem(true);
        if (!m_MenuCheckInfo[L"tcpipMenu"]) pDataPacketMenu->SetChecked(true);
        else pDataPacketMenu->SetChecked(false);

        std::vector<NetWorkConection>& vNetWorkConection = mIfTableTrafficMonitor.GetNetCardInfo();
        //! add all
        {
            CMenuElementUI* pNew = new CMenuElementUI;
            pNew->SetName(L"All");
            pNew->SetText(L"All");
            pNew->SetIcon(_T("right.png"));
            pNew->SetIconSize(9,9);
            pNew->SetCheckItem(true);
            if (pDataPacketMenu->GetChecked() && 0 == mstrConnectionName.compare("All"))
                pNew->SetChecked(true);
            else
                pNew->SetChecked(false);
            pDataPacketMenu->Add(pNew);
        }


        std::wstring strTemp;
        for (size_t i = 0;i < vNetWorkConection.size();i ++)
        {
            CMenuElementUI* pNew = new CMenuElementUI;
            strTemp = JGW_A2W(vNetWorkConection.at(i).description);
            pNew->SetName(strTemp.c_str());
            pNew->SetText(strTemp.c_str());
            pNew->SetIcon(_T("right.png"));
            pNew->SetIconSize(9,9);
            pNew->SetCheckItem(true);
            if (pDataPacketMenu->GetChecked() && 0 == mstrConnectionName.compare(vNetWorkConection.at(i).description))
                pNew->SetChecked(true);
            else
                pNew->SetChecked(false);

            pDataPacketMenu->Add(pNew);
        }
        rootMenu->Add(pDataPacketMenu);
        pMenu->ResizeMenu();
    }

    void CCJGW_DonglesThroughputDlg::OnEditMenuClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(m_PaintManager.GetPaintWindow(), &point);
        pMenu->Init(NULL,_T("edit_menu.xml"),point,&m_PaintManager,&m_MenuCheckInfo);
    }

    bool CCJGW_DonglesThroughputDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnRspAppendRichedt((const wchar_t*)pTEventUI->wParam);
            break;
        case WM_THREAD_TEST_START_MSG:
            if (1 == pTEventUI->wParam)
                OnClickStartBtn();
            else 
                OnTestStatusMsg(E_TEST_FAIL);
            break;
        case WM_HWND_TEST_SATRT_MSG:
            OnTestStatusMsg(E_TEST_BUSY);
            break;
        case WM_TEST_FAIL_MSG:
            OnTestStatusMsg(E_TEST_FAIL);
            break;
        case WM_TIMER:
            OnTimerMsg(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_MENUCLICK:
            {
                std::wstring strTemp = (const wchar_t*)pTEventUI->wParam;
                delete [](const wchar_t*)pTEventUI->wParam;
                OnResponMenuClick(strTemp);
            }          
            break;
        default:
            break;
        }
        return true;
    }

    void CCJGW_DonglesThroughputDlg::OnResponMenuClick(const std::wstring& strClickName)
    {
        if (mpSelMenuBtn == mpDataCollectionModeMenu)
        {
            if (0 == strClickName.compare(L"tcpipMenu")) { mbSelectIfTableTrafficMonitor = false;return;}
            //! tcpipMenu
            mstrConnectionName = JGW_W2A(strClickName);
            m_MenuCheckInfo[L"tcpipMenu"] = false;
            mIfTableTrafficMonitor.SelectNetCardInfo();
            mbSelectIfTableTrafficMonitor = true;
        }
        else if (mpSelMenuBtn == mpEditFuncMenu)
        {
            if (NULL == JGW_WStrComparenoCaseWStr(strClickName.c_str(),L"clearThroughputMenu"))
            {
                mpWkeBrowser->ClearBpsInfo();
            }
            else if (NULL == JGW_WStrComparenoCaseWStr(strClickName.c_str(),L"turntableMenuEle"))
            {
                if (!OpenTurnTableComPort()) return ;
                CCJGW_TruntableSettingDlg dlg(msThroughputTestConfig,mSCLLibHelper,m_hWnd);
                int nTruntaleComPort = dlg.FindTruntableComPort();
                if (0 == nTruntaleComPort)
                {
                    MessageBox(m_hWnd,L"未找到转台设备端口，请确认转台设备是否已经插入",L"错误",MB_ICONERROR);
                }
                else
                {
                    dlg.DoModal();
                }    
            }
        }
    }

    bool CCJGW_DonglesThroughputDlg::OpenTurnTableComPort()
    {
        if (!msThroughputTestConfig.mbEnableTurnTable)
        {
            MessageBox(m_hWnd,L"请先开启转台开关",L"错误",MB_ICONERROR);
            return false;
        }
        //! 加载SCLLIB库
        if (!mSCLLibHelper.IsLoadedSCLLibHelperLibrary()) mSCLLibHelper.LoadSCLLibHelperLibrary();

        int nTruntaleComPort = CCJGW_TruntableSettingDlg::FindTruntableComPort();
        if (0 == nTruntaleComPort)
        {
            MessageBox(m_hWnd,L"未找到转台设备端口，请确认转台设备是否已经插入",L"错误",MB_ICONERROR);
            return false;
        }

        if (!mSCLLibHelper.Open(nTruntaleComPort,9600))
        {
            MessageBox(m_hWnd,L"打开转台设备端口失败，请确认转台设备是否已经被占用",L"错误",MB_ICONERROR);
            return false;
        }

        return  mSCLLibHelper.WriteMotorEnable(TURNTABLE_NODE_ID)?true:false;
    }

    void CCJGW_DonglesThroughputDlg::GetBpsString(const ULONGLONG& uBps,std::wstring& strBps)
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

    void CCJGW_DonglesThroughputDlg::OnTimerMsg(const WPARAM wParam,LPARAM lParam)
    {
        if ((int)wParam == WM_TIMER_UPDATE_LOG_INFO)
        {
            UpdateBpsDataInfo();
        }
    }

    void CCJGW_DonglesThroughputDlg::CreateLogFile()
    {
        std::wstring strLogFilePath,strTemp;
        if (mFileStream.is_open()) mFileStream.close();
        JGW_GetExecSubFolder(strLogFilePath,L"log\\Throughput\\");
        strLogFilePath += JGW_GetTimeFolder();
        strLogFilePath += L"\\";
        JGW_CreateDirectory(strLogFilePath.c_str(),true);
        if (msThroughputTestConfig.mbReadDonglesSN)
            JGW_FormatWString(strTemp,L"%s_%u.csv",mstrDonglesSerialNumber.c_str(),JGW_GetTimeOfDay());
        else
            JGW_FormatWString(strTemp,L"throughput_%u.csv",JGW_GetTimeOfDay());
        strLogFilePath += strTemp;

        mFileStream.imbue( std::locale("") );
        mFileStream.open( strLogFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!mFileStream.is_open())
        {
            MessageBox(m_PaintManager.GetPaintWindow(),L"无法保存LOG文件",L"警告",MB_ICONWARNING);
            return ;
        }

        if (msThroughputTestConfig.mbEnableTurnTable)
            mFileStream << L"角度" << L"," << L"Time" << L"," << L"速率(Mb/s)" << std::endl;
        else 
            mFileStream << L"Time" << L"," << L"速率(Mb/s)" << std::endl;
    }

    void CCJGW_DonglesThroughputDlg::UpdateBpsDataInfo()
    {
        if (NULL == mpTcpStatsImplThread) return ;
        //! 获取下载速率
        LONGLONG dwCounts = 0;
        if (mbSelectIfTableTrafficMonitor)
        {
            dwCounts = mIfTableTrafficMonitor.GetTotalSentReceived() /** 8*/;
        }
        else
        {
            //! 1.在链路层，由以太网的物理特性决定了数据帧的长度为（46＋18）－（1500＋18），其中的18是数据帧的头和尾，也就是说数据帧的内容最大为1500（不包括帧头和帧尾），即MTU（Maximum Transmission Unit）为150　
            //  2.在网络层，因为IP包的首部要占用20字节，所以这的MTU为1500－20＝1480；　
            //  3.在传输层，对于UDP包的首部要占用8字节，所以这的MTU为1480－8＝1472；　　
            // 所以，在应用层，你的Data最大长度为1472。 （当我们的UDP包中的数据多于MTU(1472)时，发送方的IP层需要分片fragmentation进行传输，而在接收方IP层则需要进行数据报重组，由于UDP是不可靠的传输协议，如果分片丢失导致重组失败，将导致UDP数据包被丢弃
            //! 所以每发1472的数据包就得发送 46 + 18 + 20 + 8 +18的数据包损耗
            //! 获取数据发送量 TCP 包的大小就应该是 1500 - IP头(20) - TCP头(20) = 1460 (Bytes)
            dwCounts = mpTcpStatsImplThread->GetTotalReceived() + mpTcpStatsImplThread->GetTotalSent();
            //! dwCounts += (dwCounts) * 0.06293;
        }

        ULARGE_INTEGER Bps;
        double dwTime = mtm.elapsed();
        mtm.restart();
        u_int delay = dwTime * 1000000;
        /* 获取每秒的比特数b/s */
        Bps.QuadPart = (dwCounts * 8 * 1000000) / (delay);
        /*#ifdef _DEBUG*/
        std::wstring strBps;
        GetBpsString(Bps.QuadPart,strBps);
        strBps += L"\n";
        OnRspAppendRichedt(strBps.c_str());
        /*#endif*/
        //! 更新Bps数据信息
        double speed = (double)Bps.QuadPart / (1024.00 * 1024.00);
        //! 更新UI信息
        if (!msThroughputTestConfig.mbEnableTurnTable)
        {
            mpWkeBrowser->UpdateBpsInfo(mnTestTimeIndex,speed);
            if (mFileStream.is_open()) mFileStream << mnTestTimeIndex << L"," << speed << std::endl;
        }
        else
        {
            mpWkeBrowser->UpdateBpsInfo(mnTestTimeIndex,mnCurrentTurnTableAngle,speed);
            if (mFileStream.is_open()) mFileStream << mnCurrentTurnTableAngle << L"," << mnTestTimeIndex << L"," << speed << std::endl;
        }

        //! 如果相等 客户端测试则直接停止
        ++ mnTestTimeIndex;
        if (E_CLIENT_TYPE == msThroughputTestConfig.meSocketTerimalType && msThroughputTestConfig.mnTestTimes < mnTestTimeIndex)
        {
            OnClickStopBtn();
        }
    }

    void CCJGW_DonglesThroughputDlg::OnRspAppendRichedt(const wchar_t* strParam)
    {
        mpLogEdt->AppendText(strParam);
        mpLogEdt->EndDown();
    }

    void CCJGW_DonglesThroughputDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGW_DonglesThroughputDlg::OnEventCustomMessage,0);
        mSCLLibHelper.Close();
        OnTestStatusMsg(E_TEST_FAIL);
        GetThroughputConfigToControl();
        SaveXmlConfig();     
    }

    void CCJGW_DonglesThroughputDlg::OnTestStatusMsg(E_TEST_STATUS_TYPE eTestStatusType)
    {
        switch (eTestStatusType)
        {
        case E_TEST_BUSY:
            SetTimer(m_PaintManager.GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO,1000,NULL);
            mtm.restart();
            mIfTableTrafficMonitor.GetTotalSentReceived();
            mpStopBtn->SetEnabled(true);
            break;
        case E_TEST_FAIL:
            OnClickStopBtn();    
            break;
        }  
    }

    CControlUI* CCJGW_DonglesThroughputDlg::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_WKEWINDOWLESSBROWSER) == 0)
            return	new CWkeWindowlessUI();
        return NULL;
    }

    void CCJGW_DonglesThroughputDlg::GetThroughputConfigToControl()
    {
        msThroughputTestConfig.meSocketTerimalType = (E_SOCKET_TERIMALTYPE)mpTerminalType->GetCurSel();
        msThroughputTestConfig.mstrIPAddress = mpIPAddressEdt->GetText().GetData();
        msThroughputTestConfig.mnSocketPort = _ttoi(mpSocketPortEdt->GetText().GetData());
        msThroughputTestConfig.meTranservicesType = (E_Transceivers_TYPE)(mpDataTransceivers->GetCurSel());
        msThroughputTestConfig.mnTestTimes = _ttoi(mpTestTimeEdt->GetText().GetData());
        msThroughputTestConfig.mnSocketCount = _ttoi(mpSocketCount->GetText().GetData());
        msThroughputTestConfig.mnDataLength = _ttoi(mpDataLengthEdt->GetText().GetData());
        msThroughputTestConfig.meSendPolicy = (EnSendPolicy)mpSendPolicy->GetCurSel();
        msThroughputTestConfig.mnIntervTime = _ttoi(mpTestIntervTimeEdt->GetText().GetData());
        msThroughputTestConfig.mnServerThreadCount = _ttoi(mpThreadCountEdt->GetText().GetData());
        if (0 == msThroughputTestConfig.mnServerThreadCount) msThroughputTestConfig.mnServerThreadCount = min((::SysGetNumberOfProcessors() * 2 + 2), 500);
        msThroughputTestConfig.mnServerMaxConnSocketCount = _ttoi(mpMaxSocketCount->GetText().GetData());
        msThroughputTestConfig.mTurnTableAngleStart = _ttoi(mpTurntableStartAngle->GetText().GetData());
        msThroughputTestConfig.mTurnTableAngleEnd = _ttoi(mpTurntableEndAngle->GetText().GetData());
        msThroughputTestConfig.mTurnTableStep = _ttoi(mpTurnTableStep->GetText().GetData());
    }

    void CCJGW_DonglesThroughputDlg::UpdateThroughputConfigToXMLConfig()
    {
        std::wstring strTemp;

        mpTerminalType->SelectItem(msThroughputTestConfig.meSocketTerimalType);
        mpIPAddressEdt->SetText(msThroughputTestConfig.mstrIPAddress.c_str());
        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnSocketPort);
        mpSocketPortEdt->SetText(strTemp.c_str());
        mpDataTransceivers->SelectItem(msThroughputTestConfig.meTranservicesType);

        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnTestTimes);
        mpTestTimeEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnSocketCount);
        mpSocketCount->SetText(strTemp.c_str());

        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnDataLength);
        mpDataLengthEdt->SetText(strTemp.c_str());
        mpSendPolicy->SelectItem(msThroughputTestConfig.meSendPolicy);

        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnIntervTime);
        mpTestIntervTimeEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnServerThreadCount);
        mpThreadCountEdt->SetText(strTemp.c_str());
        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mnServerMaxConnSocketCount);
        mpMaxSocketCount->SetText(strTemp.c_str());

        mpEnableTurnTable->SetCheck(msThroughputTestConfig.mbEnableTurnTable);
        JGW_FormatWString(strTemp,L"%d",msThroughputTestConfig.mTurnTableStep);
        mpTurnTableStep->SetText(strTemp.c_str());

        if (msThroughputTestConfig.mbEnableTurnTable) OpenTurnTableComPort();
    }


    void CCJGW_DonglesThroughputDlg::SetControlEnable(bool enable /* = true */)
    {
        mpSendPolicy->SetEnabled(enable);
        mpDataLengthEdt->SetEnabled(enable);
        mpIPAddressEdt->SetEnabled(enable);
        mpTerminalType->SetEnabled(enable);
        mpSocketPortEdt->SetEnabled(enable);
        mpDataTransceivers->SetEnabled(enable);
        mpTestTimeEdt->SetEnabled(enable);
        mpSocketCount->SetEnabled(enable);
        mpTestIntervTimeEdt->SetEnabled(enable);
        mpThreadCountEdt->SetEnabled(enable);
        mpMaxSocketCount->SetEnabled(enable);
        mpStartBtn->SetEnabled(enable);
        //! mpStopBtn->SetEnabled(!enable); 
        mpEnableTurnTable->SetEnabled(enable);
        if (msThroughputTestConfig.mbEnableTurnTable)
        {
            mpTurnTableStep->SetEnabled(enable);
            mpTurntableStartAngle->SetEnabled(enable);
            mpTurntableEndAngle->SetEnabled(enable);
        }
    }

    void CCJGW_DonglesThroughputDlg::OnStartDonglesthroughputTest(WPARAM& wParam,LPARAM& lParam)
    {
        //if (!ReadDonglesSerialNumber()) wParam = 0;
        wParam = ReadDonglesSerialNumber()?1:0;
//             msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
//         else 
//             msThroughputTestConfig.mpAsyncWndMessage->PutAsyncMessage(WM_THREAD_TEST_START_MSG);
    }

    void CCJGW_DonglesThroughputDlg::OnClickStartBtn()
    {
        //! 初始化
        GetThroughputConfigToControl();
        mpWkeBrowser->ClearBpsInfo();
        CreateLogFile();
        while (!mvTurnTableAngleQueue.empty()) mvTurnTableAngleQueue.pop();
        //! 初始化TCP/IP服务线程
        if (E_CLIENT_TYPE == msThroughputTestConfig.meSocketTerimalType)
        {
            mpTcpStatsImplThread = new CCJGW_TcpClientThread(msThroughputTestConfig);    
        }
        else
        {
            mpTcpStatsImplThread = new CCJGW_TcpServerThread(msThroughputTestConfig);
        }
        //! 创建消息线程
        mpTcpStatsImplThread->CreateMessageThread(0,0,true);
        //! 是否开启转台角度队列
        if (msThroughputTestConfig.mbEnableTurnTable)
        {
            //! 添加天线角度队列
            for (int i = msThroughputTestConfig.mTurnTableAngleStart;i <= msThroughputTestConfig.mTurnTableAngleEnd;i += msThroughputTestConfig.mTurnTableStep)
            {
                mvTurnTableAngleQueue.push(i);
            }
            if (mvTurnTableAngleQueue.empty()) return ;
            //! 调整天线角度
            mSCLLibHelper.RelMove(TURNTABLE_NODE_ID,msThroughputTestConfig.mTurnTableAngleStart * PER_ANGLE_TURN,&msThroughputTestConfig.mfVelocity,&msThroughputTestConfig.mfAcceleration,&msThroughputTestConfig.mfDeceleration);
            //! 开启转台测试队列
            OnStartTurnTableTestQueque();
        }
        else
        {
            mpTcpStatsImplThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN);
            mnTestTimeIndex = 1;
        }
        SetControlEnable(false);
    }

    void CCJGW_DonglesThroughputDlg::OnClickStopBtn()
    {
        KillTimer(m_PaintManager.GetPaintWindow(),WM_TIMER_UPDATE_LOG_INFO);
        mpStopBtn->SetEnabled(false);
        //! 退出当前测试
        if (mpTcpStatsImplThread)
        {
            mpTcpStatsImplThread->ExitTestThread();
            Sleep(1000 * 1);
            if (mpTcpStatsImplThread->PostThreadMessage(WM_TEST_THREAD_MSG_CLEANUP)) mpTcpStatsImplThread->CheckExitComplete();
        }
        //! 当转台队列测试完成后
        if (mvTurnTableAngleQueue.empty())
        {
            if (mpTcpStatsImplThread)
            {
                delete mpTcpStatsImplThread;
                mpTcpStatsImplThread = NULL;
            } 
            SetControlEnable(true);
            if (mFileStream.is_open()) mFileStream.close();
        } 
        else 
        {
            //! 旋转相对路径
            mSCLLibHelper.RelMove(TURNTABLE_NODE_ID,msThroughputTestConfig.mTurnTableStep * PER_ANGLE_TURN,&msThroughputTestConfig.mfVelocity,&msThroughputTestConfig.mfAcceleration,&msThroughputTestConfig.mfDeceleration);
            //! Sleep(1000);
            OnStartTurnTableTestQueque();
        }
    }

    void CCJGW_DonglesThroughputDlg::SaveXmlConfig()
    {
        CCJGW_ThroughputTestConfig throughputTestConfig(msThroughputTestConfig);
        std::string strFilePath = JGW_W2A_W(JGW_GetApplicationFolder());
        strFilePath += DonglesThroughputTestConfigName;
        throughputTestConfig.SaveThroughputTestXMLConfig(strFilePath);
    }

    void CCJGW_DonglesThroughputDlg::OnStartTurnTableTestQueque()
    {
        while (mvTurnTableAngleQueue.empty()) OnClickStopBtn();
        mnCurrentTurnTableAngle = mvTurnTableAngleQueue.front();
        mpTcpStatsImplThread->ClearRecvSentCount();
        mvTurnTableAngleQueue.pop();
        mpTcpStatsImplThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN);
        mnTestTimeIndex = 1;     
    }

    bool CCJGW_DonglesThroughputDlg::ReadDonglesSerialNumber()
    {
        if (!msThroughputTestConfig.mbReadDonglesSN) return true;
        if (!OpenPerasoComPortView()) return false;

        if (!mDonglesComPort.ReadSerialNumber(mstrDonglesSerialNumber))
        {
            msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"读取设备SN失败\n");
            mDonglesComPort.CloseDonglesComPort();
            return false;
        }
         msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString( L"读取设备SN:%s\n",mstrDonglesSerialNumber.c_str()).c_str());
        mDonglesComPort.CloseDonglesComPort();
        return true;
    }

    bool CCJGW_DonglesThroughputDlg::OpenPerasoComPortView()
    {
        std::wstring strComPort;
        msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"正在查找设备插入\n");
        for (int i = 0;i < 20;i ++)
        {
            if (0 == (i % 3))
            {
                msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"正在查找设备插入.\n");
            }
            else if (1 == (i % 3))
            {
                msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"正在查找设备插入..\n");
            }
            else
            {
                msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"正在查找设备插入...\n");
            }
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (strComPort.empty()) { Sleep(1000); continue; }
            msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"正在打开设备端口\n");
            if (mDonglesComPort.OpenDonglesComPort(strComPort))
            {
                msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"打开设备端口成功\n");
                return true;
            }
            msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"打开设备端口失败,请检查端口是否被占用\n");
            Sleep(500);
        }
        msThroughputTestConfig.mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"未查找到设备插入,请检查dongles是否已经插入\n");
        return false;
    }
}