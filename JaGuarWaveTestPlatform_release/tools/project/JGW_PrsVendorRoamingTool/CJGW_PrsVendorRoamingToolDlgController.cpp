#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingToolDlgController.h"
#include "CJGW_PrsVendorRoamingToolDlg.h"

#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include <regex>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include <fstream>

#include "CJGW_PrsRoamingTestThread.h"
#include <JGW_WindowsFuncPlugin/TimeUtils.h>

const wchar_t gszBkColor[][12] = {L"#ff00ff",L"#800000",L"#FF386382"};
#define RESET_COMMAND "31C556A5-DD94-4214-8954-E9DCC67B3117"

//! 重置显示
#define RESET_COMMAND "31C556A5-DD94-4214-8954-E9DCC67B3117"
//! 设置图表最大的显示点数
#define SETTING_MAX_LIST_COUNT "33F55E00-35A5-4B62-9D2B-3BC988AF44BB"
//! StartAutoScroll <Guid("B191FCFF-D841-4093-9FFC-1AB19641FFAC")>
#define START_AUTO_SCROLL "B191FCFF-D841-4093-9FFC-1AB19641FFAC"
//! StopAutoScroll <Guid("CCE46875-6001-459A-AE4F-0B5B85338BEF")>
#define STOP_AUTO_SCROLL "CCE46875-6001-459A-AE4F-0B5B85338BEF"

namespace JGW
{
    CCJGW_PrsVendorRoamingToolDlgController::CCJGW_PrsVendorRoamingToolDlgController(CCJGW_PrsVendorRoamingToolDlg* pView) : mpView(pView),mpPrsRoamingTestThread(NULL),mbIsStartUpdatePrsRoamingInfoTimer(false),mbIsStartUpdateIperfPingPrsInfoTimer(false)
    {
        msTestParams.meSocketTerimalType = (E_SERVICE_TYPE);
        msTestParams.meProtocolType = (E_TCP_TYPE);
        msTestParams.mnThreadCount = (4);
        msTestParams.mnTestIndex = (0);
        msTestParams.mbIsSaveIPQLog = true;
        //msTestParams.mbTestDelayTime = false;
        msTestParams.mbIsPrsVendorAppMsg = false;
        msTestParams.mbIsSendRoamingGraphMsg = false;
        msTestParams.mstrUserNameSSH = "root";
        msTestParams.mstrPasswordSSH = "20171015";
        msTestParams.mstrCheck11ADConnectCommand = "iw wlan0 station dump";
        msTestParams.mstr11ADConnectedFindString = "signal";
        msTestParams.mstrMcsConfig = "rx bitrate:";
        msTestParams.mstrSignalConfig = "signal:";
        msTestParams.mnSettingMaxListCount = 300;
        msTestParams.mnCheckSignalTimeIntervalMS = 400;
        mpAsyncWndMessage = mpView->GetAsyncWndMessage();
        JGW_FormatWString(mstrConfigFilePath,L"%s%s",JGW_GetApplicationFolder(),L"PrsVendorRoamingTool.ini");
    }


    CCJGW_PrsVendorRoamingToolDlgController::~CCJGW_PrsVendorRoamingToolDlgController(void)
    {
		if (NULL != mpPrsRoamingTestThread)
		{
			delete mpPrsRoamingTestThread;
			mpPrsRoamingTestThread = NULL;
		}
    }

	void CCJGW_PrsVendorRoamingToolDlgController::StartUpdatePrsRoamingInfoTimer()
	{
		if (!mbIsStartUpdatePrsRoamingInfoTimer)
		{
			SetTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME,UPDATE_PRS_ROAMING_INFO_UI_FREQ,NULL);
			mbIsStartUpdatePrsRoamingInfoTimer = true;
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::StopUpdatePrsRoamingInfoTimer()
	{
		if (mbIsStartUpdatePrsRoamingInfoTimer)
		{
			KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TIME);
			mbIsStartUpdatePrsRoamingInfoTimer = false;
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::StartUpdateIperfPingPrsInfoTimer()
	{
		if (!mbIsStartUpdateIperfPingPrsInfoTimer)
		{
			//SetTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TEST_TIME,UPDATE_IPERF_PING_PRS_INFO_UI_FREQ,NULL);
            SetTimer(mpView->GetHWND(),WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT,GRAPH_UPDATE_FREQ_TIMER_MSEC,NULL);
            
			mbIsStartUpdateIperfPingPrsInfoTimer = true;
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::StopUpdateIperfPingPrsInfoTimer()
	{
		if (mbIsStartUpdateIperfPingPrsInfoTimer)
		{
			//KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_TEST_TIME);
            KillTimer(mpView->GetHWND(),WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT);
			mbIsStartUpdateIperfPingPrsInfoTimer = false;
		}
	}

	std::wstring GetPrsRomingConnectStatusStr(E_PRS_ROAMING_CONNECT_STATUS ePrsRomingConnectStatus)
	{
		switch (ePrsRomingConnectStatus)
		{
        case E_UNKNOW_PRS_ROAMING_CONNECT:
            return L"Unknown";
		case E_DISCONNECTED_STATUS:
			return L"Disconnected";
		case E_CONNECTED_STATUS:
			return L"Connected";
		case E_CONNECTED_STATUS_2:
			return L"Connected";
		case E_SCAN_COMMAND:
			return L"Scan";
		case E_SCAN_COMPLETE:
			return L"scan_complete";
		case E_JOIN_COMMAND:
			return L"Join";
		default:
			return L"Disconnected";
		}
	}

    bool CCJGW_PrsVendorRoamingToolDlgController::OnInit()
    {
		//! 加载PrsVendorRoamingTool.ini
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
			msTestParams.mstrHtmlFile = configIni.GetIniKeyValue(L"MAIN",L"WKE_HTML",L"html\\prs_connect_status.html");
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
			msTestParams.mnSettingMaxListCount = configIni.GetIniKeyIntValue(L"MAIN",L"SettingMaxListCount",100);
			msTestParams.mstrLogPrefix = configIni.GetIniKeyValue(L"MAIN",L"LOG_PREFIX");
			msTestParams.mstrIperfRelativePath = configIni.GetIniKeyValue(L"MAIN",L"IperfRelativePath",L"tool\\iperf3.exe");
			msTestParams.mbIsSaveIPQLog = 1 == configIni.GetIniKeyIntValue(L"MAIN",L"SaveIPQLogCheckBox",1) ? true : false;
            msTestParams.mbIsSendRoamingGraphMsg = configIni.GetIniKeyBoolValue(L"MAIN",L"IsSendRoamingGraphMsg",false);

            std::vector<int> vInitTemp;
            std::wstring strTemp = configIni.GetIniKeyValue(L"MAIN",L"SSH_INIT");
            JGW_ParserIntW(strTemp.c_str(),L",",vInitTemp);
            for (std::vector<int>::iterator it = vInitTemp.begin();it != vInitTemp.end();++it)
            {
                S_SSH_COMMAND sSSHCommand;
                JGW_FormatWString(strTemp,L"%d",it[0]);
                sSSHCommand.mbIsAndCommandOk = true;
                sSSHCommand.mnErrorRetry = configIni.GetIniKeyIntValue(strTemp.c_str(),L"COMMAND_RETRY",3);
                sSSHCommand.mstrCommand = configIni.GetIniKeyValueA(strTemp.c_str(),L"COMMAND");
                sSSHCommand.mstrCommandOk = configIni.GetIniKeyValueA(strTemp.c_str(),L"COMMAND_OK");
                if (std::wstring::npos != sSSHCommand.mstrCommandOk.find("||||"))
                {
                    sSSHCommand.mbIsAndCommandOk = false;
                    JGW_ParserStrA(sSSHCommand.mstrCommandOk.c_str(),"||||",sSSHCommand.mvstrCommandOk);
                }
                else
                {
                    JGW_ParserStrA(sSSHCommand.mstrCommandOk.c_str(),"&&&&",sSSHCommand.mvstrCommandOk);
                }

                for (std::vector<std::string>::iterator it = sSSHCommand.mvstrCommandOk.begin();it != sSSHCommand.mvstrCommandOk.end();++it)
                {
                    JGW_EraseLastAndFristTwoChars(it[0]);
                    JGW_EraseLastAndFristTwoChars(it[0],' ');
                }

                msTestParams.mvInitSSHCommand.push_back(sSSHCommand);
            }

            std::vector<int> vExitTemp;
            strTemp = configIni.GetIniKeyValue(L"MAIN",L"SSH_EXIT");
            JGW_ParserIntW(strTemp.c_str(),L",",vExitTemp);
            for (std::vector<int>::iterator it = vExitTemp.begin();it != vExitTemp.end();++it)
            {
                S_SSH_COMMAND sSSHCommand;
                JGW_FormatWString(strTemp,L"%d",it[0]);
                sSSHCommand.mbIsAndCommandOk = true;
                sSSHCommand.mnErrorRetry = configIni.GetIniKeyIntValue(strTemp.c_str(),L"COMMAND_RETRY",3);
                sSSHCommand.mstrCommand = configIni.GetIniKeyValueA(strTemp.c_str(),L"COMMAND");
                sSSHCommand.mstrCommandOk = configIni.GetIniKeyValueA(strTemp.c_str(),L"COMMAND_OK");
                if (std::wstring::npos != sSSHCommand.mstrCommandOk.find("||||"))
                {
                    sSSHCommand.mbIsAndCommandOk = false;
                    JGW_ParserStrA(sSSHCommand.mstrCommandOk.c_str(),"||||",sSSHCommand.mvstrCommandOk);
                }
                else
                {
                    JGW_ParserStrA(sSSHCommand.mstrCommandOk.c_str(),"&&&&",sSSHCommand.mvstrCommandOk);
                }
                for (std::vector<std::string>::iterator it = sSSHCommand.mvstrCommandOk.begin();it != sSSHCommand.mvstrCommandOk.end();++it)
                {
                    JGW_EraseLastAndFristTwoChars(it[0]);
                    JGW_EraseLastAndFristTwoChars(it[0],' ');
                }
                msTestParams.mvExitSSHCommand.push_back(sSSHCommand);
            }
		}
		//! 导入配置到UI视图中
		{
			mpView->mpTerminalType->SelectItem(msTestParams.meSocketTerimalType);
			mpView->mpProtocolType->SelectItem(msTestParams.meProtocolType);
			mpView->mpIperfIPEdt->SetText(msTestParams.mstrIperfIPAddr.c_str());
			mpView->mpSshIpAddressEdt->SetText(msTestParams.mstrSSHIPAddr.c_str());
			mpView->mpSaveIPQLogCheckBox->SetCheck(msTestParams.mbIsSaveIPQLog);
			mpView->mpTestTimeEdt->SetText(JGW_GetFormatWString(L"%d",msTestParams.mnTestTimes).c_str());
			mpView->mpThreadCountEdt->SetText(JGW_GetFormatWString(L"%d",msTestParams.mnThreadCount).c_str());
			mpView->mpExtraParameterEdt->SetText(msTestParams.mstrExtraParameter.c_str());
			mpView->mpLogPrefixEdt->SetText(msTestParams.mstrLogPrefix.c_str());

			mpView->mpStaMacCombo->SetEnabled(false);
		}
        OnClickUpdateConfigOptionOption();

        return true;
    }

    void CCJGW_PrsVendorRoamingToolDlgController::SaveConfig()
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

    void CCJGW_PrsVendorRoamingToolDlgController::OnCloseWindow()
    {
		//! 停止更新UI视图
		StopUpdatePrsRoamingInfoTimer();
		//!
		StopUpdateIperfPingPrsInfoTimer();
		//! 点击停止按钮
        OnClickStopBtn();
    }

    bool CCJGW_PrsVendorRoamingToolDlgController::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        switch (pTEventUI->Type)
        {
        case WM_HWND_RICHEDIT_APPEND_TEST_MSG:
			//! 测试消息日志更新
            OnAppendRichEditTestMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_PRS_VENDOR_DATA_HANDLE_MSG:
            //! 处理prs_vendor_info_log msg CJGW_PrsVendorRoamingDataHandle.cpp
            OnHandlePrsVendorRoamingMsg((int)pTEventUI->wParam,(const wchar_t*)(pTEventUI->lParam));
            break;
        case WM_TIMER:
            if (pTEventUI->wParam == WM_TIMER_UPDATE_TIME)
                OnUpdatePrsRoamingInfoTimer();
            else if (pTEventUI->wParam == WM_TIMER_UPDATE_DOWNLOAD_PROGRESS_EVENT) 
                OnUpdateWkeHtmlAllYSeriesDataTimer();
            break;
        case WM_TEST_PASS_MSG:
			//! 测试完成消息
            OnTestCompletedMsg();
            break;
        case WM_THROUGHPUT_RATE_MSG:
			//! 接收到吞吐率消息
            OnThroughputRateMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_TIME_DELAY_MSG:
			//! PING时延
            OnTimeDelayMsg((const wchar_t*)(pTEventUI->wParam));
            break;
        case WM_11AD_CONNECT_STATUS_MSG:
            OnAppendRichEditTestMsg((const wchar_t*)(pTEventUI->wParam));
#if 0
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
#endif
            break;
        }

        return true;
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnViewNotify(TNotifyUI& msg)
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
            else if (msg.pSender == mpView->mpLogFolderBtn)
            {
                if (!msTestParams.mstrLogFolder.empty())
                {
                    ShellExecute(NULL,_T("open"),msTestParams.mstrLogFolder.c_str(),_T(""),_T(""),SW_SHOWNORMAL);
                }
            }
            else if (msg.pSender == mpView->mpLogDebugBtn)
            {
                std::wstring strDebugLogPath;

                mpView->mpLogDebugFilePathEdt->SetText(L"");
                //! 
                if( !JGW_ChooseFile(strDebugLogPath,L"roaming*.log;*.log\0roaming*.log;*.log\0all file(*.*)\0*.*\0\0",mpView->GetHWND() ) )  return ;

                mpView->mpLogDebugFilePathEdt->SetText(strDebugLogPath.c_str());
            }
        }
        else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            if (msg.pSender == mpView->mpUpdateConfigOption)
            {
                OnClickUpdateConfigOptionOption();
                mpView->mpUpdateConfigOption->Selected(false);
            }
            else if (msg.pSender == mpView->mpLogDebugTestCheckBox)
            {
                if (mpView->mpLogDebugTestCheckBox->GetCheck())
                {
                    mpView->mpLogDebugTestHorizontalLayout->SetVisible(true);
                }
                else
                {
                    mpView->mpLogDebugFilePathEdt->SetText(L"");
                    mpView->mpLogDebugTestHorizontalLayout->SetVisible(false);
                }
            }
        }
		else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if (msg.pSender == mpView->mpApMacCombo)
			{
				//! 清空当前AP视图
				mpView->mpApPrsRoamingInfoRichEdit->Clear();
				for (std::map<std::wstring,S_AP_UI_INFO>::iterator it = mmapApMacPrsRoamingInfo.begin();it != mmapApMacPrsRoamingInfo.end();++it)
				{
					it->second.mnPrsRoamingInfoIndex = 0;
				}
			}
			else if (msg.pSender == mpView->mpStaMacCombo)
			{

			}
		}
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnClickUpdateConfigOptionOption()
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

    bool CCJGW_PrsVendorRoamingToolDlgController::CheckIPAddressValid(const std::wstring& strIPAddress)
    {
        std::wstring strRegex = L"^((25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)\\.){3}(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]?\\d)$";
        std::wregex partten(strRegex,std::wregex::icase);;
        return regex_match (strIPAddress,partten);
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnClickStartBtn()
    {
        OnClickUpdateConfigOptionOption();
		//! 开始测试的时候禁止关闭按钮点击
		mpView->mpCloseBtn->SetEnabled(false);
		//! prs roaming log的文件路径有效性
        if (mpView->mpLogDebugTestCheckBox->GetCheck())
        {
            msTestParams.mstrLogDebugFilePath = mpView->mpLogDebugFilePathEdt->GetText().GetData();
            if (!JGW_FileExistsToFilePath(msTestParams.mstrLogDebugFilePath.c_str()))
            {
                MessageBox(mpView->GetHWND(),JGW_GetFormatWString(L"Log Debug File(%s) does not exist",msTestParams.mstrLogDebugFilePath.c_str()).c_str(),L"Error",MB_ICONERROR);
                return;
            }
        }
        else
        {
            msTestParams.mstrLogDebugFilePath = L"";
        }

        //! 日志文件目录
        msTestParams.mnTimeStamp = JGW_GetTimeOfDay();
        int index = 0;
        do 
        {
            JGW_FormatWString(msTestParams.mstrLogFolder,L"%slog\\PrsVendorRoamingTool\\%s\\%s_%d",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetCurrentTimeStampHMS(_T("h%02d_m%02d_s%02d")).c_str(),index++);
        } while (JGW_IsFolderExist(msTestParams.mstrLogFolder));
        //! 检查日志目录是否存在，不存在则重新创建
        if (!JGW_IsFolderExist(msTestParams.mstrLogFolder))
        {
            JGW_CreateDirectory(msTestParams.mstrLogFolder.c_str(),true);
        }
        //! 测试记录
        JGW_FormatWString(mstrIperfPingPrsInfoTestLogFilePath,L"%s\\%s_%u.csv",msTestParams.mstrLogFolder.c_str(),msTestParams.mstrLogPrefix.c_str(),msTestParams.mnTimeStamp);
        //! time,throughput rate(Mbits/sec),time delay(ms),sta1,sta2
        mIperfPingPrsInfoTestLogStream.imbue(std::locale(""));
        mIperfPingPrsInfoTestLogStream.open(mstrIperfPingPrsInfoTestLogFilePath.c_str(),std::ios_base::app);
        if (!mIperfPingPrsInfoTestLogStream.is_open())
        {
            MessageBox(mpView->GetHWND(),JGW_GetFormatWString(L"Error : Create IperfPingPrsInfoTestLog File(%s) Failed",mstrIperfPingPrsInfoTestLogFilePath.c_str()).c_str(),L"Error",MB_ICONERROR);
            return;
        }
        else
        {
            mIperfPingPrsInfoTestLogStream << L"time,throughput rate(Mbits/sec),time delay(ms),sta1,sta2,IsPrsIperfUpdate,IsPrsConnectStatusUpdate" << std::endl;
        }
        //! RichEditTestMsgLogFilePath
        JGW_FormatWString(mstrRichEditTestMsgLogFilePath,L"%s\\%s_richedit_%u.log",msTestParams.mstrLogFolder.c_str(),msTestParams.mstrLogPrefix.c_str(),msTestParams.mnTimeStamp);
        mRichEditTestMsgLogStream.imbue(std::locale(""));
        mRichEditTestMsgLogStream.open(mstrRichEditTestMsgLogFilePath.c_str(),std::ios_base::app);
        if (!mRichEditTestMsgLogStream.is_open())
        {
            MessageBox(mpView->GetHWND(),JGW_GetFormatWString(L"Error : Create RichEditTestMsgLog File(%s) Failed",mstrRichEditTestMsgLogFilePath.c_str()).c_str(),L"Error",MB_ICONERROR);
            return;
        }
        //! WKE HTML LOG
        JGW_FormatWString(mstrWkeHtmlTestLogFilePath,L"%s\\%s_%u_html.csv",msTestParams.mstrLogFolder.c_str(),msTestParams.mstrLogPrefix.c_str(),msTestParams.mnTimeStamp);
        mWkeHtmlTestLogFilePathStream.imbue(std::locale(""));
        mWkeHtmlTestLogFilePathStream.open(mstrWkeHtmlTestLogFilePath.c_str(),std::ios_base::app);
        if (!mWkeHtmlTestLogFilePathStream.is_open())
        {
            MessageBox(mpView->GetHWND(),JGW_GetFormatWString(L"Error : Create WkeHtmlTestLog File(%s) Failed",mstrWkeHtmlTestLogFilePath.c_str()).c_str(),L"Error",MB_ICONERROR);
            return;
        }
        else
        {
            mWkeHtmlTestLogFilePathStream << L"Time,Time Stamp,Status,Status ID,STA MAC,SSID,AP MAC,Rssi,Snr,Distance,Tx Mcs,Rx Mcs,Tx Sector,Rx Sector,TxThroughputMbps,RxThroughputMbps,Associated Time" << std::endl;
        }


		//! 清空AP
		mpView->mpApMacCombo->RemoveAll();
		mmapApMacPrsRoamingInfo.clear();
		mpView->mpApPrsRoamingInfoRichEdit->Clear();
		mpView->mpApPrsRoamingInfoRichEdit->SetText(L"");
		//! 清空STA
		mpView->mpStaMacCombo->RemoveAll();
		mmapStaMacPrsRoamingInfo.clear();
		mpView->mpConnectApMacEdit->SetText(L"");
		mpView->mpStaPrsRoamingInfoRichEdit->Clear();
		mpView->mpStaPrsRoamingInfoRichEdit->SetText(L"");
		while (mpView->mpStaPrsRoamingInfoVerticalLayout->GetCount() > 1) mpView->mpStaPrsRoamingInfoVerticalLayout->RemoveAt(1);
		//! 界面选择框取消显示
        mpView->mpClientConfigHorizontalLayout->SetEnabled(false);
        mpView->mpStartBtn->SetEnabled(false);
        mpView->mpStopBtn->SetEnabled(true);
        mpView->mpLogEdt->SetText(L"");

        msTestParams.mnTestIndex = 1;
        msLogInfo.muUnixTestLogTimeStamp = 0;
        //msLogInfo.mstrTime = L"";
        msLogInfo.mstrThroughputRate = L"";
        msLogInfo.mstrPingTimeDelay = L"";

#if 1
        mpView->mpWkeBrowser->ReloadLocation();
        mpView->mpWkeBrowser->SettingMaxListCount(msTestParams.mnSettingMaxListCount);
        mpView->mpWkeBrowser->StartAutoScroll();

#endif
		//! 开始测试线程
        if (NULL == mpPrsRoamingTestThread)
        {
            mpPrsRoamingTestThread = new CCJGW_PrsRoamingTestThread(&msTestParams,mpAsyncWndMessage);
            mpPrsRoamingTestThread->CreateMessageThread(0,0,true);
        }
        mpPrsRoamingTestThread->SendCEF3DataUDPMessage(RESET_COMMAND);
        std::string strSettingMaxListCount;
        JGW_FormatString(strSettingMaxListCount,"%s %d",SETTING_MAX_LIST_COUNT,msTestParams.mnSettingMaxListCount);
        mpPrsRoamingTestThread->SendCEF3DataUDPMessage(strSettingMaxListCount);
        mpPrsRoamingTestThread->SendCEF3DataUDPMessage(START_AUTO_SCROLL);
		//! 开启UI更新定时器
		StartUpdatePrsRoamingInfoTimer();
		//!
		StartUpdateIperfPingPrsInfoTimer();
        mpPrsRoamingTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN);
        Log4WI(L"Start Throughput Test...");
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnClickStopBtn()
    {
        OnAppendRichEditTestMsg(L"Click Stop Btn");
        mpView->mpStopBtn->SetEnabled(false);
        if (NULL != mpPrsRoamingTestThread)
        {
            mpPrsRoamingTestThread->StopThroughputTestThread();
        }
        //Log4WI(L"Stop Throughput Test...");
    }

    float gfTestSpeed = 0.00f;
    void CCJGW_PrsVendorRoamingToolDlgController::OnAppendRichEditTestMsg(const wchar_t* strMsg)
    {
        if (NULL == strMsg) return;
        static int line = 0;
        if (line++ > 3000) {mpView->mpLogEdt->Clear();line = 0;}
        WriteRichEditTestLog(strMsg);
        mpView->mpLogEdt->AppendText(strMsg);
        mpView->mpLogEdt->AppendText(L"\r\n");
        mpView->mpLogEdt->EndDown();
    }

    void CCJGW_PrsVendorRoamingToolDlgController::WriteIperfPingPrsInfoTestLogToOfStream(bool bIsPrsIperfUpdate /* = false */,bool bIsPrsConnectStatusUpdate /* = false */)
    {
        if (mIperfPingPrsInfoTestLogStream.is_open())
        {
            //! 获取IPERF PING PRS INFO
            std::wstring strIperfPingPrsInfo;
            JGW_FormatWString(strIperfPingPrsInfo,L"%s,%s,%s",CTimeUtils::GetCurrentTimeString(false).c_str(),msLogInfo.mstrThroughputRate.c_str(),msLogInfo.mstrPingTimeDelay.c_str());
            mIperfPingPrsInfoTestLogStream << strIperfPingPrsInfo;
            for (std::map<std::wstring,S_STA_UI_INFO>::iterator it = mmapStaMacPrsRoamingInfo.begin();it != mmapStaMacPrsRoamingInfo.end();++it)
            {
                if (E_UNKNOW_PRS_ROAMING_CONNECT == it->second.msStaApTestInfo.GetPrsRomingConnectStatus()) continue;
                mIperfPingPrsInfoTestLogStream << JGW_GetFormatWString(L",\"%s,%I64d,%s,%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\""
                    ,JGW_GetTimeStringToTimestampW(it->second.msStaApTestInfo.mdw64TimeStampMSec).c_str()
                    ,it->second.msStaApTestInfo.mdw64TimeStampMSec
                    ,GetPrsRomingConnectStatusStr(it->second.msStaApTestInfo.GetPrsRomingConnectStatus()).c_str()
                    ,(int)it->second.msStaApTestInfo.GetPrsRomingConnectStatus()
                    ,it->second.msStaApTestInfo.mstrStaMac.c_str()
                    ,it->second.msStaApTestInfo.mstrSsid.c_str()
                    ,it->second.msStaApTestInfo.mstrApMac.c_str()
                    ,it->second.msStaApTestInfo.mnRssi
                    ,it->second.msStaApTestInfo.mnSnr
                    ,it->second.msStaApTestInfo.mnDistance
                    ,it->second.msStaApTestInfo.mnTxMcs
                    ,it->second.msStaApTestInfo.mnRxMcs
                    ,it->second.msStaApTestInfo.mnTxBeamSector
                    ,it->second.msStaApTestInfo.mnRxBeamSector
                    ,it->second.msStaApTestInfo.mnTxThroughputMbps
                    ,it->second.msStaApTestInfo.mnRxThroughputMbps);
            }
            mIperfPingPrsInfoTestLogStream << L"," << bIsPrsIperfUpdate ? L"1" : L"0";
            mIperfPingPrsInfoTestLogStream << L"," << bIsPrsConnectStatusUpdate ? L"1" : L"0";
            mIperfPingPrsInfoTestLogStream << std::endl;
        }
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnChangePrsRomingConnectStatus(LPVOID pContext,S_STA_AP_TestInfo* psStaApTestInfo)
    {
        CCJGW_PrsVendorRoamingToolDlgController* pPrsVendorRoamingToolDlgController = (CCJGW_PrsVendorRoamingToolDlgController*)pContext;
        if (NULL != pPrsVendorRoamingToolDlgController)
        {
            pPrsVendorRoamingToolDlgController->ChangePrsRomingConnectStatus(psStaApTestInfo);
        }
    }

    void CCJGW_PrsVendorRoamingToolDlgController::ChangePrsRomingConnectStatus(S_STA_AP_TestInfo* psStaApTestInfo)
    {
        WriteIperfPingPrsInfoTestLogToOfStream(false,true);
        UpdateWkeHtmlYSeriesData(*psStaApTestInfo);
        //OnUpdateWkeHtmlAllYSeriesDataTimer();
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnUpdatePrsRoamingInfoTimer()
    {
		//! 更新AP 视图
		std::wstring strSelectApMac = mpView->mpApMacCombo->GetText().GetData();
		std::map<std::wstring,S_AP_UI_INFO>::iterator it = mmapApMacPrsRoamingInfo.find(strSelectApMac);
		if (it != mmapApMacPrsRoamingInfo.end())
		{
			for (size_t nIndex = 0;it->second.mnPrsRoamingInfoIndex < it->second.mvstrPrsRoamingInfo.size() && nIndex < MAX_APPEND_RICHEDIT_MSG;nIndex++,it->second.mnPrsRoamingInfoIndex++)
			{
				AppendRichEditTestMsg(mpView->mpApPrsRoamingInfoRichEdit,it->second.mvstrPrsRoamingInfo[it->second.mnPrsRoamingInfoIndex].c_str());
			}
		}
		//! 更新STA 视图
		for (std::map<std::wstring,S_STA_UI_INFO>::iterator itStaUiInfo = mmapStaMacPrsRoamingInfo.begin();itStaUiInfo != mmapStaMacPrsRoamingInfo.end();++itStaUiInfo)
		{
			for (size_t nIndex = 0;itStaUiInfo->second.mnPrsRoamingInfoIndex < itStaUiInfo->second.mvstrPrsRoamingInfo.size() && nIndex < MAX_APPEND_RICHEDIT_MSG;nIndex++,itStaUiInfo->second.mnPrsRoamingInfoIndex++)
			{
				AppendRichEditTestMsg(itStaUiInfo->second.mpStaPrsRoamingInfoRichEdit,itStaUiInfo->second.mvstrPrsRoamingInfo[itStaUiInfo->second.mnPrsRoamingInfoIndex].c_str());
			}
		}
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnTestCompletedMsg()
    {
        mpView->mpStartBtn->SetEnabled(true);
        mpView->mpStopBtn->SetEnabled(false);
        mpView->mpClientConfigHorizontalLayout->SetEnabled(true);
        StopUpdatePrsRoamingInfoTimer();
		StopUpdateIperfPingPrsInfoTimer();
        OnAppendRichEditTestMsg(mstrIperfPingPrsInfoTestLogFilePath.c_str());
        OnAppendRichEditTestMsg(L"Test is completed.");
		mpView->mpCloseBtn->SetEnabled(true);
        mIperfPingPrsInfoTestLogStream.flush();
        mIperfPingPrsInfoTestLogStream.close();
        mRichEditTestMsgLogStream.flush();
        mRichEditTestMsgLogStream.close();
        mWkeHtmlTestLogFilePathStream.flush();
        mWkeHtmlTestLogFilePathStream.close();
        mpView->mpWkeBrowser->StopAutoScroll();
        mpPrsRoamingTestThread->SendCEF3DataUDPMessage(STOP_AUTO_SCROLL);
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnThroughputRateMsg(const wchar_t* strMsg)
    {
        if (NULL != strMsg)
        {
            msLogInfo.mstrThroughputRate = strMsg;
            msLogInfo.muUnixTestLogTimeStamp = JGW_GetTimeOfDay();
            WriteIperfPingPrsInfoTestLogToOfStream(true);
        }
        else
        {
            msLogInfo.mstrThroughputRate = L"";
        }
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnTimeDelayMsg(const wchar_t* strMsg)
    {
        if (NULL != strMsg)
        {
            msLogInfo.mstrPingTimeDelay = strMsg;
            WriteIperfPingPrsInfoTestLogToOfStream();
        }
    }

	void CCJGW_PrsVendorRoamingToolDlgController::WritePrsRoamingLog(const std::wstring& strPrsRoamingLogPath,const std::wstring& strPrsRoamingLog)
	{
		std::wofstream oFileStream;
		oFileStream.imbue(std::locale(""));
		oFileStream.open(strPrsRoamingLogPath.c_str(),std::ios_base::app);
		if (oFileStream.is_open())
		{
			oFileStream << strPrsRoamingLog << std::endl;
			oFileStream.flush();
			oFileStream.close();
		}
	}

    void CCJGW_PrsVendorRoamingToolDlgController::OnUpdateWkeHtmlAllYSeriesDataTimer()
    {
        for (std::map<std::wstring,S_STA_UI_INFO>::iterator it = mmapStaMacPrsRoamingInfo.begin();it != mmapStaMacPrsRoamingInfo.end();++it)
        {
            UpdateWkeHtmlYSeriesData(it->second.msStaApTestInfo);
        }
    }

	void CCJGW_PrsVendorRoamingToolDlgController::UpdateWkeHtmlYSeriesData(S_STA_AP_TestInfo& sStaApTestInfo)
	{
        if (E_UNKNOW_PRS_ROAMING_CONNECT == sStaApTestInfo.mePrivarePrsRomingConnectStatus) return;
		std::wstring strTemp = JGW_GetFormatWString(L"%s,%I64d,%s,%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",JGW_GetTimeStringToTimestampW(sStaApTestInfo.mdw64TimeStampMSec).c_str(),sStaApTestInfo.mdw64TimeStampMSec,GetPrsRomingConnectStatusStr(sStaApTestInfo.GetPrsRomingConnectStatus()).c_str(),(int)sStaApTestInfo.GetPrsRomingConnectStatus(),sStaApTestInfo.mstrStaMac.c_str(),sStaApTestInfo.mstrSsid.c_str(),sStaApTestInfo.mstrApMac.c_str(),sStaApTestInfo.mnRssi,sStaApTestInfo.mnSnr,sStaApTestInfo.mnDistance,sStaApTestInfo.mnTxMcs,sStaApTestInfo.mnRxMcs,sStaApTestInfo.mnTxBeamSector,sStaApTestInfo.mnRxBeamSector,sStaApTestInfo.mnTxThroughputMbps,sStaApTestInfo.mnRxThroughputMbps,sStaApTestInfo.mstrAssociatedTimeSec.c_str());
        //sStaApTestInfo.mstrAssociatedTimeSec = L"";
        if (0 != sStaApTestInfo.mstrPrevUpdateWkeHtmlData.compare(strTemp))
        {
            if (mWkeHtmlTestLogFilePathStream.is_open())
            {
                mWkeHtmlTestLogFilePathStream << strTemp << std::endl;
            }
            mpView->mpWkeBrowser->AddYSeriesData(sStaApTestInfo.mstrStaMac,sStaApTestInfo.mdw64TimeStampMSec,(int)sStaApTestInfo.GetPrsRomingConnectStatus(),strTemp);
            mpPrsRoamingTestThread->SendCEF3DataUDPMessage(JGW_W2A(strTemp));
            sStaApTestInfo.mstrPrevUpdateWkeHtmlData = strTemp;
        }  
	}

	void CCJGW_PrsVendorRoamingToolDlgController::WritePrsRoamingLog(S_STA_AP_TestInfo& sStaApTestInfo)
	{
         if (E_UNKNOW_PRS_ROAMING_CONNECT == sStaApTestInfo.GetPrsRomingConnectStatus()) return;
		std::wofstream oFileStream;
		oFileStream.imbue(std::locale(""));
		oFileStream.open(sStaApTestInfo.mstrStaMacFilePath.c_str(),std::ios_base::app);
		if (oFileStream.is_open())
		{
			//! Time,Time Stamp,status,Status ID,STA MAC,SSID,AP MAC,Rssi,Snr,Distance,Tx Mcs,Rx Mcs,Tx Sector,Rx Sector,TxThroughputMbps,RxThroughputMbps
			std::wstring strTemp = JGW_GetFormatWString(L"%s,%I64d,%s,%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s",JGW_GetTimeStringToTimestampW(sStaApTestInfo.mdw64TimeStampMSec).c_str(),sStaApTestInfo.mdw64TimeStampMSec,GetPrsRomingConnectStatusStr(sStaApTestInfo.GetPrsRomingConnectStatus()).c_str(),(int)sStaApTestInfo.GetPrsRomingConnectStatus(),sStaApTestInfo.mstrStaMac.c_str(),sStaApTestInfo.mstrSsid.c_str(),sStaApTestInfo.mstrApMac.c_str(),sStaApTestInfo.mnRssi,sStaApTestInfo.mnSnr,sStaApTestInfo.mnDistance,sStaApTestInfo.mnTxMcs,sStaApTestInfo.mnRxMcs,sStaApTestInfo.mnTxBeamSector,sStaApTestInfo.mnRxBeamSector,sStaApTestInfo.mnTxThroughputMbps,sStaApTestInfo.mnRxThroughputMbps,sStaApTestInfo.mstrAssociatedTimeSec.c_str());
            sStaApTestInfo.mstrAssociatedTimeSec = L"";
			oFileStream << strTemp << std::endl;
			oFileStream.flush();
			oFileStream.close();
		}
	}

    void CCJGW_PrsVendorRoamingToolDlgController::WriteRichEditTestLog(const std::wstring& strTestLog)
    {
        if (mRichEditTestMsgLogStream.is_open())
        {
            mRichEditTestMsgLogStream << strTestLog << std::endl;
        }
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnHandlePrsVendorRoamingMsg(int nServiceID,const wchar_t* strMsg)
    {
        switch (nServiceID)
        {
        case BEAM_FORMING_SERVICE_WPARAM:
            //! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
            OnBeamfromingService(strMsg);
            break;
        case LINK_STATUS_SERVICE_WPARAM:
            //! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
            OnLinkStatusService(strMsg);
            break;
        case NETWORK_DISCOVERY_REQUEST_SERVICE_WPARAM:
            //! Time,STA MAC ===== SCAN
            OnScanNetworkDiscoveryRequestService(strMsg);
            break;
        case NETWORK_DISCOVERY_RESULTS_SERVICE_WPARAM:
            //! Time ,STA MAC,AP MAC LIST,RSSI LIST ==== scan complete
            OnScanCompleteNetworkDiscoveryResultsService(strMsg);
            break;
        case NETWORK_DISCOVERY_COMPLETE_SERVICE_WPARAM:
            //! Time,STA MAC ==== scan complete
            OnScanCompleteNetworkDiscoveryCompleteService(strMsg);
            break;
        case NETWORK_CONNECT_REQUEST_SERVICE_WPARAM:
            //! Time,STA MAC,AP MAC,SSID ==== Join
            OnJoinNetworkConnectRequestService(strMsg);
            break;
        case SCAN_SUMMARY_SERVICE_WPARAM:
            //! Time,AP MAC,RSSI,SNR,DISTANCE
            onScanSummaryService(strMsg);
            break;
        case PERF_STATS_SERVICE_WPARAM:
            //! Time,STA MAC,is Tx,ThroughputMbps
            OnPerfStatsService(strMsg);
            break;
        }
    }

	S_AP_UI_INFO* CCJGW_PrsVendorRoamingToolDlgController::AppendApMacInfo(__int64 nTimeStampMSec,const std::wstring& strApMac,const wchar_t* strFormatInfo /* = NULL */)
	{
		std::map<std::wstring,S_AP_UI_INFO>::iterator it = mmapApMacPrsRoamingInfo.find(strApMac);
		//! 查找当前AP Mac是否已记录
		if (it != mmapApMacPrsRoamingInfo.end())
		{
			//! 已记录，则更新时间戳以及加入PRA ROAMING 信息
			it->second.mnLastUpdateTimeStampMSec = nTimeStampMSec;
			if (NULL != strFormatInfo) it->second.mvstrPrsRoamingInfo.push_back(strFormatInfo);
			return &it->second;
		}

		//! 添加当前AP记录
		mmapApMacPrsRoamingInfo[strApMac].mnLastUpdateTimeStampMSec = nTimeStampMSec;
		it = mmapApMacPrsRoamingInfo.find(strApMac);
		//! 初始化当前结构体
		it->second.mnPrsRoamingInfoIndex = 0;
		if (NULL != strFormatInfo) it->second.mvstrPrsRoamingInfo.push_back(strFormatInfo);

		//! 添加下拉列表框选项
		CListLabelElementUI* pListLabelEUi = new CListLabelElementUI();
		pListLabelEUi->SetText(strApMac.c_str());
		pListLabelEUi->SetDate(&it->second.mvstrPrsRoamingInfo);
		mpView->mpApMacCombo->Add(pListLabelEUi);
		//! 默认选择第一项
		if (mpView->mpApMacCombo->GetCount() > 0 && -1 == mpView->mpApMacCombo->GetCurSel())
		{
			mpView->mpApMacCombo->SelectItem(0);
		}

		return &it->second;
	}

	S_STA_UI_INFO* CCJGW_PrsVendorRoamingToolDlgController::AppendStaMacInfo(__int64 nTimeStampMSec,const std::wstring& strStaMac,const wchar_t* strFormatInfo /* = NULL */)
	{
		std::map<std::wstring,S_STA_UI_INFO>::iterator it = mmapStaMacPrsRoamingInfo.find(strStaMac);
		//! 查找当前STA Mac是否已记录
		if (it != mmapStaMacPrsRoamingInfo.end())
		{
			it->second.mnLastUpdateTimeStampMSec = nTimeStampMSec;
			if (NULL != strFormatInfo) it->second.mvstrPrsRoamingInfo.push_back(strFormatInfo);
			return &it->second;
		}
		//! 添加当前AP记录
		mmapStaMacPrsRoamingInfo[strStaMac].mnLastUpdateTimeStampMSec = nTimeStampMSec;
		it = mmapStaMacPrsRoamingInfo.find(strStaMac);
		//! 初始化当前结构体
		it->second.mnPrsRoamingInfoIndex = 0;
		if (NULL != strFormatInfo) it->second.mvstrPrsRoamingInfo.push_back(strFormatInfo);
        it->second.msStaApTestInfo.mpContext = this;
        it->second.msStaApTestInfo.mePrivarePrsRomingConnectStatus = E_UNKNOW_PRS_ROAMING_CONNECT;
        it->second.msStaApTestInfo.mpChangePrsRomingConnectStatus = OnChangePrsRomingConnectStatus;
		it->second.msStaApTestInfo.mdw64TimeStampMSec = nTimeStampMSec;
		it->second.msStaApTestInfo.mstrStaMac = strStaMac;
		it->second.msStaApTestInfo.mstrSsid = L"";
		it->second.msStaApTestInfo.mstrApMac = L"";
		it->second.msStaApTestInfo.mnRssi = MIN_RSSI;
		it->second.msStaApTestInfo.mnSnr = INVAILD_SNR;
		it->second.msStaApTestInfo.mnDistance = 0;
		it->second.msStaApTestInfo.mnTxMcs = BAD_RX_MCS;
		it->second.msStaApTestInfo.mnRxMcs = BAD_RX_MCS;
		it->second.msStaApTestInfo.mnTxBeamSector = -1;
		it->second.msStaApTestInfo.mnRxBeamSector = -1;
		it->second.msStaApTestInfo.mnRxThroughputMbps = 0;
		it->second.msStaApTestInfo.mnTxThroughputMbps = 0;
		it->second.mstrStaMac = strStaMac;

		std::wstring strTempStaMac(strStaMac);
		JGW_ReplaceStringW(strTempStaMac,L":",L"");
		//! 保存当STA数据文件路径
		JGW_FormatWString(it->second.msStaApTestInfo.mstrStaMacFilePath,L"%s\\%s.csv",msTestParams.mstrLogFolder.c_str(),strTempStaMac.c_str());
		//! 写入表头 Time	STA MAC	SSID AP MAC	Rssi	Snr	Distance	Tx Mcs	Rx Mcs	Tx Sector	Rx Sector	TxThroughputMbps	RxThroughputMbps
		WritePrsRoamingLog(it->second.msStaApTestInfo.mstrStaMacFilePath,L"Time,Time Stamp,Status,Status ID,STA MAC,SSID,AP MAC,Rssi,Snr,Distance,Tx Mcs,Rx Mcs,Tx Sector,Rx Sector,TxThroughputMbps,RxThroughputMbps,Associated Time");

		//! 添加下拉列表框选项
		CListLabelElementUI* pListLabelEUi = new CListLabelElementUI();
		pListLabelEUi->SetText(strStaMac.c_str());
		mpView->mpStaMacCombo->Add(pListLabelEUi);
		//! 默认选择第一项
		if (mpView->mpStaMacCombo->GetCount() > 0 && -1 == mpView->mpStaMacCombo->GetCurSel())
		{
			mpView->mpStaMacCombo->SelectItem(0);
		}

		//! 判断当前STA MAC是否是首个
		if (1 == mmapStaMacPrsRoamingInfo.size())
		{
			it->second.mpConnectApMacEdit = mpView->mpConnectApMacEdit;
			it->second.mpStaPrsRoamingInfoRichEdit = mpView->mpStaPrsRoamingInfoRichEdit;
			it->second.mpStaMacEdit = NULL;
			//mpView->mpWkeBrowser->ChangeLegendData(0,it->second.mstrStaMac);
            mpView->mpWkeBrowser->AddYSeries(it->second.mstrStaMac);
		}
		else
		{
			//! 添加STA记录栏
			CDialogBuilder builder;
			CHorizontalLayoutUI* pHorizontalLayoutUI = (CHorizontalLayoutUI*)(builder.Create(L"<Window>\
				<HorizontalLayout>\
					<VerticalLayout>\
						<HorizontalLayout visible=\"true\" height=\"35\" padding=\"5,0,5,0\">\
							<Control width=\"5\"/>\
							<Label text=\"STA Mac:\" width=\"80\" textcolor=\"#0000ff\"/>\
							<Edit text=\"\" padding=\"5,5,5,5\" tooltip=\"\" enabled=\"true\" height=\"25\" textpadding=\"4,3,4,3\" textcolor=\"#FF000000\" maxchar=\"17\" disabledtextcolor=\"#FFA7A6AA\" />\
							<Control width=\"5\"/>\
							<Label text=\"AP Mac:\" width=\"80\" textcolor=\"#0000ff\"/>\
							<Edit padding=\"5,5,5,5\" enabled=\"true\" height=\"25\" textpadding=\"4,3,4,3\" readonly=\"true\" textcolor=\"#FF000000\" maxchar=\"17\" disabledtextcolor=\"#FFA7A6AA\" />\
						</HorizontalLayout>\
					<HorizontalLayout visible=\"true\" height=\"65\" padding=\"5,0,5,0\">\
						<RichEdit enabled=\"true\"  bkcolor=\"#FFFFFFFF\" textpadding=\"4,3,4,3\" textcolor=\"#FF000000\" disabledtextcolor=\"#FFA7A6AA\"  maxchar=\"9000\"  align=\"left\" vscrollbar=\"true\" hscrollbar=\"false\" multiline=\"true\" autohscroll=\"false\" autovscroll=\"true\" readonly=\"false\"/>\
					</HorizontalLayout></VerticalLayout>\
				</HorizontalLayout>\
				</Window>",0,NULL,&mpView->m_PaintManager));
			mpView->mpStaPrsRoamingInfoVerticalLayout->Add(pHorizontalLayoutUI);
			//! 重新设置高度
			mpView->mpStaPrsRoamingInfoVerticalLayout->SetAttribute(L"height",JGW_GetFormatWString(L"%d",mpView->mpApPrsRoamingInfoRichEdit->GetHeight() + 100).c_str());
			//! UI 视图配置
			CVerticalLayoutUI* mpVerticalLayout1 = (CVerticalLayoutUI*)pHorizontalLayoutUI->GetItemAt(0);
			CHorizontalLayoutUI* mpHorizontalLayout2 = (CHorizontalLayoutUI*)mpVerticalLayout1->GetItemAt(0);
			it->second.mpStaMacEdit = (CEditUI*)mpHorizontalLayout2->GetItemAt(2);
			it->second.mpConnectApMacEdit =  (CEditUI*)mpHorizontalLayout2->GetItemAt(5);
			CHorizontalLayoutUI* mpHorizontalLayout3 = (CHorizontalLayoutUI*)mpVerticalLayout1->GetItemAt(1);
			it->second.mpStaPrsRoamingInfoRichEdit = (CRichEditUI*)mpHorizontalLayout3->GetItemAt(0);
			it->second.mpStaMacEdit->SetText(strStaMac.c_str());
			mpView->mpWkeBrowser->AddYSeries(it->second.mstrStaMac);
		}

		return &it->second;
	}

	void CCJGW_PrsVendorRoamingToolDlgController::AppendConnectStaApMac(__int64 nTimeStampMSec,const std::wstring& strStaMac,const std::wstring& strApMac,const wchar_t* strFormatInfo)
	{
		S_AP_UI_INFO* psApUiInfo = AppendApMacInfo(nTimeStampMSec,strApMac);
		S_STA_UI_INFO* psStaUiInfo = AppendStaMacInfo(nTimeStampMSec,strStaMac);

        //! 1、检查当前连接的AP MAC是否为空
        if (psStaUiInfo->mstrConnectedApMac.empty()) 
        {
            psStaUiInfo->mnConnectedApMacTimeStampMSec = nTimeStampMSec;
        }
        else
        {
            //! 检查当前连接的AP MAC与更新的AP MAC是否一致
            if (0 != psStaUiInfo->mstrConnectedApMac.compare(strApMac))
            {
                //! 不一致则表示已经更新了连接AP，记录下从连接AP到断开连接AP时间
                int nConnectTimeStamp = (int)(nTimeStampMSec - psStaUiInfo->mnConnectedApMacTimeStampMSec);
                //JGW_FormatWString(psStaUiInfo->msStaApTestInfo.mstrAssociatedTimeSec,L"%.2f", nConnectTimeStamp / 1000.00f);
                JGW_FormatWString(psStaUiInfo->msStaApTestInfo.mstrAssociatedTimeSec,L"%.2f(%I64d-%I64d)",nConnectTimeStamp / 1000.00f,nTimeStampMSec,psStaUiInfo->mnConnectedApMacTimeStampMSec);
                //psStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = nTimeStampMSec;
                //WritePrsRoamingLog(psStaUiInfo->msStaApTestInfo);
                //psStaUiInfo->msStaApTestInfo.mstrAssociatedTimeSec = L"";
                psStaUiInfo->mnConnectedApMacTimeStampMSec = nTimeStampMSec;
            }
        }
		psApUiInfo->mstrConnectedStaMac = strStaMac;
		psStaUiInfo->mstrConnectedApMac = strApMac;
		psStaUiInfo->mpConnectApMacEdit->SetText(psStaUiInfo->mstrConnectedApMac.c_str());
		if (NULL != strFormatInfo)
		{
			psStaUiInfo->mvstrPrsRoamingInfo.push_back(strFormatInfo);
			psApUiInfo->mvstrPrsRoamingInfo.push_back(strFormatInfo);
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::RemoveConnectStaApMac(__int64 nTimeStampMSec,const std::wstring& strStaMac,const std::wstring& strApMac,const wchar_t* strFormatInfo /* = NULL */)
	{
		S_AP_UI_INFO* psApUiInfo = AppendApMacInfo(nTimeStampMSec,strApMac);
		S_STA_UI_INFO* psStaUiInfo = AppendStaMacInfo(nTimeStampMSec,strStaMac);
        //! 1、检查当前连接的AP MAC是否为空
        if (!psStaUiInfo->mstrConnectedApMac.empty()) 
        {
            int nConnectTimeStamp = (int)(nTimeStampMSec - psStaUiInfo->mnConnectedApMacTimeStampMSec);
            JGW_FormatWString(psStaUiInfo->msStaApTestInfo.mstrAssociatedTimeSec,L"%.2f(%I64d-%I64d)",nConnectTimeStamp / 1000.00f,nTimeStampMSec,psStaUiInfo->mnConnectedApMacTimeStampMSec);
            //psStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = nTimeStampMSec;
            //WritePrsRoamingLog(psStaUiInfo->msStaApTestInfo);
            //psStaUiInfo->msStaApTestInfo.mstrAssociatedTimeSec = L"";
            psStaUiInfo->mnConnectedApMacTimeStampMSec = 0;
        }

		psApUiInfo->mstrConnectedStaMac = L"";
		psStaUiInfo->mpConnectApMacEdit->SetText(L"");
		psStaUiInfo->mstrConnectedApMac = L"";

		if (NULL != strFormatInfo)
		{
			psStaUiInfo->mvstrPrsRoamingInfo.push_back(strFormatInfo);
			psApUiInfo->mvstrPrsRoamingInfo.push_back(strFormatInfo);
		}
	}


	void CCJGW_PrsVendorRoamingToolDlgController::AppendRichEditTestMsg(CRichEditUI* pRichEditUI,const wchar_t* strMsg)
	{
		if (NULL != pRichEditUI && NULL != strMsg)
		{
			pRichEditUI->AppendText(strMsg);
			pRichEditUI->AppendText(L"\r\n");
			pRichEditUI->EndDown();
		}
	}

    void CCJGW_PrsVendorRoamingToolDlgController::OnBeamfromingService(const wchar_t* strMsg)
    {
        //! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
        std::vector<std::wstring> vstrTemp;

        JGW_ParserStrW(strMsg,L",",vstrTemp);
        if (vstrTemp.size() != 7)
        {
            OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse BeamfromingService (%s)",strMsg).c_str());
            return ;
        }
        //! 填充beamforming结构体
        int nIndex = 0;
        S_BeamformingService_Info sBeamFormingServiceInfo;
        sBeamFormingServiceInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[nIndex++].c_str());
        sBeamFormingServiceInfo.mstrApMac = vstrTemp[nIndex++];
        sBeamFormingServiceInfo.mnRssi = _ttoi(vstrTemp[nIndex++].c_str());
        sBeamFormingServiceInfo.mnSnr = _ttoi(vstrTemp[nIndex++].c_str());
        sBeamFormingServiceInfo.mnDistance = _ttoi(vstrTemp[nIndex++].c_str());
        sBeamFormingServiceInfo.mnTxBeamSector = _ttoi(vstrTemp[nIndex++].c_str());
        sBeamFormingServiceInfo.mnRxBeamSector = _ttoi(vstrTemp[nIndex++].c_str());

        
        std::wstring strFormatBeamFormingServiceInfo;
        JGW_FormatWString(strFormatBeamFormingServiceInfo,L"%s [%s] rssi : %d,snr : %d,distance : %d,tx sector : %d,rx sector : %d",JGW_GetTimeStringToTimestampW(sBeamFormingServiceInfo.mdw64TimeStampMSec).c_str(),sBeamFormingServiceInfo.mstrApMac.c_str(),sBeamFormingServiceInfo.mnRssi,sBeamFormingServiceInfo.mnSnr,sBeamFormingServiceInfo.mnDistance,sBeamFormingServiceInfo.mnTxBeamSector,sBeamFormingServiceInfo.mnRxBeamSector);

		//! 发送消息到指令AP视图
		S_AP_UI_INFO* sApUiInfo = AppendApMacInfo(sBeamFormingServiceInfo.mdw64TimeStampMSec,sBeamFormingServiceInfo.mstrApMac,strFormatBeamFormingServiceInfo.c_str());
		//! 通过AP查找是否有已连接STA信息，并传送到STA视图中
		if (!sApUiInfo->mstrConnectedStaMac.empty())
		{
			S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sBeamFormingServiceInfo.mdw64TimeStampMSec,sApUiInfo->mstrConnectedStaMac,strFormatBeamFormingServiceInfo.c_str());
			if (NULL != pStaUiInfo)
			{
				pStaUiInfo->msStaApTestInfo.mstrApMac = sBeamFormingServiceInfo.mstrApMac;
				pStaUiInfo->msStaApTestInfo.mnRssi = sBeamFormingServiceInfo.mnRssi;
				pStaUiInfo->msStaApTestInfo.mnSnr = sBeamFormingServiceInfo.mnSnr;
				pStaUiInfo->msStaApTestInfo.mnDistance = sBeamFormingServiceInfo.mnDistance;
				pStaUiInfo->msStaApTestInfo.mnTxBeamSector = sBeamFormingServiceInfo.mnTxBeamSector;
				pStaUiInfo->msStaApTestInfo.mnRxBeamSector = sBeamFormingServiceInfo.mnRxBeamSector;
				pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sBeamFormingServiceInfo.mdw64TimeStampMSec;
				WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
			}	
		}
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnLinkStatusService(const wchar_t* strMsg)
    {
        //! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
        std::vector<std::wstring> vstrTemp;

        JGW_ParserStrW(strMsg,L",",vstrTemp);
        if (vstrTemp.size() != 6)
        {
            OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse LinkStatusService (%s)",strMsg).c_str());
            return ;
        }

        //! 填充LinkStatusService结构体
        int nIndex = 0;
        S_LinkStatusService_Info sLinkStatusServiceInfo;
        sLinkStatusServiceInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[nIndex++].c_str());
        sLinkStatusServiceInfo.mstrSTAMac = vstrTemp[nIndex++];
        sLinkStatusServiceInfo.mePrsRoamingConnectStatus = (E_PRS_ROAMING_CONNECT_STATUS)_ttoi(vstrTemp[nIndex++].c_str());
        sLinkStatusServiceInfo.mstrAPMac =  vstrTemp[nIndex++];
        sLinkStatusServiceInfo.mnTxMcs = _ttoi(vstrTemp[nIndex++].c_str());
        sLinkStatusServiceInfo.mnRxMcs = _ttoi(vstrTemp[nIndex++].c_str());


        std::wstring strFormatLinkStatus;
        //! STA [24, 214, 28, 161, 13, 242] Connected AP [32, 206, 196, 2, 162, 208],Tx Mcs : 1,Rx Mcs : 1
        JGW_FormatWString(strFormatLinkStatus,L"%s STA[%s] %s AP[%s] ,Tx Mcs : %d,Rx Mcs : %d",JGW_GetTimeStringToTimestampW(sLinkStatusServiceInfo.mdw64TimeStampMSec).c_str(),sLinkStatusServiceInfo.mstrSTAMac.c_str(),E_DISCONNECTED_STATUS == sLinkStatusServiceInfo.mePrsRoamingConnectStatus ? L"Disconnect" : L"Connected",sLinkStatusServiceInfo.mstrAPMac.c_str(),sLinkStatusServiceInfo.mnTxMcs,sLinkStatusServiceInfo.mnRxMcs);

        if (E_DISCONNECTED_STATUS == sLinkStatusServiceInfo.mePrsRoamingConnectStatus)
        {
            RemoveConnectStaApMac(sLinkStatusServiceInfo.mdw64TimeStampMSec,sLinkStatusServiceInfo.mstrSTAMac,sLinkStatusServiceInfo.mstrAPMac,strFormatLinkStatus.c_str());
        }
        else
        {
			AppendConnectStaApMac(sLinkStatusServiceInfo.mdw64TimeStampMSec,sLinkStatusServiceInfo.mstrSTAMac,sLinkStatusServiceInfo.mstrAPMac,strFormatLinkStatus.c_str());
        }

		S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sLinkStatusServiceInfo.mdw64TimeStampMSec,sLinkStatusServiceInfo.mstrSTAMac,NULL);
		if (NULL != pStaUiInfo)
		{
			//pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = sLinkStatusServiceInfo.mePrsRoamingConnectStatus;
			if (E_DISCONNECTED_STATUS == sLinkStatusServiceInfo.mePrsRoamingConnectStatus)
			{
				pStaUiInfo->msStaApTestInfo.mnRxThroughputMbps = 0;
				pStaUiInfo->msStaApTestInfo.mnTxThroughputMbps = 0;
				pStaUiInfo->msStaApTestInfo.mnRssi = MIN_RSSI;
				pStaUiInfo->msStaApTestInfo.mstrApMac = L"";
				//pStaUiInfo->msStaApTestInfo.mnTxMcs = BAD_RX_MCS;
				//pStaUiInfo->msStaApTestInfo.mnRxMcs = BAD_RX_MCS;
				pStaUiInfo->msStaApTestInfo.mnSnr = INVAILD_SNR;
				pStaUiInfo->msStaApTestInfo.mnDistance = 0;
				pStaUiInfo->msStaApTestInfo.mnTxBeamSector = -1;
				pStaUiInfo->msStaApTestInfo.mnRxBeamSector = -1;
				pStaUiInfo->msStaApTestInfo.mnRxThroughputMbps = 0;
				pStaUiInfo->msStaApTestInfo.mnTxThroughputMbps = 0;
			}
			pStaUiInfo->msStaApTestInfo.mnTxMcs = sLinkStatusServiceInfo.mnTxMcs;
			pStaUiInfo->msStaApTestInfo.mnRxMcs = sLinkStatusServiceInfo.mnRxMcs;
			pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sLinkStatusServiceInfo.mdw64TimeStampMSec;
            pStaUiInfo->msStaApTestInfo.SetPrsRomingConnectStatus(sLinkStatusServiceInfo.mePrsRoamingConnectStatus);
			WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
		}
    }

    void CCJGW_PrsVendorRoamingToolDlgController::OnScanNetworkDiscoveryRequestService(const wchar_t* strMsg)
    {
         //! Time,STA MAC ===== SCAN
        std::vector<std::wstring> vstrTemp;

        JGW_ParserStrW(strMsg,L",",vstrTemp);
        if (vstrTemp.size() != 2)
        {
            OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse ScanNetworkDiscoveryRequest (%s)",strMsg).c_str());
            return ;
        }

        __int64 dw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
        std::wstring strStaMac = vstrTemp[1];

        std::wstring strFormatLinkStatus;
        JGW_FormatWString(strFormatLinkStatus,L"%s [%s] scan",JGW_GetTimeStringToTimestampW(dw64TimeStampMSec).c_str(),strStaMac.c_str());

		//! 发送消息到指定STA视图
		S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(dw64TimeStampMSec,strStaMac,strFormatLinkStatus.c_str());
		if (NULL != pStaUiInfo)
		{
			//pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = E_SCAN_COMMAND;
			pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = dw64TimeStampMSec;
            pStaUiInfo->msStaApTestInfo.SetPrsRomingConnectStatus(E_SCAN_COMMAND);
			WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
		}
    }

	void CCJGW_PrsVendorRoamingToolDlgController::OnScanCompleteNetworkDiscoveryResultsService(const wchar_t* strMsg)
	{
		//! Time ,STA MAC,AP MAC LIST,RSSI LIST ==== scan complete
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strMsg,L",",vstrTemp);
		if (vstrTemp.size() != 4)
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse ScanCompleteNetworkDiscoveryResults (%s)",strMsg).c_str());
			return ;
		}

		S_ScanCompleteNetworkDiscoveryResults_Info sScanCompleteNetworkDiscoveryResultsInfo;
		sScanCompleteNetworkDiscoveryResultsInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
		sScanCompleteNetworkDiscoveryResultsInfo.mstrSTAMac = vstrTemp[1];
		JGW_ParserStrW(vstrTemp[2].c_str(),L"**",sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac);
		JGW_ParserIntW(vstrTemp[3].c_str(),L"**",sScanCompleteNetworkDiscoveryResultsInfo.mvnRssi);
		if (sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac.size() != sScanCompleteNetworkDiscoveryResultsInfo.mvnRssi.size())
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse AP List Or Rssi List (%s)",strMsg).c_str());
			return ;
		}

		std::wstring strFormatLinkStatus;
		for (size_t nIndex = 0;nIndex < sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac.size();nIndex++)
		{
			JGW_FormatWString(strFormatLinkStatus,L"%s STA[%s] => AP [%s],rssi : %d . scan complete",JGW_GetTimeStringToTimestampW(sScanCompleteNetworkDiscoveryResultsInfo.mdw64TimeStampMSec).c_str(),sScanCompleteNetworkDiscoveryResultsInfo.mstrSTAMac.c_str(),sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac[nIndex].c_str(),sScanCompleteNetworkDiscoveryResultsInfo.mvnRssi[nIndex]);
			S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sScanCompleteNetworkDiscoveryResultsInfo.mdw64TimeStampMSec,sScanCompleteNetworkDiscoveryResultsInfo.mstrSTAMac,strFormatLinkStatus.c_str());
			if (NULL != pStaUiInfo && 0 == pStaUiInfo->mstrConnectedApMac.compare(sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac[nIndex]))
			{
				pStaUiInfo->msStaApTestInfo.mnRssi = sScanCompleteNetworkDiscoveryResultsInfo.mvnRssi[nIndex];
				pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sScanCompleteNetworkDiscoveryResultsInfo.mdw64TimeStampMSec;
				//pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = E_SCAN_COMPLETE;
                pStaUiInfo->msStaApTestInfo.SetPrsRomingConnectStatus(E_SCAN_COMPLETE);
				WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
			}
			AppendApMacInfo(sScanCompleteNetworkDiscoveryResultsInfo.mdw64TimeStampMSec,sScanCompleteNetworkDiscoveryResultsInfo.mvstrApMac[nIndex],strFormatLinkStatus.c_str());
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::OnScanCompleteNetworkDiscoveryCompleteService(const wchar_t* strMsg)
	{
		//! Time,STA MAC ==== scan complete
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strMsg,L",",vstrTemp);
		if (vstrTemp.size() != 2)
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse ScanCompleteNetworkDiscoveryComplete (%s)",strMsg).c_str());
			return ;
		}

		__int64 dw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
		std::wstring strStaMac = vstrTemp[1];

		std::wstring strFormatLinkStatus;
		JGW_FormatWString(strFormatLinkStatus,L"%s [%s] scan complete",JGW_GetTimeStringToTimestampW(dw64TimeStampMSec).c_str(),strStaMac.c_str());

		//! 发送消息到指定STA视图
		S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(dw64TimeStampMSec,strStaMac,strFormatLinkStatus.c_str());
		if (NULL != pStaUiInfo)
		{
			pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = dw64TimeStampMSec;
			//pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = E_SCAN_COMPLETE;
            pStaUiInfo->msStaApTestInfo.SetPrsRomingConnectStatus(E_SCAN_COMPLETE);
			WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::OnJoinNetworkConnectRequestService(const wchar_t* strMsg)
	{
		//! Time,STA MAC,AP MAC,SSID ==== Join
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strMsg,L",",vstrTemp);
		if (vstrTemp.size() != 4)
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse JoinNetworkConnectRequest (%s)",strMsg).c_str());
			return ;
		}


		S_JoinNetworkConnectRequest_Info sJoinNetworkConnectRequestInfo;
		sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
		sJoinNetworkConnectRequestInfo.mstrSTAMac = vstrTemp[1];
		sJoinNetworkConnectRequestInfo.mstrAPMac = vstrTemp[2];
		sJoinNetworkConnectRequestInfo.mstrSSID = vstrTemp[3];

		std::wstring strFormat;
		std::wstring strSrcConnectApMac;
		JGW_FormatWString(strFormat,L"%s STA [%s] => AP [%s],SSID : %s  Join",JGW_GetTimeStringToTimestampW(sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec).c_str(),sJoinNetworkConnectRequestInfo.mstrSTAMac.c_str(),sJoinNetworkConnectRequestInfo.mstrAPMac.c_str(),sJoinNetworkConnectRequestInfo.mstrSSID.c_str());

		S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec,sJoinNetworkConnectRequestInfo.mstrSTAMac,NULL);
		if (NULL != pStaUiInfo) strSrcConnectApMac = pStaUiInfo->mstrConnectedApMac;
		//! 发送消息到指定STA视图 ,Join 包括与新的AP建立连接，并且断开旧的AP信息
		AppendConnectStaApMac(sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec,sJoinNetworkConnectRequestInfo.mstrSTAMac,sJoinNetworkConnectRequestInfo.mstrAPMac,strFormat.c_str());

		//! 
		if (NULL != pStaUiInfo)
		{
#if 0
			//! 原连接的AP与现在连接AP不一致则补充一个断开连接的消息(是否需要补一个??????????????????)
			if (0 != strSrcConnectApMac.compare(sJoinNetworkConnectRequestInfo.mstrAPMac))
			{
				pStaUiInfo->msStaApTestInfo.mstrApMac = L"";
				pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = E_DISCONNECTED_STATUS;
				pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec;
				WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
			}
#endif
			pStaUiInfo->msStaApTestInfo.mstrApMac = sJoinNetworkConnectRequestInfo.mstrAPMac;
			pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sJoinNetworkConnectRequestInfo.mdw64TimeStampMSec;
			//pStaUiInfo->msStaApTestInfo.mePrsRomingConnectStatus = E_JOIN_COMMAND;
			pStaUiInfo->msStaApTestInfo.mstrSsid = sJoinNetworkConnectRequestInfo.mstrSSID;
            pStaUiInfo->msStaApTestInfo.SetPrsRomingConnectStatus(E_JOIN_COMMAND);
			WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
		}

	}

	void CCJGW_PrsVendorRoamingToolDlgController::onScanSummaryService(const wchar_t* strMsg)
	{
		//! Time,AP MAC,RSSI,SNR,DISTANCE
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strMsg,L",",vstrTemp);
		if (vstrTemp.size() != 5)
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse ScanSummary (%s)",strMsg).c_str());
			return ;
		}

		S_ScanSummary_Info sScanSummaryInfo;
		sScanSummaryInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
		sScanSummaryInfo.mstrAPMac = vstrTemp[1];
		sScanSummaryInfo.mnRssi = _ttoi(vstrTemp[2].c_str());
		sScanSummaryInfo.mnSnr = _ttoi(vstrTemp[3].c_str());
		sScanSummaryInfo.mnDistance = _ttoi(vstrTemp[4].c_str());

		std::wstring strFormat;
		JGW_FormatWString(strFormat,L"%s AP[%s],rssi : %d,snr : %d,distance : %d",JGW_GetTimeStringToTimestampW(sScanSummaryInfo.mdw64TimeStampMSec).c_str(),sScanSummaryInfo.mstrAPMac.c_str(),sScanSummaryInfo.mnRssi,sScanSummaryInfo.mnSnr,sScanSummaryInfo.mnDistance);
		//! 发送消息到指定AP视图
		S_AP_UI_INFO* sApUiInfo = AppendApMacInfo(sScanSummaryInfo.mdw64TimeStampMSec,sScanSummaryInfo.mstrAPMac,strFormat.c_str());
		//! 检查当前AP是否已经连接，如果已连接将信息发送到STA视图中
		if (NULL != sApUiInfo && !sApUiInfo->mstrConnectedStaMac.empty())
		{
			S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sScanSummaryInfo.mdw64TimeStampMSec,sApUiInfo->mstrConnectedStaMac,strFormat.c_str());
			if (NULL != pStaUiInfo)
			{
				pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sScanSummaryInfo.mdw64TimeStampMSec;
				pStaUiInfo->msStaApTestInfo.mnRssi = sScanSummaryInfo.mnRssi;
				pStaUiInfo->msStaApTestInfo.mnSnr = sScanSummaryInfo.mnSnr;
				pStaUiInfo->msStaApTestInfo.mnDistance = sScanSummaryInfo.mnDistance;
				WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
			}
		}
	}

	void CCJGW_PrsVendorRoamingToolDlgController::OnPerfStatsService(const wchar_t* strMsg)
	{
		//! Time,STA MAC,is Tx,ThroughputMbps
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strMsg,L",",vstrTemp);
		if (vstrTemp.size() != 4)
		{
			OnAppendRichEditTestMsg(JGW_GetFormatWString(L"Error : Parse PerfStats (%s)",strMsg).c_str());
			return ;
		}

		S_PerfStats_Info sPerfStatsInfo;
		sPerfStatsInfo.mdw64TimeStampMSec = _ttoi64(vstrTemp[0].c_str());
		sPerfStatsInfo.mstrSTAMac = vstrTemp[1];
		sPerfStatsInfo.mbIsTx = (1 == _ttoi(vstrTemp[2].c_str()));
		sPerfStatsInfo.mnThroughputMbps = _ttoi(vstrTemp[3].c_str());

		std::wstring strFormat;
		JGW_FormatWString(strFormat,L"%s STA[%s] %s ThroughputMbps : %d",JGW_GetTimeStringToTimestampW(sPerfStatsInfo.mdw64TimeStampMSec).c_str(),sPerfStatsInfo.mstrSTAMac.c_str(),sPerfStatsInfo.mbIsTx ? L"Tx" : L"Rx",sPerfStatsInfo.mnThroughputMbps);
		//! 发送消息到指定STA视图
		S_STA_UI_INFO* pStaUiInfo = AppendStaMacInfo(sPerfStatsInfo.mdw64TimeStampMSec,sPerfStatsInfo.mstrSTAMac,strFormat.c_str());
		if (NULL != pStaUiInfo)
		{
			pStaUiInfo->msStaApTestInfo.mdw64TimeStampMSec = sPerfStatsInfo.mdw64TimeStampMSec;
			if (sPerfStatsInfo.mbIsTx)
			{
				pStaUiInfo->msStaApTestInfo.mnTxThroughputMbps = sPerfStatsInfo.mnThroughputMbps;
			}
			else
			{
				pStaUiInfo->msStaApTestInfo.mnRxThroughputMbps = sPerfStatsInfo.mnThroughputMbps;
			}
			WritePrsRoamingLog(pStaUiInfo->msStaApTestInfo);
		}
	}
}

