// MobileMifiService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PublicDefine.h"
#include <ShellAPI.h>
#include "TLHELP32.H"
//  #include "ICMPDefine.h"
SERVICE_STATUS sServiceStatus = {0};
SERVICE_STATUS_HANDLE sServiceStatusHandld = {0};
bool	g_IsFristInsert = true;

VOID /*WINAPI*/ SqlSrvServiceCtrlHandle(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		WLOGA("Monitoring stopped.");
		sServiceStatus.dwWin32ExitCode = 0; 
		sServiceStatus.dwCurrentState = SERVICE_STOPPED; 
		SetServiceStatus (sServiceStatusHandld, &sServiceStatus);
		return;
	case SERVICE_CONTROL_SHUTDOWN:
		WLOGA("Monitoring stopped.");
		sServiceStatus.dwWin32ExitCode = 0; 
		sServiceStatus.dwCurrentState = SERVICE_STOPPED; 
		SetServiceStatus (sServiceStatusHandld, &sServiceStatus);
		return;
	default:
		break;
	}
	// Report current status
	SetServiceStatus (sServiceStatusHandld, &sServiceStatus);
	return;
}

VOID /*WINAPI*/ MobileMifiServiceStartMain(int argc, char** argv)
{
	DWORD status = 0;
	DWORD specificError = 0;
	sServiceStatus.dwServiceType = SERVICE_WIN32;
	sServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	sServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;/* SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE*/;
	sServiceStatus.dwWin32ExitCode = 0;
	sServiceStatus.dwServiceSpecificExitCode = 0;
	sServiceStatus.dwCheckPoint = 0;
	sServiceStatus.dwWaitHint = 0;
	sServiceStatusHandld = RegisterServiceCtrlHandler(g_configInfo.m_szService_Name,(LPHANDLER_FUNCTION)SqlSrvServiceCtrlHandle);

	// 向SCM 报告运行状态
	sServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus (sServiceStatusHandld, &sServiceStatus);
	LoadConfigInfo();
	if( !GetDefaultBrowserPath(g_configInfo.m_szBrowserPath,sizeof(g_configInfo.m_szBrowserPath)) )
	{
		memset(g_configInfo.m_szBrowserPath,0x00,sizeof(g_configInfo.m_szBrowserPath));
		if (IsWow64System())
		{
			_tcscpy(g_configInfo.m_szBrowserPath,_T("C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe"));
		}
		else
		{
			_tcscpy(g_configInfo.m_szBrowserPath,_T("C:\\Program Files\\Internet Explorer\\iexplore.exe"));
		}
	}
	WriteServiceDescribe();

	// 下面就开始任务循环了，你可以添加你自己希望服务做的工作
	if( IsSelectPortExist() )
	{
		OpenUrl();
		Sleep(1000 * 10);
	}

	while(sServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		CRegWindowMessage();
		Sleep(g_configInfo.m_nWait_time * 1000);
	}

	WLOGA("service stopped");
}

void main()
{
	LoadConfigInfo();
	
	//UrlIsOpenExplore();
//	IsDeviceNetWorking();

	SERVICE_TABLE_ENTRY sDispathTable[] = {
		{g_configInfo.m_szService_Name,(LPSERVICE_MAIN_FUNCTION)MobileMifiServiceStartMain},
		{NULL,NULL}
	};

	if ( !StartServiceCtrlDispatcher(sDispathTable) )
	{
		WLOGA("StartServiceCtrlDispatcher Fail(%d)\n",GetLastError());
	}
	// 	CRegWindowMessage();
}
