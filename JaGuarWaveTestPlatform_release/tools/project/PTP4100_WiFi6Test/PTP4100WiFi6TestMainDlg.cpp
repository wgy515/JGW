#include "StdAfx.h"
#include "PTP4100WiFi6TestMainDlg.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <regex>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CPTP4100WiFi6TestMainDlg::CPTP4100WiFi6TestMainDlg(void) : mpWiFi6TestThread(NULL)
    {
        msTestParam.muTxAttenVga1Start = 0x00;
        msTestParam.muTxAttenVga1End = 0x1F;
        msTestParam.muTxAttenVga2Start = 0x00;
        msTestParam.muTxAttenVga2End = 0x1F;
        msTestParam.muRxAttenStart = 0x00;
        msTestParam.muRxAttenEnd = 0x1F;
        msTestParam.mnTxPowerStart = 0;
        msTestParam.mnTxPowerEnd = 18;
        msTestParam.muTxAttenVga1Step = 1;
        msTestParam.muTxAttenVga2Step = 1;
        msTestParam.muRxAttenStep = 1;
        msTestParam.mnTxPowerStep = 1;
        msTestParam.mbIsUseSerialPort = true;
        msTestParam.mnPacketCollectionTime = 1000;

        msTestParam.mnErrorRetryTest = 3;
        msTestParam.mnMinRssi = -70;
        msTestParam.mnMinTotalPackets = 0;
        msTestParam.mnMinGoodPackets = 0;
        msTestParam.mfMinEvm = 0.0f;
    }


    CPTP4100WiFi6TestMainDlg::~CPTP4100WiFi6TestMainDlg(void)
    {
    }

    void CPTP4100WiFi6TestMainDlg::OnCloseWindow()
    {
        if (mpWiFi6TestThread)
        {
            mpWiFi6TestThread->TerminateMessageThread();
            delete mpWiFi6TestThread;
            mpWiFi6TestThread = NULL;
        }
        m_PaintManager.GetEventSource() -= MakeDelegate(this,&CPTP4100WiFi6TestMainDlg::OnEventCustomMessage,0);
        SaveConfigIni();
    }

    void CPTP4100WiFi6TestMainDlg::OnInitWindow()
    {
        {
            bool bFindControlOk = true;

            mpTxIPQIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxIPQIPAddressEdt"));
            mpTxUsbIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxUsbIPAddressEdt"));
            //mpTxSerialPortCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"TxSerialPortCombo"));
            mpRxIPQIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"RxIPQIPAddressEdt"));
            mpRxUsbIPAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"RxUsbIPAddressEdt"));
            //mpRxSerialPortCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"RxSerialPortCombo"));
            mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"StartBtn"));
            mpTestLogRichEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"TestLogRichEdt"));

            mpTxPowerStepEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxPowerStepEdt"));
            mpTxPowerStartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxPowerStartEdt"));
            mpTxPowerEndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxPowerEndEdt"));
            mpTxDVG1StepEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG1StepEdt"));
            mpTxDVG1StartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG1StartEdt"));
            mpTxDVG1EndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG1EndEdt"));
            mpTxDVG2StepEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG2StepEdt"));
            mpTxDVG2StartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG2StartEdt"));
            mpTxDVG2EndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"TxDVG2EndEdt"));
            mpRxAttenStepEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"RxAttenStepEdt"));
            mpRxAttenStartEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"RxAttenStartEdt"));
            mpRxAttenEndEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"RxAttenEndEdt"));
            mpPacketCollectionTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"PacketCollectionTimeEdt"));



            bFindControlOk &= (NULL != mpTxIPQIPAddressEdt);
            bFindControlOk &= (NULL != mpTxUsbIPAddressEdt);
            //bFindControlOk &= (NULL != mpTxSerialPortCombo);
            bFindControlOk &= (NULL != mpRxIPQIPAddressEdt);
            bFindControlOk &= (NULL != mpRxUsbIPAddressEdt);
            //bFindControlOk &= (NULL != mpRxSerialPortCombo);
            bFindControlOk &= (NULL != mpStartBtn);
            bFindControlOk &= (NULL != mpTestLogRichEdt);
            bFindControlOk &= (NULL != mpTxPowerStartEdt);
            bFindControlOk &= (NULL != mpTxPowerEndEdt);
            bFindControlOk &= (NULL != mpTxDVG1StartEdt);
            bFindControlOk &= (NULL != mpTxDVG1EndEdt);
            bFindControlOk &= (NULL != mpTxDVG2StartEdt);
            bFindControlOk &= (NULL != mpTxDVG2EndEdt);
            bFindControlOk &= (NULL != mpRxAttenStartEdt);
            bFindControlOk &= (NULL != mpRxAttenEndEdt);
            bFindControlOk &= (NULL != mpRxAttenStepEdt);
            bFindControlOk &= (NULL != mpTxPowerStepEdt);
            bFindControlOk &= (NULL != mpTxDVG1StepEdt);
            bFindControlOk &= (NULL != mpTxDVG2StepEdt);
            bFindControlOk &= (NULL != mpPacketCollectionTimeEdt);

            if (!bFindControlOk)
            {
                MessageBox(m_hWnd,L"初始化UI异常",L"错误",MB_ICONERROR);
                Close(IDCANCEL);
                return;
            }
        }
        {
            mpWiFi6TestThread = new CPTP4100WiFi6TestThread();
            mpWiFi6TestThread->CreateMessageThread(0,0,true);
            m_PaintManager.GetEventSource() += MakeDelegate(this,&CPTP4100WiFi6TestMainDlg::OnEventCustomMessage,0);
            LoadConfigIni();
        }
        {
            std::wstring strTemp;

            JGW_FormatWString(strTemp,L"0x%x",msTestParam.mnTxPowerStep);
            mpTxPowerStepEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.mnTxPowerStart);
            mpTxPowerStartEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.mnTxPowerEnd);
            mpTxPowerEndEdt->SetText(strTemp.c_str());

            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga1Step);
            mpTxDVG1StepEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga1Start);
            mpTxDVG1StartEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga1End);
            mpTxDVG1EndEdt->SetText(strTemp.c_str());

            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga2Step);
            mpTxDVG2StepEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga2Start);
            mpTxDVG2StartEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muTxAttenVga2End);
            mpTxDVG2EndEdt->SetText(strTemp.c_str());

            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muRxAttenStep);
            mpRxAttenStepEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muRxAttenStart);
            mpRxAttenStartEdt->SetText(strTemp.c_str());
            JGW_FormatWString(strTemp,L"0x%x",msTestParam.muRxAttenEnd);
            mpRxAttenEndEdt->SetText(strTemp.c_str());

            JGW_FormatWString(strTemp,L"%d",msTestParam.mnPacketCollectionTime);
            mpPacketCollectionTimeEdt->SetText(strTemp.c_str());

            mpTxIPQIPAddressEdt->SetText(msTestParam.mstrTxIPQAddress.c_str());
            mpRxIPQIPAddressEdt->SetText(msTestParam.mstrRxIPQAddress.c_str());
            mpTxUsbIPAddressEdt->SetText(msTestParam.mstrTxUsbNetCardAddress.c_str());
            mpRxUsbIPAddressEdt->SetText(msTestParam.mstrRxUsbNetCardAddress.c_str());

            mpStartBtn->SetEnabled(true);
        }
    }

    void CPTP4100WiFi6TestMainDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (mpStartBtn == msg.pSender)
            {
                OnClickStartBtn();
            }
        }
    }

    void CPTP4100WiFi6TestMainDlg::OnClickStartBtn()
    {
        LPTSTR lpstr = NULL;

        mpTestLogRichEdt->SetText(L"");
        msTestParam.mstrRxIPQAddress = mpRxIPQIPAddressEdt->GetText().GetData();
        msTestParam.mstrTxIPQAddress = mpTxIPQIPAddressEdt->GetText().GetData();
        msTestParam.mstrRxUsbNetCardAddress = mpRxUsbIPAddressEdt->GetText().GetData();
        msTestParam.mstrTxUsbNetCardAddress = mpTxUsbIPAddressEdt->GetText().GetData();
        msTestParam.mnTxPowerStart = wcstol(mpTxPowerStartEdt->GetText().GetData(),&lpstr,16);
        msTestParam.mnTxPowerEnd = wcstol(mpTxPowerEndEdt->GetText().GetData(),&lpstr,16);
        msTestParam.muTxAttenVga1Start = wcstol(mpTxDVG1StartEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muTxAttenVga1End = wcstol(mpTxDVG1EndEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muTxAttenVga2Start = wcstol(mpTxDVG2StartEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muTxAttenVga2End = wcstol(mpTxDVG2EndEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muRxAttenStart = wcstol(mpRxAttenStartEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muRxAttenEnd = wcstol(mpRxAttenEndEdt->GetText().GetData(),&lpstr,16) & 0x1F;

        msTestParam.mnTxPowerStep = wcstol(mpTxPowerStepEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muTxAttenVga1Step = wcstol(mpTxDVG1StepEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muTxAttenVga2Step = wcstol(mpTxDVG2StepEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.muRxAttenStep = wcstol(mpRxAttenStepEdt->GetText().GetData(),&lpstr,16) & 0x1F;
        msTestParam.mnPacketCollectionTime = _ttoi(mpPacketCollectionTimeEdt->GetText().GetData());
        if (msTestParam.mnPacketCollectionTime <= 0) msTestParam.mnPacketCollectionTime = 1000;

        if (msTestParam.mnTxPowerStart > msTestParam.mnTxPowerEnd || msTestParam.muTxAttenVga1Start > msTestParam.muTxAttenVga1End || msTestParam.muTxAttenVga2Start > msTestParam.muTxAttenVga2End || msTestParam.muRxAttenStart > msTestParam.muRxAttenEnd || 0 == msTestParam.mnTxPowerStep || 0 == msTestParam.muTxAttenVga1Step || 0 == msTestParam.muTxAttenVga2Step || 0 == msTestParam.muRxAttenStep)
        {
            MessageBox(m_hWnd,L"输入参数异常",L"错误",MB_ICONERROR);
            return;
        }

        if (msTestParam.mstrRxIPQAddress.empty() || msTestParam.mstrTxIPQAddress.empty()
            || msTestParam.mstrRxUsbNetCardAddress.empty() || msTestParam.mstrTxUsbNetCardAddress.empty())
        {
            MessageBox(m_hWnd,L"请选择对应参数文件",L"错误",MB_ICONERROR);
            return ;
        }

        if (0 == msTestParam.muTxAttenVga1End && 0 == msTestParam.muRxAttenEnd && 0 == msTestParam.muTxAttenVga2End)
        {
            msTestParam.mbIsUseSerialPort = false;
        }

        if (!ValidationIPAddress(msTestParam.mstrRxUsbNetCardAddress) || !ValidationIPAddress(msTestParam.mstrTxUsbNetCardAddress))
        {
            MessageBox(m_hWnd, L"Invalid Usb Net Card IP address", L"Error", MB_ICONERROR);
            return;
        }

        if (!ValidationIPAddress(msTestParam.mstrRxIPQAddress) || !ValidationIPAddress(msTestParam.mstrTxIPQAddress))
        {
            MessageBox(m_hWnd, L"Invalid IPQ IP address", L"Error", MB_ICONERROR);
            return;
        }

        if (0 == msTestParam.mstrRxIPQAddress.compare(msTestParam.mstrTxIPQAddress))
        {
            MessageBox(m_hWnd, L"Cannot be the same .the tx and rx IPQ ip address", L"Error", MB_ICONERROR);
            return;
        }

        if (0 == msTestParam.mstrRxUsbNetCardAddress.compare(msTestParam.mstrTxUsbNetCardAddress))
        {
            MessageBox(m_hWnd, L"Cannot be the same .the tx and rx USB Net Card address", L"Error", MB_ICONERROR);
            return;
        }

        //msWiFiTestTemplateParam.mnTemperatureIndex = mpTemperatureComBo->GetCurSel();
        mpTestLogRichEdt->SetEnabled(false);
        mpStartBtn->SetEnabled(false);
        msTestParam.mhMainWindow = m_hWnd;
        mpWiFi6TestThread->PostThreadMessage(WM_THREAD_TEST_START_MSG,WPARAM(&msTestParam),LPARAM(&mAsyncWndMessage));
    }

    bool CPTP4100WiFi6TestMainDlg::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_TEST_PASS_MSG:
            mpTestLogRichEdt->SetEnabled(true);
            mpStartBtn->SetEnabled(true);
            break;
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnAppendRichEditTestLog(pTEventUI->wParam,pTEventUI->lParam);
            break;
        default:
            break;
        }
        return true;
    }

    void CPTP4100WiFi6TestMainDlg::OnAppendRichEditTestLog(const WPARAM& wParam, const LPARAM& lParam)
    {
        static size_t index = 0;
        if (index++ >= 2000) 
        {
            mpTestLogRichEdt->SetText(L"");
            index = 0;
            mpTestLogRichEdt->SetEnabled(true);
            mpTestLogRichEdt->SetEnabled(false);
        }
        std::wstring strTemp((const wchar_t*)wParam);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        mpTestLogRichEdt->AppendText(strTemp.c_str());
        mpTestLogRichEdt->AppendText(L"\r\n");
        mpTestLogRichEdt->EndDown();  
    }

    bool CPTP4100WiFi6TestMainDlg::ValidationIPAddress(const std::wstring& strAddress)
    {
        if (strAddress.empty())
        {
            return false;
        }
        std::wregex regIpPattern(L"((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");

        return regex_match(strAddress, regIpPattern);
    }

    void CPTP4100WiFi6TestMainDlg::LoadConfigIni()
    {
        CCJGW_ConfigIni configIni;
        std::wstring strFilePath;

        JGW_FormatWString(strFilePath,L"%s\\config.ini",JGW_GetApplicationFolder());
        configIni.InitIniFilePath(strFilePath.c_str());

        msTestParam.muTxAttenVga1Start = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga1Start",0x00);
        msTestParam.muTxAttenVga1End = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga1End",0x1F);
        msTestParam.muTxAttenVga2Start = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga2Start",0x00);
        msTestParam.muTxAttenVga2End = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga2End",0x1F);
        msTestParam.muRxAttenStart = configIni.GetIniKeyIntValue(L"MAIN",L"RxAttenStart",0x00);
        msTestParam.muRxAttenEnd = configIni.GetIniKeyIntValue(L"MAIN",L"RxAttenEnd",0x1F);
        msTestParam.mnTxPowerStart = configIni.GetIniKeyIntValue(L"MAIN",L"TxPowerStart",0x00);
        msTestParam.mnTxPowerEnd = configIni.GetIniKeyIntValue(L"MAIN",L"TxPowerEnd",0x12);
        msTestParam.muTxAttenVga1Step = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga1Step",1);
        msTestParam.muTxAttenVga2Step = configIni.GetIniKeyIntValue(L"MAIN",L"TxAttenVga2Step",1);
        msTestParam.muRxAttenStep = configIni.GetIniKeyIntValue(L"MAIN",L"RxAttenStep",1);
        msTestParam.mnTxPowerStep = configIni.GetIniKeyIntValue(L"MAIN",L"TxPowerStep",1);
        msTestParam.mnPacketCollectionTime = configIni.GetIniKeyIntValue(L"MAIN",L"PacketCollectionTime",1000);

        msTestParam.mstrTxUsbNetCardAddress = configIni.GetIniKeyValue(L"MAIN",L"TxUsbNetCardAddress",L"192.168.10.5");
        msTestParam.mstrRxUsbNetCardAddress = configIni.GetIniKeyValue(L"MAIN",L"RxUsbNetCardAddress",L"192.168.10.6");
        msTestParam.mstrRxIPQAddress = configIni.GetIniKeyValue(L"MAIN",L"RxIPQAddress",L"192.168.10.2");
        msTestParam.mstrTxIPQAddress = configIni.GetIniKeyValue(L"MAIN",L"TxIPQAddress",L"192.168.10.1");

        
        msTestParam.mnErrorRetryTest = configIni.GetIniKeyIntValue(L"MAIN",L"ErrorRetryTest",3);
        msTestParam.mnMinRssi = configIni.GetIniKeyIntValue(L"MAIN",L"MinRssi",-70);
        msTestParam.mnMinTotalPackets = configIni.GetIniKeyIntValue(L"MAIN",L"MinTotalPackets",0);
        msTestParam.mnMinGoodPackets = configIni.GetIniKeyIntValue(L"MAIN",L"MinGoodPackets",0);
        msTestParam.mfMinEvm = configIni.GetIniKeyFloatValue(L"MAIN",L"MinEvm",L"0");

    }

    void CPTP4100WiFi6TestMainDlg::SaveConfigIni()
    {
        CCJGW_ConfigIni configIni;
        std::wstring strFilePath;

        JGW_FormatWString(strFilePath,L"%s\\config.ini",JGW_GetApplicationFolder());
        configIni.InitIniFilePath(strFilePath.c_str());

        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga1Start",msTestParam.muTxAttenVga1Start);
        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga1End",msTestParam.muTxAttenVga1End);
        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga2Start",msTestParam.muTxAttenVga2Start);
        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga2End",msTestParam.muTxAttenVga2End);
        configIni.SetIniKeyValue(L"MAIN",L"RxAttenStart",msTestParam.muRxAttenStart);
        configIni.SetIniKeyValue(L"MAIN",L"RxAttenEnd",msTestParam.muRxAttenEnd);
        configIni.SetIniKeyValue(L"MAIN",L"TxPowerStart", msTestParam.mnTxPowerStart);
        configIni.SetIniKeyValue(L"MAIN",L"TxPowerEnd",msTestParam.mnTxPowerEnd);
        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga1Step",msTestParam.muTxAttenVga1Step);
        configIni.SetIniKeyValue(L"MAIN",L"TxAttenVga2Step",msTestParam.muTxAttenVga2Step);
        configIni.SetIniKeyValue(L"MAIN",L"RxAttenStep", msTestParam.muRxAttenStep);
        configIni.SetIniKeyValue(L"MAIN",L"TxPowerStep",msTestParam.mnTxPowerStep );
        configIni.SetIniKeyValue(L"MAIN",L"PacketCollectionTime", msTestParam.mnPacketCollectionTime);

        configIni.SetIniKeyValue(L"MAIN",L"TxUsbNetCardAddress",msTestParam.mstrTxUsbNetCardAddress.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"RxUsbNetCardAddress",msTestParam.mstrRxUsbNetCardAddress.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"RxIPQAddress",msTestParam.mstrRxIPQAddress.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"TxIPQAddress",msTestParam.mstrTxIPQAddress.c_str());
    }
}
