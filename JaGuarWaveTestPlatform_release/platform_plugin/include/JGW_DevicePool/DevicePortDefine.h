#pragma once
#include <dbt.h>
#include <string>
#include <string>
#include <vector>
#include <map>

#define INIT_DEVICE_MSG_INFO_SIZE 100
#define DEVICE_MSG_TIME_INRERVAL		100

//! #define RECORDER_DEVICE_POOL_LOG

#ifdef RECORDER_DEVICE_POOL_LOG
	#define DEVICEPOOL_LOG_INFO Log4WI
	#define DEVICEPOOL_LOG_ERROR Log4WE
	#define DEVICEPOOL_LOG_INFO_FORMAT Log4WI_F
	#define DEVICEPOOL_LOG_ERROR_FORMAT Log4WE_F
#else
	#define DEVICEPOOL_LOG_INFO Log4WD
	#define DEVICEPOOL_LOG_ERROR Log4WE
	#define DEVICEPOOL_LOG_INFO_FORMAT Log4WD_F
	#define DEVICEPOOL_LOG_ERROR_FORMAT Log4WE_F
#endif


#define MAX_REG_GUID				10
// E_DEVICE_MODE   meDeviceMode;
// HWND            mhWnd;
// int				m_nmsgid;
// GUID            mGuid;   
#define REG_DEVICEPOOL_MSG(devicepoolThreadId,msgid,hwnd,guid,devicemode)\
{\
    JGW::S_REG_DEVICE_POOL_DEVINFO* lpReg = new JGW::S_REG_DEVICE_POOL_DEVINFO;\
    lpReg->mGuid = guid;\
    lpReg->mhWnd = hwnd;\
    lpReg->meDeviceMode = devicemode;\
    lpReg->m_nmsgid = msgid;\
    JGW::JGW_PostThreadMsg(devicepoolThreadId,REG_DEVICEPOOL_MSG_DEFINE,WPARAM(lpReg),NULL);\
}

#define REMOVE_DEVICEPOOL_MSG(devicepoolThreadId,hwnd)\
{\
    JGW::JGW_PostThreadMsg(devicepoolThreadId,REMOVE_PLUGIN_REG_DEVICEPOLL_DEFINE,WPARAM(hwnd),NULL);\
}

#if 1
//!GUID_USB_NETWORK LAN \\?\USB#Vid_3136&Pid_0001#Device_Serial_Number#{ad498944-762f-11d0-8dcb-00c04fc3358c}\{BF7215D0-31CD-41EC-9F7D-1FAB04F110C4}
#define GUID_NDIS_LAN_CLASS { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }

//! 通用串行总线控制器 GUID_DEVINTERFACE_USB_DEVICE
#define GUID_DEVINTERFACE_USB_DEVICE { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } }

//! ADB 端口  如ADB 驱动 fastboot 驱动 
#define GUID_ADB_INTERFACE_USB_DEVICE { 0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}}

//！COM 端口
#define GUID_COM_INTERFACE_USB_DEVICE  { 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } }

//! Modem 端口
#define GUID_MODEM_INTERFACE_USB_DEVICE {0x2c7089aa, 0x2e0e, 0x11d1, { 0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4 } }

// GUID_DEVINTERFACE_HID, 
#define GUID_DEVINTERFACE_HID { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }

// GUID_DEVINTERFACE_DISK
#define GUID_DEVINTERFACE_DISK 	{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } }

namespace JGW
{
    const wchar_t g_szDeviceMode[][16] = {L"",L"ADB",L"DIAG",
        L"MODEM",L"FASTBOOT",L"DOWNLOADER",
        L"LAN",L"ALL"};
#if 1
    enum E_DEVICE_MODE
    {
        E_DEVICE_UNKNOWMODE = 0,
        E_ADB_MODE = 1,
        E_DIAG_MODE = 2,
        E_MODEM_MODE = 4,
        E_FASTBOOT_MODE = 8,
        E_DOWNLOADER_MODE = 16,
        E_LAN_MODE = 32,
        E_ALL_MODE = 255,
    };
#else
    enum E_DEVICE_MODE
    {
        E_DEVICE_UNKNOWMODE = 0,
        E_ADB_MODE = 1,
        E_DIAG_MODE = 2,
        E_MODEM_MODE,
        E_FASTBOOT_MODE,
        E_DOWNLOADER_MODE,
        E_LAN_MODE,
        E_ALL_MODE,
        E_MAX_MODE
    };
#endif

    enum E_GUID_TYPE
    {
        E_ADB_GUID_TYPE = 0,
        E_COM_GUID_TYPE,
        E_MODEM_GUID_TYPE,
        E_LAN_GUID_TYPE,
        E_MAX_GUID_TYPE
    };

    const wchar_t g_szGuidTypeString[][10] = {L"ADB",L"COM",L"MODEM",L"LAN"};

    typedef struct
    { 
        E_GUID_TYPE meGuid;
        const wchar_t* mszGuidName;
        GUID mGuid; 
    }S_GUID_INFO;

    //! GUID的数组信息
    const S_GUID_INFO g_svGuidInfo[] = {
        {E_ADB_GUID_TYPE,g_szGuidTypeString[E_ADB_GUID_TYPE],GUID_ADB_INTERFACE_USB_DEVICE},
        {E_COM_GUID_TYPE,g_szGuidTypeString[E_COM_GUID_TYPE],GUID_COM_INTERFACE_USB_DEVICE},
        {E_MODEM_GUID_TYPE,g_szGuidTypeString[E_MODEM_GUID_TYPE],GUID_MODEM_INTERFACE_USB_DEVICE},
        {E_LAN_GUID_TYPE,g_szGuidTypeString[E_LAN_GUID_TYPE],GUID_NDIS_LAN_CLASS},
    };

    struct  S_USB_CONFIG
    {
        SHORT/*E_DEVICE_MODE*/   meDeviceMode; //! 设备模式
        DWORD           mnPID; //! PID
        DWORD           mnVID; //! VID
        std::wstring    mstrService; //!REG Service
        std::wstring    mstrDeviceDes; //! REG DeviceDesc
        std::wstring    mstrClass;

        S_USB_CONFIG():meDeviceMode(E_DEVICE_UNKNOWMODE)
            ,mnPID(0)
            ,mnVID(0)
        {

        }

        void InitUsbConfig()
        {
            meDeviceMode = E_DEVICE_UNKNOWMODE;
            mnPID = 0;
            mnVID = 0;
            mstrService = L"";
            mstrDeviceDes = L"";
            mstrClass = L"";
        }
    };

    typedef struct  
    {
        E_GUID_TYPE     meGuidType;
        std::vector<S_USB_CONFIG> mvUsbConfig;
    }S_GUID_USB_CONFIG,*PS_GUID_USB_CONFIG;

    typedef struct  
    {
        SHORT/*E_DEVICE_MODE*/   meDeviceMode;
        HWND            mhWnd;
        int				m_nmsgid;
        GUID            mGuid;   
    }S_REG_DEVICE_POOL_DEVINFO,*PS_REG_DEVICE_POOL_DEVINFO;


    typedef struct
    {
        std::vector<S_REG_DEVICE_POOL_DEVINFO> mvsRegDevPoolInfo;
    }S_GUID_REG_DEVICE_POOL_DEVINFO,*PS_GUID_REG_DEVICE_POOL_DEVINFO;

    typedef struct  
    {
        //! bool        mbIsRegGuidType; //! 当前GUID是否注册
        HDEVNOTIFY  mhDevNotify; //! 注册USB句柄
        int         count; //! 计数 
        E_GUID_TYPE meGuidType;
        std::vector<S_REG_DEVICE_POOL_DEVINFO> mvsRegDevPoolInfo; //! 当前GUID注册的消息
    }S_GUID_DEVICE_POOL_INFO,*PS_GUID_DEVICE_POOL_INFO;

    class GUID_KEY
    {
    public:
        GUID_KEY(GUID guid):mGuid(guid)
        {

        }

        bool operator < (const GUID_KEY &x) const 
        {
            return memcmp(&this->mGuid,&(x.mGuid),sizeof(GUID)) < 0;
        }

        bool operator == (const GUID_KEY &x) const
        {
            return (0 == memcmp(&this->mGuid,&(x.mGuid),sizeof(GUID)));
        }
    public:
        GUID mGuid;
    };

    /*DBT_DEVICEARRIVAL*/
    struct SDeviceMsg_Info
    {
        SDeviceMsg_Info():m_strDbcc_name(L"")
            ,m_nDeviceMsgType(DBT_DEVICEARRIVAL)
            // ,guidtype(E_MAX_GUID_TYPE)
            ,m_nDeviceMode(E_DEVICE_UNKNOWMODE)
            ,m_nRefCount(0)
            ,m_strRegUsbPidVid(L"")
            ,m_nSortIndex(-1)
            ,m_strSeralNumber(L"")
            ,m_nComPort(-1)
            ,m_bIsCache(false)
        {

        }

        void InitDeviceMsg()
        {
            m_strDbcc_name = (L"");
            m_nDeviceMsgType = (DBT_DEVICEARRIVAL);
            m_nDeviceMode = (E_DEVICE_UNKNOWMODE);
            m_nRefCount = (0);
            m_strRegUsbPidVid = (L"");
            m_nSortIndex = (-1);
            m_strSeralNumber = (L"");
            m_nComPort = (-1);
        }

        SHORT	        m_nDeviceMode;// 当前设备模式 如工厂模式  9006、9008、fastboot== 模式
        int				m_nSortIndex;  //! 排序索引
        int				m_nRefCount;//引用计数
        int				m_nComPort;
        DWORD			m_nDeviceMsgType;//插入 还是拔出
        GUID			m_guid;// 消息GUID
        std::wstring	m_strDbcc_name;//\\?\USB#Vid_05c6&Pid_9091&MI_00#7&30607750&1&0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
        std::wstring	m_strRegUsbPidVid;//! USB\Vid_05c6&Pid_9008\6&7c2d8f9&0&5
        std::wstring	m_strSeralNumber;//! adb 或者 fastboot SerialNumber
        bool            m_bIsCache;
        //! int				guidtype;//响应消息
        
        void Clear()
        {
            InterlockedExchange((LPLONG)&m_nRefCount,0);
        }

        void Dec()
        {
            InterlockedDecrement( (LPLONG)&m_nRefCount);
        }

        void Add()
        {
            if ( m_nRefCount < 0)
            {
                InterlockedExchange((LPLONG)&m_nRefCount,0);
            }
            InterlockedIncrement( (LPLONG)&m_nRefCount );
        }
    };  //->传到下面
}

#else
enum E_DEVICE_MSG
{
	E_UNKNOW_MSG = 0,
	E_COM_MSG,	// USB com口连接消息
	E_NetCard_MSG, // USB 网口插入消息
	E_ADB_MSG,
	E_MODEM_MSG,
	E_ALL_MSG
};

enum  DeviceModeEnum
{
	DEVICE_UNKNOWMODE = 0,
	DEVICE_9008 = 1,
	DEVICE_9006 ,
	DEVICE_FACTORY,
	DEVICE_FASTBOOT,
	DEVICE_LAN,
	E_DEVICE_ALL_MSG = 255
};

#define INSTANCE_9008_ L"VID_05C6&PID_9008"
/*#define INSTANCE_900E_ L"VID_05C6&PID_900E"*/
#define INSTANCE_9006_ L"VID_05C6&PID_9006"
#define INSTANCE_900E_ L"VID_05C6&PID_900"
#define INSTANCE_9091_ L"VID_05C6&PID_9091"
#define INSTANCE_VID05C6_	L"VID_05C6&PID"
#define INSTANCE_FASTBOOT_	L"VID_18D1&PID_D00D"
#define INSTANCE_LAN_		L"VID_3136&PID_0001"
#define INSTANCE_LAN_IN		L"VID_3136&PID_"

class CRegMsgType
{
public:
	CRegMsgType():m_hWnd(NULL)
		,m_guidtype(E_UNKNOW_MSG)
		,m_eDeviceMode(DEVICE_UNKNOWMODE)
		,m_nmsgid(0)
	{

	}
	int				m_guidtype;//响应消息
	int				m_eDeviceMode;//响应设备模式消息
	HWND			m_hWnd;
	GUID			m_guid;
	int				m_nmsgid;
};

// #define REG_DEVICEPOOL_MSG(devicepoolThreadId,msgid,hwnd,guid,guidtype,devicemode)\
// {\
// 	CRegMsgType* lpReg = new CRegMsgType;\
// 	lpReg->m_guid = guid;\
// 	lpReg->m_hWnd = hwnd;\
// 	lpReg->m_eDeviceMode = devicemode;\
// 	lpReg->m_guidtype = guidtype;\
// 	lpReg->m_nmsgid = msgid;\
// 	JGW::JGW_PostThreadMsg(devicepoolThreadId,REG_DEVICEPOOL_MSG_DEFINE,WPARAM(lpReg),NULL);\
// }

// #define ADD_DEVICEPOOL_MSG(devicepoolThreadId,guid,dbcc_name,devicemsgtype)\
// {\
// 	SDeviceMsg_Info* lpdevicemsg = new SDeviceMsg_Info;\
// 	lpdevicemsg->m_nDeviceMsgType = devicemsgtype;\
// 	wchar_t* szdbcc_name = new wchar_t[wcslen(dbcc_name)+1];\
// 	if(!szdbcc_name) return FALSE;\
// 	wcscpy_s(szdbcc_name,wcslen(dbcc_name)+1,dbcc_name);\
// 	lpdevicemsg->m_lpDbcc_name = szdbcc_name;\
// 	GUID* lpguid = new GUID;\
// 	memcpy(lpguid,&guid,sizeof(GUID));\
// 	PostThreadMsg(devicepoolThreadId,ADD_DEVICEPOOL_MSG_DEFINE,WPARAM(lpdevicemsg),LPARAM(lpguid));\
// }

//!GUID_USB_NETWORK \\?\USB#Vid_3136&Pid_0001#Device_Serial_Number#{ad498944-762f-11d0-8dcb-00c04fc3358c}\{BF7215D0-31CD-41EC-9F7D-1FAB04F110C4}
#define GUID_NDIS_LAN_CLASS { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }

//! 通用串行总线控制器 GUID_DEVINTERFACE_USB_DEVICE
#define GUID_DEVINTERFACE_USB_DEVICE { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } }

//! ADB 端口  如ADB 驱动 fastboot 驱动 
#define GUID_ADB_INTERFACE_USB_DEVICE { 0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}}

//！COM 端口
#define GUID_COM_INTERFACE_USB_DEVICE  { 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } }

//! Modem 端口
#define GUID_MODEM_INTERFACE_USB_DEVICE {0x2c7089aa, 0x2e0e, 0x11d1, { 0xb1, 0x14, 0x00, 0xc0, 0x4f, 0xc2, 0xaa, 0xe4 } }

// GUID_DEVINTERFACE_HID, 
#define GUID_DEVINTERFACE_HID { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }

// GUID_DEVINTERFACE_DISK
#define GUID_DEVINTERFACE_DISK 	{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } }

class S_REG_PIDVID
{
public:
	S_REG_PIDVID():m_strRegPidVid(L"")
		,m_nSortIndex(-1)
	{

	}
	std::wstring m_strRegPidVid;
	int			 m_nSortIndex;
};

/*DBT_DEVICEARRIVAL*/
//class SDeviceMsg_Info
//{
//public:
//	SDeviceMsg_Info():m_strDbcc_name(L"")
//		,m_nDeviceMsgType(DBT_DEVICEARRIVAL)
//		,guidtype(E_UNKNOW_MSG)
//		,m_nDeviceMode(DEVICE_UNKNOWMODE)
//		,m_nRefCount(0)
//		,m_strRegUsbPidVid(L"")
//		,m_nSortIndex(-1)
//		,m_strSeralNumber(L"")
//		,m_nComPort(-1)
//	{
//
//	}
//	std::wstring	m_strDbcc_name;//\\?\USB#Vid_05c6&Pid_9091&MI_00#7&30607750&1&0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
//	std::wstring	m_strRegUsbPidVid;//! USB\Vid_05c6&Pid_9008\6&7c2d8f9&0&5
//	std::wstring	m_strSeralNumber;//! adb 或者 fastboot SerialNumber
//	int				m_nSortIndex;  //! 排序索引
//	int				m_nDeviceMsgType;//插入 还是拔出
//	int				guidtype;//响应消息
//	BYTE			m_nDeviceMode;// 当前设备模式 如工厂模式  9006、9008、fastboot== 模式
//	int				m_nRefCount;//引用计数
//	GUID			m_guid;// 消息GUID
//	int				m_nComPort;
//	void Clear()
//	{
//		InterlockedExchange((LPLONG)&m_nRefCount,0);
//	}
//	void Dec()
//	{
//		InterlockedDecrement( (LPLONG)&m_nRefCount);
//	}
//	void Add()
//	{
//		if ( m_nRefCount < 0)
//		{
//			InterlockedExchange((LPLONG)&m_nRefCount,0);
//		}
//		InterlockedIncrement( (LPLONG)&m_nRefCount );
//	}
};  //->传到下面
#endif
