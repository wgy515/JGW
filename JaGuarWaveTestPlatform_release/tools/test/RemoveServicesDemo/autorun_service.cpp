#define AUTORUN_EWALK_CPP
#include "autorun.h"
#include <ShlObj.h>
SC_HANDLE hCurrentSCManager = NULL;

#ifndef CSIDL_COMMON_APPDATA
#define CSIDL_COMMON_APPDATA            0x0023
#endif
#define MIFISERVICENAMEDEFAULT _T("MobileMifiService")
#define MIFILOGNAMEDEFAULT	"C:\\WINDOWS\\system32\\MobileMifiServiceRun.log"
#ifdef _UNICODE
#define T2CHAR WcharToChar
#define A2TCHAR CharToWchar
#else
#define T2CHAR 
#define A2TCHAR 
#endif

struct STRUCT_LOAD_CONFIG_INFO
{
	TCHAR m_szService_FolderName[MAX_PATH];
	TCHAR m_szService_Name[MAX_PATH];
	TCHAR m_szDescribe[MAX_PATH];
	bool  m_bIsOpenService;
	CHAR m_szService_logPath[MAX_PATH];
	CHAR m_szPid[MAX_PATH];
	int	  m_nWait_time;
	TCHAR m_szUrl[MAX_PATH];
	bool  m_bIsAuto_open_url;
	TCHAR m_szService_Exename[MAX_PATH];
};

STRUCT_LOAD_CONFIG_INFO g_configInfo = {MIFISERVICENAMEDEFAULT,MIFISERVICENAMEDEFAULT,_T("Provide service for mobile broadband device"),true,MIFILOGNAMEDEFAULT,"VID_1DBC&PID_0312",1,_T("http://192.168.8.1"),true,_T("MobileMifiService.exe")};

wchar_t* CharToWchar (char *m_char)
{
	UINT m_int = MultiByteToWideChar (CP_ACP,0, m_char,-1,NULL ,0);
	wchar_t	*szWcharTemp = new wchar_t[m_int + 1];
	memset(szWcharTemp ,0x00,m_int + 1);
	MultiByteToWideChar(CP_ACP ,0,m_char, m_int ,szWcharTemp, m_int);
	szWcharTemp[m_int] = '\0';

	return szWcharTemp ;
}

char* WcharToChar (wchar_t *m_wchar)
{
	UINT m_int = WideCharToMultiByte (CP_ACP,0, m_wchar,-1,NULL ,0,NULL, FALSE);
	char	*szCharTemp = new char[m_int + 1];
	memset(szCharTemp ,0x00,m_int + 1);
	WideCharToMultiByte(CP_ACP ,0,m_wchar,-1, szCharTemp,m_int ,NULL, FALSE);
	szCharTemp[m_int] = '\0';

	return szCharTemp ;
}


void LoadConfigInfo()
{
	TCHAR szPath[512] = {0};
	TCHAR szConfigFile[512] = {0};

	TCHAR szDir[MAX_PATH]        = {0};
	TCHAR szDrive[MAX_PATH]      = {0};
	char szTemp[MAX_PATH]    = {0};

	//取软件存储路径
	GetModuleFileName(NULL,szPath,512);
	_tsplitpath(szPath,szDrive,szDir,NULL,NULL);
	memset(szPath,0x00,sizeof(szPath));
	//_splitpath_s( szPath, szDrive, MAX_PATH, szDir, MAX_PATH, NULL, NULL, NULL, NULL );
	_stprintf( szPath, _T("%s%s"), szDrive, szDir ); // ChenYao add, 2012-04-26

	// 	SHGetFolderPath(0,CSIDL_COMMON_APPDATA,0,0,szPath);
	// 	if (_tcslen(szPath) < 5)
	// 	{
	// 		WLOGA("%s\t%s fail",__FUNCTION__,"SHGetFolderPath");
	// 		return;
	// 	}

	_stprintf(szConfigFile,_T("%s\\%s"),szPath,_T("MifiConfig.dat"));
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
	{
		MYLOG( _T("%s\t%s file not exits"),__FUNCTION__,szPath);
		return;
	}

	memset(&g_configInfo,0x00,sizeof(STRUCT_LOAD_CONFIG_INFO));
	GetPrivateProfileString(_T("INSTALL"),_T("service_FolderName"),MIFISERVICENAMEDEFAULT,g_configInfo.m_szService_FolderName,MAX_PATH,szConfigFile);

	GetPrivateProfileString(_T("INSTALL"),_T("service_name"),MIFISERVICENAMEDEFAULT,g_configInfo.m_szService_Name,MAX_PATH,szConfigFile);

	GetPrivateProfileString(_T("INSTALL"),_T("service_exename"),_T("MobileMifiService.exe"),g_configInfo.m_szService_Exename,MAX_PATH,szConfigFile);

	GetPrivateProfileString(_T("INSTALL"),_T("service_descDetail"),_T("Provide service for mobile broadband device"),g_configInfo.m_szDescribe,MAX_PATH,szConfigFile);

	g_configInfo.m_bIsOpenService = (GetPrivateProfileInt(_T("INSTALL"),_T("Openservice"),1,szConfigFile) == 1)?true:false;

	char *pszbuffer = WcharToChar(szConfigFile);
	GetPrivateProfileStringA("INSTALL","service_logPath","MobileMifiServiceRun.log",szTemp,MAX_PATH,pszbuffer);
	GetPrivateProfileStringA("PRODUCT_ID","pid","VID_1DBC&PID_0312",g_configInfo.m_szPid,MAX_PATH,pszbuffer);
	delete []pszbuffer;
	char *psztemp = WcharToChar(szPath);
	sprintf(g_configInfo.m_szService_logPath,"%s\\%s",psztemp,szTemp);
	delete []psztemp;

	g_configInfo.m_nWait_time = GetPrivateProfileInt(_T("OPENBR_TIME"),_T("wait_time"),1,szConfigFile);

	GetPrivateProfileString(_T("OPEN_URL"),_T("url"),_T("http://192.168.8.1"),g_configInfo.m_szUrl,MAX_PATH,szConfigFile);

	g_configInfo.m_bIsAuto_open_url = (GetPrivateProfileInt(_T("OPEN_URL"),_T("auto_open_url"),1,szConfigFile) == 1)?true:false;

}

BOOL InstallMifiService()
{
	TCHAR serviceName[MAX_PATH] = {0};
	DWORD serviceCount = 260;
	TCHAR szCurrentPath[MAX_PATH] = {0};
	TCHAR szServiceEexPath[MAX_PATH] = {0};
	SC_HANDLE schService = NULL;
	TCHAR szPath[512] = {0};
	TCHAR szConfigInfo[MAX_PATH] = {0};
	
	if(FALSE == GetCurrentPath(szCurrentPath))//获取当前目录,如使szCurrentPath为"e:\hwq_2009-5-11_work\AutoRun_FromEnhou090617\AutoRun_2.0.0.17_RC23\AutoRun\debug\"
	{
		MYLOG(_T("%s(GetCurrentPath) return_1\n"), _T(__FUNCTION__));
		return FALSE;
	}

	_sntprintf(szServiceEexPath,MAX_PATH,_T("%s\\%s"),szCurrentPath,g_configInfo.m_szService_Exename);
	_sntprintf(szConfigInfo,MAX_PATH,_T("%s\\MifiConfig.dat"),szCurrentPath);
	memset(szCurrentPath,0x00,MAX_PATH);

	SHGetFolderPath(0,CSIDL_COMMON_APPDATA,0,0,szCurrentPath);
	if (_tcslen(szCurrentPath) < 5)
	{
		MYLOG(_T("%s(SHGetFolderPath) return_1\n"), _T(__FUNCTION__));
		return FALSE;
	}

	_tcscat(szCurrentPath,_T("\\"));
	_tcscat(szCurrentPath,g_configInfo.m_szService_FolderName);
	if(!CreateDirectory(szCurrentPath,NULL))
	{
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MYLOG(_T("%s(CreateDirectoryA) return_1\n"), _T(__FUNCTION__));
			return FALSE;
		}
	}

	_sntprintf(szPath,MAX_PATH,_T("%s\\%s"),szCurrentPath,g_configInfo.m_szService_Exename);

	if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
	{
		if ( !CopyFile(szServiceEexPath,szPath,FALSE) )
		{
			MYLOG(_T("Copy SeviceExeFile Fail %s--->%s\n (%d)"),szServiceEexPath,szPath,GetLastError());
			return FALSE;
		}
	}
	memset(szServiceEexPath,0x00,sizeof(szServiceEexPath));
	_tcscpy(szServiceEexPath,szPath);
	memset(szPath,0x00,sizeof(szPath));
	_sntprintf(szPath,MAX_PATH,_T("%s\\MifiConfig.dat"),szCurrentPath);

	if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(szPath))
	{
		if ( !CopyFile(szConfigInfo,szPath,FALSE) )
		{
			MYLOG(_T("Copy MifiConfig Fail"));
			//return FALSE;
		}
	}


	// Create the service 
	schService = CreateService( 
		hCurrentSCManager,              // SCM database
		g_configInfo.m_szService_Name,                   // name of service
		g_configInfo.m_szService_Name,                   // service name to display
		SERVICE_ALL_ACCESS,        // desired access
		SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS, // service type
		SERVICE_AUTO_START,      // start type
		SERVICE_ERROR_NORMAL,      // error control type
		szServiceEexPath,                    // path to service's binary
		NULL,                      // no load ordering group
		NULL,                      // no tag identifier
		NULL,                      // no dependencies
		NULL,                      // LocalSystem account
		NULL);                     // no password 

	if (schService == NULL) 
	{
		MYLOG(_T("CreateService failed (%ld)\n"), GetLastError());
		return FALSE;
	}
	else 
	{
		MYLOG(_T("Service installedsuccessfully\n"));  
	}

	if ( !StartMifiService(schService) )
	{
		CloseServiceHandle(schService);
		return FALSE;
	}

	CloseServiceHandle(schService);
	return TRUE;
}

BOOL IsInstallMifiService()
{
	LoadConfigInfo();
	if ( !g_configInfo.m_bIsOpenService )
	{
		return TRUE;
	}

	TCHAR serviceName[MAX_PATH] = {0};
	DWORD serviceCount = 260;
	SC_HANDLE sc_Service = NULL;
	DWORD dwResult = 0;
	hCurrentSCManager = OpenSCManager ( NULL, NULL ,SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE );	

	if (hCurrentSCManager == NULL )
	{
		MYLOG(_T("OpenSCManager failed (%ld)\n"), GetLastError());
		return FALSE;
	}
		

	if( GetServiceKeyName (hCurrentSCManager,g_configInfo.m_szService_Name, serviceName ,& serviceCount ) )
	{
		sc_Service = OpenService (hCurrentSCManager , serviceName, SERVICE_QUERY_STATUS | SERVICE_START );
		if ( !StartMifiService(sc_Service) )
		{
			CloseServiceHandle(sc_Service);
			return FALSE;
		}
		CloseServiceHandle(sc_Service);
	}
	else
	{
		if( !InstallMifiService() )
		{
			return FALSE;
		}
	}
	
	CloseServiceHandle(hCurrentSCManager);

	return TRUE;
}

BOOL GetQueryMifiServiceStatus(SC_HANDLE sc_Service,DWORD   dwCurrentState)
{
	SERVICE_STATUS_PROCESS ssStatus = {0};
	DWORD dwBytesNeeded = 0;
	DWORD dwOldCheckPoint; 
	DWORD dwStartTickCount;
	DWORD dwWaitTime;

	if (!QueryServiceStatusEx( 
		sc_Service,                     //handle to service 
		SC_STATUS_PROCESS_INFO,         //info level
		(LPBYTE) &ssStatus,            // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )             // if buffer too small

	{
		MYLOG(_T("QueryServiceStatusEx failed (%ld)\n"), GetLastError());
		return FALSE; 
	} 
	// Save the tick count and initial checkpoint. 
	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint; 

	// Wait for the service to stop before attempting to start it. 
	while (ssStatus.dwCurrentState == dwCurrentState)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds.  
		dwWaitTime = ssStatus.dwWaitHint / 10; 
		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000; 

		Sleep( dwWaitTime ); 
		// Check the status until the service is no longer stop pending.  
		if ( !QueryServiceStatusEx( 
			sc_Service,                     // handle to service 
			SC_STATUS_PROCESS_INFO,         // information level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS),// size of structure
			&dwBytesNeeded ) )              // size needed if buffer is toosmall
		{
			MYLOG(_T("QueryServiceStatusEx failed (%ld)\n"), GetLastError());
			return FALSE; 
		} 

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// Continue to wait and check. 
			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()- dwStartTickCount > ssStatus.dwWaitHint)
			{
				MYLOG(_T("Timeout waitingfor service to stop\n"));
				return FALSE;  
			}
		}
	} 

	return TRUE;
}

BOOL StartMifiService(SC_HANDLE sc_Service)
{
	SERVICE_STATUS_PROCESS ssStatus = {0}; 

	DWORD dwBytesNeeded; 

	// Get a handle to the SCM database.  

	if (NULL == sc_Service) 
	{
		MYLOG(_T("OpenService failed (%ld)\n"),GetLastError());
		return FALSE;
	}   

	// Check the status in case the service is not stopped.  
	if (!QueryServiceStatusEx( 
		sc_Service,                     //handle to service 
		SC_STATUS_PROCESS_INFO,         //information level
		(LPBYTE) &ssStatus,            // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )             // size needed if buffer is too small
	{
		MYLOG(_T("QueryServiceStatusEx failed (%ld)\n"), GetLastError());
		return FALSE; 
	} 

	// Check if the service is already running. It would be possible 
	// to stop the service here, but for simplicity this example justreturns.  

	if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)

	{
		MYLOG(_T("Cannot start the service because it is alreadyrunning\n"));
		return TRUE; 
	} 

	if ( !GetQueryMifiServiceStatus(sc_Service,SERVICE_STOP_PENDING) )
	{
		return FALSE;
	}


	// Attempt to start the service. 
	if (!StartService(
		sc_Service,  // handle to service 
		0,           // number ofarguments 
		NULL) )      // no arguments 
	{
		MYLOG(_T("StartService failed (%ld)\n"), GetLastError());
		return FALSE;
	}
	else 
	{
		MYLOG(_T("Service start pending...\n")); 
	} 

	Sleep(200);
	// Determine whether the service is running. 
	// Check the status in case the service is not stopped.  
	if (!QueryServiceStatusEx( 
		sc_Service,                     //handle to service 
		SC_STATUS_PROCESS_INFO,         //information level
		(LPBYTE) &ssStatus,            // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded ) )             // size needed if buffer is too small
	{
		MYLOG(_T("QueryServiceStatusEx failed (%ld)\n"), GetLastError());
		return FALSE; 
	} 

	// Check if the service is already running. It would be possible 
	// to stop the service here, but for simplicity this example justreturns.  

	if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)

	{
		MYLOG(_T("Cannot start the service because it is alreadyrunning\n"));
		return TRUE; 
	} 
// 	if ( GetQueryMifiServiceStatus(sc_Service,SERVICE_RUNNING) ) 
// 	{
// 		MYLOG(_T("Service started successfully.\n")); 
// 	}
 	else 
 	{
 		MYLOG(_T("Service not started. \n"));
 		MYLOG(_T("  Current State:%ld\n"), ssStatus.dwCurrentState); 
 		MYLOG(_T("  Exit Code:%ld\n"), ssStatus.dwWin32ExitCode); 
 		MYLOG(_T("  Check Point:%ld\n"), ssStatus.dwCheckPoint); 
 		MYLOG(_T("  Wait Hint:%ld\n"), ssStatus.dwWaitHint); 
 		return FALSE;
 	} 

	return TRUE;
}

BOOL DeleteMifiService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	// Get a handle to the SCM database. 
	//LoadConfigInfo();
	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL ==schSCManager)
	{
		MYLOG(_T("OpenSCManager failed (%ld)\n"), GetLastError());
		return FALSE;
	} 

	// Get a handle to the service. 
	schService = OpenService(
		schSCManager,       // SCM database
		g_configInfo.m_szService_Name,          // name of service
		DELETE);            // need delete access 

	if (schService == NULL)
	{
		MYLOG(_T("OpenService failed (%ld)\n"), GetLastError());
		CloseServiceHandle(schSCManager);
		return FALSE;
	} 

	// Delete the service. 

	if (!DeleteService(schService) )
	{
		MYLOG(_T("DeleteService failed (%ld)\n"), GetLastError());
	}

	else 
	{
		MYLOG(_T("Service deletedsuccessfully\n")); 
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return TRUE;
}

#undef AUTORUN_EWALK_CPP