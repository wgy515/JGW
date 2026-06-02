#include "StdAfx.h"
#include "UsbInfoQuery.h"
#include "CAutoUsbPort.h"
#include <JGW_DevicePool/DevicePortDefine.h>


CUsbInfoQuery::CUsbInfoQuery (void)
{
}


CUsbInfoQuery::~CUsbInfoQuery (void)
{
}

bool CUsbInfoQuery ::GetInstanceIDSortIndex( int &nSortIndex ,LPCTSTR strInstanceID)
{
	CCAutoUsbPort *lpAutoUsbPort = CCAutoUsbPort:: GetInstance();
	lpAutoUsbPort->EnumerateHostControllers ();
	PHUBPORTPOINTINFO lpHubInfo = NULL;
	size_t nComLength = _tcslen( strInstanceID );

	for (size_t nIndex = 0; nIndex < g_vSortPortInfo .size(); nIndex ++)
	{
		lpHubInfo = g_vSortPortInfo .at( nIndex);
		if (NULL == _tcsnicmp(lpHubInfo ->m_sPortInstanceInfo.m_szInstanceID.c_str(),strInstanceID ,nComLength) && nComLength == lpHubInfo ->m_sPortInstanceInfo.m_szInstanceID.length())
		{
			nSortIndex = (int )nIndex/* + 1*/ ;
			return true ;
		}
        DEVICEPOOL_LOG_INFO_FORMAT(L"%s --- %s",lpHubInfo ->m_sPortInstanceInfo.m_szInstanceID.c_str(),strInstanceID);
		for (size_t i = 0; i < lpHubInfo->m_sPortInstanceInfo .m_vChildInstanceID. size();i ++)
		{
            DEVICEPOOL_LOG_INFO_FORMAT(L"%s --- %s",lpHubInfo->m_sPortInstanceInfo.m_vChildInstanceID.at(i).c_str(),strInstanceID);
			if (NULL == _tcsnicmp(lpHubInfo->m_sPortInstanceInfo.m_vChildInstanceID.at(i).c_str(),strInstanceID, nComLength)
				&& nComLength == lpHubInfo->m_sPortInstanceInfo.m_vChildInstanceID.at(i).length() )
			{
				nSortIndex = (int )nIndex /*+ 1*/;
				return true ;
			}
		}
	}
	nSortIndex = -1;

	return false ;
}

bool CUsbInfoQuery::GetInstanceIDSortIndexAndSerialNumber(int &nSortIndex,const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize)
{
	CCAutoUsbPort *lpAutoUsbPort = CCAutoUsbPort:: GetInstance();
	lpAutoUsbPort->EnumerateHostControllers ();
	PHUBPORTPOINTINFO lpHubInfo = NULL;
	size_t nComLength = _tcslen( strInstanceID);
	//size_t nSerialNumberSize = 253;
	//ShowDeviceInfo();
	for (size_t nIndex = 0; nIndex < g_vSortPortInfo .size(); nIndex ++)
	{
		lpHubInfo = g_vSortPortInfo .at( nIndex);
		if (NULL == _tcsnicmp(lpHubInfo ->m_sPortInstanceInfo.m_szInstanceID.c_str(),strInstanceID ,nComLength) && nComLength == lpHubInfo ->m_sPortInstanceInfo.m_szInstanceID.length())
		{
			nSortIndex = (int )nIndex/* + 1*/ ;
			/*if ( nSerialNumberSize < nBufferSize )*/wcscpy_s(strSerialNumber,nBufferSize,lpHubInfo->m_sPortInstanceInfo.m_szServerNumber.c_str());
			nBufferSize = wcslen(strSerialNumber);
			return true ;
		}

		for (size_t i = 0; i < lpHubInfo->m_sPortInstanceInfo .m_vChildInstanceID. size();i ++)
		{
			if (NULL == _tcsnicmp(lpHubInfo ->m_sPortInstanceInfo. m_vChildInstanceID.at(i).c_str (),strInstanceID, nComLength) && nComLength == lpHubInfo->m_sPortInstanceInfo.m_vChildInstanceID.at(i).length() )
			{
				nSortIndex = (int )nIndex /*+ 1*/;
				/*if ( nSerialNumberSize < nBufferSize )*/wcscpy_s(strSerialNumber,nBufferSize,lpHubInfo->m_sPortInstanceInfo.m_szServerNumber.c_str());
				nBufferSize = wcslen(strSerialNumber);
				return true ;
			}
		}
	}
	nSortIndex = -1;

	return false ;
}

size_t CUsbInfoQuery ::GetUsbSortCounts()
{
	if ( g_vSortPortInfo .size() == 0 )
	{
		CCAutoUsbPort *lpAutoUsbPort = CCAutoUsbPort:: GetInstance();
		lpAutoUsbPort->EnumerateHostControllers ();
	}

	return g_vSortPortInfo .size();
}

// bool CUsbInfoQuery::QuerySortIndex( int nIndex,_Out_ LPPORTINSTANCEINFO lpPortInstanceID )
// {
//    // ²éÕÒµÄÅÅÐòË÷ÒýÎ´³¬¹ý·¶Î§
//    TESTRETURNOK( g_vSortPortInfo.size() > nIndex );
//    lpPortInstanceID = &g_vSortPortInfo.at(nIndex)->m_sPortInstanceInfo;
//
//
//
//    return true;
// }