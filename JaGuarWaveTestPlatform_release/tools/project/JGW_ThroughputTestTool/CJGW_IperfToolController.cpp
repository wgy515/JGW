#include "StdAfx.h"
#include "CJGW_IperfToolController.h"
#include "CJGW_IperfToolDlg.h"

#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_TruntableSettingDlg.h"
#include "CJGW_AttenuatorSettingDlg.h"
#include "CJGW_AttenuatorRangeConfigDlg.h"
#include "CJGW_TrunTableRangeConfigDlg.h"
#include "CJGW_TRxSectorDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>

namespace JGW
{
    CCJGW_IperfToolController::CCJGW_IperfToolController(CCJGW_IperfToolDlg* pView) : mpView(pView)
    {
    }


    CCJGW_IperfToolController::~CCJGW_IperfToolController(void)
    {
    }

    bool CCJGW_IperfToolController::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnAppendRichEditTestMsg((const wchar_t*)(pTEventUI->wParam));
            break;
#if 0
        case WM_TIMER:
            if (pTEventUI->wParam == WM_TIMER_UPDATE_LOG_INFO) OnTimerUpdateLogInfo();
            break;
#endif;
        case WM_TEST_PASS_MSG:
            mpView->EnableTestUI(true);
            break;
            //! KillTimer(mpView->m_hWnd,WM_TIMER_UPDATE_LOG_INFO);
        case WM_MENUCLICK: //! 响应菜单栏点击消息
            OnMenuElementClick(pTEventUI);   
            break;
        case WM_HWND_UPDATE_BPS_INFO:
            OnUpdateBpsData(pTEventUI->wParam,pTEventUI->lParam);
            break;
        case WM_HWND_CLEAR_BPS_INFO:
            OnClearBpsData();
            break;
        }

        return true;
    }

    void CCJGW_IperfToolController::OnViewNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (mpView->mpTerminalType == msg.pSender)
            {
                mpView->mpClientConfigHorizontalLayout->SetVisible(E_SERVICE_TYPE == mpView->mpTerminalType->GetCurSel()?false:true);
            }
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpView->mpStartBtn == msg.pSender)
            {
                JGW::JGW_KillProcessToName(L"iperf3.exe");
                JGW::JGW_KillProcessToName(L"iperf.exe");
                mpView->msIperfToolConfig.mbStartTestStatus = true;
                mpView->UpdateIperfToolConfigParam();
                mpView->BuildIperfOutputLogFilePath();
                mpView->UpdateIperfCommand();
                mpView->EnableTestUI(false);
                mpView->mpWkeBrowser->ClearBpsInfo();
                OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Iperf Command: %s",mpView->msIperfToolConfig.mstrIperfCommand.c_str()).c_str());
                mpView->mTestThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,NULL,NULL);
            }
            else if (mpView->mpStopBtn == msg.pSender)
            {
                mpView->msIperfToolConfig.mbStartTestStatus = false;
                Sleep(1 * 1000);
                JGW::JGW_KillProcessToName(L"iperf3.exe");
                JGW::JGW_KillProcessToName(L"iperf.exe");  
            }
            else if (msg.pSender->GetName() == L"SettingMenuBtn")
            {
                OnSettingMenuBtnClick(msg);
            }
            else if (msg.pSender->GetName() == L"configMenuBtn")
            {
                OnConfigMenuBtnClick(msg);     
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (msg.pSender->GetName() == L"enableTurnTableCheckBox")
            {
                mpView->msIperfToolConfig.mbEnableTrunTable = ((CCheckBoxUI*)msg.pSender)->GetCheck();
                OnEnableTruntableCheck(msg);
                mpView->OnUpdateConfigInfoUI();
            }
            else if (msg.pSender->GetName() == L"enableAttenuatorCheckBox")
            {
                mpView->msIperfToolConfig.mbEnableAttenutor = ((CCheckBoxUI*)msg.pSender)->GetCheck();
                OnEnableAttenuatorCheck(msg);
                mpView->OnUpdateConfigInfoUI();
            }
        }
    }

    void CCJGW_IperfToolController::OnAppendRichEditTestMsg(const wchar_t* strMsg)
    {
        static int line = 0;
        if (line++ > 3000) {mpView->mpLogEdt->Clear();line = 0;}
        mpView->mpLogEdt->AppendText(strMsg);
        mpView->mpLogEdt->AppendText(L"\r\n");
        mpView->mpLogEdt->EndDown();
    }
#if 0
    void CCJGW_IperfToolController::OnTimerUpdateLogInfo()
    {
        std::wifstream mFileStream;
        std::wstring strTemp;

        if (mpView->msIperfToolConfig.mnTestTime > mpView->msIperfToolConfig.mnTestTimeIndex)
            JGW_FormatWString(strTemp,L"%d.00-%d.00",mpView->msIperfToolConfig.mnTestTimeIndex,mpView->msIperfToolConfig.mnTestTimeIndex + 1);
        else 
            JGW_FormatWString(strTemp,L"0.00-%d.00",mpView->msIperfToolConfig.mnTestTimeIndex);

        mFileStream.open(mpView->msIperfToolConfig.mstrSaveOutPutLog.c_str());
        if (!mFileStream.is_open()) return ;
        mFileStream.seekg(mpView->msIperfToolConfig.mnSeekgLog);

        while (mFileStream.getline(szReadBuf,READ_BUFFER_MAX_LEN))
        {
            if (NULL != wcsstr(szReadBuf,strTemp.c_str()))
            {
                if (mpView->msIperfToolConfig.mbMultThread && !wcsstr(szReadBuf,L"[SUM]")) continue;
                mpView->msIperfToolConfig.mnSeekgLog = mFileStream.tellg();
                OnAppendRichEditTestMsg(szReadBuf);

                //! update 
                if (mpView->msIperfToolConfig.mnTestTime > mpView->msIperfToolConfig.mnTestTimeIndex ++)
                {
                    std::vector<std::wstring> vstrTemp;
                    JGW_ParserStrW(szReadBuf,L" ",vstrTemp);

                    if (mpView->msIperfToolConfig.mbMultThread)
                        mpView->mpWkeBrowser->UpdateBpsInfo(mpView->msIperfToolConfig.mnTestTimeIndex,_ttof(vstrTemp[5].c_str()));
                    else 
                        mpView->mpWkeBrowser->UpdateBpsInfo(mpView->msIperfToolConfig.mnTestTimeIndex,_ttof(vstrTemp[6].c_str()));                 
                    break;
                } 
            } 
        }
        mFileStream.close();
    }
#endif
    void CCJGW_IperfToolController::OnSettingMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("setting_menu.xml"),point,&mpView->m_PaintManager,NULL);

        if (!mpView->msIperfToolConfig.mbEnableTrunTable)
        {
            pMenu->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
        } 

        if (!mpView->msIperfToolConfig.mbEnableAttenutor)
        {
            pMenu->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
        }
    }

    void CCJGW_IperfToolController::OnConfigMenuBtnClick(TNotifyUI& msg)
    {
        CMenuWnd* pMenu = new CMenuWnd();
        CPoint point;
        point.x = msg.pSender->GetX();
        point.y = msg.pSender->GetY() + msg.pSender->GetHeight();
        ClientToScreen(mpView->m_hWnd, &point);
        pMenu->Init(NULL,_T("config_menu.xml"),point,&mpView->m_PaintManager,NULL);
        if (!mpView->msIperfToolConfig.mbEnableTrunTable)
        {
            pMenu->GetMenuUI()->GetItemAt(0)->SetEnabled(false);
        }

        if (!mpView->msIperfToolConfig.mbEnableAttenutor)
        {
            pMenu->GetMenuUI()->GetItemAt(1)->SetEnabled(false);
        }
    }

    void CCJGW_IperfToolController::OnEnableAttenuatorCheck(TNotifyUI& msg)
    {
        if (!mpView->msIperfToolConfig.mbEnableAttenutor)
        {
            GetAttenuatorEquipment().CloseAttenuatorEquipment();
        }
        else
        {
            try
            { 
                switch (mpView->msIperfToolConfig.msMonitorInfo.m_cInterfaceType)
                {
                case E_VISA_HIVE:
                    GetAttenuatorEquipment().SetVisa(
                        mpView->msIperfToolConfig.msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress.c_str());
                    break;
                case E_GPIB_HIVE:
                default:
                    GetAttenuatorEquipment().SetGPIB(
                        mpView->msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType,
                        mpView->msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface,
                        mpView->msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress,
                        mpView->msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
                    break;

                }     
            }
            catch (...)
            {
                ((CCheckBoxUI*)msg.pSender)->SetCheck(false);
                mpView->msIperfToolConfig.mbEnableAttenutor = false;
            }
        }
    }

    void CCJGW_IperfToolController::OnEnableTruntableCheck(TNotifyUI& msg)
    {
        if (!mpView->msIperfToolConfig.mbEnableTrunTable)
        {
            if (GetTrunTableSCLLibHelper().IsLoadedSCLLibHelperLibrary()) GetTrunTableSCLLibHelper().Close();
            return ;
        }

        if (!OpenTurnTableComPort())
        {
            ((CCheckBoxUI*)msg.pSender)->SetCheck(false);
            mpView->msIperfToolConfig.mbEnableTrunTable = false;
        }
    }

    bool CCJGW_IperfToolController::OpenTurnTableComPort()
    {
        if (!mpView->msIperfToolConfig.mbEnableTrunTable)
        {
            MessageBox(mpView->m_hWnd,L"请先开启转台开关",L"错误",MB_ICONERROR);
            return false;
        }
        //! 加载SCLLIB库
        if (!GetTrunTableSCLLibHelper().IsLoadedSCLLibHelperLibrary()) GetTrunTableSCLLibHelper().LoadSCLLibHelperLibrary();

        int nTruntaleComPort = CCJGW_TruntableSettingDlg::FindTruntableComPort();
        if (0 == nTruntaleComPort)
        {
            MessageBox(mpView->m_hWnd,L"未找到转台设备端口，请确认转台设备是否已经插入",L"错误",MB_ICONERROR);
            return false;
        }
        std::wstring strCom;
        JGW_FormatWString(strCom,L"\\\\.\\COM%d",nTruntaleComPort);
        HANDLE hCom = CreateFile(strCom.c_str(),0,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
        if (INVALID_HANDLE_VALUE == hCom)
        {
            MessageBox(mpView->m_hWnd,L"打开转台设备端口失败，请确认转台设备是否已经被占用或者已经生效",L"错误",MB_ICONERROR);
            return false;
        }

        CloseHandle(hCom);
        if (!GetTrunTableSCLLibHelper().Open(nTruntaleComPort,9600))
        {
            MessageBox(mpView->m_hWnd,L"打开转台设备端口失败，请确认转台设备是否已经被占用",L"错误",MB_ICONERROR);
            return false;
        }

       /* ret = */GetTrunTableSCLLibHelper().SetCommParam(0, FALSE);

        return  GetTrunTableSCLLibHelper().WriteMotorEnable(TURNTABLE_NODE_ID)?true:false;
    }

    void CCJGW_IperfToolController::OnMenuElementClick(TEventUI* pTEventUI)
    {
        const wchar_t* strName = (const wchar_t*)pTEventUI->wParam;
        if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("turntableSettingMenuEle")))
        {
            OnTurntableSettingMenuElementClick();
        } 
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,L"AttenuatorSettingMenuEle"))
        {
            CCJGW_AttenuatorSettingDlg dlg(mpView->m_hWnd);
            dlg.DoModal();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,L"SectorConfigMenuEle"))
        {
            CCJGW_TRxSectorDlg dlg(mpView->msIperfToolConfig,mpView->m_hWnd);
            dlg.DoModal();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,L"clearThroughputMenu"))
        {
            mpView->mpWkeBrowser->ClearBpsInfo();
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("turntableConfigMenuEle")))
        {
            CCJGW_TrunTableRangeConfigDlg dlg(mpView->msIperfToolConfig.msTrunTableParam,mpView->m_hWnd);
            dlg.DoModal();
            mpView->OnUpdateConfigInfoUI();
        } 
        else if (NULL == JGW_WStrComparenoCaseWStr(strName,_T("AttenuatorConfigMenuEle")))
        {
            CCJGW_AttenuatorRangeConfigDlg dlg(mpView->msIperfToolConfig.msAttenutorParam,mpView->m_hWnd);
            dlg.DoModal();
            mpView->OnUpdateConfigInfoUI();
        } 
    }

    void CCJGW_IperfToolController::OnTurntableSettingMenuElementClick()
    {
        CCJGW_TruntableSettingDlg dlg(mpView->msIperfToolConfig,GetTrunTableSCLLibHelper(),mpView->m_hWnd);
        dlg.DoModal();
    }

    void CCJGW_IperfToolController::OnUpdateBpsData(WPARAM& wParam,LPARAM lParam)
    {
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW((const wchar_t*)wParam,L",",vstrTemp);
       /* if (2 == vstrTemp.size())
            mpView->mpWkeBrowser->UpdateBpsInfo(_ttoi(vstrTemp[0].c_str()),_ttof(vstrTemp[1].c_str()));       
        else
            mpView->mpWkeBrowser->UpdateBpsInfo(_ttoi(vstrTemp[1].c_str()),vstrTemp[0],_ttof(vstrTemp[2].c_str())); */
        mpView->mpWkeBrowser->UpdateAngleMbps(vstrTemp[0],_ttof(vstrTemp[1].c_str()));
    }

    void CCJGW_IperfToolController::OnClearBpsData()
    {
        mpView->mpWkeBrowser->ClearBpsInfo();
    }
}