#include "StdAfx.h"
#include <conio.h>
#include <objbase.h>
#include <rpcsal.h>
#include <wlanapi.h>
#include <sstream>
#include <JGW_Wlan/CJGW_WlanHandle.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
// the max lenght of the reason string in characters
#define WLSAMPLE_REASON_STRING_LEN 256

namespace JGW
{
	CCJGW_WlanHandle::CCJGW_WlanHandle(void)
	{
	}


	CCJGW_WlanHandle::~CCJGW_WlanHandle(void)
	{
	}

	const wchar_t* CCJGW_WlanHandle::GetWlanErrorMsg()
	{
		return mstrErrorMsg.c_str();
	}

	// get interface state string
	const wchar_t* CCJGW_WlanHandle::GetInterfaceStateString(__in int wlanInterfaceState)
	{
		LPWSTR strRetCode;

		switch((WLAN_INTERFACE_STATE)wlanInterfaceState)
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

	void CCJGW_WlanHandle::PrintErrorMsg(
		__in const wchar_t* strCommand,
		__in DWORD dwError
		)
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
				JGW_FormatWString(mstrErrorMsg,L"The parameter for %s is not correct.",strCommand);
				PELOG4WW_F(L"Please use help %s to check the usage of the command.",strCommand);
			}
			else if (dwError == ERROR_BAD_PROFILE)
			{
				JGW_FormatWString(mstrErrorMsg,L"The given profile is not valid.");
				PELOG4WW(L"The given profile is not valid.");
			}
			else if (dwError == ERROR_NOT_SUPPORTED)
			{
				JGW_FormatWString(mstrErrorMsg,L"Function %s is not supported.",strCommand);
				PELOG4WW_F(L"Function %s is not supported.",strCommand);
			}
			else
			{
				PELOG4WW_F(L"Got error %d for command %s",dwError,strCommand);
				JGW_FormatWString(mstrErrorMsg,L"Got error %d for command %s",dwError,strCommand);
			}
		}
	}

	// open a WLAN client handle and check version
	DWORD CCJGW_WlanHandle::OpenHandleAndCheckVersion(
		PHANDLE phClient
		)
	{
		DWORD dwError = ERROR_SUCCESS;
		DWORD dwServiceVersion;
		HANDLE hClient = NULL;

		__try
		{
			*phClient = NULL;

			// open a handle to the service
			if ((dwError = WlanOpenHandle(
				WLAN_API_VERSION,
				NULL,               // reserved
				&dwServiceVersion,
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}

			// check service version
			if (WLAN_API_VERSION_MAJOR(dwServiceVersion) < WLAN_API_VERSION_MAJOR(WLAN_API_VERSION_2_0))
			{
				// No-op, because the version check is for demonstration purpose only.
				// You can add your own logic here.
			}

			*phClient = hClient;

			// set hClient to NULL so it will not be closed
			hClient = NULL;
		}
		__finally
		{
			if (hClient != NULL)
			{
				// clean up
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}

		return dwError;
	}

	bool CCJGW_WlanHandle::GetWlanInterfaceDescriptions(std::map<std::wstring, std::wstring>& mapInterfaceDescriptions)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		PWLAN_INTERFACE_INFO_LIST pIntfList = NULL;
		UINT i = 0;
		//! std::wstring strGUID;


		// open handle
		if ((dwError = OpenHandleAndCheckVersion(
			&hClient
			)) != ERROR_SUCCESS)
		{
			PrintErrorMsg(L"GetWlanInterfaceDescriptions", dwError);
			return false;
		}

		// enumerate wireless interfaces
		if ((dwError = WlanEnumInterfaces(
			hClient,
			NULL,               // reserved
			&pIntfList
			)) != ERROR_SUCCESS)
		{
			PrintErrorMsg(L"GetWlanInterfaceDescriptions", dwError);
			WlanCloseHandle(hClient, NULL/* reserved*/);
			return false;
		}

		PDLOG4WW_F(L"There are %d interfaces in the system.",pIntfList->dwNumberOfItems);

		// print out interface information
		for (i = 0; i < pIntfList->dwNumberOfItems; i++)
		{
			JGW_FormatWString(mstrGUID,L"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"
				,pIntfList->InterfaceInfo[i].InterfaceGuid.Data1
				,pIntfList->InterfaceInfo[i].InterfaceGuid.Data2
				,pIntfList->InterfaceInfo[i].InterfaceGuid.Data3
				,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[0]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[1]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[2]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[3]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[4]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[5]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[6]
			,pIntfList->InterfaceInfo[i].InterfaceGuid.Data4[7]);
			mapInterfaceDescriptions.insert(std::map<std::wstring, std::wstring>::value_type(pIntfList->InterfaceInfo[i].strInterfaceDescription,mstrGUID));
			PDLOG4WW_F(L"%s-%s State: %s",pIntfList->InterfaceInfo[i].strInterfaceDescription,mstrGUID.c_str(),GetInterfaceStateString(pIntfList->InterfaceInfo[i].isState));
		}


		// clean up
		if (pIntfList != NULL)
		{
			WlanFreeMemory(pIntfList);
		}

		if (hClient != NULL)
		{
			WlanCloseHandle(
				hClient, 
				NULL            // reserved
				);
		}

		PrintErrorMsg(L"GetWlanInterfaceDescriptions", dwError);
		return (dwError == ERROR_SUCCESS);
	}

	std::wstring CCJGW_WlanHandle::FindWalnGUIDToInterfaceDescriptionKeyword(const std::wstring& strInterfaceDesKeyword)
	{
		std::map<__InterfaceDescription,__GUID> mapWlanInterfaceDescriptionsGUID;
		if (!GetWlanInterfaceDescriptions(mapWlanInterfaceDescriptionsGUID)) return L"";

		for (std::map<__InterfaceDescription,__GUID>::iterator it = mapWlanInterfaceDescriptionsGUID.begin();
			it != mapWlanInterfaceDescriptionsGUID.end();
			++ it)
		{
			if (NULL != JGW_WStrCaseWStr(it->first.c_str(),strInterfaceDesKeyword.c_str())) return it->second;
		}
		return L"";
	}

	bool CCJGW_WlanHandle::WlanScan(const std::wstring& strGUID)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;

		__try
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				//! wcerr << L"Invalid GUID " << argv[1] << endl;
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
				dwError = ERROR_INVALID_PARAMETER;
				__leave;
			}

			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}

			// scan
			dwError = ::WlanScan(
				hClient, 
				&guidIntf, 
				NULL,                   // don't perform additional probe for a specific SSID
				NULL,                   // no IE data for the additional probe
				NULL                    // reserved
				);

		}
		__finally
		{
			// clean up
			if (hClient != NULL)
			{
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}

		PrintErrorMsg(L"WlanScan", dwError);
		return (ERROR_SUCCESS == dwError);
	}

	// copy SSID to a null-terminated WCHAR string
	// count is the number of WCHAR in the buffer.
	LPWSTR
		SsidToStringW(
		__out_ecount(count) LPWSTR   buf,
		__in ULONG   count,
		__in PDOT11_SSID pSsid
		)
	{
		ULONG   bytes, i;

		bytes = min( count-1, pSsid->uSSIDLength);
		for( i=0; i<bytes; i++)
			mbtowc( &buf[i], (const char *)&pSsid->ucSSID[i], 1);
		buf[bytes] = '\0';

		return buf;
	}

	bool CCJGW_WlanHandle::GetWlanBssList(const std::wstring& strGUID,std::map<std::wstring, int>& mapBss,int scan_time /* = 1000 */)
	{
		if (!WlanScan(strGUID)) return false;

		Sleep(scan_time);
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;
		DOT11_SSID dot11Ssid = {0};
		PDOT11_SSID pDot11Ssid = NULL;
		DOT11_BSS_TYPE dot11BssType = dot11_BSS_type_any;
		BOOL bSecurityEnabled = TRUE;
		PWLAN_BSS_LIST pWlanBssList = NULL;
		WCHAR strSsid[DOT11_SSID_MAX_LENGTH+1];
		UINT i;


		// get the interface GUID
		if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
		{
			JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
			return false;
		}

		// open handle
		if ((dwError = OpenHandleAndCheckVersion(
			&hClient
			)) != ERROR_SUCCESS)
		{
			return false;
		}

		if ((dwError = WlanGetNetworkBssList(
			hClient,
			&guidIntf,
			pDot11Ssid,
			dot11BssType,
			bSecurityEnabled,
			NULL,                       // reserved
			&pWlanBssList
			)) != ERROR_SUCCESS)
		{
			if (hClient != NULL) WlanCloseHandle(hClient,NULL);
			PrintErrorMsg(L"GetWlanBssList", dwError);
			return false;
		}

		for (i = 0; i < pWlanBssList->dwNumberOfItems; i++)
		{
			//! PrintBssInfo(&pWlanBssList->wlanBssEntries[i]);
			mapBss.insert(std::map<std::wstring,int>::value_type(SsidToStringW(strSsid, sizeof(strSsid)/sizeof(WCHAR), &pWlanBssList->wlanBssEntries[i].dot11Ssid),pWlanBssList->wlanBssEntries[i].dot11BssType));
		}

		WlanFreeMemory(pWlanBssList);

		// clean up
		if (hClient != NULL)
		{
			WlanCloseHandle(
				hClient, 
				NULL            // reserved
				);
		}

		PrintErrorMsg(L"GetWlanBssList", dwError);

		return true;
	}

	VOID CCJGW_WlanHandle::PrintReason(__in DWORD reason)
	{
		WCHAR strReason[WLSAMPLE_REASON_STRING_LEN];

		if (WlanReasonCodeToString(
			reason, 
			WLSAMPLE_REASON_STRING_LEN,
			strReason, 
			NULL            // reserved
			) == ERROR_SUCCESS)
		{
			JGW_FormatWString(mstrErrorMsg,L" The reason is %s",strReason);
		}
		else
		{
			JGW_FormatWString(mstrErrorMsg,L" The reason code is %d",reason);
		}
	}

	bool CCJGW_WlanHandle::SetWlanProfile(const std::wstring& strGUID,const std::wstring& strWlanProfile)
	{
		DWORD dwError;
		HANDLE hClient = NULL;
		GUID guidIntf;
		DWORD dwReason;

		// __try and __leave cannot be used here because of COM object
		do
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
				return false;
			}

			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				return false;
			}

			// set profile
			dwError = WlanSetProfile(
				hClient, 
				&guidIntf, 
				0,          // no flags for the profile 
				strWlanProfile.c_str(), 
				NULL,       // use the default ACL
				TRUE,		// overwrite a profile if it already exists
				NULL,       // reserved
				&dwReason
				);
			if (dwError == ERROR_BAD_PROFILE)
			{
				PrintReason(dwReason);
			}
		} while (FALSE);

		// clean up
		if (hClient != NULL)
		{
			WlanCloseHandle(
				hClient, 
				NULL            // reserved
				);
		}

		PrintErrorMsg(L"SetWlanProfile", dwError);
		return (ERROR_SUCCESS == dwError);
	}

	DWORD
		StringWToSsid(
		__in LPCWSTR strSsid, 
		__out PDOT11_SSID pSsid
		)
	{
		DWORD dwRetCode = ERROR_SUCCESS;
		BYTE pbSsid[DOT11_SSID_MAX_LENGTH + 1] = {0};

		if (strSsid == NULL || pSsid == NULL)
		{
			dwRetCode = ERROR_INVALID_PARAMETER;
		}
		else
		{
			pSsid->uSSIDLength = WideCharToMultiByte (CP_ACP,
				0,
				strSsid,
				-1,
				(LPSTR)pbSsid,
				sizeof(pbSsid),
				NULL,
				NULL);

			pSsid->uSSIDLength--;
			memcpy(&pSsid->ucSSID, pbSsid, pSsid->uSSIDLength);
		}

		return dwRetCode;
	}

    // get ACM notification string
    LPWSTR
        GetAcmNotificationString(
        __in DWORD acmNotif
        )
    {
        LPWSTR strRetCode;

        switch(acmNotif)
        {
        case wlan_notification_acm_autoconf_enabled:
            strRetCode = L"\"autoconf enabled\"";
            break;
        case wlan_notification_acm_autoconf_disabled:
            strRetCode = L"\"autoconf disabled\"";
            break;
        case wlan_notification_acm_background_scan_enabled:
            strRetCode = L"\"background scan enabled\"";
            break;
        case wlan_notification_acm_background_scan_disabled:
            strRetCode = L"\"background scan disabled\"";
            break;
        case wlan_notification_acm_power_setting_change:
            strRetCode = L"\"power setting change\"";
            break;
        case wlan_notification_acm_scan_complete:
            strRetCode = L"\"scan complete\"";
            break;
        case wlan_notification_acm_scan_fail:
            strRetCode = L"\"scan fail\"";
            break;
        case wlan_notification_acm_connection_start:
            strRetCode = L"\"connection start\"";
            break;
        case wlan_notification_acm_connection_complete:
            strRetCode = L"\"connection complete\"";
            break;
        case wlan_notification_acm_connection_attempt_fail:
            strRetCode = L"\"connection fail\"";
            break;
        case wlan_notification_acm_filter_list_change:
            strRetCode = L"\"filter list change\"";
            break;
        case wlan_notification_acm_interface_arrival:
            strRetCode = L"\"interface arrival\"";
            break;
        case wlan_notification_acm_interface_removal:
            strRetCode = L"\"interface removal\"";
            break;
        case wlan_notification_acm_profile_change:
            strRetCode = L"\"profile change\"";
            break;
        case wlan_notification_acm_profiles_exhausted:
            strRetCode = L"\"profiles exhausted\"";
            break;
        case wlan_notification_acm_network_not_available:
            strRetCode = L"\"network not available\"";
            break;
        case wlan_notification_acm_network_available:
            strRetCode = L"\"network available\"";
            break;
        case wlan_notification_acm_disconnecting:
            strRetCode = L"\"disconnecting\"";
            break;
        case wlan_notification_acm_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_notification_acm_adhoc_network_state_change:
            strRetCode = L"\"ad hoc network state changes\"";
            break;
        default:
            strRetCode = L"\"unknown ACM notification\"";
        }

        return strRetCode;
    }

    void CCJGW_WlanHandle::SetConnectedStatus(DWORD status)
    {
        mdwConnectStatus = status;
    }

    // get MSMM notification string
    LPWSTR
        GetMsmNotificationString(
        __in DWORD msmNotif
        )
    {
        LPWSTR strRetCode;

        switch(msmNotif)
        {
        case wlan_notification_msm_associating:
            strRetCode = L"\"associating\"";
            break;
        case wlan_notification_msm_associated:
            strRetCode = L"\"associated\"";
            break;
        case wlan_notification_msm_authenticating:
            strRetCode = L"\"authenticating\"";
            break;
        case wlan_notification_msm_connected:
            strRetCode = L"\"connected\"";
            break;
        case wlan_notification_msm_roaming_start:
            strRetCode = L"\"roaming start\"";
            break;
        case wlan_notification_msm_roaming_end:
            strRetCode = L"\"roaming end\"";
            break;
        case wlan_notification_msm_radio_state_change:
            strRetCode = L"\"radio state change\"";
            break;
        case wlan_notification_msm_signal_quality_change:
            strRetCode = L"\"signal quality change\"";
            break;
        case wlan_notification_msm_disassociating:
            strRetCode = L"\"disassociating\"";
            break;
        case wlan_notification_msm_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_notification_msm_peer_join:
            strRetCode = L"\"a peer joins the ad hoc network\"";
            break;
        case wlan_notification_msm_peer_leave:
            strRetCode = L"\"a peer leaves the ad hoc network\"";
            break;
        case wlan_notification_msm_adapter_removal:
            strRetCode = L"\"adapter is in a bad state\"";
            break;
        default:
            strRetCode = L"\"unknown MSM notification\"";
        }

        return strRetCode;
    }

    // get connection mode string
    LPWSTR
        GetConnectionModeString(
        __in WLAN_CONNECTION_MODE wlanConnMode
        )
    {
        LPWSTR strRetCode;

        switch(wlanConnMode)
        {
        case wlan_connection_mode_profile:
            strRetCode = L"\"manual connection with a profile\"";
            break;
        case wlan_connection_mode_temporary_profile:
            strRetCode = L"\"manual connection with a temporary profile\"";
            break;
        case wlan_connection_mode_discovery_secure:
            strRetCode = L"\"connection to a secure network without a profile\"";
            break;
        case wlan_connection_mode_discovery_unsecure:
            strRetCode = L"\"connection to an unsecure network without a profile\"";
            break;
        case wlan_connection_mode_auto:
            strRetCode = L"\"automatic connection with a profile\"";
            break;
        default:
            strRetCode = L"\"invalid connection mode\"";
        }

        return strRetCode;
    }

    // get BSS type string
    LPWSTR 
        GetBssTypeString(
        __in DOT11_BSS_TYPE dot11BssType
        )
    {
        LPWSTR strRetCode;

        switch(dot11BssType)
        {
        case dot11_BSS_type_infrastructure:
            strRetCode = L"\"Infrastructure\"";
            break;
        case dot11_BSS_type_independent:
            strRetCode = L"\"Ad hoc\"";
            break;
        case dot11_BSS_type_any:
            strRetCode = L"\"Any\"";
            break;
        default:
            strRetCode = L"\"Unknown BSS type\"";
        }

        return strRetCode;
    }

    // Notification callback function
    VOID WINAPI
        NotificationCallback(
        __in PWLAN_NOTIFICATION_DATA pNotifData, 
        __in_opt PVOID pContext  // this parameter is not used
        )
    {
        WCHAR strSsid[DOT11_SSID_MAX_LENGTH+1];
        PWLAN_CONNECTION_NOTIFICATION_DATA pConnNotifData = NULL;
        CCJGW_WlanHandle* pWlanHandle = (CCJGW_WlanHandle*)pContext;
        std::wostringstream strTemp;

        if (pNotifData != NULL)
        {
            switch(pNotifData->NotificationSource)
            {
            case WLAN_NOTIFICATION_SOURCE_ACM:
                strTemp << L"Got notification " << GetAcmNotificationString(pNotifData->NotificationCode) << L" from ACM." << std::endl; 
                // print some notifications as examples
                switch(pNotifData->NotificationCode)
                {
                case wlan_notification_acm_connection_complete:
                    if (pNotifData->dwDataSize < sizeof(WLAN_CONNECTION_NOTIFICATION_DATA))
                    {
                        break;
                    }
                    pConnNotifData = (PWLAN_CONNECTION_NOTIFICATION_DATA)pNotifData->pData;
                    if (pConnNotifData->wlanReasonCode == WLAN_REASON_CODE_SUCCESS)
                    {
                        pWlanHandle->SetConnectedStatus(wlan_notification_acm_connection_complete);
                        strTemp << L"The connection succeeded." << std::endl;
                        
                        if (pConnNotifData->wlanConnectionMode == wlan_connection_mode_discovery_secure ||
                            pConnNotifData->wlanConnectionMode == wlan_connection_mode_discovery_unsecure)
                        {
                            // the temporary profile generated for discovery
                            strTemp << L"The profile used for this connection is as follows:" << std::endl;
                            strTemp << pConnNotifData->strProfileXml << std::endl;
                        }
                    }
                    else
                    {
                        pWlanHandle->SetConnectedStatus(wlan_notification_acm_connection_attempt_fail);
                        //! wcout << L"The connection failed.";
                        pWlanHandle->PrintReason(pConnNotifData->wlanReasonCode);
                    }
                    break;
                case wlan_notification_acm_connection_start:
                    if (pNotifData->dwDataSize != sizeof(WLAN_CONNECTION_NOTIFICATION_DATA))
                    {
                        break;
                    }
                    pConnNotifData = (PWLAN_CONNECTION_NOTIFICATION_DATA)pNotifData->pData;
                    // print out some connection information
                    strTemp << L"\tCurrently connecting to " << SsidToStringW(strSsid, sizeof(strSsid)/sizeof(WCHAR), &pConnNotifData->dot11Ssid);

                    strTemp << L" using profile " << pConnNotifData->strProfileName;
                    strTemp << L", connection mode is " << GetConnectionModeString(pConnNotifData->wlanConnectionMode);
                    strTemp << L", BSS type is " << GetBssTypeString(pConnNotifData->dot11BssType) << std::endl;

                    break;
                }

                break;
            case WLAN_NOTIFICATION_SOURCE_MSM:
                strTemp << L"Got notification " << GetMsmNotificationString(pNotifData->NotificationCode) << L" from MSM." << std::endl; 
                break;
            }
            PDLOG4WW(strTemp.str().c_str());
        }
    }

	bool CCJGW_WlanHandle::ConnectWlan(const std::wstring& strGUID,const std::wstring& strSSID,const std::wstring& strProfileName,int dot11BssType)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;
		DOT11_SSID dot11Ssid = {0};
		WLAN_CONNECTION_PARAMETERS wlanConnPara;
        DWORD dwPrevNotifType = 0;
        bool bConnect = false;
        int dwCount = 30 * 5; //! 如果30s还没登录上就报错说明密码错误

		__try
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
				dwError = ERROR_INVALID_PARAMETER;
				__leave;
			}

			// get SSID
			if ((dwError = StringWToSsid(strSSID.c_str(), &dot11Ssid)) != ERROR_SUCCESS)
			{
				__leave;
			}

			// set the connection mode (connecting using a profile)
			wlanConnPara.wlanConnectionMode = wlan_connection_mode_profile;
			// set the profile name
			wlanConnPara.strProfile = strProfileName.c_str();
			// set the SSID
			wlanConnPara.pDot11Ssid = &dot11Ssid;

			// get BSS type
			wlanConnPara.dot11BssType = (DOT11_BSS_TYPE)dot11BssType;

			// the desired BSSID list is empty
			wlanConnPara.pDesiredBssidList = NULL;
			// no connection flags
			wlanConnPara.dwFlags = 0;

			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}

            SetConnectedStatus(wlan_notification_acm_connection_start);
            PDLOG4WW(L"ACM and MSM notifications are successfully registered. Press any key to exit.");
            // register for ACM and MSM notifications
            if ((dwError = WlanRegisterNotification(
                hClient,
                WLAN_NOTIFICATION_SOURCE_ACM | WLAN_NOTIFICATION_SOURCE_MSM,
                FALSE,			// do not ignore duplications
                NotificationCallback,
                this,			// no callback context is needed
                NULL,           // reserved
                &dwPrevNotifType
                )) != ERROR_SUCCESS)
            {
                __leave;
            }  

			dwError = WlanConnect(
				hClient,
				&guidIntf,
				&wlanConnPara,
				NULL            // reserved
				);

            PrintErrorMsg(L"ConnectWlan", dwError);

            while (ERROR_SUCCESS == dwError && dwCount-- >= 0)
            {
                if (wlan_notification_acm_connection_complete == mdwConnectStatus)
                {
                    bConnect = true;
                    break;
                }
                else if (wlan_notification_acm_connection_attempt_fail == mdwConnectStatus)
                {
                    bConnect = false;
                    break;
                }
                Sleep(200);
            }

            // unregister notifications
            if ((dwError = WlanRegisterNotification(
                hClient,
                WLAN_NOTIFICATION_SOURCE_NONE,
                FALSE,          // do not ignore duplications
                NULL,           // no callback function is needed
                NULL,           // no callback context is needed
                NULL,           // reserved
                &dwPrevNotifType
                )) == ERROR_SUCCESS)
            {
                PDLOG4WW(L"ACM and MSM notifications are successfully unregistered.");
            }
            else
            {
                PDLOG4WW_F(L"Error %d occurs when unresiger ACM and MSM notifications.",dwError);
            }
		}
		__finally
		{
			// clean up
			if (hClient != NULL)
			{
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}
		return bConnect;
	}

	bool CCJGW_WlanHandle::DisConnectWlan(const std::wstring& strGUID)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;

		__try
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
				dwError = ERROR_INVALID_PARAMETER;
				__leave;
			}

			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}

			dwError = WlanDisconnect(
				hClient, 
				&guidIntf, 
				NULL            // reserved
				);

		}
		__finally
		{
			// clean up
			if (hClient != NULL)
			{
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}

		PrintErrorMsg(L"DisConnectWlan", dwError);
		return (ERROR_SUCCESS == dwError);
	}

	bool CCJGW_WlanHandle::ConnectOpenWlan(const std::wstring& strGUID,const std::wstring& strSSID,int dot11BssType,int nWlanConnectMode /* = 2 */)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;
		DOT11_SSID dot11Ssid = {0};
		WLAN_CONNECTION_PARAMETERS wlanConnPara;

		__try
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s",strGUID.c_str());
				dwError = ERROR_INVALID_PARAMETER;
				__leave;
			}

			// get SSID
			if ((dwError = StringWToSsid(strSSID.c_str(), &dot11Ssid)) != ERROR_SUCCESS)
			{
				__leave;
			}

			// profile is ignored for discovery
			wlanConnPara.strProfile = NULL;
			// set the SSID
			wlanConnPara.pDot11Ssid = &dot11Ssid;

			// get BSS type
			wlanConnPara.dot11BssType = (DOT11_BSS_TYPE)dot11BssType;

			// get whether security enabled or not wlan_connection_mode_discovery_secure  && wlan_connection_mode_discovery_unsecure
			wlanConnPara.wlanConnectionMode = (WLAN_CONNECTION_MODE)nWlanConnectMode;

			// the desired BSSID list is empty
			wlanConnPara.pDesiredBssidList = NULL;
			// no connection flags
			wlanConnPara.dwFlags = 0;

			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}


			dwError = WlanConnect(
				hClient,
				&guidIntf,
				&wlanConnPara,
				NULL            // reserved
				);

		}
		__finally
		{
			// clean up
			if (hClient != NULL)
			{
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}

		PrintErrorMsg(L"ConnectOpenWlan", dwError);
		return (ERROR_SUCCESS == dwError);
	}

	bool CCJGW_WlanHandle::DeleteWlanProfile(const std::wstring& strGUID,const std::wstring& strWlanProfile)
	{
		DWORD dwError = ERROR_SUCCESS;
		HANDLE hClient = NULL;
		GUID guidIntf;

		__try
		{
			// get the interface GUID
			if (UuidFromString((RPC_WSTR)strGUID.c_str(), &guidIntf) != RPC_S_OK)
			{
				JGW_FormatWString(mstrErrorMsg,L"Invalid GUID %s\n",strGUID.c_str());
				dwError = ERROR_INVALID_PARAMETER;
				__leave;
			}


			// open handle
			if ((dwError = OpenHandleAndCheckVersion(
				&hClient
				)) != ERROR_SUCCESS)
			{
				__leave;
			}

			// delete profile
			dwError = WlanDeleteProfile(
				hClient, 
				&guidIntf, 
				strWlanProfile.c_str(),        // profile name
				NULL            // reserved
				);

		}
		__finally
		{
			// clean up
			if (hClient != NULL)
			{
				WlanCloseHandle(
					hClient, 
					NULL            // reserved
					);
			}
		}

		PrintErrorMsg(L"DeleteWlanProfile", dwError);
		return (ERROR_SUCCESS == dwError);
	}
}