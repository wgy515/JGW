#include "StdAfx.h"
#include "CJGW_FilteUsbConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_DevicePool/Auto_ID_Usb.h>

namespace JGW
{
    CCJGW_FilteUsbConfig::CCJGW_FilteUsbConfig(void)
    {
    }


    CCJGW_FilteUsbConfig::~CCJGW_FilteUsbConfig(void)
    {
    }

    bool CCJGW_FilteUsbConfig::FilteUsbConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice)
    {
        switch (psGuidUsbConfig->meGuidType)
        {
        case E_ADB_GUID_TYPE:
            return FilteADBConfigInfo(psGuidUsbConfig,lpdevice,NULL);
        case E_COM_GUID_TYPE:
            return FilteComConfigInfo(psGuidUsbConfig,lpdevice,NULL);
        case E_MODEM_GUID_TYPE:
            return FilteModemonfigInfo(psGuidUsbConfig,lpdevice,NULL);
        case E_LAN_GUID_TYPE:
            return FilteLanConfigInfo(psGuidUsbConfig,lpdevice,NULL);
        case E_MAX_GUID_TYPE:
            return FilteMaxUsbConfigInfo(psGuidUsbConfig,lpdevice,NULL);
        }

        return true;
    }

    bool CCJGW_FilteUsbConfig::FilteADBConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo)
    {
        for (std::vector<S_USB_CONFIG>::iterator it = psGuidUsbConfig->mvUsbConfig.begin();
            it != psGuidUsbConfig->mvUsbConfig.end();
            ++ it)
        {
            if (CheckUsbPidVid(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbService(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbDeviceDes(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbClass(lpdevice->m_strRegUsbPidVid.c_str(),&it[0]))
            {
                lpdevice->m_nDeviceMode = it[0].meDeviceMode;
                /*lpdevice->m_strSeralNumber = GetADBSerialNumber(lpdevice->m_strRegUsbPidVid.c_str());*/
                return true;
            }
        }
        return false;
    }

    bool CCJGW_FilteUsbConfig::FilteComConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo)
    {
        for (std::vector<S_USB_CONFIG>::iterator it = psGuidUsbConfig->mvUsbConfig.begin();
            it != psGuidUsbConfig->mvUsbConfig.end();
            ++ it)
        {
            if (CheckUsbPidVid(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbService(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbDeviceDes(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbClass(lpdevice->m_strRegUsbPidVid.c_str(),&it[0]))
            {
                lpdevice->m_nDeviceMode = it[0].meDeviceMode;
                lpdevice->m_nComPort = GetUsbComPort(lpdevice->m_strRegUsbPidVid.c_str());
                return true;
            }
        }
        return false;
    }

    bool CCJGW_FilteUsbConfig::FilteModemonfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo)
    {
        for (std::vector<S_USB_CONFIG>::iterator it = psGuidUsbConfig->mvUsbConfig.begin();
            it != psGuidUsbConfig->mvUsbConfig.end();
            ++ it)
        {
            if (CheckUsbPidVid(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbService(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbDeviceDes(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbClass(lpdevice->m_strRegUsbPidVid.c_str(),&it[0]))
            {
                lpdevice->m_nDeviceMode = it[0].meDeviceMode;
                lpdevice->m_nComPort = GetUsbComPort(lpdevice->m_strRegUsbPidVid.c_str());
                return true;
            }
        }
        return false;
    }

    bool CCJGW_FilteUsbConfig::FilteLanConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo)
    {
        for (std::vector<S_USB_CONFIG>::iterator it = psGuidUsbConfig->mvUsbConfig.begin();
            it != psGuidUsbConfig->mvUsbConfig.end();
            ++ it)
        {
            if (CheckUsbPidVid(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbService(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbDeviceDes(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbClass(lpdevice->m_strRegUsbPidVid.c_str(),&it[0]))
            {
                lpdevice->m_nDeviceMode = it[0].meDeviceMode;
                return true;
            }
        }
        return false;
    }

    bool CCJGW_FilteUsbConfig::FilteMaxUsbConfigInfo(PS_GUID_USB_CONFIG& psGuidUsbConfig,SDeviceMsg_Info* lpdevice,PS_REG_DEVICE_POOL_DEVINFO psRegDevPoolInfo)
    {
        for (std::vector<S_USB_CONFIG>::iterator it = psGuidUsbConfig->mvUsbConfig.begin();
            it != psGuidUsbConfig->mvUsbConfig.end();
            ++ it)
        {
            if (CheckUsbPidVid(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbService(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbDeviceDes(lpdevice->m_strRegUsbPidVid.c_str(),&it[0])
                && CheckUsbClass(lpdevice->m_strRegUsbPidVid.c_str(),&it[0]))
            {
                lpdevice->m_nDeviceMode = it[0].meDeviceMode;
                return true;
            }
        }
        return false;
    }

    bool CCJGW_FilteUsbConfig::CheckUsbPidVid(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig)
    {
        wchar_t szTemp[20] = {0};

        if (psUsbConfig->mnPID > 0)
        {
            _stprintf_s(szTemp,20,L"PID_%04X",psUsbConfig->mnPID);
            if (NULL == JGW_WStrCaseWStr(strInstance,szTemp)) 
                return false;
        }

        if (psUsbConfig->mnVID > 0)
        {
            _stprintf_s(szTemp,20,L"VID_%04X",psUsbConfig->mnVID);
            if (NULL == JGW_WStrCaseWStr(strInstance,szTemp)) 
                return false;
        }

        return true;
    }

    bool CCJGW_FilteUsbConfig::CheckUsbService(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig)
    {
        if (psUsbConfig->mstrService.empty()) return true;
        std::wstring strTemp;
        strTemp = JGW_GetRegUsbInfoToTag(strInstance,L"Service");

        return (NULL == JGW_WStrComparenoCaseWStr(psUsbConfig->mstrService.c_str(),strTemp.c_str()));
    }

    bool CCJGW_FilteUsbConfig::CheckUsbClass(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig)
    {
        if (psUsbConfig->mstrClass.empty()) return true;

        std::wstring strTemp;
        strTemp = JGW_GetRegUsbInfoToTag(strInstance,L"Class");

        return (NULL == JGW_WStrComparenoCaseWStr(psUsbConfig->mstrClass.c_str(),strTemp.c_str()));
    }

    bool CCJGW_FilteUsbConfig::CheckUsbDeviceDes(const wchar_t* strInstance,S_USB_CONFIG* psUsbConfig)
    {
        if (psUsbConfig->mstrDeviceDes.empty()) return true;

        std::wstring strTemp;
        strTemp = JGW_GetRegUsbInfoToTag(strInstance,L"DeviceDesc");

        return (NULL == JGW_WStrCaseWStr(strTemp.c_str(),psUsbConfig->mstrClass.c_str()));
    }

//     std::wstring CCJGW_FilteUsbConfig::GetADBSerialNumber(const wchar_t* strInstance)
//     {
//         TCHAR szBuffer[260] = {0};
//         size_t nSize = 260;
//         GetSerialNumber(strInstance,szBuffer,nSize);
//         return szBuffer;
//     }

    int CCJGW_FilteUsbConfig::GetUsbComPort(const wchar_t* strInstance)
    {
        int nComPort;
        JGW_GetComportToUsbPidVid(strInstance,nComPort);
        return nComPort;
    }
}

