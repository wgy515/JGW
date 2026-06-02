// JaGuarWaveTestPlatform.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "help/CJGW_HelpDlg.h"
#include "CJGW_LoginMESDlg.h"
#include "JGWMainDlg.h"
#include "CJGW_MESConfig.h"
#include "CJGW_StartMESServiceDlg.h"

#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_WindowsFuncPlugin/crashdump.h>
#include "../../include/JaGuarWaveTestPlatformHelp/CCJGW_HelpThreadImpl.h"
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

#include <JGW_FoundationFunc/jgw_process_manage.h>
bool LoadMESConfig(S_MES_INFO& sMesInfo);
bool LoadHelpConfig(JGW::S_JGW_HELP_CONFIG& sHelpConfig);
#define JGW_Application_Instance_GUID "{84D3C8C4-3043-4915-81C2-7E6627220C92}"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    //! 防止当前应用实例同时运行
    if(!JGW::jgw_single_application_instance_module(_T(JGW_Application_Instance_GUID),hInstance))
        return FALSE;
    //! 设置当前语言
    InitLanguage(E_CHS_LOCAL);
    //! exec crash dump log
    CrashDump dump;
    CPaintManagerUI::SetInstance(hInstance);
    // 初始化MES配置信息
    S_MES_INFO sMesInfo;
    sMesInfo.mbIsForceTerminateProcess = true;
    // 关于NET CSTSE配置暂不使用
    sMesInfo.msCSTSEServerConfig.mbOpenCSTSEServerModule = false;
    sMesInfo.msCSTSEServerConfig.mnCSTSEServerSocketPort = 3008;

    JGW::S_JGW_HELP_CONFIG sHelpConfig;
    // JaGuarWaveHelpV3 程序配置文件，主要是上传LOG以及程序自动下载更新
    sHelpConfig.mnSocketTimeoutSec = 50;
    sHelpConfig.mnUpdateServiceSocketPort = 3002;
    sHelpConfig.mstrHelpServiceIPAddr = "127.0.0.1";

    //! 加载MES配置文件
    if (!LoadMESConfig(sMesInfo))
    {
        goto EXEC_END;
    }
    //! 加载自动升级配置信息
    if (!LoadHelpConfig(sHelpConfig))
    {
        goto EXEC_END;
    }

#if 1
    // 如果当前程序处于在线模式
    if (E_ONLINE_MES == sMesInfo.msMesConnectInfo.meMESModel)
    {
        LPWSTR *szArgList;
        int argCount;
        szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
        //! JaGuarWaveStart程序打开当前程序的时候会新增JaGuarWaveStart参数启动
        if (argCount >= 2 && 0 == JGW::JGW_WStrComparenoCaseWStr(L"JaGuarWaveStart",szArgList[1]))
        {
            // 开启MES后台程序
            JGW::CCJGW_StartMESServiceDlg startMESServices(NULL);
            startMESServices.DoModal();
            sMesInfo.msMesConnectInfo.meMESStatus = E_MES_OK_STATUS;
        }
        else
        {
            // 开启MES后台程序
			JGW::CCJGW_StartMESServiceDlg startMESServices(NULL);
			startMESServices.DoModal();
            // 并打开登录窗口进行配置
            JGW::CCJGW_LoginMESDlg loginMesDlg(sMesInfo);
            if (IDCANCEL == loginMesDlg.DoModal())
            {
                goto EXEC_END;
            }
        }
    }
#else
    //! 如果是在线模式，则登录MES
    if (E_ONLINE_MES == sMesInfo.msMesConnectInfo.meMESModel)
    {
        JGW::CCJGW_LoginMESDlg loginMesDlg(sMesInfo);
        if (IDCANCEL == loginMesDlg.DoModal())
        {
            goto EXEC_END;
        }
    }
#endif
	//! 开启FTP 日志后台自动更新程序 #define HELP_SERVICE_APPLICATION_NAME  L"JaGuarWaveHelpV2.exe"
    if (!sMesInfo.mstrFTPLogBackGroundExec.empty())
	{
		if (-1 == JGW::JGW_GetProcessID2Name(sMesInfo.mstrFTPLogBackGroundExec))
		{
			std::wstring strTemp = JGW::JGW_GetApplicationFolder();
			strTemp += sMesInfo.mstrFTPLogBackGroundExec;
			if (NULL == JGW::JGW_CreateProcess(strTemp.c_str()))
			{
				return -1;
			}
		}
	}
    //! 开启自动升级检测
    if (sMesInfo.mbAutoUpdate/* && E_MES_STATUS_NOTONLINE != sMesInfo.msMesConnectInfo.meMESStatus*/)
    {
        sHelpConfig.mstrTaskNumber = JGW::JGW_A2W(sMesInfo.mstrTaskNumber);
        // JaGuarWaveHelp后台程序
        JGW::CCJGW_HelpDlg helpDlg(sHelpConfig);
        if (IDCANCEL == helpDlg.DoModal())
        {
            JGW::StopHelpModuleThread();
            goto EXEC_END;
        }
    }
    /*
#if 1
    //! STL本地化设置切回默认值
    //! std::locale::global(std::locale(""));  
    {
        JGW::CCJGWMainDlg* dlg = new JGW::CCJGWMainDlg(sMesInfo,sHelpConfig);
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main.xml"),_T("JaGuarWave Factory Test Application"));
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main.xml"),_T("JaGuarWave Factory Test Application"));
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("main.xml"),_T("JaGuarWave Factory Test Application"));
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVETESTPLATFORM);
        dlg->ShowModal();
        delete dlg;
    }
#endif
    CPaintManagerUI::Term();
    */
    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;
    // 打开主窗口
    {
        JGW::CCJGWMainDlg* dlg = new JGW::CCJGWMainDlg(sMesInfo,sHelpConfig);
#ifdef USE_ZIP_RESOURCE_ENABLE
        dlg->SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main.xml"),_T("JaGuarWave Factory Test Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#elif defined USE_ZIP_FILE_ENABLE
        dlg->SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        dlg->CreateNoBorderStyleWindow(_T(""),_T("main.xml"),_T("JaGuarWave Factory Test Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#else
        dlg->CreateNoBorderStyleWindow(_T("skin\\JaGuarWave"),_T("main.xml"),_T("JaGuarWave Factory Test Application"),NULL,UI_WNDSTYLE_FRAME,UI_WNDSTYLE_EX_FRAME);
#endif
        dlg->CenterWindow();
        dlg->SetIcon(IDI_JAGUARWAVETESTPLATFORM);
        dlg->ShowWindow();
        //dlg->ShowModal();
        //delete dlg;
    }
    CPaintManagerUI::MessageLoop();
    CPaintManagerUI::Term();
    ::CoUninitialize();
EXEC_END:
#ifndef _DEBUG
    if (sMesInfo.mbIsForceTerminateProcess)
    {
        HANDLE hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId()); 
        TerminateProcess(hp,0);
    }
#endif

    return 0;
}

bool LoadMESConfig(S_MES_INFO& msMesInfo)
{
    msMesInfo.msMesConnectInfo.meMESModel = E_ONLINE_MES;
	msMesInfo.mstrFTPLogBackGroundExec = L"";
    msMesInfo.msMesConnectInfo.meMESStatus = E_MES_UNKNOWN_STATUS;
    msMesInfo.msMesConnectInfo.mstrMESConfigFilePath = JGW::JGW_GetApplicationFolder();
    msMesInfo.msMesConnectInfo.mstrMESConfigFilePath += L"jgw_mes_config.data";
    msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig = JGW::JGW_GetApplicationFolder();
    msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig += L"jgw_mes_workstage.xml";
    Log4WD_F(L"MES Config File Path: %s,MES Work Stage Config File Path: %s",msMesInfo.msMesConnectInfo.mstrMESConfigFilePath.c_str(),msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig.c_str());

    JGW::CCJGW_MESConfig config;
    //! 加载站点ID以及站点名称
    config.LoadMESWorkStageXmlConfig(msMesInfo.msMesConnectInfo.mstrMESWorkStageConfig,&msMesInfo);
    //! 加载MES配置文件
    if (!config.LoadMESIniConfig(msMesInfo.msMesConnectInfo.mstrMESConfigFilePath,&msMesInfo))
    {
        ::MessageBox(NULL,GetString(E_LOAD_MESCONFIG_FAIL),_T("MES Config"),MB_ICONERROR);
        return false;
    }
    return true;
}

bool LoadHelpConfig(JGW::S_JGW_HELP_CONFIG& sHelpConfig)
{
    std::wstring strFilePath(JGW::JGW_GetApplicationFolder());
    strFilePath += L"help.ini";

    JGW::CCJGW_ConfigIni helpConfigIni;
    helpConfigIni.InitIniFilePath(strFilePath.c_str());
    sHelpConfig.mstrHelpServiceIPAddr = helpConfigIni.GetIniKeyValueA(L"help",L"Host",L"127.0.0.1");
    sHelpConfig.mnSocketTimeoutSec = helpConfigIni.GetIniKeyIntValue(L"help",L"SocketTimeOutSec",50);
    sHelpConfig.mnUpdateServiceSocketPort = helpConfigIni.GetIniKeyIntValue(L"help",L"SocketPort",3002);

    return true;
}