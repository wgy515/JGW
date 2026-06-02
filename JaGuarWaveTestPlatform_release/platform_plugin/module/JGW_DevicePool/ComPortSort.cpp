#include "StdAfx.h"
#include "ComPortSort.h"
#include "usb_host/AutoUsbDefine.h"
#include <JGW_DevicePool/DevicePortDefine.h>

extern int g_nUserPortSort;

CComPortSort::CComPortSort(void)
{
	mstrIniPath = GetFilterIniConfigPath();
	LoadSortPortInfoToIniConfig();
}


CComPortSort::~CComPortSort(void)
{
}

void CComPortSort::LoadSortPortInfoToIniConfig()
{
	int nSortCount = GetPrivateProfileInt(_T("MAIN" ),_T( "DevCounts"),0,mstrIniPath );
	TCHAR szAppName[100] = {0};
	int port = 0;

	for ( int i = 1; i <= nSortCount; i ++ )
	{
		_stprintf_s(szAppName ,100,_T( "DEV_%d"),i);
		port = GetPrivateProfileInt(_T("MAIN" ),_T("Port"),0,mstrIniPath );
		DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] CComPortSort::LoadSortPortInfoToIniConfig DEV_%d Port=%d",i,port);
		mport_index_map[port] = i - 1;
	}
}

int CComPortSort::GetSortIndexToComPort( int port )
{
	if ( port < 1 ) return -1;
	//! 当存在当前排序索引直接返回索引 如果不存在则修改配置文件增加索引
	if ( mport_index_map.end() != mport_index_map.find( port ) )
	{
		DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] CComPortSort::GetSortIndexToComPort DEV_%d Port=%d",mport_index_map[port],port);
		return mport_index_map[port];
	}
	return UpdatePortInfoToIniConfig(port);
}

int CComPortSort::UpdatePortInfoToIniConfig( int port )
{
	TCHAR szAppName[100] = {0};
	TCHAR szTemp[100] = {0};
	int nSortCounts = GetPrivateProfileInt(_T("MAIN" ),_T( "DevCounts"),0,mstrIniPath ) + 1;
	mport_index_map[port] = nSortCounts;
	
	_stprintf_s(szTemp ,100,_T( "%d"),nSortCounts);
	WritePrivateProfileString(_T("MAIN" ),_T( "DevCounts"),szTemp,mstrIniPath);
	_stprintf_s(szAppName ,100,_T( "DEV_%d"),nSortCounts);
	_stprintf_s(szTemp ,100,_T( "%d"),port);
	WritePrivateProfileString(szAppName,_T( "Port"),szTemp,mstrIniPath);
	DEVICEPOOL_LOG_INFO_FORMAT(L"[DevicePool] CComPortSort::UpdatePortInfoToIniConfig DEV_%d Port=%d",nSortCounts,port);

	return nSortCounts;
}

int GetSortIndexToComport( int port )
{
	static CComPortSort comport_sort;
	return comport_sort.GetSortIndexToComPort( port ) - 1;
}