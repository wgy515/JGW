#include "StdAfx.h"
#include "CJGW_IperfToolDlg.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_IperfToolDlg::CCJGW_IperfToolDlg(void) : mUIController(this)
    {
#ifdef _DEBUG
        msIperfToolConfig.mbEnableAttenutor = false;
        msIperfToolConfig.mbEnableTrunTable = false;
#else
        msIperfToolConfig.mbEnableAttenutor = false;
        msIperfToolConfig.mbEnableTrunTable = false;
#endif 
        msIperfToolConfig.mbReadOmniRSSI = false;
        msIperfToolConfig.mstrIperfCommand = L"iperf3.exe -s";
        msIperfToolConfig.msMonitorInfo.m_cInterfaceType = E_GPIB_HIVE;
        msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType = AGILENT_GPIB_TYPE;
        msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface = 0;
        msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress = 17;
        msIperfToolConfig.msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress = 0;
        msIperfToolConfig.msAttenutorParam.mAttenutorAdjTime = 5 * 1000;
        msIperfToolConfig.msTrunTableParam.mTurnTableAdjTime = 3 * 1000;

        msIperfToolConfig.mnChannel = 2;
        msIperfToolConfig.mstrCurrentTxSector = L"00";
        msIperfToolConfig.mstrCurrentRxSector = L"00";
    }

    CCJGW_IperfToolDlg::~CCJGW_IperfToolDlg(void)
    {
    }

    void CCJGW_IperfToolDlg::InitPropertyConfig()
    {
        msIperfToolConfig.mbReadOmniRSSI = GET_PROPERTY->GetBool(L"throughput.omni.rssi");
        msIperfToolConfig.mnTerminalType = GET_PROPERTY->GetInt(L"throughput.terminal.type");
        msIperfToolConfig.mnProtocolType = GET_PROPERTY->GetInt(L"throughput.protocol.type");
        msIperfToolConfig.mstrIPAddress = GET_PROPERTY->GetString(L"throughput.ip.address");
        if (msIperfToolConfig.mstrIPAddress.empty())  msIperfToolConfig.mstrIPAddress = L"192.168.133.32";
        msIperfToolConfig.mstrTestTime = GET_PROPERTY->GetString(L"throughput.test.time");  
        if (msIperfToolConfig.mstrTestTime.empty())  msIperfToolConfig.mstrTestTime = L"10";
        msIperfToolConfig.mstrBandWidth = GET_PROPERTY->GetString(L"throughput.iperf.bandwidth");  
        msIperfToolConfig.mstrExtraParameterEdt = GET_PROPERTY->GetString(L"throughput.iperf.extraparameter");
        msIperfToolConfig.mstrThreadCount = GET_PROPERTY->GetString(L"throughput.iperf.thread.count");
        if (msIperfToolConfig.mstrThreadCount.empty()) msIperfToolConfig.mstrThreadCount = L"1";

        msIperfToolConfig.mstrHtmlFile = GET_PROPERTY->GetString(L"throughput.html.filepath");
        if (msIperfToolConfig.mstrHtmlFile.empty()) msIperfToolConfig.mstrHtmlFile = L"html\\current_echarts_qt.html";

        msIperfToolConfig.mstrIperfExeFilePath = GET_PROPERTY->GetString(L"throughput.iperf.exec.filepath").c_str();
        if (msIperfToolConfig.mstrIperfExeFilePath.empty()) msIperfToolConfig.mstrIperfExeFilePath =  L"tool\\iperf3.exe";
        msIperfToolConfig.msTrunTableParam.mfAcceleration = GET_PROPERTY->GetDouble(L"throughput.turntable.param.acceleration");
        if (0 == msIperfToolConfig.msTrunTableParam.mfAcceleration) msIperfToolConfig.msTrunTableParam.mfAcceleration = 100;

        msIperfToolConfig.msTrunTableParam.mfDeceleration = GET_PROPERTY->GetDouble(L"throughput.turntable.param.dcceleration");
        if (0 == msIperfToolConfig.msTrunTableParam.mfDeceleration) msIperfToolConfig.msTrunTableParam.mfDeceleration = 100;

        msIperfToolConfig.msTrunTableParam.mfVelocity = GET_PROPERTY->GetDouble(L"throughput.turntable.param.velocity");
        if (0 == msIperfToolConfig.msTrunTableParam.mfVelocity) msIperfToolConfig.msTrunTableParam.mfVelocity = 5;

        msIperfToolConfig.msTrunTableParam.mTurnTableAngleStart = GET_PROPERTY->GetInt(L"throughput.turntable.param.angle.start");
        msIperfToolConfig.msTrunTableParam.mTurnTableAngleEnd = GET_PROPERTY->GetInt(L"throughput.turntable.param.angle.end");
        msIperfToolConfig.msTrunTableParam.mTurnTableStep = GET_PROPERTY->GetInt(L"throughput.turntable.param.angle.step");
        if (0 == msIperfToolConfig.msTrunTableParam.mTurnTableStep) msIperfToolConfig.msTrunTableParam.mTurnTableStep = 5;

        msIperfToolConfig.msAttenutorParam.mfAttenutorStart = GET_PROPERTY->GetFloat(L"throughput.attenutor.val.start");
        msIperfToolConfig.msAttenutorParam.mfAttenutorEnd = GET_PROPERTY->GetFloat(L"throughput.attenutor.val.end");
        msIperfToolConfig.msAttenutorParam.mfAttenutorStep = GET_PROPERTY->GetFloat(L"throughput.attenutor.val.step");
        if (0 == msIperfToolConfig.msAttenutorParam.mfAttenutorStep) msIperfToolConfig.msAttenutorParam.mfAttenutorStep = 5;


        msIperfToolConfig.msAttenutorParam.mAttenutorAdjTime = GET_PROPERTY->GetInt(L"throughput.attenutor.adj.time");
        if (0 == msIperfToolConfig.msAttenutorParam.mAttenutorAdjTime) msIperfToolConfig.msAttenutorParam.mAttenutorAdjTime = 5 * 1000;

        msIperfToolConfig.msTrunTableParam.mTurnTableAdjTime = GET_PROPERTY->GetInt(L"throughput.turntable.adj.time");
        if (0 == msIperfToolConfig.msTrunTableParam.mTurnTableAdjTime) msIperfToolConfig.msTrunTableParam.mTurnTableAdjTime = 3 * 1000;
    }

    void CCJGW_IperfToolDlg::UpdatePropertyConfig()
    {
        GET_PROPERTY->PutBool(L"throughput.omni.rssi",msIperfToolConfig.mbReadOmniRSSI);
        //GET_PROPERTY->PutBool(L"throughput.turntable.enable",msIperfToolConfig.mbEnableTrunTable);
        GET_PROPERTY->PutDouble(L"throughput.turntable.param.acceleration",msIperfToolConfig.msTrunTableParam.mfAcceleration);
        GET_PROPERTY->PutDouble(L"throughput.turntable.param.dcceleration",msIperfToolConfig.msTrunTableParam.mfDeceleration);
        GET_PROPERTY->PutDouble(L"throughput.turntable.param.velocity",msIperfToolConfig.msTrunTableParam.mfVelocity);
        GET_PROPERTY->PutInt(L"throughput.turntable.param.angle.start",msIperfToolConfig.msTrunTableParam.mTurnTableAngleStart);
        GET_PROPERTY->PutInt(L"throughput.turntable.param.angle.end",msIperfToolConfig.msTrunTableParam.mTurnTableAngleEnd);
        GET_PROPERTY->PutInt(L"throughput.turntable.param.angle.step",msIperfToolConfig.msTrunTableParam.mTurnTableStep);

        GET_PROPERTY->PutInt(L"throughput.terminal.type",msIperfToolConfig.mnTerminalType);
        GET_PROPERTY->PutInt(L"throughput.protocol.type",msIperfToolConfig.mnProtocolType);
        GET_PROPERTY->PutString(L"throughput.html.filepath",msIperfToolConfig.mstrHtmlFile);
        GET_PROPERTY->PutString(L"throughput.ip.address",msIperfToolConfig.mstrIPAddress);
        GET_PROPERTY->PutString(L"throughput.test.time",msIperfToolConfig.mstrTestTime);  

        GET_PROPERTY->PutString(L"throughput.iperf.bandwidth",msIperfToolConfig.mstrBandWidth);  
        GET_PROPERTY->PutString(L"throughput.iperf.extraparameter",msIperfToolConfig.mstrExtraParameterEdt);
        GET_PROPERTY->PutString(L"throughput.iperf.thread.count",msIperfToolConfig.mstrThreadCount);
        //! GET_PROPERTY->PutString(L"throughput.iperf.command",msIperfToolConfig.mstrIperfCommand);
        GET_PROPERTY->PutString(L"throughput.iperf.exec.filepath",msIperfToolConfig.mstrIperfExeFilePath);


        //GET_PROPERTY->PutBool(L"throughput.attenutor.enable",msIperfToolConfig.mbEnableAttenutor);
        GET_PROPERTY->PutFloat(L"throughput.attenutor.val.start",msIperfToolConfig.msAttenutorParam.mfAttenutorStart);
        GET_PROPERTY->PutFloat(L"throughput.attenutor.val.end",msIperfToolConfig.msAttenutorParam.mfAttenutorEnd);
        GET_PROPERTY->PutFloat(L"throughput.attenutor.val.step",msIperfToolConfig.msAttenutorParam.mfAttenutorStep);
    }

    bool CCJGW_IperfToolDlg::GetUIControl()
    {
        bool bSuccess = true;

        mpWkeBrowser = static_cast<CWkeWindowlessUI*>(m_PaintManager.FindControl(L"wkeBrowser"));
        mpTerminalType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"terminalType"));
        mpProtocolType = static_cast<CComboUI*>(m_PaintManager.FindControl(L"protocolType"));

        mpClientConfigHorizontalLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"clientConfigHorizontalLayout"));
        mpIpAddressEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"ipAddressEdt"));
        mpTestTimeEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"testTimeEdt"));
        mpBandWidthEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"bandWidthEdt"));
        mpThreadEdt = static_cast<CEditUI*>(m_PaintManager.FindControl(L"threadEdt"));

        mpExtraParameterEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"extraParameterEdt"));
        mpLogEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"logEdt"));
        mpStartBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"startBtn"));
        mpStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"stopBtn"));
        mpConfigInfoEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"configInfoEdt"));
        bSuccess &= (NULL != mpWkeBrowser);
        bSuccess &= (NULL != mpTerminalType);
        bSuccess &= (NULL != mpProtocolType);
        bSuccess &= (NULL != mpClientConfigHorizontalLayout);
        bSuccess &= (NULL != mpIpAddressEdt);
        bSuccess &= (NULL != mpTestTimeEdt);
        bSuccess &= (NULL != mpBandWidthEdt);
        bSuccess &= (NULL != mpExtraParameterEdt);
        bSuccess &= (NULL != mpLogEdt);
        bSuccess &= (NULL != mpConfigInfoEdt);

        return bSuccess;
    }

    void CCJGW_IperfToolDlg::OnInitWindow()
    {
        if (!GetUIControl())
        {
            Close(IDCANCEL);
            ::MessageBox(GetHWND(),_T("¿Ø¼þ¼ÓÔØÊ§°Ü"),_T("ÊÓÍ¼³õÊ¼»¯´íÎó"),MB_ICONERROR);
            return ;
        }

        {
           
            InitPropertyConfig();
            mpWkeBrowser->InitBrowser(WM_TIMER_WKE_BROWSER);
            std::wstring strHtmlFile = JGW_RealativePathToAbsPath(msIperfToolConfig.mstrHtmlFile.c_str());
            std::wstring strEncodeHtmlFilePath,strDecodeHtmlFilePath;
            JGW_URLEncodeW(strHtmlFile,strEncodeHtmlFilePath);
            JGW_URLDecodeW(strEncodeHtmlFilePath,strDecodeHtmlFilePath);
            mpWkeBrowser->LoadFile(strEncodeHtmlFilePath.c_str());
            m_PaintManager.GetEventSource() += MakeDelegate(&mUIController,&CCJGW_IperfToolController::OnEventCustomMessage,0);
        } 
        {
            mTestThread.CreateMessageThread();
            Sleep(250);
            mTestThread.PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(&msIperfToolConfig),LPARAM(GetAsyncWndMessage()));
        }
        {
            UpdateIperfUIInfo();
            OnUpdateConfigInfoUI();
        }
    }

    void CCJGW_IperfToolDlg::OnNotify(TNotifyUI& msg)
    {
        mUIController.OnViewNotify(msg);
    }

    void CCJGW_IperfToolDlg::OnCloseWindow()
    {
        m_PaintManager.GetEventSource() -= MakeDelegate(&mUIController,&CCJGW_IperfToolController::OnEventCustomMessage,0);
        msIperfToolConfig.mbStartTestStatus = false;
        Sleep(1000 * 1);
        mTestThread.TerminateMessageThread();
        if (GetTrunTableSCLLibHelper().IsLoadedSCLLibHelperLibrary()) GetTrunTableSCLLibHelper().Close();
        UpdateIperfCommand();
        UpdateIperfToolConfigParam();
        UpdatePropertyConfig();
    }

    CControlUI* CCJGW_IperfToolDlg::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, DUI_CTR_WKEWINDOWLESSBROWSER) == 0)
            return	new CWkeWindowlessUI();
        return NULL;
    }

    void CCJGW_IperfToolDlg::UpdateIperfUIInfo()
    {
        mpTerminalType->SelectItem(msIperfToolConfig.mnTerminalType);
        mpProtocolType->SelectItem(msIperfToolConfig.mnProtocolType);
        mpExtraParameterEdt->SetText(msIperfToolConfig.mstrExtraParameterEdt.c_str());
        if (E_SERVICE_TYPE == msIperfToolConfig.mnTerminalType) 
        {
            mpClientConfigHorizontalLayout->SetVisible(false);
        }
        else
            mpClientConfigHorizontalLayout->SetVisible(true);

        mpIpAddressEdt->SetText(msIperfToolConfig.mstrIPAddress.c_str());
        mpTestTimeEdt->SetText(msIperfToolConfig.mstrTestTime.c_str());
        mpBandWidthEdt->SetText(msIperfToolConfig.mstrBandWidth.c_str());
        mpThreadEdt->SetText(msIperfToolConfig.mstrThreadCount.c_str());
        if (_ttoi(msIperfToolConfig.mstrThreadCount.c_str()) > 1) msIperfToolConfig.mbMultThread = true;
        else msIperfToolConfig.mbMultThread = false;
    }

    void CCJGW_IperfToolDlg::UpdateIperfToolConfigParam()
    {
        msIperfToolConfig.mnTerminalType = mpTerminalType->GetCurSel();
        msIperfToolConfig.mnProtocolType = mpProtocolType->GetCurSel();
        msIperfToolConfig.mstrExtraParameterEdt = mpExtraParameterEdt->GetText().GetData();
        msIperfToolConfig.mstrIPAddress = mpIpAddressEdt->GetText().GetData();
        msIperfToolConfig.mstrTestTime = mpTestTimeEdt->GetText().GetData();
        msIperfToolConfig.mnTestTime = _ttoi(msIperfToolConfig.mstrTestTime.c_str());
        msIperfToolConfig.mstrBandWidth = mpBandWidthEdt->GetText().GetData();
        msIperfToolConfig.mstrThreadCount = mpThreadEdt->GetText().GetData();
        if (_ttoi(msIperfToolConfig.mstrThreadCount.c_str()) > 1) msIperfToolConfig.mbMultThread = true;
        else msIperfToolConfig.mbMultThread = false;
        msIperfToolConfig.mnTestTimeIndex = 0;
        msIperfToolConfig.mnSeekgLog = 0;
    }

    void CCJGW_IperfToolDlg::UpdateIperfCommand()
    {
        if (E_SERVICE_TYPE == mpTerminalType->GetCurSel())
        {
            msIperfToolConfig.mstrIperfCommand = msIperfToolConfig.mstrIperfExeFilePath;
            msIperfToolConfig.mstrIperfCommand += L" -s";
            if (1 == mpProtocolType->GetCurSel()) msIperfToolConfig.mstrIperfCommand += L" -u";
            if (!msIperfToolConfig.mstrExtraParameterEdt.empty())
            {
                msIperfToolConfig.mstrIperfCommand += L" ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrExtraParameterEdt;
            }
            msIperfToolConfig.mstrIperfCommand += L" --logfile ";
            msIperfToolConfig.mstrIperfCommand += L"\"";
            msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrSaveOutPutLog;
            msIperfToolConfig.mstrIperfCommand += L"\"";
        }
        else
        {
            msIperfToolConfig.mstrIperfCommand = msIperfToolConfig.mstrIperfExeFilePath;
            msIperfToolConfig.mstrIperfCommand += L" -c";

            if (!msIperfToolConfig.mstrIPAddress.empty()) 
            {
                msIperfToolConfig.mstrIperfCommand += L" ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrIPAddress;
            }

            msIperfToolConfig.mstrIperfCommand += L" --logfile ";
            msIperfToolConfig.mstrIperfCommand += L"\"";
            msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrSaveOutPutLog;
            msIperfToolConfig.mstrIperfCommand += L"\"";
            if (!msIperfToolConfig.mstrThreadCount.empty())
            {
                msIperfToolConfig.mstrIperfCommand += L" -P ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrThreadCount;
            }
            if (!msIperfToolConfig.mstrTestTime.empty()) 
            {
                msIperfToolConfig.mstrIperfCommand += L" -t ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrTestTime;
            }
            if (!msIperfToolConfig.mstrBandWidth.empty()) 
            {
                msIperfToolConfig.mstrIperfCommand += L" -b ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrBandWidth;
            }
            if (!msIperfToolConfig.mstrExtraParameterEdt.empty())
            {
                msIperfToolConfig.mstrIperfCommand += L" ";
                msIperfToolConfig.mstrIperfCommand += msIperfToolConfig.mstrExtraParameterEdt;
            }
            
        }
        msIperfToolConfig.mstrIperfCommand += L" -f m";
        // msIperfToolConfig.mstrIperfCommand += L"\n";
    }

    void CCJGW_IperfToolDlg::EnableTestUI(bool bEnable /* = false */)
    {
        mpProtocolType->SetEnabled(bEnable);
        mpTerminalType->SetEnabled(bEnable);
        mpExtraParameterEdt->SetEnabled(bEnable);
        mpClientConfigHorizontalLayout->SetEnabled(bEnable);
        mpStopBtn->SetEnabled(!bEnable);
        mpStartBtn->SetEnabled(bEnable);
    }

    void CCJGW_IperfToolDlg::BuildIperfOutputLogFilePath()
    {
        std::wstring strLogFilePath,strTemp;
        JGW_GetExecSubFolder(strLogFilePath,L"log\\Iperf\\");
        strLogFilePath += JGW_GetTimeFolder();
        strLogFilePath += L"\\";
        JGW_CreateDirectory(strLogFilePath.c_str(),true);
        JGW_FormatWString(strTemp,L"Iperf_%u.txt",JGW_GetTimeOfDay());
        msIperfToolConfig.mstrSaveOutPutLog = strLogFilePath + strTemp;
    }

    void CCJGW_IperfToolDlg::OnUpdateConfigInfoUI()
    {
        mpConfigInfoEdt->SetText(L"Config Info:\n");

        if (msIperfToolConfig.mbEnableAttenutor)
        {
            mpConfigInfoEdt->AppendText(JGW_GetFormatWString(L"Attenutor: %.2f,%.2f,%.2f\n"
                ,msIperfToolConfig.msAttenutorParam.mfAttenutorStart
                ,msIperfToolConfig.msAttenutorParam.mfAttenutorEnd
                ,msIperfToolConfig.msAttenutorParam.mfAttenutorStep).c_str());
        }
        if (msIperfToolConfig.mbEnableTrunTable)
        {
            mpConfigInfoEdt->AppendText(JGW_GetFormatWString(L"Turntable: %d,%d,%d\n"
                ,msIperfToolConfig.msTrunTableParam.mTurnTableAngleStart
                ,msIperfToolConfig.msTrunTableParam.mTurnTableAngleEnd
                ,msIperfToolConfig.msTrunTableParam.mTurnTableStep).c_str());
        }

        
    }
}