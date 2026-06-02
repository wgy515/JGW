// JaGuarWaveFTPLogBackGround.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Resource.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_WindowFunc.h>

#include <JaGuarWaveHelpV3/CJaGuarWaveHelpV3Define.h>
#include "CJaGuarWaveFTPLogBackGroundDlg.h"

#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#define JGW_Application_Instance_GUID "{60399275-B7B2-45DE-B671-5FFE4994D381}"
#define APPLICATION_WINDOW_NAME L"JaGuarWave FTP Log BackGround"

#include <io.h>

void HelpV3ConfigDefault(JGW::S_HELP_V3_CONFIG& sHelpV3Config);
void LoadHelpIniConfig(JGW::S_HELP_V3_CONFIG& sHelpV3Config);
void LoadJGWMesConfigDataConfig(JGW::S_HELP_V3_CONFIG& sHelpV3Config);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	JGW::S_HELP_V3_CONFIG sHelpV3Config;

	HelpV3ConfigDefault(sHelpV3Config);
	LoadHelpIniConfig(sHelpV3Config);
	LoadJGWMesConfigDataConfig(sHelpV3Config);
	JGW::JGW_RefreshTaskbarIcon();

	ghInstance = hInstance;
	//! STL本地化设置切回默认值
	if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),NULL))
	{
		return -1;
	}

	CPaintManagerUI::SetInstance(hInstance);
	{
		JGW::CCJaGuarWaveFTPLogBackGroundDlg* dlg = new JGW::CCJaGuarWaveFTPLogBackGroundDlg(sHelpV3Config);
#if 0
		dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveHelp"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else  
#ifdef USE_ZIP_RESOURCE_ENABLE
		dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
		dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#elif defined USE_ZIP_FILE_ENABLE
		dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWaveHelp.zip");
		dlg->CreateNoBorderStyleWindow(_T(""),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#else
		dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWaveHelp"),_T("main_dlg.xml"),APPLICATION_WINDOW_NAME);
#endif
#endif
		dlg->CenterWindow();
		dlg->SetIcon(IDI_JAGUARWAVEFTPLOGBACKGROUND);
		dlg->ShowModal(SW_HIDE);
		delete dlg;
	}
	CPaintManagerUI::Term();
	return 0;
}

void HelpV3ConfigDefault(JGW::S_HELP_V3_CONFIG& sHelpV3Config)
{
	sHelpV3Config.mlHelpDBFtpUpdateTime = 0;
	sHelpV3Config.mbIsDownloadDB = false;
	sHelpV3Config.mbIsOpenUploadTimer = false;

	sHelpV3Config.msDownloadThreadParam.mstrTaskNumber = L"";
	sHelpV3Config.mnCheckSoftwareUpdateIntervalTimeSec = 300;
	sHelpV3Config.mnSocketTimeoutSec = 10;
	sHelpV3Config.mnUpdateServiceSocketPort = 3002;

	//! TaskSQL <=> TaskConfig
	sHelpV3Config.msTaskSQL.msTaskConfig.mbIsUpdateDBLogConfig = false;
	sHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadLog = false;
	sHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadTestLog = true;
#ifdef _DEBUG
	sHelpV3Config.msTaskSQL.msTaskConfig.mnUploadLogSec = 10;
	sHelpV3Config.msTaskSQL.msTaskConfig.mnUploadTestLogSec = 10;
#else
	sHelpV3Config.msTaskSQL.msTaskConfig.mnUploadLogSec = 300;
	sHelpV3Config.msTaskSQL.msTaskConfig.mnUploadTestLogSec = 300;
#endif
	sHelpV3Config.msTaskSQL.msTaskConfig.mstrCustomLogDir = L"";

	sHelpV3Config.msFTPlogInfo.mnPort = 26;
	sHelpV3Config.msFTPlogInfo.mstrHost = "192.168.8.10";
	sHelpV3Config.msFTPlogInfo.mstrLogRootPath = L"/JaGuarWaveTestPlatform_Log/";
	sHelpV3Config.msFTPlogInfo.mstrRootPath = L"/JGWPTP_Help/";
	JGW::CCJGW_CryptPassword cryptPassword;
	sHelpV3Config.msFTPlogInfo.mstrUserName = cryptPassword.DecryptPassword("lKNG7Im5x0ith8DsldhiTw==");
	sHelpV3Config.msFTPlogInfo.mstrUserPassword = cryptPassword.DecryptPassword("WWC7r11A4ezrNs6aEyHcIg==");

	sHelpV3Config.msDownloadThreadParam.mstrExecFolder = JGW::JGW_GetApplicationFolder();
}

void LoadHelpIniConfig(JGW::S_HELP_V3_CONFIG& sHelpV3Config)
{
	std::wstring strHelpConfigFilePath (JGW::JGW_GetApplicationFolder());
	strHelpConfigFilePath += L"help.ini";

	JGW::CCJGW_ConfigIni configIni;
	JGW::CCJGW_CryptPassword cryptPassword;
	configIni.InitIniFilePath(strHelpConfigFilePath.c_str());

	sHelpV3Config.mnSocketTimeoutSec = configIni.GetIniKeyIntValue(L"help",L"SocketTimeOutSec",10);
	sHelpV3Config.mnUpdateServiceSocketPort = configIni.GetIniKeyIntValue(L"help",L"SocketPort",3002);
	sHelpV3Config.mnCheckSoftwareUpdateIntervalTimeSec = configIni.GetIniKeyIntValue(L"help",L"SoftwareUpdateIntervalTimeSec",5 * 60);

	sHelpV3Config.msFTPlogInfo.mnPort = configIni.GetIniKeyIntValue(L"FTP",L"Port",26);
	sHelpV3Config.msFTPlogInfo.mstrHost = configIni.GetIniKeyValueA(L"FTP",L"Host",L"192.168.8.10");
	sHelpV3Config.msFTPlogInfo.mstrRootPath = configIni.GetIniKeyValue(L"FTP",L"RootPath",L"/JaGuarWaveTestPlatform_Version/");
	sHelpV3Config.msFTPlogInfo.mstrLogRootPath = configIni.GetIniKeyValue(L"FTP",L"LogRootPath",L"/JaGuarWaveTestPlatform_Version/");
	std::string strUserName = configIni.GetIniKeyValueA(L"FTP",L"UserName",L"lKNG7Im5x0ith8DsldhiTw==");
	std::string strUserPassword = configIni.GetIniKeyValueA(L"FTP",L"Password",L"WWC7r11A4ezrNs6aEyHcIg==");
	sHelpV3Config.msFTPlogInfo.mstrUserName = cryptPassword.DecryptPassword(strUserName);
	sHelpV3Config.msFTPlogInfo.mstrUserPassword = cryptPassword.DecryptPassword(strUserPassword);
}
// jgw_mes_config.data
void LoadJGWMesConfigDataConfig(JGW::S_HELP_V3_CONFIG& sHelpV3Config)
{
	std::wstring strHelpConfigFilePath (JGW::JGW_GetApplicationFolder());
	strHelpConfigFilePath += L"jgw_mes_config.data";

	JGW::CCJGW_ConfigIni configIni;
	configIni.InitIniFilePath(strHelpConfigFilePath.c_str());

	sHelpV3Config.msDownloadThreadParam.mstrTaskNumber = configIni.GetIniKeyValue(L"MESClient",L"TaskNumber");
}

