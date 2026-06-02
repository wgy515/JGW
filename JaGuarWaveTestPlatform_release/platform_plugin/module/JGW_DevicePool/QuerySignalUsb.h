#pragma once
#include "UsbQueryBase.h"

class CQuerySignalUsb :public CUsbQueryBase
{
public:
	CQuerySignalUsb(void );
	~ CQuerySignalUsb(void );

	bool                           GetHubCurrentPortsInfo(int nHCDIndex,int nPortIndex, int nExtHubPort ,PHUBPORTPOINTINFO lpHubPortPointInfo);

public:
	bool                           QueryEnumerateHubPorts(HANDLE hHubDevice ,ULONG uPortIndex);
	bool                           QueryCurrentHub( LPTSTR HubName,PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo );

private:
	PHUBPORTPOINTINFO       m_lpHubPortPointInfo ;
	PUSB_NODE_CONNECTION_INFORMATION  m_lpConnectionInfo ;
	int                                  m_nConnectionInfoBytes;
};
