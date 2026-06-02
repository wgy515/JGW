#include "StdAfx.h"
#include "UsbQueryBase.h"
#include <JGW_DevicePool/DevicePortDefine.h>

CUsbQueryBase:: CUsbQueryBase(void )
{
}


CUsbQueryBase::~CUsbQueryBase (void)
{

}


void CUsbQueryBase ::ClearDeviceAllSortInfo()
{
	for ( _vectorSort::iterator it = g_vSortPortInfo .begin(); it != g_vSortPortInfo.end();++it)
	{
		delete it[0] ;
	}
	g_vSortPortInfo.clear ();
}

void CUsbQueryBase ::ShowDeviceAllInfo()
{
	for ( _vectorSort:: iterator it = g_vSortPortInfo .begin(); it != g_vSortPortInfo .end(); ++it )
	{
		ShowDeviceInfoToHubPortInfo(it[0]);
	}
}

void CUsbQueryBase::ShowDeviceInfoToHubPortInfo( PHUBPORTPOINTINFO lpHubPortInfo )
{
	DEVICEPOOL_LOG_INFO_FORMAT(_T("PhysicalAddress : %s"),lpHubPortInfo->m_strPhysicalAddress.c_str());
	DEVICEPOOL_LOG_INFO_FORMAT(L"InstanceID : %s",lpHubPortInfo->m_sPortInstanceInfo.m_szInstanceID.c_str());
	for (USHORT i = 0; i < lpHubPortInfo ->m_sPortInstanceInfo.m_vChildInstanceID.size(); i ++)
	{
		DEVICEPOOL_LOG_INFO_FORMAT(_T("ChildInstanceID %s"),lpHubPortInfo-> m_sPortInstanceInfo.m_vChildInstanceID.at(i).c_str());
	}
}

PHUBPORTPOINTINFO CUsbQueryBase ::GetEnumHubCollectionInfoToPhysicalAddress( std::wstring strPhysicalAddress )
{
	for (_vectorSort::iterator it = g_vSortPortInfo .begin(); it != g_vSortPortInfo.end();++it)
	{
		if ( strPhysicalAddress == it[0]->m_strPhysicalAddress )
			return it[0];
	}
	PHUBPORTPOINTINFO lpTemp = new HUBPORTPOINTINFO;
	g_vSortPortInfo.push_back (lpTemp);
	return lpTemp ;
}

HANDLE CUsbQueryBase ::OpenHostControllersToHCDIndex( int nHCDIndex )
{
	TCHAR HCName[MAX_PATH ] = {0}; //主控名
	_stprintf_s(HCName ,MAX_PATH, _T("\\\\.\\HCD%d" ), nHCDIndex);
	// 如果句柄有效,则成功打开了一个主控制器显示主控制器本身的信息 ,然后枚举该主控制器上附有的根 Hub.
	return CreateFile( HCName,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_EXISTING,0, NULL );
}
//! 获取hub端口数据
HANDLE CUsbQueryBase ::GetHubPortsCountToHubName( LPTSTR strHunName ,int &nHubPortsCounts,PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo)
{
	// 初始化局部变量到没分配内存状态以免错误的清除
	HANDLE					hHubDevice = INVALID_HANDLE_VALUE;
	TCHAR					deviceName[ MAX_PATH] = {0};
	// 分配USBDEVICEINFO 结构空间
	USBDEVICEINFO			info = {0};
	DWORD					nBytes = 0;
	USB_NODE_INFORMATION	usbHubInfo;
	memset(&usbHubInfo,0x00,sizeof(USB_NODE_INFORMATION));
	// 保存到info 中
	info.HubName = strHunName;
	info.ConnectionInfo = ConnectionInfo;
	info.ConfigDesc = NULL;
	info.StringDescs = NULL;
	// 发配USB_NODE_INFORMATION 结构空间
	info.HubInfo = &usbHubInfo/*(PUSB_NODE_INFORMATION) new CHAR[sizeof (USB_NODE_INFORMATION)]*/;

	// 产生全hub 设备名
	_stprintf_s(deviceName ,MAX_PATH, _T("\\\\.\\%s" ), strHunName);
	//! _tcscpy_s(deviceName + sizeof( "\\\\.\\") - 1, MAX_PATH - sizeof( "\\\\.\\" ),info. HubName);
	// 打开HUB 设备
	hHubDevice = CreateFile (deviceName, GENERIC_WRITE,FILE_SHARE_WRITE ,
		NULL,OPEN_EXISTING ,0,NULL);

	if ( INVALID_HANDLE_VALUE == hHubDevice ) goto EnumerateHubError ;

	// 现在查询该HUB USB_NODE_INFORMATION结构的 USBHUB.
	// 得到需枚举的端口数及其他
	if ( !DeviceIoControl (hHubDevice ,
		IOCTL_USB_GET_NODE_INFORMATION,
		info.HubInfo ,
		sizeof(USB_NODE_INFORMATION ),
		info.HubInfo ,
		sizeof(USB_NODE_INFORMATION ),
		& nBytes,
		NULL)
		)
	{
		goto EnumerateHubError ;
	}

	nHubPortsCounts = info.HubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts;
	return hHubDevice ;
	// 错误
EnumerateHubError:
	if ( INVALID_HANDLE_VALUE != hHubDevice ) CloseHandle(hHubDevice );
	return INVALID_HANDLE_VALUE ;
}

// 取根HUB 名称
PUSB_ROOT_HUB_NAME CUsbQueryBase::GetRootHubName( HANDLE HostController )
{
	ULONG               nBytes = 0;
	USB_ROOT_HUB_NAME   rootHubName = {0};
	PUSB_ROOT_HUB_NAME  pUsbRootHubName = NULL;

	// 取Host控制器上Root Hub名称的长度
	if ( !DeviceIoControl (HostController ,IOCTL_USB_GET_ROOT_HUB_NAME,0,0,& rootHubName ,
		sizeof(rootHubName ),&nBytes, NULL) ) goto GetRootHubNameError ;

	// 分配内存
	nBytes = rootHubName.ActualLength;
	pUsbRootHubName = (USB_ROOT_HUB_NAME*)new CHAR[nBytes];
	if ( !pUsbRootHubName ) goto GetRootHubNameError ;
	memset(pUsbRootHubName,0x00,nBytes);

	// 取Host 控制器上Root Hub的名称
	if ( !DeviceIoControl (HostController ,IOCTL_USB_GET_ROOT_HUB_NAME, NULL ,
		0, pUsbRootHubName,nBytes ,&nBytes, NULL) ) goto GetRootHubNameError ;

	return pUsbRootHubName ;
	// 错误
GetRootHubNameError:
	if ( pUsbRootHubName ) delete [] pUsbRootHubName ;
	return NULL ;
}


PUSB_NODE_CONNECTION_DRIVERKEY_NAME CUsbQueryBase ::GetDriverKeyName( HANDLE Hub ,ULONG ConnectionIndex )
{
	ULONG                               nBytes;
	USB_NODE_CONNECTION_DRIVERKEY_NAME  usb_Node_Connection_DriverKey_Name = {0};
	PUSB_NODE_CONNECTION_DRIVERKEY_NAME pUsbNodeConnectionDriverKeyName = NULL;
	//driverKeyNameA = NULL;
	// 取指定口的driver key名称的长度
	usb_Node_Connection_DriverKey_Name.ConnectionIndex = ConnectionIndex;

	if ( !DeviceIoControl (Hub ,IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME ,
		& usb_Node_Connection_DriverKey_Name,sizeof (usb_Node_Connection_DriverKey_Name),
		& usb_Node_Connection_DriverKey_Name,sizeof (usb_Node_Connection_DriverKey_Name),& nBytes,NULL ) )
		goto GetDriverKeyNameError ;

	// 分配内存
	nBytes = usb_Node_Connection_DriverKey_Name.ActualLength;
	if ( nBytes <= sizeof( usb_Node_Connection_DriverKey_Name) ) goto GetDriverKeyNameError ;

	pUsbNodeConnectionDriverKeyName = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME )new CHAR[nBytes];
	if ( !pUsbNodeConnectionDriverKeyName ) goto GetDriverKeyNameError;
	memset(pUsbNodeConnectionDriverKeyName,0x00,nBytes);

	// 取指定口的driver key名称
	pUsbNodeConnectionDriverKeyName->ConnectionIndex = ConnectionIndex;
	if ( !DeviceIoControl (Hub ,IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME ,
		pUsbNodeConnectionDriverKeyName,nBytes ,pUsbNodeConnectionDriverKeyName, nBytes,&nBytes ,NULL) ) goto GetDriverKeyNameError ;

	return pUsbNodeConnectionDriverKeyName ;
	// 错误
GetDriverKeyNameError:
	// 有错误, 释放内存
	if ( pUsbNodeConnectionDriverKeyName ) delete[] pUsbNodeConnectionDriverKeyName ;
	return NULL;
}

LPPORTINSTANCEINFO CUsbQueryBase ::DriverNameToDeviceDesc( LPTSTR DriverName ,LPTSTR strDeviceNameDesc , LPPORTINSTANCEINFO psPortInstanceInfo /* = NULL */ )
{
	if ( !psPortInstanceInfo ) psPortInstanceInfo = new SPORTINSTANCEINFO;
	psPortInstanceInfo->Init();
	DEVINST     devInst ;
	DEVINST     devInstNext ;
	CONFIGRET   cr ;
	ULONG       len ;
	ULONG       walkDone = 0;

	// 取根节点DevNode
	cr = CM_Locate_DevNode (&devInst, NULL,0);
	TESTRETURNOK(cr == CR_SUCCESS );

	// 深一步搜索与DriverName匹配的 DevNode
	while(!walkDone )
	{ // 取DriverName
		len = MAX_PATH ;
		cr = CM_Get_DevNode_Registry_Property (devInst, CM_DRP_DRIVER,NULL ,strDeviceNameDesc,&len,0);
		// 如果与DriverName 匹配,返回设备描述符

		if ( cr == CR_SUCCESS && _tcscmp( DriverName ,strDeviceNameDesc ) == 0)
		{
			len = MAX_PATH ;
			cr = CM_Get_DevNode_Registry_Property (devInst, CM_DRP_DEVICEDESC,NULL ,strDeviceNameDesc,&len,0);
			TCHAR szBuffer [MAX_PATH] = {0};
			if (CM_Get_Device_ID (devInst, szBuffer,260,NULL ) == CR_SUCCESS)
			{
				psPortInstanceInfo->m_szInstanceID = szBuffer;
				EnumChildHubPortControl(devInst ,psPortInstanceInfo);
			}
			if (cr == CR_SUCCESS ) return psPortInstanceInfo ;
			break;
		}
		// 该DevNode 不匹配,再深一级到第一个子节点
		cr = CM_Get_Child (&devInstNext, devInst,0);
		if ( cr == CR_SUCCESS )
		{
			devInst = devInstNext ;
			continue;
		}
		// 不能再往下,就到兄弟节点如果没兄弟 ,朝上一级直到有兄弟
		// 如果不能再朝上,那么到了根节点，也就做完了
		for (;;)
		{
			cr = CM_Get_Sibling (&devInstNext, devInst,0);
			if (cr == CR_SUCCESS )
			{
				devInst = devInstNext ;
				break;
			}
			cr = CM_Get_Parent (&devInstNext, devInst,0);
			if (cr == CR_SUCCESS )
			{
				devInst = devInstNext ;
			}
			else
			{
				walkDone = 1;
				break;
			}
		}
	}
	//
	return NULL ;
}

bool CUsbQueryBase ::EnumChildHubPortControl( DEVINST dev,LPPORTINSTANCEINFO lpParam )
{
	//static std ::wstring strChildInstanceID;
	static TCHAR szBuffer[ MAX_PATH] = {0};
	DEVINST     devInstNext = NULL;
	CONFIGRET   cr = CR_SUCCESS;
	ULONG       len = 0;
	ULONG       walkDone = 0;

	cr = CM_Get_Child (&devInstNext, dev,0);
	if (cr != CR_SUCCESS )
	{
		return false ;
	}
	dev = devInstNext ;

	// 深一步搜索与DriverName匹配的 DevNode
	while(!walkDone )
	{ // 取DriverName

		if (CM_Get_Device_ID (dev, szBuffer,260,NULL ) == CR_SUCCESS)
		{
			//strChildInstanceID = szBuffer ;
			lpParam->m_vChildInstanceID.push_back( szBuffer );
		}

		cr = CM_Get_Sibling (&devInstNext, dev,0);
		if (cr != CR_SUCCESS )
		{
			return false ;
		}
		dev = devInstNext ;
	}
	return true ;
}

// 取外部HUB 名称
PUSB_NODE_CONNECTION_NAME CUsbQueryBase::GetExternalHubName( HANDLE Hub ,ULONG ConnectionIndex )
{
	BOOL                        success;
	ULONG                       nBytes;
	USB_NODE_CONNECTION_NAME    extHubName = {0};
	PUSB_NODE_CONNECTION_NAME	pExtUsbNodeConnentionName = NULL;
	//! success = DeviceIoControl (HostController ,IOCTL_USB_GET_ROOT_HUB_NAME,0,0,& rootHubName ,sizeof(rootHubName ),&nBytes, NULL);
	// 取指定口上外部HUB的名称的长度
	extHubName.ConnectionIndex = ConnectionIndex;
	success = DeviceIoControl (Hub ,IOCTL_USB_GET_NODE_CONNECTION_NAME ,&extHubName, sizeof(extHubName ),
		&extHubName,sizeof(extHubName),&nBytes,NULL );
	if (!success ) goto GetExternalHubNameError ;

	// 分配内存
	nBytes = extHubName.ActualLength;
	if ( nBytes <= sizeof( extHubName ) ) goto GetExternalHubNameError ;

	pExtUsbNodeConnentionName = (PUSB_NODE_CONNECTION_NAME) new CHAR[nBytes];
	if ( !pExtUsbNodeConnentionName ) goto GetExternalHubNameError ;

	// 取名称
	pExtUsbNodeConnentionName->ConnectionIndex = ConnectionIndex;
	success = DeviceIoControl (Hub,IOCTL_USB_GET_NODE_CONNECTION_NAME,pExtUsbNodeConnentionName,nBytes,pExtUsbNodeConnentionName,nBytes ,&nBytes, NULL);
	if (!success ) goto GetExternalHubNameError ;
	// 变换名称
	//extHubNameA = WideStrToMultiStr(extHubNameW->NodeName);
	// 完成, 释放未发现的 driver key名称并返回发现的driver key名称
	return pExtUsbNodeConnentionName ;
	// 错误
GetExternalHubNameError:
	if ( pExtUsbNodeConnentionName ) delete[] pExtUsbNodeConnentionName ;
	return NULL ;
}


// 取Host Control 键名
bool CUsbQueryBase ::GetHCDDriverKeyName( HANDLE HCD ,LPTSTR pszDriverKeyName)
{
	BOOL                    success;
	ULONG                   nBytes;
	USB_HCD_DRIVERKEY_NAME  driverKeyName ;
	PUSB_HCD_DRIVERKEY_NAME driverKeyNameW ;
	//PCHAR                   driverKeyNameA;

	driverKeyNameW = NULL ;
	//driverKeyNameA = NULL;
	// 取HCD 的driver key名称的长度
	success = DeviceIoControl (HCD , IOCTL_GET_HCD_DRIVERKEY_NAME,& driverKeyName,sizeof (driverKeyName),
		& driverKeyName,sizeof (driverKeyName),& nBytes,NULL );
	if (!success )
	{
		//AfxMessageBox("Oop23!");
		goto GetHCDDriverKeyNameError ;
	}
	// 分配内存
	nBytes = driverKeyName .ActualLength;
	if (nBytes <= sizeof( driverKeyName))
	{
		//AfxMessageBox("Oop24!");
		goto GetHCDDriverKeyNameError ;
	}
	driverKeyNameW = new USB_HCD_DRIVERKEY_NAME[ nBytes];
	if (driverKeyNameW == NULL)
	{
		//AfxMessageBox("Oop25!");
		goto GetHCDDriverKeyNameError ;
	}
	// 取设备指定口的driver key名称
	success = DeviceIoControl (HCD , IOCTL_GET_HCD_DRIVERKEY_NAME, driverKeyNameW,nBytes ,driverKeyNameW,
		nBytes,&nBytes ,NULL);
	if (!success )
	{
		//AfxMessageBox("Oop26!");
		goto GetHCDDriverKeyNameError ;
	}
	//*pszDriverKeyName = new TCHAR[driverKeyNameW->ActualLength + 1];
	_tcscpy_s(pszDriverKeyName ,driverKeyNameW-> ActualLength + 1,driverKeyNameW->DriverKeyName );
	// 变换名称
	//driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName);
	// 完成, 释放没找到的 driver key名称并返回找到的driver key名称
	delete [] driverKeyNameW ;

	return true ;
	// 错误
GetHCDDriverKeyNameError :
	if (driverKeyNameW != NULL)
	{
		delete [] driverKeyNameW ;
		driverKeyNameW = NULL ;
	}
	return false ;
}

bool CUsbQueryBase::FilterPidVid( PUSB_NODE_CONNECTION_INFORMATION connectionInfo)
{
	TESTRETURNOK(connectionInfo );
	//! 没有过滤PID VID则不对PIDVID进行过滤
	if ( !m_vFilterPidVid.size() ) return true ;

	for (size_t nIndex = 0; nIndex < m_vFilterPidVid.size() ;nIndex ++)
	{
		if (m_vFilterPidVid .at( nIndex).m_nPid == connectionInfo->DeviceDescriptor.idProduct)
        {
            if (0 == m_vFilterPidVid.at(nIndex ).m_nVid || m_vFilterPidVid.at(nIndex ).m_nVid == connectionInfo->DeviceDescriptor.idVendor)
            {
                return true;
            }  
        }
	}

	return false ;
}
