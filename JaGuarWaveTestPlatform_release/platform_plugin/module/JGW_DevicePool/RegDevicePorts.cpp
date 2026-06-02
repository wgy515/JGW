#include "StdAfx.h"
#include "RegDevicePorts.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{

	CRegDevicePorts::CRegDevicePorts(void)
	{
	}


	CRegDevicePorts::~CRegDevicePorts(void)
	{
	}

	bool CRegDevicePorts::GetModemComport(const wchar_t* strRegUsbPidVid,int& nComPort)
	{
		return GetComport(strRegUsbPidVid,nComPort,L"Modem");
	}

	bool CRegDevicePorts::GetDiagDownLoadComport(const wchar_t* strRegUsbPidVid,int& nComPort)
	{
		return GetComport(strRegUsbPidVid,nComPort,L"QDLoader");
	}

	bool CRegDevicePorts::GetDiagFactoryComport(const wchar_t* strRegUsbPidVid,int& nComPort)
	{
		return GetComport(strRegUsbPidVid,nComPort,L"Diag");
	}

	//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\VID_05C6&PID_9091&MI_00\6&33bf1261&0&0000\Device Parameters\PortName
	bool CRegDevicePorts::GetComport(const wchar_t* strRegUsbPidVid,int& nComport,const wchar_t* strComportName)
	{
		std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
		DWORD			dwType = REG_SZ;
		TCHAR			byData [MAX_PATH ]= {0};
		DWORD			dwSize = 260;
		HKEY			m_hKey = NULL;
		TCHAR			szTemp[MAX_PATH] = {0};

		strSubKey += strRegUsbPidVid;
		if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
		{
			return false;
		}

		if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("DeviceDesc"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
		{
			RegCloseKey(m_hKey);
			return false;
		}

		if( !wstrcasewstr(byData,strComportName) )
		{
#ifdef _DEBUG
			Log4WE_F(L"GetComport fail %s",byData);
#endif
			RegCloseKey(m_hKey);
			return false;
		}
		RegCloseKey(m_hKey);

		strSubKey += L"\\Device Parameters";
		if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
		{
			return false;
		}
		memset(byData,0x00,260*2);
		dwSize = 260;
		if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("PortName"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
		{
			RegCloseKey(m_hKey);
			return false;
		}

		RegCloseKey(m_hKey);

		if ( dwSize > 3 )
		{
			nComport = _ttoi(byData + 3);
#ifdef _DEBUG
			Log4WD_F(L"ComPort%d: [%s]",nComport,strRegUsbPidVid);
#endif
			return true;
		}

		return false;
	}
}