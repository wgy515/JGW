#pragma once
#include "..\usb\usbioctl.h"
#include <WinIoCtl.h>
#include <cfgmgr32.h>
#include <vector>
#include <string>
#include <tchar.h>

#define TESTRETURNOK(x)\
	if ( !(x ) )\
{\
return false ;\
}

#define FILTERINICONFIGNAME _T( "\\UseFilterPortConfig.ini" )
typedef struct  
{
	int         m_nPid;
	int         m_nVid;
}PIDANDVIDINFO,* PPIDANDVIDINFO;
// const LPTSTR strPidVidfilter[][20] = {
//    _T("VID_05C6&PID_9025"),
//    _T("VID_05C6&PID_9006"),
//    _T("VID_05C6&PID_9008"),
//    _T("VID_18D1&PID_D00D"),
//    _T("VID_1DBC&PID_0312")
// }

#define NUM_HCS_TO_CHECK                                  10 // Assumed number

typedef ULONG                          ARBFUNC;
typedef DWORD                                     DEVNODE, DEVINST ;
typedef DEVNODE                    * PDEVNODE, *PDEVINST ;
typedef CHAR                * DEVNODEID_A, *DEVINSTID_A ; // Device ID ANSI name.
typedef WCHAR                         * DEVNODEID_W, *DEVINSTID_W ; // Device ID Unicode name.
typedef CHAR                * DEVNODEID_A, *DEVINSTID_A ; // Device ID ANSI name.
typedef WCHAR                         * DEVNODEID_W, *DEVINSTID_W ; // Device ID Unicode name.

typedef DWORD                          LOG_CONF;
//typedef WORD                             CONFIGRET;

struct    nodelist_s
{
	struct nodelist_s              * nl_Next;                       // Next node element
	struct nodelist_s              * nl_Previous;    // Previous node element
	DEVNODE                                  nl_ItsDevNode;         // The dev node it represent
	LOG_CONF                     nl_Test_Req;            // Test resource alloc request
	ULONG                                       nl_ulSortDWord;       // Specifies the sort order
};

struct    nodelistheader_s
{
	struct nodelist_s              * nlh_Head;                    // First node element
	struct nodelist_s              * nlh_Tail;                       // Last node element
};

typedef struct nodelistheader_s   NODELISTHEADER;
typedef NODELISTHEADER                     * PNODELISTHEADER;

typedef struct _STRING_DESCRIPTOR_NODE
{
	struct _STRING_DESCRIPTOR_NODE    *Next;
	UCHAR                           DescriptorIndex;
	USHORT                          LanguageID;
	USB_STRING_DESCRIPTOR           StringDescriptor[0];
} STRING_DESCRIPTOR_NODE, * PSTRING_DESCRIPTOR_NODE;
//
typedef struct
{
	PUSB_NODE_INFORMATION               HubInfo;        // NULL if not a HUB
	LPTSTR                               HubName;        // NULL if not a HUB
	PUSB_NODE_CONNECTION_INFORMATION    ConnectionInfo ; // NULL if root HUB
	PUSB_DESCRIPTOR_REQUEST             ConfigDesc;     // NULL if root HUB
	PSTRING_DESCRIPTOR_NODE             StringDescs;
} USBDEVICEINFO, * PUSBDEVICEINFO;

//extern struct HUBPORTPOINTINFO;
// typedef struct 
// {
//    
// }HUBPORTPOINT,*PHUBPORTPOINT;
struct  EXTHUBPORTPOINTINFO
{
	int                                     m_nExtHubPortNumberIndex;
	TCHAR									m_szInstanceID[MAX_PATH ];//设备范例 ID
	std::vector <std:: wstring>				m_vChildInstanceID ;
};

class SPORTINSTANCEINFO 
{
public:
	std::wstring                        m_szInstanceID;//设备范例 ID
	std::vector <std:: wstring>			m_vChildInstanceID ;//子实例信息
	std::wstring  						m_szServerNumber;
public:
	void Init(){m_szServerNumber=L"";m_vChildInstanceID.clear();m_szInstanceID=L"";}
};
typedef class SPORTINSTANCEINFO *LPPORTINSTANCEINFO;

typedef struct  
{
	/*std::vector<int>        m_nExtHubPortNumberPointInfo;*/
	std::vector <EXTHUBPORTPOINTINFO>     m_vExtHubAllInfo;
}EXTHUBPORTPOINTCollectionINFO,* PEXTHUBPORTPOINCollectionTINFO;

class HUBPORTPOINTINFO
{
public:
    USHORT                              mbcdUSB; //! USB 1.1 0x0110,USB 2.0  0x0200,USB3.0 0x0300 
	std::wstring						m_strPhysicalAddress;// 物理端口地址
	SPORTINSTANCEINFO					m_sPortInstanceInfo ;//设备信息
	void Init(){m_strPhysicalAddress=L"";m_sPortInstanceInfo.Init();}
};
typedef class HUBPORTPOINTINFO *PHUBPORTPOINTINFO;// 直系信息

// typedef struct  
// {
// 	int         m_nHdc;
// 	int         m_nHubPorts;
// //	int         m_nExtHubPorts;
// 	std::wstring m_nExtHubPorts;
// 	void  Init ()
// 	{
// 		m_nHdc = -1;
// 		m_nHubPorts = -1;
// 		m_nExtHubPorts = L"-1";
// 	}
// }SPORTLOCATION,* LPPORTLOCATION;// 端口位置信息

extern std::vector <PHUBPORTPOINTINFO> g_vSortPortInfo;// 所有端口信息
//extern std::vector<LPPORTLOCATION>        g_vPortLocation;// 端口排序信息

typedef std::vector <PHUBPORTPOINTINFO> _vectorSort;

extern LPCTSTR     GetApplicationPath();
extern LPCTSTR   GetFilterIniConfigPath();
extern void         ParserLimiter(LPCTSTR ListStr, LPCTSTR delimiter,std::vector <std:: wstring> &_vectorInt );

// extern std::wstring FormatWString(std::wstring& strFormat,const wchar_t *szFormat,...);
//extern void ReplaceCharW(std::wstring& str,const wchar_t src, const wchar_t des);