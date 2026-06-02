// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#define AUTO_ID_USB_EXPORTS
#include "usb_host/UsbInfoQuery.h"
#include <JGW_DevicePool/Auto_ID_Usb.h>
#include "usb_host/AutoUsbDefine.h"
#include "usb_host/CAutoUsbPort.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "CJGW_DevicePoolThread.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

POCO_BEGIN_MANIFEST(JGW::CCJGW_DevicePoolImpl)
    POCO_EXPORT_SINGLETON(JGW::CCJGW_DevicePoolThread)
    POCO_END_MANIFEST


boost::mutex io_mutex;

bool  GetInstanceIDSortIndex(int &nSortIndex,const wchar_t *strInstanceID)
{
    boost::mutex::scoped_lock lock(io_mutex);  
	static CUsbInfoQuery cUsbQuery;

	return cUsbQuery.GetInstanceIDSortIndex( nSortIndex,strInstanceID );
}

bool GetInstanceIDSortIndexAndSerialNumber(int &nSortIndex,const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize)
{
	if( !strSerialNumber || !nBufferSize )return false;
	boost::mutex::scoped_lock lock(io_mutex); 
	static CUsbInfoQuery cUsbQuery;

	return cUsbQuery.GetInstanceIDSortIndexAndSerialNumber( nSortIndex,strInstanceID ,strSerialNumber,nBufferSize);
}


void ClearSortIndex ()
{
	boost::mutex::scoped_lock lock(io_mutex); 
	CCAutoUsbPort::GetInstance ()->ClearSortInfo();
}

void GetSerialNumber(const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize)
{
	boost::mutex::scoped_lock lock(io_mutex); 
	int nIndex = 0;
	GetInstanceIDSortIndexAndSerialNumber(nIndex,strInstanceID,strSerialNumber,nBufferSize);
}

unsigned short GetUsbProtocolVersionToInstance(const wchar_t *strInstanceID)
{
    if(!strInstanceID) return 0x00;
    boost::mutex::scoped_lock lock(io_mutex); 
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
            return lpHubInfo->mbcdUSB;
        }

        for (size_t i = 0; i < lpHubInfo->m_sPortInstanceInfo .m_vChildInstanceID. size();i ++)
        {
            if (NULL == _tcsnicmp(lpHubInfo ->m_sPortInstanceInfo. m_vChildInstanceID.at(i).c_str (),strInstanceID, nComLength) && nComLength == lpHubInfo->m_sPortInstanceInfo.m_vChildInstanceID.at(i).length() )
            {
                return lpHubInfo->mbcdUSB;
            }
        }
    }
    return 0x00;
}

void GetInstanceInfoToPc(void* lpBuffer)
{
	if( !lpBuffer ) return;
	boost::mutex::scoped_lock lock(io_mutex); 
	std::vector<PHUBPORTPOINTINFO>* lpHubInfo = (std::vector <PHUBPORTPOINTINFO>*)lpBuffer;
	CCAutoUsbPort *lpAutoUsbPort = CCAutoUsbPort:: GetInstance();
	lpAutoUsbPort->EnumerateHostControllers ();

	_vectorSort:: iterator it ;
	for (it = g_vSortPortInfo .begin(); it != g_vSortPortInfo .end(); it ++)
	{
		PHUBPORTPOINTINFO lpHubPort = new HUBPORTPOINTINFO;

		lpHubPort->Init();
		lpHubPort->m_strPhysicalAddress = it[0]->m_strPhysicalAddress;
		lpHubPort->m_sPortInstanceInfo.m_szInstanceID = it[0]->m_sPortInstanceInfo.m_szInstanceID;
		lpHubPort->m_sPortInstanceInfo.m_szServerNumber = it[0]->m_sPortInstanceInfo.m_szServerNumber;

		for (USHORT i = 0; i < it[0]->m_sPortInstanceInfo.m_vChildInstanceID . size(); i ++)
		{
			lpHubPort->m_sPortInstanceInfo.m_vChildInstanceID.push_back(it[0]->m_sPortInstanceInfo.m_vChildInstanceID.at(i));
		}
		lpHubInfo->push_back(lpHubPort);
	}
}
#if 0
#include "CJGW_DevicePoolThread.h"
#include <JGW_DevicePool/DevicePoolImpl.h>
JGW::CCJGW_DevicePoolImpl* GetDevicePoolImplInstance()
{
	return JGW::CCJGW_DevicePoolImpl::GetDevicePoolInstance();
}
#endif

#ifdef _MANAGED
#pragma managed (push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule ,
	DWORD  ul_reason_for_call ,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH :
		CCAutoUsbPort::GetInstance();
#ifdef IS_ENCRYPT_MODULE
        return JGW::EncryptModuleIsLoaded(hModule) ? TRUE : FALSE;
#endif  
		break;
	case DLL_PROCESS_DETACH :
		break;
	case DLL_THREAD_ATTACH :
		break;
	case DLL_THREAD_DETACH :
		break;
	}
	return TRUE ;
}

#ifdef _MANAGED
#pragma managed (pop)
#endif