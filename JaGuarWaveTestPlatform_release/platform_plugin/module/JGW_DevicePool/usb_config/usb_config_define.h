#pragma once
#include <string>
#include <vector>
#include <map>
#if 0
namespace JGW
{

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
    //! 设备模式
    const wchar_t g_szDeviceMode[][16] = {L"",L"ADB",L"DIAG",
        L"MODEM",L"FASTBOOT",L"DOWNLOADER",
        L"LAN",L"ALL",L""};
    
    enum E_DEVICE_MODE
    {
        E_DEVICE_UNKNOWMODE = 0,
        E_ADB_MODE,
        E_DIAG_MODE,
        E_MODEM_MODE,
        E_FASTBOOT_MODE,
        E_DOWNLOADER_MODE,
        E_LAN_MODE,
        E_ALL_MODE,
        E_MAX_MODE
    };

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
        E_DEVICE_MODE   meDeviceMode; //! 设备模式
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
        E_DEVICE_MODE   meDeviceMode;
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
    private:
        GUID mGuid;
    };
}
#endif