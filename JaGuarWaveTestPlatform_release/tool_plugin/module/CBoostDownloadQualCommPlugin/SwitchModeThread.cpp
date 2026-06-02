#include "StdAfx.h"
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include "SwitchModeThread.h"
#include "DUComm.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

#define DELETEPOINTER(x)\
	if( (x) )\
	{\
	delete[] x;\
	x = NULL;\
	}

namespace JGW
{

	CSwitchModeThread::CSwitchModeThread(void):m_strRegUsbPidVid(NULL)
		,m_nSerialNumberSize(260)
	{
	}


	CSwitchModeThread::~CSwitchModeThread(void)
	{
	}

	BEGIN_MYTHREAD_MESSAGE_MAP(CSwitchModeThread,CCMessageThread)
		ON_MYTHREAD_MESSGAE(SWITCH_EDL_MODE_THREAD_MSG,&CSwitchModeThread::OnDialogSwitchEdl)
		ON_MYTHREAD_MESSGAE(SWITCH_FASTOOT_MODE_THREAD_MSG,&CSwitchModeThread::OnFactoryModeMsg)
		ON_MYTHREAD_MESSGAE(SWITCH_DOWNLOAD_MODE_THREAD_MSG,&CSwitchModeThread::OnFastbootModeMsg)
	END_MYTHREAD_MESSAGE_MAP()

	void ClearDevice(SDeviceMsg_Info* lpDevice)
	{
		lpDevice->Dec();
	}

	void CSwitchModeThread::OnFactoryModeMsg(WPARAM wParam,LPARAM lParam)
	{
		if( !wParam ) return ;

		SDeviceMsg_Info* lpdevice = (SDeviceMsg_Info*)wParam;
		std::shared_ptr<SDeviceMsg_Info> ps(lpdevice,ClearDevice);
		m_strRegUsbPidVid = ps->m_strRegUsbPidVid.c_str();

		if ( !g_sCacheConfigInfo.m_bAutoSwitchComport || !m_strRegUsbPidVid)
		{
			return ;
		}

		if ( !CheckUsbVidIsAdbMsg() )
		{
			return ;
		}

		int i = 0;
		for (;i< 3;i++)
		{
			if ( CheckAdbPort(/*strSerialNumber*/) )
			{
				break;
			}
			Sleep(1000);
		}

		if(i >= 3)	
		{
			return ;
		}

		SwitchFastboot(/*strSerialNumber*/);
	}

	bool CSwitchModeThread::CheckUsbVidIsAdbMsg()
	{
		std::wstring		strError = _T("");
		std::wstring		strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
		DWORD		dwType = REG_SZ;
		TCHAR		byData [MAX_PATH ]= {0};
		DWORD		dwSize = 260;
		HKEY		m_hKey = NULL;

		strSubKey += m_strRegUsbPidVid;
		if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
		{
			strError = _T("Open Reg Fail!");
			RegCloseKey(m_hKey);
			return false;
		}

		if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("Class"), NULL, &dwType, (LPBYTE)byData, &dwSize) )
		{
			strError = _T("RegQueryValueEx Fail!");
			RegCloseKey(m_hKey);
			return false;
		}
		strError = byData;

		if (0 != strError.compare(_T("AndroidUsbDeviceClass")))
		{
			RegCloseKey(m_hKey);
			return false;
		}

		RegCloseKey(m_hKey);
		return true;
	}


	bool CSwitchModeThread::CheckAdbPort(/*CString &strSerialNumber*/)
	{
		memset(m_szSerialNumber,0x00,260);
		m_nSerialNumberSize = MAX_PATH;
		//	GetSerialNumber(m_strRegUsbPidVid,m_szSerialNumber,m_nSerialNumberSize);

		return m_nSerialNumberSize>2?true:false;
	}

	bool CSwitchModeThread::SwitchFastboot(/*CString strSerialNumber*/)
	{
		if ( wcslen(m_szSerialNumber) < 2) return false;
		std::wstring strTemp = _T("");
		m_cPeekNamePide.InitPideCMD();
		m_cPeekNamePide.ReadPideFile(strTemp);
		FormatWString(strTemp,g_szGetState,m_szSerialNumber);

		if ( !m_cPeekNamePide.WrPideFile(/*_T("adb get-state")*/strTemp,_T("device"),3))
		{
			m_cPeekNamePide.ClosePideHandle();
			return false;
		}
		FormatWString(strTemp,g_szRebootbooloader,m_szSerialNumber);
		m_cPeekNamePide.WritePideFile(strTemp.c_str(),strTemp.length());
		m_cPeekNamePide.ReadPideFile(strTemp);

		m_cPeekNamePide.ClosePideHandle();

		return true;
	}

	void CSwitchModeThread::OnFastbootModeMsg(WPARAM wParam,LPARAM lParam)
	{
		if( !wParam ) return ;

		SDeviceMsg_Info* lpdevice = (SDeviceMsg_Info*)wParam;
		std::shared_ptr<SDeviceMsg_Info> ps(lpdevice,ClearDevice);
		m_strRegUsbPidVid = ps->m_strRegUsbPidVid.c_str();

		if ( !g_sCacheConfigInfo.m_bAutoSwitchComport || !m_strRegUsbPidVid)
		{
			return ;
		}

		for (int i = 0;i< 3;i++)
		{
			if ( CheckAdbPort(/*strSerialNumber*/) )
			{
				break;
			}
			Sleep(1000);
		}

		SwitchDownLoadMode();
	}

	bool CSwitchModeThread::SwitchDownLoadMode(/*CString strSerialNumber*/)
	{
		if ( wcslen(m_szSerialNumber) < 2) return false;
		std::wstring strTemp = _T("");
		m_cPeekNamePide.InitPideCMD();
		m_cPeekNamePide.ReadPideFile(strTemp);

		if( !CheckDeviceState() )
		{
			m_cPeekNamePide.ClosePideHandle();
			return false;
		}
		FormatWString(m_strRead,g_szFasterasePartion,L"sbl1",m_szSerialNumber);
		m_cPeekNamePide.WrPideFile(m_strRead.c_str(),_T("okay"));
		OutputDebugString(m_strRead.c_str());
		FormatWString(m_strRead, g_szFastbootReboot , m_szSerialNumber );
		m_cPeekNamePide.WrPideFile(m_strRead.c_str(),_T("rebooting"));
		m_cPeekNamePide.ClosePideHandle();
		return true;
	}

	bool CSwitchModeThread::CheckDeviceState()
	{
		m_cPeekNamePide.SetTimeoutSecond(20);
		//获取fastdevice状态
		for (int nIndex = 0;nIndex < 3;nIndex ++)
		{
			if ( GetFastbootDeviceState() )
			{
				return true;
			}
			Sleep(1000);
		}
		return false;
	}

	bool CSwitchModeThread::GetFastbootDeviceState()
	{
		//检查设备当前模式是否为fastboot模式
		std::wstring strTemp(_T(""));
		m_strRead = _T("");
		FormatWString(strTemp,g_szFastbootDevices,m_szSerialNumber);

		if ( m_cPeekNamePide.WrPideFile(strTemp.c_str(),m_strRead) )
		{
			JGW_ReplaceStringW(m_strRead,strTemp,_T(""));

			if ( -1 != m_strRead.find(m_szSerialNumber))
			{
				return true;
			}
		}

		return false;
	}

	void CSwitchModeThread::OnDialogSwitchEdl(WPARAM wParam,LPARAM lParam)
	{
		if( !wParam ) return ;
		SDeviceMsg_Info* lpdevice = (SDeviceMsg_Info*)wParam;
		std::shared_ptr<SDeviceMsg_Info> ps(lpdevice,ClearDevice);
		m_strRegUsbPidVid = ps->m_strRegUsbPidVid.c_str();

		if ( !g_sCacheConfigInfo.m_bAutoSwitchComport || !m_strRegUsbPidVid)
		{
			return ;
		}

		if ( !CheckPidVidIsDialog() )
		{
			return;
		}

		int		nPort = 0;
		if ( !GetDialogComPort(nPort) )
		{
			return;
		}

		if ( !ConnectServer(nPort) )
		{
			return;
		}


		SwitchEdl();
		return ;
	}

	bool CSwitchModeThread::CheckPidVidIsDialog()
	{
		std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
		DWORD			dwType = REG_SZ;
		TCHAR			byData [MAX_PATH ]= {0};
		DWORD			dwSize = 260;
		HKEY			m_hKey = NULL;
		TCHAR			szTemp[MAX_PATH] = {0};

		strSubKey += m_strRegUsbPidVid;
		//strSubKey += L"\\Device Parameters";

		if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
		{
			RegCloseKey(m_hKey);
			return false;
		}

		if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("DeviceDesc"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
		{
			RegCloseKey(m_hKey);
			return false;
		}
		RegCloseKey(m_hKey);
		strSubKey = byData;

		if ( NULL != wstrcasewstr(strSubKey.c_str(),L"Diag")/*std::wstring::npos != strSubKey.find(L"Diag")*/ )
		{
			return true;
		}

		return false;
	}

	bool CSwitchModeThread::GetDialogComPort(int& nPort)
	{
		std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
		DWORD			dwType = REG_SZ;
		TCHAR			byData [MAX_PATH ]= {0};
		DWORD			dwSize = 260;
		HKEY			m_hKey = NULL;
		TCHAR			szTemp[MAX_PATH] = {0};

		strSubKey += m_strRegUsbPidVid;
		strSubKey += L"\\Device Parameters";

		if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
		{
			RegCloseKey(m_hKey);
			return false;
		}

		if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("PortName"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
		{
			RegCloseKey(m_hKey);
			return false;
		}

		RegCloseKey(m_hKey);

		if ( dwSize > 3 )
		{
			nPort = _ttoi(byData + 3);
			return true;
		}

		return false;
	}

	bool CSwitchModeThread::ConnectServer(int nPort)
	{
		return m_cwrnv.ConnectPhone(1,nPort);
	}

	bool CSwitchModeThread::SwitchEdl()
	{
		m_cwrnv.SwitchPhoneEDL();/*QLIB_QPHONEMS_SwitchToEDL(m_hResourceContext)*/
		m_cwrnv.DisconnectServer();
		return true;
	}
}