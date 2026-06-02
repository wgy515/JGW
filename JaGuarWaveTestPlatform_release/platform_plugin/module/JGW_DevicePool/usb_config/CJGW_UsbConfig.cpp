#include "StdAfx.h"
#include "CJGW_UsbConfig.h"
#include <ObjBase.h>
namespace JGW
{
    CCJGW_UsbConfig::CCJGW_UsbConfig(void)
    {
    }


    CCJGW_UsbConfig::~CCJGW_UsbConfig(void)
    {
    }

    bool CCJGW_UsbConfig::LoadUsbConfigToConfigPath(const wchar_t* strConfigPath,std::map<GUID_KEY,S_GUID_USB_CONFIG>& sGuidUsbConfig)
    {
        try
        {
            mIniConfig.InitIniFilePath(strConfigPath);

            DWORD dUsbCount = mIniConfig.GetIniKeyIntValue(L"USB",L"USB_COUNT");
            S_USB_CONFIG sSubConfig;
            E_GUID_TYPE eGuidTyep = E_MAX_GUID_TYPE;

            for (DWORD dwIndex = 1;dwIndex <= dUsbCount;dwIndex ++ )
            {
                GUID guid = {0};
                //sGuidUsbConfig[GetUsbConfigInfo(dwIndex,sSubConfig)].mvUsbConfig.push_back(sSubConfig);
                if (!GetUsbConfigGuid(dwIndex,eGuidTyep,guid)) continue;
                GetUsbConfigInfo(dwIndex,sSubConfig);
                sGuidUsbConfig[guid].mvUsbConfig.push_back(sSubConfig);
                sGuidUsbConfig[guid].meGuidType = eGuidTyep;
                Log4WD_F(L"Usb Device Mode : %s\n\tUsb Service : %s\n\tUsb DeviceDes : %s\n\tUsb Class : %s\n\tUsb PID : %04X\n\tUsb VID : %04X",g_szDeviceMode[sSubConfig.meDeviceMode],sSubConfig.mstrService.c_str(),sSubConfig.mstrDeviceDes.c_str(),sSubConfig.mstrClass.c_str(),sSubConfig.mnPID,sSubConfig.mnVID);
            }
            return true;
        }
        catch (wchar_t e[260])
        {
            Log4WE_F(L"CCJGW_UsbConfig::LoadUsbConfigToConfigPath:ERROR(%s)",e);
            return false;
        }

        return false;
    }

    bool CCJGW_UsbConfig::GetUsbConfigGuid(DWORD dwIndex,E_GUID_TYPE& eGuidTyep,GUID &guid)
    {
        TCHAR szTemp[260] = {0};
        std::wstring strTemp;
        //! USB_GUID
        swprintf_s(szTemp,260,L"%d",dwIndex);
        strTemp = mIniConfig.GetIniKeyValue(szTemp,L"USB_GUID");

        for (int i = 0;i < E_MAX_GUID_TYPE;i ++)
        {
            if (NULL == wcscmp(g_svGuidInfo[i].mszGuidName,strTemp.c_str()))
            {
                guid = g_svGuidInfo[i].mGuid;
                eGuidTyep = g_svGuidInfo[i].meGuid;
                return true;
            }
        }
        eGuidTyep = E_MAX_GUID_TYPE;

        return (S_OK == CLSIDFromString((LPCOLESTR)strTemp.c_str(),(LPCLSID)&guid));
    }

    void CCJGW_UsbConfig::GetUsbConfigInfo(DWORD dwIndex,S_USB_CONFIG& sSubConfig)
    {
        TCHAR szTemp[260] = {0};
        std::wstring strTemp;
        LPTSTR lpstr ;
        //E_GUID_TYPE eGuidType = E_ADB_GUID_TYPE;
        sSubConfig.InitUsbConfig();
        //! USB_GUID
        swprintf_s(szTemp,260,L"%d",dwIndex);
        //! USB_VID
        strTemp = mIniConfig.GetIniKeyValue(szTemp,L"USB_VID");
        sSubConfig.mnVID = (int) wcstol(strTemp.c_str() ,&lpstr,16);
        //! sSubConfig.mnVID = mIniConfig.GetIniKeyIntValue(szTemp,L"USB_VID");
        //! USB_PID
        strTemp = mIniConfig.GetIniKeyValue(szTemp,L"USB_PID");
        sSubConfig.mnPID = (int) wcstol(strTemp.c_str() ,&lpstr,16);
        //! sSubConfig.mnPID = mIniConfig.GetIniKeyIntValue(szTemp,L"USB_PID");
        //! USB_DEV_MODE
        strTemp = mIniConfig.GetIniKeyValue(szTemp,L"USB_DEV_MODE");
        for (int i = 1;i < _countof(g_szDeviceMode);i ++)
        {
            if (NULL == wcscmp(g_szDeviceMode[i],strTemp.c_str()))
            {
                sSubConfig.meDeviceMode = (i == _countof(g_szDeviceMode) - 1) ? E_ALL_MODE:(E_DEVICE_MODE)((int)pow(2.0f,i - 1));
                break;
            }
        }
        //! USB_SERVICE
        sSubConfig.mstrService = mIniConfig.GetIniKeyValue(szTemp,L"USB_SERVICE");
        //! USB_DEVICEDES
        sSubConfig.mstrDeviceDes = mIniConfig.GetIniKeyValue(szTemp,L"USB_DEVICEDES");
        //! USB_CLASS
        sSubConfig.mstrClass = mIniConfig.GetIniKeyValue(szTemp,L"USB_CLASS");

        //return eGuidType;
    }
}

