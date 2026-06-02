#include "StdAfx.h"
#include <JGW_Wlan/CJGW_WlanInterface.h>
#include <conio.h>
#include <objbase.h>
#include <rpcsal.h>
#include <wlanapi.h>
#include <sstream>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCJGW_WlanInterface::CCJGW_WlanInterface(const std::wstring& strWlanInterfaceDescriptionKey) : mstrWlanInterfaceDescriptionKey(strWlanInterfaceDescriptionKey),mbGuidIsValid(false),mhClientWlanHanle(NULL)
    {
    }


    CCJGW_WlanInterface::~CCJGW_WlanInterface(void)
    {
        CloseWlanHandle();
    }

    std::wstring CCJGW_WlanInterface::GetWlanInterfaceDescriptionToEnumKey()
    {
        return mstrWlanInterfaceDescription;
    }

	LPWSTR GetInterfaceStateString(__in WLAN_INTERFACE_STATE wlanInterfaceState)
	{
		LPWSTR strRetCode;

		switch(wlanInterfaceState)
		{
		case wlan_interface_state_not_ready:
			strRetCode = L"\"not ready\"";
			break;
		case wlan_interface_state_connected:
			strRetCode = L"\"connected\"";
			break;
		case wlan_interface_state_ad_hoc_network_formed:
			strRetCode = L"\"ad hoc network formed\"";
			break;
		case wlan_interface_state_disconnecting:
			strRetCode = L"\"disconnecting\"";
			break;
		case wlan_interface_state_disconnected:
			strRetCode = L"\"disconnected\"";
			break;
		case wlan_interface_state_associating:
			strRetCode = L"\"associating\"";
			break;
		case wlan_interface_state_discovering:
			strRetCode = L"\"discovering\"";
			break;
		case wlan_interface_state_authenticating:
			strRetCode = L"\"authenticating\"";
			break;
		default:
			strRetCode = L"\"invalid interface state\"";
		}

		return strRetCode;
	}
	// the max lenght of the reason string in characters
	#define WLSAMPLE_REASON_STRING_LEN 256

	// print the reason string
	VOID PrintReason(__in WLAN_REASON_CODE reason)
	{
		WCHAR strReason[WLSAMPLE_REASON_STRING_LEN] = {0};

		if (WlanReasonCodeToString(
			reason, 
			WLSAMPLE_REASON_STRING_LEN,
			strReason, 
			NULL            // reserved
			) == ERROR_SUCCESS)
		{
			PELOG4WW_F(L"The reason is %s",strReason);
		}
		else
		{
			PELOG4WW_F(L"The reason code is %d",reason);
		}
	}


    bool CCJGW_WlanInterface::OpenWlanHandle()
    {
        DWORD dwServiceVersion,dwError;
        CloseWlanHandle();

		dwError = WlanOpenHandle(WLAN_API_VERSION,NULL,&dwServiceVersion,&mhClientWlanHanle);
		PrintErrorMsg(L"WlanOpenHandle",dwError);

        return (dwError == ERROR_SUCCESS);
    }

    bool CCJGW_WlanInterface::GetWlanInterfaceDescriptions(std::map<std::wstring, std::wstring>& mapInterfaceDescriptions)
    {
        PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
		DWORD dwError = ERROR_SUCCESS;
        RPC_WSTR strGuid = NULL;

		dwError = WlanEnumInterfaces(mhClientWlanHanle,NULL,&pIntfList);
		PrintErrorMsg(L"WlanEnumInterfaces",dwError);
        if (ERROR_SUCCESS != dwError) return false;

        // print out interface information 如果WIFI已经连接，则断开当前WIFI连接
        for (UINT i = 0; i < pIntfList->dwNumberOfItems; i++)
        {
            if (UuidToStringW(&pIntfList->InterfaceInfo[i].InterfaceGuid, &strGuid) == RPC_S_OK)
            {
                //! wcout << L"\tGUID: " << (LPWSTR)strGuid << endl;
                mapInterfaceDescriptions.insert(std::map<std::wstring, std::wstring>::value_type(pIntfList->InterfaceInfo[i].strInterfaceDescription,(LPWSTR)strGuid));
				PDLOG4WW_F(L"InterfaceDescription: %s , GUID : %s",pIntfList->InterfaceInfo[i].strInterfaceDescription,(LPWSTR)strGuid);
                if (mstrWlanInterfaceDescriptionKey.empty() || NULL != JGW_WStrCaseWStr(pIntfList->InterfaceInfo[i].strInterfaceDescription,mstrWlanInterfaceDescriptionKey.c_str()))
                {
                    mWlanInterfaceGuid = pIntfList->InterfaceInfo[i].InterfaceGuid;
                    mstrWlanInterfaceDescription = pIntfList->InterfaceInfo[i].strInterfaceDescription;
                    mbGuidIsValid = true;
                }
                RpcStringFreeW(&strGuid);
            }
        }
        // clean up
        if (NULL != pIntfList) WlanFreeMemory(pIntfList);
        return true;
    }

    bool CCJGW_WlanInterface::EnumWlanInterface(const std::wstring& strWlanInterfaceDescriptionKey)
    {
        std::map<std::wstring, std::wstring> mapInterfaceDescriptions;
        mbGuidIsValid = false;
        mstrWlanInterfaceDescriptionKey = strWlanInterfaceDescriptionKey;
        if (!GetWlanInterfaceDescriptions(mapInterfaceDescriptions)) return false;
        return mbGuidIsValid;
    }

    bool CCJGW_WlanInterface::GetScanNetworkList(std::map<std::string, int>& mapBss)
    {
        DWORD dwError = ERROR_SUCCESS;
        PWLAN_AVAILABLE_NETWORK_LIST pVList = NULL;
        mapBss.clear();

        if (!ScanClientWlan(""))
        {
            return false;
        }

        dwError = WlanGetAvailableNetworkList(mhClientWlanHanle,&mWlanInterfaceGuid,0,NULL,&pVList);
        PrintErrorMsg(L"WlanGetAvailableNetworkList",dwError);
        if (dwError != ERROR_SUCCESS)
        {
            return false;
        }

        for (UINT i = 0; i < pVList->dwNumberOfItems; i++)
        {
            mapBss[std::string((char*)pVList->Network[i].dot11Ssid.ucSSID)] = (int)pVList->Network[i].wlanSignalQuality;
            PDLOG4WW_F(L"dot11Ssid: %s,wlanSignalQuality: %d",JGW_A2W_A((char*)pVList->Network[i].dot11Ssid.ucSSID).c_str(),pVList->Network[i].wlanSignalQuality);
            //! std::cout << pVList->Network[i].dot11Ssid.ucSSID << "," << pVList->Network[i].dot11BssType << std::endl;
        }

        if (pVList) WlanFreeMemory(pVList);

        return true;
    }

	bool CCJGW_WlanInterface::GetVisibleNetworkList(std::map<std::string, int>& mapBss)
	{
		DWORD dwError = ERROR_SUCCESS;
		PWLAN_AVAILABLE_NETWORK_LIST pVList = NULL;
		mapBss.clear();

		if (!ScanClientWlan(""))
		{
			return false;
		}

		dwError = WlanGetAvailableNetworkList(mhClientWlanHanle,&mWlanInterfaceGuid,0,NULL,&pVList);
		PrintErrorMsg(L"WlanGetAvailableNetworkList",dwError);
		if (dwError != ERROR_SUCCESS)
		{
			return false;
		}

		for (UINT i = 0; i < pVList->dwNumberOfItems; i++)
		{
			mapBss[std::string((char*)pVList->Network[i].dot11Ssid.ucSSID)] = (int)pVList->Network[i].dot11BssType;
			PDLOG4WW_F(L"dot11Ssid: %s,dot11BssType: %d",JGW_A2W_A((char*)pVList->Network[i].dot11Ssid.ucSSID).c_str(),pVList->Network[i].dot11BssType);
			//! std::cout << pVList->Network[i].dot11Ssid.ucSSID << "," << pVList->Network[i].dot11BssType << std::endl;
		}

		if (pVList) WlanFreeMemory(pVList);

		return true;
	}

	bool CCJGW_WlanInterface::FindWlanSSIDToWlanBSSList(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType /* = dot11_BSS_type_infrastructure */)
	{
		DOT11_SSID dot11SSID;
		DWORD dwError = ERROR_SUCCESS;
		BOOL bSecurityEnabled = TRUE;
		bool bFindSSID = false;
		PWLAN_BSS_LIST pWlanBssList = NULL;
		dot11SSID.uSSIDLength = strSSID.length();
		strcpy_s((char*)dot11SSID.ucSSID,DOT11_SSID_MAX_LENGTH,strSSID.c_str());
		
		ScanClientWlan(strSSID);

		dwError = WlanGetNetworkBssList(mhClientWlanHanle,&mWlanInterfaceGuid,&dot11SSID,dot11BSSType,bSecurityEnabled,NULL,&pWlanBssList);
		PrintErrorMsg(L"WlanGetNetworkBssList",dwError);
		if (ERROR_SUCCESS != dwError)
		{
			return false;
		}

		for (UINT i = 0; i < pWlanBssList->dwNumberOfItems; i++)
		{
			bFindSSID = true;
			PDLOG4WW_F(L"dot11Ssid: %s,dot11BssType: %d",JGW_A2W_A((char*)pWlanBssList->wlanBssEntries[i].dot11Ssid.ucSSID).c_str(),pWlanBssList->wlanBssEntries[i].dot11BssType);
			//! std::cout << pWlanBssList->wlanBssEntries[i].dot11Ssid.ucSSID << "," << pWlanBssList->wlanBssEntries[i].dot11BssType << std::endl;
		}

		WlanFreeMemory(pWlanBssList);

		return bFindSSID;
	}

    bool CCJGW_WlanInterface::ScanClientWlan(const std::string& strSSID)
    {
		DWORD dwError = ERROR_SUCCESS;
        if (strSSID.empty())
		{
			dwError = WlanScan(mhClientWlanHanle,&mWlanInterfaceGuid,NULL,NULL,NULL);
			PrintErrorMsg(L"WlanScan",dwError);
			return (ERROR_SUCCESS == dwError);
		}

        DOT11_SSID dot11SSID;
        dot11SSID.uSSIDLength = strSSID.length();
        strcpy_s((char*)dot11SSID.ucSSID,DOT11_SSID_MAX_LENGTH,strSSID.c_str());
		dwError = WlanScan(mhClientWlanHanle,&mWlanInterfaceGuid,&dot11SSID,NULL,NULL);
		PrintErrorMsg(L"WlanScan",dwError);

        return (ERROR_SUCCESS == dwError);
    }

	void CCJGW_WlanInterface::CloseWlanHandle()
	{
		if (mhClientWlanHanle)
		{
			WlanCloseHandle(mhClientWlanHanle,NULL);
			PDLOG4WW(L"WlanCloseHandle");
			mhClientWlanHanle = NULL;
		}
	}

    bool CCJGW_WlanInterface::DisWlanConnect()
    {
		DWORD dwError = ERROR_SUCCESS;
        PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;

        // enumerate wireless interfaces
		dwError = WlanEnumInterfaces(mhClientWlanHanle,NULL,&pIntfList);
		PrintErrorMsg(L"WlanEnumInterfaces",dwError);
        if (ERROR_SUCCESS != dwError) return false;

        // print out interface information 如果WIFI已经连接，则断开当前WIFI连接
        for (UINT i = 0; i < pIntfList->dwNumberOfItems; i++)
        {
			PDLOG4WW_F(L"InterfaceDescription : %s,State: %s",pIntfList->InterfaceInfo[i].strInterfaceDescription,GetInterfaceStateString(pIntfList->InterfaceInfo[i].isState));
            if (wlan_interface_state_connected == pIntfList->InterfaceInfo[i].isState)
            {
                WlanDisconnect(mhClientWlanHanle,&pIntfList->InterfaceInfo[i].InterfaceGuid,NULL);
				//! PDLOG4WW_F(L"WlanDisconnect (%s)",pIntfList->InterfaceInfo[i].strInterfaceDescription);
            }
        }

        if (NULL != pIntfList) WlanFreeMemory(pIntfList);

        return true;
    }

	bool CCJGW_WlanInterface::SetWlanConnectProfile(const std::wstring& strWlanConnectProfile)
	{
		DWORD dwError = ERROR_SUCCESS;
		DWORD dwReasonCode = 0;

		dwError = WlanSetProfile(mhClientWlanHanle,&mWlanInterfaceGuid,0,strWlanConnectProfile.c_str(),NULL,TRUE,NULL,&dwReasonCode);
		PrintErrorMsg(L"WlanSetProfile",dwError);
		if (dwError == ERROR_BAD_PROFILE)
		{
			PrintReason(dwReasonCode);
			return false;
		}

		return (ERROR_SUCCESS == dwError);
	}

	bool CCJGW_WlanInterface::DeleteWlanConnectProfile(const std::wstring& strWlanConnectProfileName)
	{
		DWORD dwError = ERROR_SUCCESS;
		dwError = WlanDeleteProfile(mhClientWlanHanle,&mWlanInterfaceGuid,strWlanConnectProfileName.c_str(),NULL);
		PrintErrorMsg(L"WlanDeleteProfile",dwError);
		return (ERROR_SUCCESS == dwError);
	}

	bool CCJGW_WlanInterface::ConnectWlanToSSID(const std::string& strSSID,const std::wstring& strWlanConnectProfileName,DOT11_BSS_TYPE dot11BSSType /* = dot11_BSS_type_infrastructure */)
	{
		WLAN_CONNECTION_PARAMETERS wlanConnPara;
		DOT11_SSID dot11SSID;
		DWORD dwError = ERROR_SUCCESS;

		wlanConnPara.wlanConnectionMode = wlan_connection_mode_profile;
		wlanConnPara.strProfile = strWlanConnectProfileName.c_str();
		dot11SSID.uSSIDLength = strSSID.length();
		strcpy_s((char*)dot11SSID.ucSSID,DOT11_SSID_MAX_LENGTH,strSSID.c_str());
		wlanConnPara.pDot11Ssid = &dot11SSID;
		wlanConnPara.dot11BssType = dot11BSSType;
		wlanConnPara.pDesiredBssidList = NULL;
		wlanConnPara.dwFlags = 0;

		dwError = WlanConnect(mhClientWlanHanle,&mWlanInterfaceGuid,&wlanConnPara,NULL);
		PrintErrorMsg(L"WlanConnect",dwError);

		return (ERROR_SUCCESS == dwError);
	}

    bool CCJGW_WlanInterface::ConnectOpenWlanToSSID(const std::string& strSSID,DOT11_BSS_TYPE dot11BSSType /* = dot11_BSS_type_infrastructure */)
    {
        DOT11_SSID dot11SSID = {0};
        DWORD dwError = ERROR_SUCCESS;
        WLAN_CONNECTION_PARAMETERS wlanConnPara;

        dot11SSID.uSSIDLength = strSSID.length();
        strcpy_s((char*)dot11SSID.ucSSID,DOT11_SSID_MAX_LENGTH,strSSID.c_str());
        // profile is ignored for discovery
        wlanConnPara.strProfile = NULL;
        // set the SSID
        wlanConnPara.pDot11Ssid = &dot11SSID;

        // get BSS type
        wlanConnPara.dot11BssType = (DOT11_BSS_TYPE)dot11BSSType;

        // get whether security enabled or not wlan_connection_mode_discovery_secure  && wlan_connection_mode_discovery_unsecure
        wlanConnPara.wlanConnectionMode = /*(WLAN_CONNECTION_MODE)*/wlan_connection_mode_discovery_secure;

        // the desired BSSID list is empty
        wlanConnPara.pDesiredBssidList = NULL;
        // no connection flags
        wlanConnPara.dwFlags = 0;

        dwError = WlanConnect(mhClientWlanHanle,&mWlanInterfaceGuid,&wlanConnPara,NULL);
		PrintErrorMsg(L"WlanConnect",dwError);

        return (ERROR_SUCCESS == dwError);
    }

	bool CCJGW_WlanInterface::CheckWlanIsConnected(const std::string& strSSID)
	{
		bool bConnected = false;
		DWORD dwError = ERROR_SUCCESS;
#if 0
		PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
		// enumerate wireless interfaces
		if (ERROR_SUCCESS != WlanEnumInterfaces(mhClientWlanHanle,NULL,&pIntfList)) return false;

		// print out interface information 如果WIFI已经连接，则断开当前WIFI连接
		for (UINT i = 0; i < pIntfList->dwNumberOfItems; i++)
		{
			if (wlan_interface_state_connected == pIntfList->InterfaceInfo[i].isState && 0 == memcmp(&mWlanInterfaceGuid,&pIntfList->InterfaceInfo[i].InterfaceGuid,sizeof(GUID)))
			{
				//!WlanDisconnect(mhClientWlanHanle,&pIntfList->InterfaceInfo[i].InterfaceGuid,NULL);
				bConnected = true;
			}
		}

		if (NULL != pIntfList) WlanFreeMemory(pIntfList);
#else
		PVOID pData = NULL;
        PVOID pCurrentNetworkData = NULL;
		DWORD dwDataSize = 0;
		WLAN_INTERFACE_STATE isState;
		PWLAN_CONNECTION_ATTRIBUTES pCurrentNetwork = NULL;

		dwError = WlanQueryInterface(mhClientWlanHanle,&mWlanInterfaceGuid,wlan_intf_opcode_interface_state,NULL,&dwDataSize,&pData,NULL);
		PrintErrorMsg(L"WlanQueryInterface Interface State",dwError);
		if (dwError != ERROR_SUCCESS || dwDataSize != sizeof(WLAN_INTERFACE_STATE))
		{
			goto END;
		}
		//! 如果非连接状态则直接返回false
		isState = *((PWLAN_INTERFACE_STATE)pData);
		PDLOG4WW_F(L"CheckWlanIsConnected : %s",GetInterfaceStateString(isState));
		if (wlan_interface_state_connected != isState)
		{
            goto END;
		}

		WlanFreeMemory(pData);
		pData = NULL;

		dwError = WlanQueryInterface(
			mhClientWlanHanle,
			&mWlanInterfaceGuid,
			wlan_intf_opcode_current_connection,
			NULL,                       // reserved
			&dwDataSize,
			&pCurrentNetworkData,
			NULL                        // not interesed in the type of the opcode value
			);
		PrintErrorMsg(L"WlanQueryInterface Current Connection",dwError);
		if (dwError == ERROR_SUCCESS && dwDataSize == sizeof(WLAN_CONNECTION_ATTRIBUTES))
		{
			pCurrentNetwork = (PWLAN_CONNECTION_ATTRIBUTES)pCurrentNetworkData;
		}
		
		// we don't treat ERROR_INVALID_STATE as an error for querying the interface
		if (dwError == ERROR_INVALID_STATE)
		{
			dwError = ERROR_SUCCESS;
		}

		if (pCurrentNetwork == NULL)
		{
			// no connection information
			goto END;
		}
        //! 如果为空，则判断当前网卡是否已经连接
        if (strSSID.empty())
        {
            bConnected = true;
            goto END;
        }

		//! 比对SSID
		PDLOG4WW_F(L"Wlan Connect SSID %s,Exp SSID %s",JGW_A2W_A((const char*)pCurrentNetwork->wlanAssociationAttributes.dot11Ssid.ucSSID).c_str(),JGW_A2W(strSSID).c_str());
		if (NULL == JGW_StrComparenoCaseStr((const char*)pCurrentNetwork->wlanAssociationAttributes.dot11Ssid.ucSSID,strSSID.c_str()))
		{
			bConnected = true;
		}

#endif
END:
        if (pData)
        {
            WlanFreeMemory(pData);
            pData = NULL;
        }

        if (pCurrentNetworkData)
        {
            WlanFreeMemory(pCurrentNetworkData);
            pCurrentNetworkData = NULL;
        }
		return bConnected;
	}

	void CCJGW_WlanInterface::PrintErrorMsg(const wchar_t* strCommand,DWORD dwError)
	{
		if (strCommand != NULL)
		{
			if (dwError == ERROR_SUCCESS)
			{
				PDLOG4WW_F(L"Function %s completed successfully.",strCommand);
			}
			else if (dwError == ERROR_INVALID_PARAMETER)
			{
				PELOG4WW_F(L"The parameter for %s is not correct.",strCommand);
				PELOG4WW_F(L"Please use help %s to check the usage of the command.",strCommand);
			}
			else if (dwError == ERROR_BAD_PROFILE)
			{
				PELOG4WW(L"The given profile is not valid.");
			}
			else if (dwError == ERROR_NOT_SUPPORTED)
			{
				PELOG4WW_F(L"Function %s is not supported.",strCommand);
			}
			else
			{
				PELOG4WW_F(L"Got error (%s) for command %s",JGW_GetSystemErrorString(dwError).c_str(),strCommand);
			}
		}
	}
}