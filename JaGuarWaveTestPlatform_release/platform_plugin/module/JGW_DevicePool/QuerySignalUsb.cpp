#include "StdAfx.h"
#include "QuerySignalUsb.h"


CQuerySignalUsb::CQuerySignalUsb (void): m_lpHubPortPointInfo(NULL )
{
	m_nConnectionInfoBytes = sizeof (USB_NODE_CONNECTION_INFORMATION) + sizeof(USB_PIPE_INFO ) * 30;
	m_lpConnectionInfo = (PUSB_NODE_CONNECTION_INFORMATION ) new CHAR[ m_nConnectionInfoBytes ];
}


CQuerySignalUsb::~CQuerySignalUsb (void)
{
}

bool CQuerySignalUsb ::GetHubCurrentPortsInfo( int nHCDIndex ,int nPortIndex,int nExtHubPort, PHUBPORTPOINTINFO lpHubPortPointInfo )
{
	TESTRETURNOK(lpHubPortPointInfo );
	TESTRETURNOK(nHCDIndex >= 0 && nHCDIndex < NUM_HCS_TO_CHECK);
	TESTRETURNOK(nPortIndex >= 1);
	m_lpHubPortPointInfo  = lpHubPortPointInfo ;
	m_lpHubPortPointInfo->m_nHCDIndex = nHCDIndex;
	m_lpHubPortPointInfo->m_nHubPortNumberIndex = nPortIndex;
	//m_lpHubPortPointInfo->m_bIsExtHub = nExtHubPort>0?true:false;
	m_lpHubPortPointInfo->m_strPhysicalAddress = nExtHubPort;

	TCHAR   rootHubName [MAX_PATH ] = {0};
	bool  bResult = false;
	HANDLE  hHCDev = OpenCurrentHostControllers(nHCDIndex ,rootHubName);

	if ( hHCDev != INVALID_HANDLE_VALUE )
	{
		bResult = QueryCurrentHub (rootHubName , NULL);
		CloseHandle(hHCDev );
	}

	return bResult && (m_lpHubPortPointInfo-> m_bIsExtHub == nExtHubPort>0);
}

bool CQuerySignalUsb ::QueryCurrentHub( LPTSTR HubName,PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo )
{
	HANDLE hHubDevice = INVALID_HANDLE_VALUE;
	int     nHubPortsCounts = 0;
	bool  bResult = false;

	hHubDevice = OpenCurrentHubPorts(HubName ,nHubPortsCounts, ConnectionInfo);

	if ( hHubDevice == INVALID_HANDLE_VALUE )
	{
		return false ;
	}

	if ( !m_lpHubPortPointInfo ->m_bIsExtHub )
	{
		if (m_lpHubPortPointInfo ->m_nHubPortNumberIndex > nHubPortsCounts)
		{
			goto EnumerateHubError ;
		}

		bResult = QueryEnumerateHubPorts (  hHubDevice , m_lpHubPortPointInfo->m_nHubPortNumberIndex );
	}
	else
	{
// 		if (m_lpHubPortPointInfo ->m_nExtHubPortNumberIndex > nHubPortsCounts)
// 		{
// 			goto EnumerateHubError ;
// 		}
		//bResult = QueryEnumerateHubPorts (  hHubDevice , m_lpHubPortPointInfo->m_nExtHubPortNumberIndex );
	}



	CloseHandle(hHubDevice );

	return bResult ;
	// 错误
EnumerateHubError:
	if (hHubDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hHubDevice );
		hHubDevice = INVALID_HANDLE_VALUE ;
	}
	return false ;
}

bool CQuerySignalUsb ::QueryEnumerateHubPorts( HANDLE hHubDevice ,ULONG uPortIndex)
{
	TESTRETURNOK(m_lpConnectionInfo );
	memset(m_lpConnectionInfo ,0x00,m_nConnectionInfoBytes);
	TCHAR       m_szBuffer [MAX_PATH] = {0};
	TCHAR       driverKeyName [MAX_PATH ] = {0} ;
	bool        bResult = false;
	static      bool m_bIsExtHub = false ;
	// 遍历该HUB 的所有口口索引是基 ,不是基
	ULONG       nBytes   = 0;
	m_lpConnectionInfo->ConnectionIndex = uPortIndex;
	//m_lpHubPortPointInfo->m_nExtHubPortNumberIndex = -1;

	TESTRETURNOK ( DeviceIoControl ( hHubDevice ,
		IOCTL_USB_GET_NODE_CONNECTION_INFORMATION ,
		m_lpConnectionInfo,
		m_nConnectionInfoBytes ,
		m_lpConnectionInfo,
		m_nConnectionInfoBytes,
		& nBytes ,
		NULL ) );

	if ( !m_bIsExtHub )
	{
		m_lpHubPortPointInfo->m_nHubPortNumberIndex = uPortIndex;
	}
	else
	{
		//    m_lpHubPortPointInfo->m_nExtHubPortNumberIndex = uPortIndex;
	}

	if ( m_lpConnectionInfo ->DeviceIsHub )
	{
		if ( m_bIsExtHub )
		{
			:: MessageBox(FindWindow (NULL, NULL),_T("Does not support more than two layer of the HUB"), _T("Error" ),NULL);
			return false ;
		}
		m_lpHubPortPointInfo->m_bIsExtHub = true;
		m_bIsExtHub = true ;
		TCHAR extHubName [MAX_PATH]  = {0} ;

		TESTRETURNOK( GetExternalHubName(hHubDevice ,uPortIndex, extHubName) );
		QueryCurrentHub(extHubName ,m_lpConnectionInfo);
		m_bIsExtHub = false ;
		//下个端口
		return true ;
	}

	TESTRETURNOK( m_lpConnectionInfo ->ConnectionStatus != NoDeviceConnected );
	TESTRETURNOK( FilterPidVid ( m_lpConnectionInfo ) );
	TESTRETURNOK( GetDriverKeyName(hHubDevice ,uPortIndex, driverKeyName) );
	TESTRETURNOK( DriverNameToDeviceDesc(driverKeyName ,m_szBuffer,& m_lpHubPortPointInfo->m_sPortInstanceInfo ) );

	return bResult ;
}