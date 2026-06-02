#include "StdAfx.h"
#include "CJGW_ThroughputTestTool2Controller.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_ThroughputTestToolDlg2.h"
#include <regex>

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>

#include "CJGW_ThroughputTestThread.h"
#include <fstream>
//! GPS SPEED休息
#define WM_GPS_SPEED_MSG  WM_HWND_TSE_INDEX_MSG
//! 吞吐率
#define WM_THROUGHPUT_RATE_MSG WM_TEST_THREAD_TSE_INDEX_MSG_RUN
//! 时延
#define WM_TIME_DELAY_MSG WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE
//! 检查11AD是否已经连接
#define WM_11AD_CONNECT_STATUS_MSG WM_TEST_THREAD_RESET_TSE
//! 是否开始记录测试信息
#define WM_START_RECORD_TEST_INFO WM_HWND_UPDATE_DONGLES_INFO
//! HTML只需要显示IPERF有数据的时候
//! 显示速率 , GPS信息 , 速度 并进行保存
//! 手机网络通讯消息
#define WM_MPHONE_SOCKET_REMOTE_MSG WM_HWND_RESET_TSE_COMPLETE

namespace JGW
{
	/*
	root@mmwave:~# iw wlan0 station dump
	Station 7c:6c:f0:af:ff:95 (on wlan0)
	inactive time:  0 ms
	rx bytes:       44966
	rx packets:     144
	tx bytes:       4063
	tx packets:     50
	signal:         -49 dBm
	signal avg:     -49 dBm
	tx bitrate:     962.5 MBit/s
	rx bitrate:     385.0 MBit/s
	connected time: 146 seconds
	associated at [boottime]:       1743175.945s
	associated at:  18446727371884570018 ms
	current time:   1743176092037 ms
	*/
    CCJGW_ThroughputTestTool2Controller::CCJGW_ThroughputTestTool2Controller(CCJGW_ThroughputTestToolDlg2* pView) : mpView(pView),mpThroughputTestThread(NULL),mbUpdateTimerIsSetTimer(false)
    {
        msTestParams.meSocketTerimalType = (E_SERVICE_TYPE);
        msTestParams.meProtocolType = (E_TCP_TYPE);
        msTestParams.mnThreadCount = (4);
        msTestParams.mnTestIndex = (0);
        msTestParams.mbIsSaveIPQLog = true;
        //msTestParams.mbTestDelayTime = false;
        msTestParams.mstrUserNameSSH = "root";
        msTestParams.mstrPasswordSSH = "20171015";
        msTestParams.mstrCheck11ADConnectCommand = "iw wlan0 station dump";
        msTestParams.mstr11ADConnectedFindString = "signal";
        msTestParams.mstrMcsConfig = "rx bitrate:";
        msTestParams.mstrSignalConfig = "signal:";
        msTestParams.mnSettingMaxListCount = 300;
		msTestParams.mnCheckSignalTimeIntervalMS = 400;
        mpAsyncWndMessage = mpView->GetAsyncWndMessage();
#if 0
        mcIperf3Command.RegisterCallbackIperf3TestInfo(&CCJGW_ThroughputTestTool2Controller::Iperf3TestSpeedCallBack,this);
#endif
        JGW_FormatWString(mstrConfigFilePath,L"%s%s",JGW_GetApplicationFolder(),L"ThroughputTestTool2.ini");
    }


    CCJGW_ThroughputTestTool2Controller::~CCJGW_ThroughputTestTool2Controller(void)
    {
    }

    bool CCJGW_ThroughputTestTool2Controller::OnInit()
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(mstrConfigFilePath.c_str());

        msTestParams.meSocketTerimalType = E_SERVICE_TYPE == configIni.GetIniKeyIntValue(L"MAIN",L"TerminalType",E_SERVICE_TYPE) ? E_SERVICE_TYPE:E_CLIENT_TYPE;
        //msTestParams.meProtocolType = E_TCP_TYPE == configIni.GetIniKeyIntValue(L"MAIN",L"ProtocolType",E_TCP_TYPE) ? E_TCP_TYPE:E_UDP_TYPE;
        msTestParams.meProtocolType = (E_PROTOCOL_TYPE)configIni.GetIniKeyIntValue(L"MAIN",L"ProtocolType",E_TCP_TYPE);
        msTestParams.mstrIperfIPAddr = configIni.GetIniKeyValue(L"MAIN",L"IperfIP");
        msTestParams.mstrSSHIPAddr = configIni.GetIniKeyValue(L"MAIN",L"SSHIP");
        msTestParams.mstrPhoneIPAddr = configIni.GetIniKeyValue(L"MAIN",L"CellphoneIP");
        msTestParams.mbIsUseCellphoneIP = 1 == configIni.GetIniKeyIntValue(L"MAIN",L"IsUseCellphoneIP",1) ? true : false;
        msTestParams.mnTestTimes = configIni.GetIniKeyIntValue(L"MAIN",L"TestTimes",10);
        msTestParams.mnThreadCount = configIni.GetIniKeyIntValue(L"MAIN",L"Threads",4);
        msTestParams.mstrExtraParameter = configIni.GetIniKeyValue(L"MAIN",L"ExtraParameters");
        //msTestParams.mbTestDelayTime = 1 == configIni.GetIniKeyIntValue(L"MAIN",L"TestDelayTime",1) ? true : false;
        msTestParams.mstrHtmlFile = configIni.GetIniKeyValue(L"MAIN",L"WKE_HTML",L"html\\echarts_speed.html");
        msTestParams.mstrCheck11ADConnectCommand = configIni.GetIniKeyValueA(L"MAIN",L"Check11ADConnectCommand",L"iw wlan0 station dump");
        msTestParams.mstr11ADConnectedFindString = configIni.GetIniKeyValueA(L"MAIN",L"11ADConnectedFindString",L"signal");
        msTestParams.mstrMcsConfig = configIni.GetIniKeyValueA(L"MAIN",L"McsConfig",L"rx bitrate:");
        msTestParams.mstrSignalConfig = configIni.GetIniKeyValueA(L"MAIN",L"SignalConfig",L"signal:");
		msTestParams.mnCheckSignalTimeIntervalMS = configIni.GetIniKeyIntValue(L"MAIN",L"CheckSignalTimeIntervalMS",500);

        //! msTestParams.mstrUserNameSSH = configIni.GetIniKeyValue(L"MAIN",L"UserNameSSH",L"root");
        //! msTestParams.mstrPasswordSSH = configIni.GetIniKeyValue(L"MAIN",L"PasswordSS",L"20171015");
		msTestParams.mstrUserNameSSH = configIni.GetIniKeyValueA(L"MAIN",L"UserNameSSH",L"fBL/Bgdpgw4MI9v59csOow==");
		msTestParams.mstrUserNameSSH = CCJGW_CryptPassword::DecryptPassword(msTestParams.mstrUserNameSSH);
		msTestParams.mstrPasswordSSH = configIni.GetIniKeyValueA(L"MAIN",L"PasswordSS",L"1kXfi9kI3PNFAcW6UbQPYQ==");
		msTestParams.mstrPasswordSSH = CCJGW_CryptPassword::DecryptPassword(msTestParams.mstrPasswordSSH);
        msTestParams.mnSettingMaxListCount = configIni.GetIniKeyIntValue(L"MAIN",L"SettingMaxListCount",300);
        msTestParams.mstrLogPrefix = configIni.GetIniKeyValue(L"MAIN",L"LOG_PREFIX");
        msTestParams.mstrIperfRelativePath = configIni.GetIniKeyValue(L"MAIN",L"IperfRelativePath",L"tool\\iperf3.exe");
        msTestParams.mbIsSaveIPQLog = 1 == configIni.GetIniKeyIntValue(L"MAIN",L"SaveIPQLogCheckBox",1) ? true : false;


        mpView->mpTerminalType->SelectItem(msTestParams.meSocketTerimalType);
        mpView->mpProtocolType->SelectItem(msTestParams.meProtocolType);
        mpView->mpIperfIPEdt->SetText(msTestParams.mstrIperfIPAddr.c_str());
        mpView->mpSshIpAddressEdt->SetText(msTestParams.mstrSSHIPAddr.c_str());
        mpView->mpCellphoneIPEdt->SetText(msTestParams.mstrPhoneIPAddr.c_str());
        mpView->mpCheckUseCellphoneIP->SetCheck(msTestParams.mbIsUseCellphoneIP);
        mpView->mpSaveIPQLogCheckBox->SetCheck(msTestParams.mbIsSaveIPQLog);
        mpView->mpTestTimeEdt->SetText(configIni.GetIniKeyValue(L"MAIN",L"TestTimes",L"10").c_str());
        mpView->mpThreadCountEdt->SetText(configIni.GetIniKeyValue(L"MAIN",L"Threads",L"4").c_str());
        mpView->mpExtraParameterEdt->SetText(msTestParams.mstrExtraParameter.c_str());
        mpView->mpLogPrefixEdt->SetText(msTestParams.mstrLogPrefix.c_str());

        OnClickUpdateConfigOptionOption();

        return true;
    }

    void CCJGW_ThroughputTestTool2Controller::SaveConfig()
    {
        CCJGW_ConfigIni configIni;
        configIni.InitIniFilePath(mstrConfigFilePath.c_str());

        configIni.SetIniKeyValue(L"MAIN",L"TerminalType",msTestParams.meSocketTerimalType == E_SERVICE_TYPE ? 0 : 1);
        //configIni.SetIniKeyValue(L"MAIN",L"ProtocolType",msTestParams.meProtocolType == E_TCP_TYPE ? 0 : 1);
        configIni.SetIniKeyValue(L"MAIN",L"ProtocolType",msTestParams.meProtocolType);
        configIni.SetIniKeyValue(L"MAIN",L"IperfIP",msTestParams.mstrIperfIPAddr.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"SSHIP",msTestParams.mstrSSHIPAddr.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"CellphoneIP",msTestParams.mstrPhoneIPAddr.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"IsUseCellphoneIP",msTestParams.mbIsUseCellphoneIP ? 1 : 0);
        configIni.SetIniKeyValue(L"MAIN",L"TestTimes",msTestParams.mnTestTimes);
        configIni.SetIniKeyValue(L"MAIN",L"Threads",msTestParams.mnThreadCount);
        configIni.SetIniKeyValue(L"MAIN",L"ExtraParameters",msTestParams.mstrExtraParameter.c_str());
        //! configIni.SetIniKeyValue(L"MAIN",L"TestDelayTime",msTestParams.mbTestDelayTime? 1 : 0);
        configIni.SetIniKeyValue(L"MAIN",L"LOG_PREFIX",msTestParams.mstrLogPrefix.c_str());
    }

    void CCJGW_ThroughputTestTool2Controller::OnCloseWindow()
    {
        OnClickStopBtn();
        if (mbUpdateTimerIsSetTimer)
        {
            KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME);
            mbUpdateTimerIsSetTimer = false;
        }
        if (NULL != mpThroughputTestThread)
        {
            mpThroughputTestThread->TerminateMessageThread();
            delete mpThroughputTestThread;
            mpThroughputTestThread = NULL;
        }
    }

    bool CCJGW_ThroughputTestTool2Controller::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
            OnAppendRichEditTestMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_TIMER:
            if (pTEventUI->wParam == WM_TIMER_UPDATE_TIME) OnUpdateUiTimerMsg();
            break;
        case WM_TEST_PASS_MSG:
            OnTestCompletedMsg();
            break;
        case WM_GPS_SPEED_MSG:
            OnRecvGPSSpeedMsg((const wchar_t*)(pTEventUI->wParam));
            break;
		case WM_MPHONE_SOCKET_REMOTE_MSG:
			msLogInfo.mstrRemoteSocketMsg = (const wchar_t*)(pTEventUI->wParam);
			JGW_EraseLastAndFristTwoCharsW(msLogInfo.mstrRemoteSocketMsg);
			JGW_ReplaceStringW(msLogInfo.mstrRemoteSocketMsg,L"\t",L",");
			break;
        case WM_THROUGHPUT_RATE_MSG:
            OnThroughputRateMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_TIME_DELAY_MSG:
            OnTimeDelayMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_11AD_CONNECT_STATUS_MSG:
            msLogInfo.mbIs11adConnected = (1 == _ttoi((const wchar_t*)pTEventUI->wParam)) ? true : false;
            if (!msLogInfo.mbIs11adConnected)
            {
                msLogInfo.mstrThroughputRate = L"";
                msLogInfo.mstrPingTimeDelay = L"";
                msLogInfo.mnMcs = -1;
                msLogInfo.mnRssi = -128;
                msLogInfo.mnTxSector = -1;
                msLogInfo.mnRxSector = -1;
            }
            else
            {
                std::vector<int> vstrTemp;
                JGW_ParserIntW((const wchar_t*)pTEventUI->wParam,L",",vstrTemp);
                if (5 == vstrTemp.size())
                {
                    msLogInfo.mnRssi = vstrTemp[1];
                    msLogInfo.mnMcs = vstrTemp[2];
                    msLogInfo.mnTxSector = vstrTemp[3];
                    msLogInfo.mnRxSector = vstrTemp[4];
                }
            }
            break;
        case WM_START_RECORD_TEST_INFO:
            if (!mbUpdateTimerIsSetTimer)
            {
                SetTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME,1000,NULL);
                mbUpdateTimerIsSetTimer = true;
            }
            break;
        case WM_STOP_RECORD_TEST_INFO:
            if (mbUpdateTimerIsSetTimer)
            {
                KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME);
                mbUpdateTimerIsSetTimer = false;
            }
            break;
        }

        return true;
    }

    void CCJGW_ThroughputTestTool2Controller::OnViewNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpView->mpStartBtn)
            {
                OnClickStartBtn();
            }
            else if (msg.pSender == mpView->mpStopBtn)
            {
                OnClickStopBtn();
            }
            else if (msg.pSender == mpView->mpNetCardInfoComBo)
            {
                UpdateNetCardInfoComBo();
            }
            else if (msg.pSender == mpView->mpLogFolderBtn)
            {
                if (!msTestParams.mstrLogFolder.empty())
                {
                    ShellExecute(NULL,_T("open"),msTestParams.mstrLogFolder.c_str(),_T(""),_T(""),SW_SHOWNORMAL);
                    //JGW_FormatWString(mstrTestLogFilePath,L"%slog\\ThroughputTestTool2\\%s\\%s_%u.csv",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),msTestParams.mstrLogPrefix.c_str(),JGW_GetTimeOfDay());
                    //if (!JGW_IsFolderExist(JGW_GetFileFolderToFilePath(mstrTestLogFilePath.c_str())))
                    //{
                    //    JGW_CreateDirectory(mstrTestLogFilePath.c_str(),false);
                    //}
                }
                
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (msg.pSender == mpView->mpUpdateConfigOption)
            {
                UpdateNetCardInfoComBo();
                OnClickUpdateConfigOptionOption();
                mpView->mpUpdateConfigOption->Selected(false);
            }
            else if (msg.pSender == mpView->mpCheckUseCellphoneIP)
            {
                if (mpView->mpCheckUseCellphoneIP->GetCheck())
                {
                    mpView->mpCellphoneIPEdt->SetEnabled(true);
                    mpView->mpNetCardInfoComBo->SetEnabled(true);
                }
                else
                {
                    mpView->mpCellphoneIPEdt->SetEnabled(false);
                    mpView->mpNetCardInfoComBo->SetEnabled(false);
                }
            }
        }
        else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpView->mpNetCardInfoComBo)
            {
                UpdatePhoneIpConfig();
            }
        }
    }

    void CCJGW_ThroughputTestTool2Controller::OnClickUpdateConfigOptionOption()
    {
        mpView->mpStartBtn->SetEnabled(true);
        //! 保存UI界面配置信息
        msTestParams.meSocketTerimalType = (0 == mpView->mpTerminalType->GetCurSel()) ? E_SERVICE_TYPE : E_CLIENT_TYPE;
        //msTestParams.meProtocolType = (0 == mpView->mpProtocolType->GetCurSel()) ? E_TCP_TYPE : E_UDP_TYPE;
        msTestParams.meProtocolType = (E_PROTOCOL_TYPE)mpView->mpProtocolType->GetCurSel();
        msTestParams.mnTestTimes = _ttoi(mpView->mpTestTimeEdt->GetText().GetData());
        msTestParams.mnThreadCount = _ttoi(mpView->mpThreadCountEdt->GetText().GetData());
        msTestParams.mstrIperfIPAddr = mpView->mpIperfIPEdt->GetText().GetData();
        msTestParams.mstrSSHIPAddr = mpView->mpSshIpAddressEdt->GetText().GetData();
        msTestParams.mstrPhoneIPAddr = mpView->mpCellphoneIPEdt->GetText().GetData();
        msTestParams.mbIsUseCellphoneIP = mpView->mpCheckUseCellphoneIP->GetCheck();
        msTestParams.mbIsSaveIPQLog = mpView->mpSaveIPQLogCheckBox->GetCheck();
        msTestParams.mstrLogPrefix  = mpView->mpLogPrefixEdt->GetText().GetData();
        //! -w 8M -l 65000
        msTestParams.mstrExtraParameter = mpView->mpExtraParameterEdt->GetText().GetData();
        //! 检查IP地址有效性
        if (!CheckIPAddressValid(msTestParams.mstrIperfIPAddr) 
            || !CheckIPAddressValid(msTestParams.mstrSSHIPAddr) 
            || (!msTestParams.mbIsUseCellphoneIP && !CheckIPAddressValid(msTestParams.mstrPhoneIPAddr)))
        {
            MessageBox(mpView->m_hWnd,L"Invalid IP address",L"Error",MB_ICONERROR);
            return;
        }
		//! 
		//msTestParams.mbTestDelayTime = (E_PING_TYPE == msTestParams.meProtocolType) ? true : false;
		//!
        if (E_SERVICE_TYPE == msTestParams.meSocketTerimalType)
        {
            msTestParams.mstrIperfCommand = L"iperf3 -s -f m";
            mpView->mpIperfCommandEdt->SetText(msTestParams.mstrIperfCommand.c_str());
			
        }
        else
        {
            if (E_UDP_TYPE == msTestParams.meProtocolType)
            {
                JGW_FormatWString(msTestParams.mstrIperfCommand,L"iperf3 -u %s -t %u -f m -c %s ",msTestParams.mstrExtraParameter.c_str(),msTestParams.mnTestTimes,msTestParams.mstrIperfIPAddr.c_str());
                JGW_EraseLastAndFristTwoCharsW(msTestParams.mstrIperfCommand,' ',' ');
                JGW_ReplaceStringW(msTestParams.mstrIperfCommand,L"  ",L" ");
                mpView->mpIperfCommandEdt->SetText(msTestParams.mstrIperfCommand.c_str());
            }
            else if (E_PING_TYPE == msTestParams.meProtocolType)
            {
                msTestParams.mstrIperfCommand = L"";
                mpView->mpIperfCommandEdt->SetText(msTestParams.mstrIperfCommand.c_str());
            }
            else
            {
                JGW_FormatWString(msTestParams.mstrIperfCommand,L"iperf3 -c %s -t %u -P %u -f m %s",msTestParams.mstrIperfIPAddr.c_str(),msTestParams.mnTestTimes,msTestParams.mnThreadCount,msTestParams.mstrExtraParameter.c_str());
                JGW_EraseLastAndFristTwoCharsW(msTestParams.mstrIperfCommand,' ',' ');
                JGW_ReplaceStringW(msTestParams.mstrIperfCommand,L"  ",L" ");
                mpView->mpIperfCommandEdt->SetText(msTestParams.mstrIperfCommand.c_str());
            }
        }
        SaveConfig();
    }

    bool CCJGW_ThroughputTestTool2Controller::CheckIPAddressValid(const std::wstring& strIPAddress)
    {
        std::wstring strRegex = L"^((25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)\\.){3}(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)$";
        std::wregex partten(strRegex,std::wregex::icase);;
        return regex_match (strIPAddress,partten);
    }

    void CCJGW_ThroughputTestTool2Controller::OnClickStartBtn()
    {
        OnClickUpdateConfigOptionOption();
        mpView->mpClientConfigHorizontalLayout->SetEnabled(false);
        mpView->mpStartBtn->SetEnabled(false);
        mpView->mpStopBtn->SetEnabled(true);
        mpView->mpLogEdt->SetText(L"");
        msTestParams.mnTestIndex = 1;

        msLogInfo.mbIs11adConnected = false; //! 11AD 是否已经连接
        msLogInfo.muUnixTestLogTimeStamp = 0;
        msLogInfo.mstrTime = L"";
        msLogInfo.mstrSpeedPerHour = L"";
        msLogInfo.mstrLatitude = L"";
        msLogInfo.mstrLongitude = L"";
        msLogInfo.mstrThroughputRate = L"";
        msLogInfo.mstrPingTimeDelay = L"";
		msLogInfo.mstrDistance = L"";
		msLogInfo.mstrRemoteLatitude =  L"";
		msLogInfo.mstrRemoteLongitude =  L"";
        msLogInfo.mnMcs = -1;
        msLogInfo.mnRssi = -128;
        msLogInfo.mnTxSector = -1;
        msLogInfo.mnRxSector = -1;

        mpView->mpWkeBrowser->ClearThrouphputRateSpeed();
        mpView->mpWkeBrowser->SettingMaxListCount(msTestParams.mnSettingMaxListCount);

        msTestParams.mnTimeStamp = JGW_GetTimeOfDay();
        int index = 0;
#if 0
        do
        {
            JGW_FormatWString(mstrTestLogFilePath,L"%slog\\ThroughputTestTool2\\%s\\%s_%d\\%s_%u.csv",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetCurrentTimeStampHMS().c_str(),index++,msTestParams.mstrLogPrefix.c_str(),msTestParams->mnTimeStamp);
        } while (JGW_IsFolderExist(JGW_GetFileFolderToFilePath(mstrTestLogFilePath.c_str())));

        if (!JGW_IsFolderExist(JGW_GetFileFolderToFilePath(mstrTestLogFilePath.c_str())))
        {
            JGW_CreateDirectory(mstrTestLogFilePath.c_str(),false);
        }
#else
        do 
        {
            JGW_FormatWString(msTestParams.mstrLogFolder,L"%slog\\ThroughputTestTool2\\%s\\%s_%d",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetCurrentTimeStampHMS(_T("h%02d_m%02d_s%02d")).c_str(),index++);
        } while (JGW_IsFolderExist(msTestParams.mstrLogFolder));

        JGW_FormatWString(mstrTestLogFilePath,L"%s\\%s_%u.csv",msTestParams.mstrLogFolder.c_str(),msTestParams.mstrLogPrefix.c_str(),msTestParams.mnTimeStamp);
        if (!JGW_IsFolderExist(msTestParams.mstrLogFolder))
        {
            JGW_CreateDirectory(msTestParams.mstrLogFolder.c_str(),true);
        }
#endif
        if (NULL == mpThroughputTestThread)
        {
            mpThroughputTestThread = new CCJGW_ThroughputTestThread(&msTestParams,mpAsyncWndMessage);
            mpThroughputTestThread->CreateMessageThread(0,0,true);
        }
        mpThroughputTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN);
		Log4WI(L"Start Throughput Test...");
    }

    void CCJGW_ThroughputTestTool2Controller::OnClickStopBtn()
    {
        mpView->mpStopBtn->SetEnabled(false);
        if (NULL != mpThroughputTestThread)
        {
            mpThroughputTestThread->StopThroughputTestThread();
        }
		Log4WI(L"Stop Throughput Test...");
    }

    float gfTestSpeed = 0.00f;
    void CCJGW_ThroughputTestTool2Controller::OnAppendRichEditTestMsg(const wchar_t* strMsg)
    {
        static int line = 0;
        if (line++ > 3000) {mpView->mpLogEdt->Clear();line = 0;}
        mpView->mpLogEdt->AppendText(strMsg);
        mpView->mpLogEdt->AppendText(L"\r\n");
        mpView->mpLogEdt->EndDown();
    }

    void CCJGW_ThroughputTestTool2Controller::UpdateTestResult()
    {
        std::wstring strTemp;
        //! 获取界面视图显示
        JGW_FormatWString(strTemp,L"%s\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s",msLogInfo.mbIs11adConnected ? L"Connected" : L"Not connect",msLogInfo.mnRssi,msLogInfo.mnMcs,msLogInfo.mnTxSector,msLogInfo.mnRxSector,msLogInfo.mstrThroughputRate.c_str()
            ,msLogInfo.mstrPingTimeDelay.c_str(),msLogInfo.mstrLatitude.c_str(),msLogInfo.mstrLongitude.c_str(),msLogInfo.mstrSpeedPerHour.c_str(),msLogInfo.mstrDistance.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());

        //! 记录测试信息并上传到对端
        if (msTestParams.mstrUploadPhoneSocketMessage.empty()) msTestParams.mstrUploadPhoneSocketMessage = strTemp + L"\r\n";

        //! 新增远程经纬度信息
        strTemp += JGW_GetFormatWString(L"\t%s\t%s",msLogInfo.mstrRemoteLatitude.c_str(),msLogInfo.mstrRemoteLongitude.c_str());
        JGW_ReplaceStringW(strTemp,L"\t",L",");

        //! 添加对端测试信息记录
        if (!msLogInfo.mstrRemoteSocketMsg.empty())
        {
            strTemp += L",<----------------->,";
            strTemp += msLogInfo.mstrRemoteSocketMsg;
            msLogInfo.mstrRemoteSocketMsg = L"";
        }
        //! 添加时间并写入测试日志
        msLogInfo.mstrTime = JGW_GetCurrentTimeStampHMS(L"%02d:%02d:%02d");
        WriteTestLog(msLogInfo.mstrTime + L"," + strTemp);
    }

    void CCJGW_ThroughputTestTool2Controller::OnUpdateUiTimerMsg()
    {
        //! time,connect,throughput rate(Mbits/sec),time delay(ms),Latitude,Longitude,speed(km/h)
        size_t uUnixTestLogTimeStamp = JGW_GetTimeOfDay();
        //! 如果不是最近更新了测试日志，则开始更新
        if (!(msLogInfo.muUnixTestLogTimeStamp == uUnixTestLogTimeStamp - 1 || msLogInfo.muUnixTestLogTimeStamp == uUnixTestLogTimeStamp))
        {
            UpdateTestResult();
        }
    }

    void CCJGW_ThroughputTestTool2Controller::OnTestCompletedMsg()
    {
        mpView->mpStartBtn->SetEnabled(true);
        mpView->mpStopBtn->SetEnabled(false);
        mpView->mpClientConfigHorizontalLayout->SetEnabled(true);
        if (mbUpdateTimerIsSetTimer)
        {
            KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME);
            mbUpdateTimerIsSetTimer = false;
        }
        OnAppendRichEditTestMsg(mstrTestLogFilePath.c_str());
        OnAppendRichEditTestMsg(L"Test is completed.");
    }

    void CCJGW_ThroughputTestTool2Controller::UpdateNetCardInfoComBo()
    {
        mpView->mpNetCardInfoComBo->RemoveAll();
        std::vector<NetWorkConection>& vNetWorkConnection = mcAdapterCommonInfo.GetNetAdaptersInfo();
        for (size_t i = 0;i < vNetWorkConnection.size();i ++)
        {
            CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
            pListLabelEle->SetDate(&vNetWorkConnection[i]);
            mpView->mpNetCardInfoComBo->Add(pListLabelEle);
            if (NULL != JGW_StrCaseStr(vNetWorkConnection[i].description.c_str(),"Wireless") || NULL != JGW_StrCaseStr(vNetWorkConnection[i].description.c_str(),"WLAN"))
            {
                mpView->mpNetCardInfoComBo->SelectItem(i);
            }
            pListLabelEle->SetText(JGW_GetNetcardDeviceNameToAdapterName(vNetWorkConnection[i].adapter_name.c_str()).c_str());
        }

        if (-1 == mpView->mpNetCardInfoComBo->GetCurSel())
        {
            mpView->mpCellphoneIPEdt->SetText(L"0.0.0.0");
        }
    }

    void CCJGW_ThroughputTestTool2Controller::UpdatePhoneIpConfig()
    {
        if (mpView->mpNetCardInfoComBo->GetCount() >= 1 && -1 != mpView->mpNetCardInfoComBo->GetCurSel())
        {
            int index = mpView->mpNetCardInfoComBo->GetCurSel();
            NetWorkConection* pNetWorkConnection = (NetWorkConection*)mpView->mpNetCardInfoComBo->GetItemAt(index)->GetDate();
            if (NULL != pNetWorkConnection)
            {
                mpView->mpCellphoneIPEdt->SetText(pNetWorkConnection->default_gateway.c_str());
            }
        }
    }

    void CCJGW_ThroughputTestTool2Controller::OnRecvGPSSpeedMsg(const wchar_t* strMsg)
    {
        if (NULL != strMsg)
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(strMsg,L",",vstrTemp);
            if (6 == vstrTemp.size())
            {
                msLogInfo.mstrLatitude = vstrTemp[0];
                msLogInfo.mstrLongitude = vstrTemp[1];
                msLogInfo.mstrSpeedPerHour = vstrTemp[2];
				msLogInfo.mstrDistance = vstrTemp[3];
				msLogInfo.mstrRemoteLatitude = vstrTemp[4];
				msLogInfo.mstrRemoteLongitude = vstrTemp[5];
            }
            else
            {
                msLogInfo.mstrLatitude = L"";
                msLogInfo.mstrLongitude = L"";
                msLogInfo.mstrSpeedPerHour = L"";
				msLogInfo.mstrDistance = L"";
				msLogInfo.mstrRemoteLatitude =  L"";
				msLogInfo.mstrRemoteLongitude =  L"";
            }
        }
    }

    void CCJGW_ThroughputTestTool2Controller::OnThroughputRateMsg(const wchar_t* strMsg)
    {
        if (NULL != strMsg)
        {
            msLogInfo.mstrThroughputRate = strMsg;
            msLogInfo.muUnixTestLogTimeStamp = JGW_GetTimeOfDay();
            UpdateTestResult();
            mpView->mpWkeBrowser->UpdateThrouphputRateSpeed(msTestParams.mnTestIndex++,_ttof(msLogInfo.mstrThroughputRate.c_str()),_ttof(msLogInfo.mstrSpeedPerHour.c_str()));
        }
        else
        {
            msLogInfo.mstrThroughputRate = L"";
        }
    }

    void CCJGW_ThroughputTestTool2Controller::OnTimeDelayMsg(const wchar_t* strMsg)
    {
        if (NULL != strMsg)
        {
            msLogInfo.mstrPingTimeDelay = strMsg;
        }
    }

    void CCJGW_ThroughputTestTool2Controller::WriteTestLog(const std::wstring& strTestLog)
    {
        bool bTestLogExist = JGW_FileExistsToFilePath(mstrTestLogFilePath.c_str());

        std::wofstream oFileStream;
        oFileStream.imbue(std::locale(""));
        oFileStream.open(mstrTestLogFilePath.c_str(),std::ios_base::app);
        if (oFileStream.is_open())
        {
            if (!bTestLogExist)
            {
                oFileStream << L"time,connect,rssi,mcs,tx sector,rx sector,throughput rate(Mbits/sec),time delay(ms),Latitude,Longitude,speed(km/h),Distance(M),Remote Latitude,Remote Longitude," << std::endl;
            }
            oFileStream << strTestLog << std::endl;
            oFileStream.flush();
            oFileStream.close();
        }
    }
} 
