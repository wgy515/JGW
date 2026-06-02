#pragma once
#include "AutoUsbDefine.h"

class CUsbQueryBase
{
public:
	CUsbQueryBase(void );
	~ CUsbQueryBase(void );

public:
	//! 清除所有设备信息
	void                        ClearDeviceAllSortInfo();
	//! 显示指定hub port信息
	void                        ShowDeviceInfoToHubPortInfo(PHUBPORTPOINTINFO lpHubPortInfo);
	//! 显示所有已获得设备信息
	void                        ShowDeviceAllInfo();
	//! 过滤未指定的PID VID
	bool                        FilterPidVid(PUSB_NODE_CONNECTION_INFORMATION connectionInfo);
	//! 打开指定HCD索引获取 roothub名
	HANDLE                      OpenHostControllersToHCDIndex( int nHCDIndex );
	//！ 打开指定 hub,获取当前hub 所挂载端口
	HANDLE                      GetHubPortsCountToHubName(LPTSTR strHunName,int &nHubPortsCounts, PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo = NULL );
	//! 获取根Hub 名
	PUSB_ROOT_HUB_NAME          GetRootHubName( HANDLE HostController ); 
	//! 获取指定设备Driver键值
	PUSB_NODE_CONNECTION_DRIVERKEY_NAME GetDriverKeyName( HANDLE Hub , ULONG ConnectionIndex ); 
	//bool                            DriverNameToDeviceDesc( LPTSTR DriverName ,LPTSTR strDeviceNameDesc);//获取设备描述符
	//! 获取设备描述符以及设备范例 ID
	LPPORTINSTANCEINFO          DriverNameToDeviceDesc( LPTSTR DriverName ,LPTSTR strDeviceNameDesc , LPPORTINSTANCEINFO lpParam = NULL );
	//! 获取 HCD主控名
	bool                        GetHCDDriverKeyName( HANDLE HCD ,LPTSTR pszDriverKeyName );
	//! 获取设备集合中指定设备端口信息
	PHUBPORTPOINTINFO			GetEnumHubCollectionInfoToPhysicalAddress ( std::wstring strPhysicalAddress = _T("") );
	//! 获取外部Hub名
	PUSB_NODE_CONNECTION_NAME   GetExternalHubName( HANDLE Hub ,ULONG ConnectionIndex );
	//! 查找当前主控下子端口范例 ID信息
	bool                        EnumChildHubPortControl( DEVINST dev ,LPPORTINSTANCEINFO lpParam );
protected:
	std::vector <PIDANDVIDINFO>        m_vFilterPidVid;
};