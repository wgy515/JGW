// CAutoUsbPort.cpp : 定义DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CAutoUsbPort.h"
#include <algorithm>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
//#include <locale.h>
#define FUNCERRORCONTINUE(x)\
	if( !(x) )\
	continue

/*CCAutoUsbPort *CCAutoUsbPort::m_lpAutoUsbPort = NULL;*/

CCAutoUsbPort::CCAutoUsbPort ():m_lpConnectionInfo( NULL)
	, m_nConnectionInfoBytes(0)
	, m_nHCDIndex(-1)
	, m_strPhysicalAddress(L"-1")
	, m_lpHubPortPointInfo(NULL )
{
	// 分配足够的空间以容纳个管道的连接信息
	// 端口数为端口是标准的控制端口不在配置描述符中这样 IN 端口和OUT 端口都是到,
	// 所以每个设备配置最大有个端口
	m_nConnectionInfoBytes = sizeof(USB_NODE_CONNECTION_INFORMATION) + sizeof(USB_PIPE_INFO ) * 30;
	m_lpConnectionInfo = (PUSB_NODE_CONNECTION_INFORMATION ) new CHAR[ m_nConnectionInfoBytes ];
	memset( m_lpConnectionInfo ,0x00,m_nConnectionInfoBytes );
	LoadFilterIniConfig();
}

CCAutoUsbPort::~CCAutoUsbPort ()
{
	if ( m_lpConnectionInfo )
	{
		delete[] (CHAR*)m_lpConnectionInfo;
		m_lpConnectionInfo = NULL ;
	}
	ClearDeviceAllSortInfo();
}

void CCAutoUsbPort::EnumerateHostControllers()
{
	PUSB_ROOT_HUB_NAME	pUsbRootHubName = NULL;
	memset(m_lpConnectionInfo ,0x00,m_nConnectionInfoBytes);

	for ( m_nHCDIndex = 0; m_nHCDIndex < NUM_HCS_TO_CHECK ;m_nHCDIndex ++)
	{
		HANDLE  hHCDev   = OpenHostControllersToHCDIndex( m_nHCDIndex );
		if (INVALID_HANDLE_VALUE == hHCDev) continue;
		pUsbRootHubName = GetRootHubName ( hHCDev );
		if ( !pUsbRootHubName ) continue;;
		mstrRootHubName = pUsbRootHubName->RootHubName;
        mstrRootHubName.erase(mstrRootHubName.find(_T("#{")));
		EnumerateHub(pUsbRootHubName->RootHubName,NULL, NULL,NULL ,NULL,mstrRootHubName);
		delete[] pUsbRootHubName ;
		CloseHandle(hHCDev );
	}
}


void CCAutoUsbPort::EnumerateHub(LPTSTR HubName, PUSB_NODE_CONNECTION_INFORMATION ConnectionInfo , PUSB_DESCRIPTOR_REQUEST ConfigDesc , PSTRING_DESCRIPTOR_NODE StringDescs , LPTSTR DeviceDesc,std::wstring strHubName)
{
	int nHubPortsCounts = 0;
	HANDLE hHubDevice = GetHubPortsCountToHubName(HubName ,nHubPortsCounts, ConnectionInfo);
	if (hHubDevice == INVALID_HANDLE_VALUE) return;

	EnumerateHubPorts(hHubDevice,nHubPortsCounts,strHubName);
	CloseHandle(hHubDevice);
#ifdef _DEBUG
	ShowDeviceAllInfo();
#endif
	return;
}

void CCAutoUsbPort::EnumerateHubPorts(HANDLE hHubDevice, ULONG NumPorts,std::wstring strHubName)
{
	ULONG       nIndex = 0;
	TCHAR       driverKeyName [MAX_PATH] = {0} ;
	ULONG       nBytes = 0;
	std::wstring strTemp,strTempHubName;
	PUSB_NODE_CONNECTION_DRIVERKEY_NAME pUsbNodeConnectionDriverKeyName = NULL;
	// 遍历该HUB 的所有口口索引是基 ,不是基于0的索引
	for ( nIndex =1; nIndex <= NumPorts; ++nIndex  )
	{
        strTempHubName = strHubName;
		memset( m_lpConnectionInfo ,0x00,m_nConnectionInfoBytes );
		if ( pUsbNodeConnectionDriverKeyName ) { delete[] (CHAR*)pUsbNodeConnectionDriverKeyName; pUsbNodeConnectionDriverKeyName = NULL; }
		m_lpConnectionInfo->ConnectionIndex = nIndex;

		FUNCERRORCONTINUE ( DeviceIoControl ( hHubDevice ,
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION ,
			m_lpConnectionInfo,
			m_nConnectionInfoBytes ,
			m_lpConnectionInfo,
			m_nConnectionInfoBytes,
			& nBytes ,
			NULL ) );

		if ( m_lpConnectionInfo ->DeviceIsHub )
		{
			PUSB_NODE_CONNECTION_NAME pExtUsbNodeConnentionName = NULL;
			FUNCERRORCONTINUE(pExtUsbNodeConnentionName = GetExternalHubName(hHubDevice ,nIndex));
            std::wstring strExternalHubName = strHubName;
            JGW::JGW_FormatWString(strTemp,L"#%d",nIndex);
            strExternalHubName += strTemp.c_str();
			EnumerateHub(pExtUsbNodeConnentionName->NodeName ,m_lpConnectionInfo, NULL,NULL ,NULL,strExternalHubName);
			delete[] (CHAR*)pExtUsbNodeConnentionName;
			//! m_strPhysicalAddress = L"";
			//下个端口
			continue;
		}

        JGW::JGW_FormatWString(strTemp,L"\\%d",nIndex);
        strTempHubName += strTemp.c_str();
        //m_strPhysicalAddress = mstrRootHubName;
 		//m_strPhysicalAddress += m_strHubName.c_str();
        //! m_strPhysicalAddress += strTemp.c_str();
// 		m_strPhysicalAddress.erase(m_strPhysicalAddress.find(_T("#{")));
// 		ReplaceCharW(m_strPhysicalAddress,L'#',L'\\');
		_stprintf_s(driverKeyName,L"PORT%d",nIndex);
		strTempHubName += driverKeyName;
        m_strPhysicalAddress = strTempHubName;
		std::transform(m_strPhysicalAddress.begin(), m_strPhysicalAddress.end(), m_strPhysicalAddress.begin(), towupper);
#ifdef _DEBUG
		OutputDebugString(m_strPhysicalAddress.c_str());
		OutputDebugString(L"\n");
#endif
		FUNCERRORCONTINUE( m_lpConnectionInfo->ConnectionStatus != NoDeviceConnected );
		FUNCERRORCONTINUE( FilterPidVid ( m_lpConnectionInfo ) );
		FUNCERRORCONTINUE( m_lpHubPortPointInfo = GetEnumHubCollectionInfoToPhysicalAddress(m_strPhysicalAddress) );
        m_lpHubPortPointInfo->mbcdUSB = m_lpConnectionInfo->DeviceDescriptor.bcdUSB;
		FUNCERRORCONTINUE( pUsbNodeConnectionDriverKeyName = GetDriverKeyName(hHubDevice,nIndex) );
		FUNCERRORCONTINUE( DriverNameToDeviceDesc(pUsbNodeConnectionDriverKeyName->DriverKeyName,m_szBuffer,&m_lpHubPortPointInfo->m_sPortInstanceInfo ) );
		GetSerialNumber(hHubDevice,nIndex,&m_lpConnectionInfo->DeviceDescriptor);
		m_lpHubPortPointInfo->m_strPhysicalAddress = m_strPhysicalAddress;
		m_strPhysicalAddress = _T("");
		UpdateSortIniConfig();
	}
	if ( pUsbNodeConnectionDriverKeyName ) delete[] (CHAR*)pUsbNodeConnectionDriverKeyName;
}

void CCAutoUsbPort::GetSerialNumber(HANDLE hHubDevice,ULONG ConnectionIndex,PUSB_DEVICE_DESCRIPTOR DeviceDesc)
{
	if ( !DeviceDesc->iSerialNumber ) return;
	PSTRING_DESCRIPTOR_NODE supportedLanguagesString;
	PSTRING_DESCRIPTOR_NODE stringDescNodeTail;
	ULONG                   numLanguageIDs;
	USHORT                  *languageIDs;
	// 	PUCHAR                  descEnd;
	// 	PUSB_COMMON_DESCRIPTOR  commonDesc;
	// 取支持的语言ID数组,它在字串描述符的0中返回
	supportedLanguagesString = GetStringDescriptor(hHubDevice,ConnectionIndex,0,0);
	if (supportedLanguagesString == NULL) return /*NULL*/;
	numLanguageIDs = (supportedLanguagesString->StringDescriptor->bLength - 2) / 2;
	languageIDs = (USHORT*)&supportedLanguagesString->StringDescriptor->bString[0];
	stringDescNodeTail = supportedLanguagesString;
	// 取设备描述符字串
	// 	if (DeviceDesc->iManufacturer)
	// 	{
	// 		stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iManufacturer,
	// 			numLanguageIDs,languageIDs,stringDescNodeTail);
	// 	}
	// 	if (DeviceDesc->iProduct)
	// 	{
	// 		stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iProduct,
	// 			numLanguageIDs,languageIDs,stringDescNodeTail);
	// 	}

	stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iSerialNumber,
		numLanguageIDs,languageIDs,stringDescNodeTail);

	unsigned long str_len = (stringDescNodeTail->StringDescriptor->bLength -
		FIELD_OFFSET(USB_STRING_DESCRIPTOR, bString)) /
		sizeof(wchar_t);

	m_lpHubPortPointInfo->m_sPortInstanceInfo.m_szServerNumber = stringDescNodeTail->StringDescriptor->bString;
	m_lpHubPortPointInfo->m_sPortInstanceInfo.m_szServerNumber[str_len] = '\0';

	// 	unsigned long str_len = (stringDescNodeTail->StringDescriptor->bLength -
	// 		FIELD_OFFSET(USB_STRING_DESCRIPTOR, bString)) /
	// 		sizeof(wchar_t);
	// 
	// 	wchar_t szBuffer[100] = {0};
	// 
	// 	wcscpy_s(szBuffer,100,stringDescNodeTail->StringDescriptor->bString);
	// 	szBuffer[str_len] = '\0';
}

//*****************************************************************************
// 取字串描述符
// hHubDevice      - 包含被请求描述符的口的HUB设备的句柄 .
// ConnectionIndex - 挂在hub上的请求描述符的设备的索引.
// DescriptorIndex - 字串描述符索引.
// NumLanguageIDs -  被请求描述符的语言数目.
// LanguageID      - 字串描述符的语言.
//*****************************************************************************
PSTRING_DESCRIPTOR_NODE CCAutoUsbPort::GetStringDescriptors(HANDLE hHubDevice,ULONG ConnectionIndex,
											UCHAR DescriptorIndex,ULONG NumLanguageIDs,
											USHORT* LanguageIDs,PSTRING_DESCRIPTOR_NODE StringDescNodeTail)
{
    for (ULONG i=0; i<NumLanguageIDs; i++)
    {
        StringDescNodeTail->Next = GetStringDescriptor(hHubDevice,ConnectionIndex,DescriptorIndex,*LanguageIDs);
        if ( StringDescNodeTail->Next ) StringDescNodeTail = StringDescNodeTail->Next;
        LanguageIDs++;
    }
//
    return StringDescNodeTail;
}

//*****************************************************************************
// 取字串描述符
// hHubDevice      - 包含被请求描述符的口的HUB设备的句柄 .
// ConnectionIndex - 挂在hub上的请求描述符的设备的索引.
// DescriptorIndex - 字串描述符索引.
// LanguageID      - 字串描述符的语言.
//*****************************************************************************
PSTRING_DESCRIPTOR_NODE CCAutoUsbPort::GetStringDescriptor(HANDLE hHubDevice,
							ULONG ConnectionIndex,UCHAR DescriptorIndex,USHORT LanguageID)
{
    BOOL    success;
    ULONG   nBytes;
    ULONG   nBytesReturned;

    UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) + MAXIMUM_USB_STRING_LENGTH];
    PUSB_DESCRIPTOR_REQUEST stringDescReq;
    PUSB_STRING_DESCRIPTOR  stringDesc;
    PSTRING_DESCRIPTOR_NODE stringDescNode;
    nBytes = sizeof(stringDescReqBuf);
    stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
    stringDesc = (PUSB_STRING_DESCRIPTOR)(stringDescReq+1);
// 置0
    memset(stringDescReq, 0, nBytes);
// 指出所请求描述符的口
    stringDescReq->ConnectionIndex = ConnectionIndex;
// USBHUB使用URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE来处理该
// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION 请求.
// USBD会自动初始化这些段:
// bmRequest = 0x80
// bRequest  = 0x06
// 我们必须初始化下面的段:
// wValue    = 描述符类型(高字节)和描述符索引(低字节)
// wIndex    = 0 (或者对字串描述符为语言ID)
// wLength   = 描述符缓存的长度
    stringDescReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8) | DescriptorIndex;
    stringDescReq->SetupPacket.wIndex = LanguageID;
    stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));
// 发出取描述符请求.
    success = DeviceIoControl(hHubDevice,IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
                              stringDescReq,nBytes,stringDescReq,nBytes,&nBytesReturned,NULL);
// 检查返回值.
    if (!success)
    {
		//AfxMessageBox("Oop13!");
        return NULL;
    }
    if (nBytesReturned < 2)
    {
		//AfxMessageBox("Oop14!");
        return NULL;
    }
    if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE)
    {
		//AfxMessageBox("Oop15!");
        return NULL;
    }
    if (stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST))
    {
		//AfxMessageBox("Oop16!");
        return NULL;
    }
    if (stringDesc->bLength % 2 != 0)
    {
		//AfxMessageBox("Oop17!");
        return NULL;
    }
// 分配内存并复制字串描述符.
    stringDescNode=(PSTRING_DESCRIPTOR_NODE) new CHAR[sizeof(STRING_DESCRIPTOR_NODE)+stringDesc->bLength];
    if (stringDescNode == NULL)
    {
		//AfxMessageBox("Oop18!");
        return NULL;
    }
    stringDescNode->DescriptorIndex = DescriptorIndex;
    stringDescNode->LanguageID = LanguageID;
    memcpy(stringDescNode->StringDescriptor,stringDesc,stringDesc->bLength);
//
	return stringDescNode;
}

//*****************************************************************************
// 取所有字串描述符
// hHubDevice      - 包含被请求描述符的口的HUB设备的句柄 .
// ConnectionIndex - 挂在hub上的请求描述符的设备的索引.
// DeviceDesc      - 要请求字串描述符的设备描述符的.
// ConfigDesc      - 要请求字串描述符的配置描述符(及接口描述符).
//*****************************************************************************
// PSTRING_DESCRIPTOR_NODE CVcUsbViewDlg::GetAllStringDescriptors(HANDLE hHubDevice,ULONG ConnectionIndex,
// 							PUSB_DEVICE_DESCRIPTOR DeviceDesc,PUSB_CONFIGURATION_DESCRIPTOR ConfigDesc)
// {
//     PSTRING_DESCRIPTOR_NODE supportedLanguagesString;
//     PSTRING_DESCRIPTOR_NODE stringDescNodeTail;
//     ULONG                   numLanguageIDs;
//     USHORT                  *languageIDs;
// 
//     PUCHAR                  descEnd;
//     PUSB_COMMON_DESCRIPTOR  commonDesc;
// // 取支持的语言ID数组,它在字串描述符的0中返回
//     supportedLanguagesString = GetStringDescriptor(hHubDevice,ConnectionIndex,0,0);
//     if (supportedLanguagesString == NULL) return NULL;
//     numLanguageIDs = (supportedLanguagesString->StringDescriptor->bLength - 2) / 2;
//     languageIDs = (USHORT*)&supportedLanguagesString->StringDescriptor->bString[0];
//     stringDescNodeTail = supportedLanguagesString;
// // 取设备描述符字串
//     if (DeviceDesc->iManufacturer)
//     {
//         stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iManufacturer,
//                                                   numLanguageIDs,languageIDs,stringDescNodeTail);
//     }
//     if (DeviceDesc->iProduct)
//     {
//         stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iProduct,
//                                                   numLanguageIDs,languageIDs,stringDescNodeTail);
//     }
//     if (DeviceDesc->iSerialNumber)
//     {
//         stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,DeviceDesc->iSerialNumber,
//                                                   numLanguageIDs,languageIDs,stringDescNodeTail);
//     }
// // 取配置和接口描述符字串
//     descEnd = (PUCHAR)ConfigDesc + ConfigDesc->wTotalLength;
//     commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;
//     while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
// 			           (PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
//     {
//         switch (commonDesc->bDescriptorType)
//         {
//             case USB_CONFIGURATION_DESCRIPTOR_TYPE:
// 				numLanguageIDs=sizeof(USB_CONFIGURATION_DESCRIPTOR);
//                 if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))//9
//                 {
// 					AfxMessageBox("Oop11!");
//                     break;
//                 }
//                 if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
//                 {
//                     stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,
//                                              ((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration,
//                                              numLanguageIDs,languageIDs,stringDescNodeTail);
//                 }
//                 commonDesc= (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc+commonDesc->bLength);
//                 continue;
//             case USB_INTERFACE_DESCRIPTOR_TYPE:
//                 if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
//                     commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
//                 {
// 					AfxMessageBox("Oop12!");
//                     break;
//                 }
//                 if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
//                 {
//                     stringDescNodeTail = GetStringDescriptors(hHubDevice,ConnectionIndex,
//                                              ((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface,
//                                              numLanguageIDs,languageIDs,stringDescNodeTail);
//                 }
//                 commonDesc= (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc+commonDesc->bLength);
//                 continue;
//             default:
//                 commonDesc= (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc+commonDesc->bLength);
//                 continue;
//         }
//         break;
//     }
// //
//     return supportedLanguagesString;
// }

LPCTSTR     GetApplicationPath ()
{
	static TCHAR szZtePath[ MAX_PATH * 2] = {0};

	if ( _tcslen ( szZtePath ) > 1)
	{
		return szZtePath ;
	}

	GetModuleFileName(NULL , szZtePath, sizeof(szZtePath ));
	LPCTSTR pszTemp = szZtePath;
	LPCTSTR psz = NULL ;

	while ( (psz = _tcsstr( pszTemp,_T("\\" ))) != NULL )
	{
		psz ++;
		pszTemp = psz ;
	}

	szZtePath[_tcslen ( szZtePath ) - _tcslen(pszTemp )] = '\0';

	return szZtePath ;
}

LPCTSTR GetFilterIniConfigPath ()
{
	static TCHAR szIniPath[ MAX_PATH * 2] = {0};
	if ( _tcslen ( szIniPath ) > 1)
	{
		return szIniPath ;
	}
	_stprintf_s(szIniPath ,MAX_PATH * 2,_T("%s%s" ),GetApplicationPath(), FILTERINICONFIGNAME);
	return szIniPath ;
}

void ParserLimiter (LPCTSTR ListStr,LPCTSTR delimiter,std::vector <std:: wstring> &_vectorInt )
{
	if (!ListStr || !delimiter)
	{
		return;
	}

	size_t length = _tcslen( ListStr)+1;
	TCHAR *pSource = new TCHAR[length ];

	_tcscpy_s(pSource ,length, ListStr);
	TCHAR *szBuffer = NULL;

	for (TCHAR *pSubStr = _tcstok_s(pSource ,delimiter,& szBuffer);
		pSubStr;
		pSubStr = _tcstok_s (NULL, delimiter,&szBuffer ))
	{
		std::wstring temp = pSubStr;
		_vectorInt.push_back (temp);
	}

	if(pSource != NULL)
	{
		delete [] pSource ;
		pSource = NULL ;
	}
}

void reg_close_key(HKEY* pkey)
{
	if( pkey && *pkey )
	{
		RegCloseKey(*pkey);
		*pkey = NULL;
		delete pkey;
		pkey = NULL;
	}
}

void UpdateRegUsbFlags( int pid,int vid )
{
	std::wstring strSubKey = _T("SYSTEM\\CurrentControlSet\\Control\\UsbFlags\\");
	BYTE szInfo[10] = {0};
	BYTE data = 0x01;
	DWORD dwSize = 1;
	DWORD dwType = REG_BINARY;
	std::wstring strTemp;
	std::shared_ptr<HKEY> ptr_key(new HKEY,reg_close_key);

	if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE | KEY_SET_VALUE /*| KEY_WOW64_32KEY */ |KEY_WOW64_64KEY,ptr_key.get()) )
	{
		//THROW_JGW_EXCEPTION(_T("Open Reg Fail!"),GetLastError());
		return;
	}

	JGW::JGW_FormatWString(strTemp,L"IgnoreHWSerNum%04X%04X",vid,pid);

	if( ERROR_SUCCESS == RegQueryValueEx(*(ptr_key.get()), strTemp.c_str(), NULL, &dwType, szInfo, &dwSize) && 0x01 == szInfo[0])
	{
		return;
	}

	dwType = REG_BINARY;
	dwSize = 1;
	if( RegSetValueEx(*(ptr_key.get()), strTemp.c_str(), NULL, dwType, &data, dwSize) != ERROR_SUCCESS)
	{
		//THROW_JGW_EXCEPTION(_T("RegSetValueEx"),GetLastError());
		return;
	}
}
extern int g_nUserPortSort;
void CCAutoUsbPort ::LoadFilterIniConfig()
{
	m_vFilterPidVid.clear ();
	g_vSortPortInfo.clear ();
	LPCTSTR  lpIniPath = GetFilterIniConfigPath();
	TCHAR  strTemp [MAX_PATH * 2] = {0};

	if ( 2 < GetPrivateProfileString(_T("MAIN" ),_T( "FilterPidVid"),_T(""),strTemp,512, lpIniPath) )
	{
		std::vector <std:: wstring> vStrTemp ;
		ParserLimiter(strTemp ,_T( ","),vStrTemp );
		LPCTSTR  lpTemp = NULL;
		LPTSTR lpstr ;
		PIDANDVIDINFO sPidVidInfo = {0};

		for (USHORT i = 0; i < vStrTemp .size() && 256 > vStrTemp.size ();i ++)
		{
			lpTemp = vStrTemp .at( i).c_str ();

			sPidVidInfo.m_nVid = (int) _tcstol(lpTemp ,&lpstr,16);
			lpTemp = _tcsstr (lpTemp, _T("-" ));
			++ lpTemp;
			sPidVidInfo.m_nPid = (int) _tcstol(lpTemp ,&lpstr,16);
			m_vFilterPidVid.push_back (sPidVidInfo);
			UpdateRegUsbFlags(sPidVidInfo.m_nPid,sPidVidInfo.m_nVid);
		}
	}
	LoadSortIniConfig(lpIniPath );
}

void CCAutoUsbPort ::LoadSortIniConfig( LPCTSTR lpIniPath )
{
	int nSortCounts = 0;
	TCHAR szAppName[100] = {0};
	TCHAR szPhysicalAddress[260] = {0};

	g_nUserPortSort = GetPrivateProfileInt(_T("MAIN" ),_T( "DETECT_INST_PORT_NO"),0,lpIniPath );
	nSortCounts = GetPrivateProfileInt(_T("MAIN" ),_T( "DevCounts"),0,lpIniPath );
	if ( g_nUserPortSort ) return;

	for (int i = 1; i <= nSortCounts && nSortCounts < 20; i ++)
	{
		_stprintf_s(szAppName ,100,_T( "DEV_%d"),i);
		GetPrivateProfileString(szAppName,_T( "PhysicalAddress"),L"",szPhysicalAddress,260,lpIniPath);

		PHUBPORTPOINTINFO lpHubInfo = GetEnumHubCollectionInfoToPhysicalAddress(szPhysicalAddress);
// 		lpHubInfo->m_nHCDIndex = sPortLocation. m_nHdc;
// 		lpHubInfo->m_nHubPortNumberIndex = sPortLocation.m_nHubPorts ;
// 		lpHubInfo->m_bIsExtHub = _ttoi(sPortLocation.m_nExtHubPorts.c_str()) > -1?true:false ;
		lpHubInfo->m_strPhysicalAddress = szPhysicalAddress ;
	}

}

void CCAutoUsbPort ::WriteSortIniConfig( PHUBPORTPOINTINFO lpUsbInfo )
{
	if ( !lpUsbInfo )
	{
		return ;
	}
	static TCHAR        szAppName[100] = {0};
	static TCHAR        szTemp[100] = {0};
	LPCTSTR  lpIniPath = GetFilterIniConfigPath();

	_stprintf_s(szAppName ,100,_T( "DEV_%d"),g_vSortPortInfo .size());
	_stprintf_s(szTemp ,100,_T( "%d"),g_vSortPortInfo .size());
	WritePrivateProfileString(_T("MAIN" ),_T( "DevCounts"),szTemp ,lpIniPath);

	WritePrivateProfileString(szAppName ,_T( "PhysicalAddress"),lpUsbInfo->m_strPhysicalAddress.c_str(),lpIniPath);
}

void CCAutoUsbPort ::UpdateSortIniConfig()
{
	_vectorSort:: iterator it ;
	for (it = g_vSortPortInfo .begin(); it != g_vSortPortInfo .end(); it ++)
	{
		WriteSortIniConfig(it[0]);
	}
}

void CCAutoUsbPort ::ClearSortInfo()
{
	LPCTSTR  lpIniPath = GetFilterIniConfigPath();
	WritePrivateProfileString(_T("MAIN" ),_T( "DevCounts"),_T("0" ),lpIniPath);
	ClearDeviceAllSortInfo();
}
