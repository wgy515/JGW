#include "StdAfx.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_smartpointer_handle_func.h>

namespace JGW
{
    void JGW_UpdateRegUsbFlags( int pid,int vid )
    {
        std::wstring strSubKey = _T("SYSTEM\\CurrentControlSet\\Control\\UsbFlags\\");
        BYTE szInfo[10] = {0};
        BYTE data = 0x01;
        DWORD dwSize = 1;
        DWORD dwType = REG_BINARY;
        std::wstring strTemp;
        std::shared_ptr<HKEY> ptr_key(new HKEY,CloseRegKey);

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE | KEY_SET_VALUE /*| KEY_WOW64_32KEY */ |KEY_WOW64_64KEY,ptr_key.get()) )
        {
            g_strErrorMsg = _T("Open Reg Fail! ERROR: ");
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return;
        }

        JGW_FormatWString(strTemp,L"IgnoreHWSerNum%04X%04X",vid,pid);
        if( ERROR_SUCCESS == RegQueryValueEx(*(ptr_key.get()), strTemp.c_str(), NULL, &dwType, szInfo, &dwSize) && 0x01 == szInfo[0])
        {
            g_strErrorMsg = _T("RegQueryValueEx ERROR: ");
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return;
        }

        dwType = REG_BINARY;
        dwSize = 1;
        if( RegSetValueEx(*(ptr_key.get()), strTemp.c_str(), NULL, dwType, &data, dwSize) != ERROR_SUCCESS)
        {
            g_strErrorMsg = _T("RegSetValueEx ERROR: ");
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return;
        }
    }

    std::wstring JGW_GetUsbFriendlyNameToPidVID( const wchar_t* strRegUsbPidVid )
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
        DWORD			dwType = REG_SZ;
        TCHAR			byData [512 ]= {0};
        DWORD			dwSize = 512;
        HKEY			m_hKey = NULL;

        strSubKey += strRegUsbPidVid;
        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return L"";
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("FriendlyName"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return L"";
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    std::wstring JGW_GetRegUsbInfoToMultiTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
        DWORD			dwType = REG_MULTI_SZ;
        TCHAR			byData [1024]= {0};
        DWORD			dwSize = 1024;
        HKEY			m_hKey = NULL;

        strSubKey += strRegUsbPidVid;
        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return L"";
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, tag, NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return L"";
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    std::wstring JGW_GetRegUsbInfoToTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
        DWORD			dwType = REG_SZ;
        TCHAR			byData [512 ]= {0};
        DWORD			dwSize = 512;
        HKEY			m_hKey = NULL;

        strSubKey += strRegUsbPidVid;
        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return L"";
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, tag, NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return L"";
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }


    bool JGW_GetComportToUsbPidVid( const wchar_t* strRegUsbPidVid,int& nComPort )
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
        DWORD			dwType = REG_SZ;
        TCHAR			byData [MAX_PATH ]= {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;
        TCHAR			szTemp[MAX_PATH] = {0};
        std::wstring	strTemp(strRegUsbPidVid);

        JGW_ReplaceStringW(strTemp,L"\\\\?\\",L"");
        if (std::wstring::npos != strTemp.find(L"#{"))
            strTemp.erase(strTemp.find(_T("#{")));
        ReplaceCharW(strTemp,'#','\\');
        /*if( std::wstring::npos != strTemp.find(L"USB#") )
        {
        strTemp.erase(0,strTemp.find(L"USB#") );
        strTemp.erase(strTemp.find(_T("#{")));
        JGW_ReplaceCharW(strTemp,'#','\\');
        }*/

        strSubKey += strTemp;
        strSubKey += L"\\Device Parameters";

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            RegCloseKey(m_hKey);
            return false;
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("PortName"), NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return false;
        }

        RegCloseKey(m_hKey);

        if ( dwSize > 3 )
        {
            nComPort = _ttoi(byData + 3);
            return true;
        }

        return false;
    }

    size_t JGW_GetInsertUsbDeviceCountToServicesName(const wchar_t* strServicesName)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\services\\") ;
        DWORD			dwType = REG_DWORD;
        DWORD			byData = 0;
        DWORD			dwSize = sizeof(DWORD);
        HKEY			m_hKey = NULL;

        strSubKey += strServicesName;
        strSubKey += _T("\\Enum");

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return 0;
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("Count"), NULL, &dwType,(LPBYTE) (&byData), &dwSize) )
        {
            RegCloseKey(m_hKey);
            return 0;
        }

        RegCloseKey(m_hKey);

        return byData;
    }

    std::wstring JGW_GetUsbEnumPidVidToServiceName(const wchar_t* strServicesName,DWORD dwEnumID)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\services\\") ;
        DWORD			dwType = REG_DWORD;
        TCHAR			byData [MAX_PATH ]= {0};
        TCHAR			lpValueName[9] = {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;

        strSubKey += strServicesName;
        strSubKey += _T("\\Enum");

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return _T("");
        }
        _stprintf_s(lpValueName,_T("%ld"),dwEnumID);
        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return _T("");
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }


    size_t JGW_GetUsbEnumPidVidVectotToServiceName(const wchar_t* strServicesName,std::vector<std::wstring>& vEnumPidVid)
    {
        size_t nEnumCount = JGW_GetInsertUsbDeviceCountToServicesName(strServicesName);
        if( nEnumCount <= 0 )
        {
            return 0;
        }
        for (size_t i = 0; i < nEnumCount; i ++)
        {
            vEnumPidVid.push_back(JGW_GetUsbEnumPidVidToServiceName(strServicesName,i));
        }
        return nEnumCount;
    }

    std::wstring JGW_GetComPortToUsbServiceName(const wchar_t* strServiceName,const wchar_t* strUsbVID /*= L""*/,const wchar_t* strUsbPID /*= L""*/,const wchar_t* szDeviceDes/* = L""*/)
    {
        std::wstring strUsbPidVid,strComName,strDeviceDes;
        int nComPort = 0;
        size_t nCount = JGW_GetInsertUsbDeviceCountToServicesName(strServiceName);
        if (nCount <= 0) return L"";

        for (size_t i = 0;i < nCount;i ++)
        {
            strUsbPidVid = JGW_GetUsbEnumPidVidToServiceName(strServiceName,i);
            strDeviceDes = JGW_GetRegUsbInfoToTag(strUsbPidVid.c_str(),L"DeviceDesc");
            if (wcslen(strUsbVID) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbVID))
                {
                    continue;
                }
            }
            if (wcslen(strUsbPID) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbPID))
                {
                    continue;
                }
            }
            if (wcslen(szDeviceDes) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strDeviceDes.c_str(),szDeviceDes))
                {
                    continue;
                }
            }
            if (JGW_GetComportToUsbPidVid(strUsbPidVid.c_str(),nComPort))
            {
                JGW::JGW_FormatWString(strComName,L"COM%d",nComPort);
                return strComName;
            }
        }

        return L"";
    }

    void JGW_GetAllComPortToUsbServiceName(std::vector<std::wstring>& vstrComPort,const wchar_t* strServiceName,const wchar_t* strUsbVID ,const wchar_t* strUsbPID ,const wchar_t* szDeviceDes)
    {
        std::wstring strUsbPidVid,strComName,strDeviceDes;
        int nComPort = 0;
        size_t nCount = JGW_GetInsertUsbDeviceCountToServicesName(strServiceName);
        vstrComPort.clear();

        if (nCount <= 0) return ;

        for (size_t i = 0;i < nCount;i ++)
        {
            strUsbPidVid = JGW_GetUsbEnumPidVidToServiceName(strServiceName,i);
            strDeviceDes = JGW_GetRegUsbInfoToTag(strUsbPidVid.c_str(),L"DeviceDesc");
            if (wcslen(strUsbVID) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbVID))
                {
                    continue;
                }
            }
            if (wcslen(strUsbPID) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbPID))
                {
                    continue;
                }
            }
            if (wcslen(szDeviceDes) > 0)
            {
                if (NULL == JGW_WStrCaseWStr(strDeviceDes.c_str(),szDeviceDes))
                {
                    continue;
                }
            }
            if (JGW_GetComportToUsbPidVid(strUsbPidVid.c_str(),nComPort))
            {
                JGW::JGW_FormatWString(strComName,L"COM%d",nComPort);
                vstrComPort.push_back(strComName);
            }
            /*if (JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbVID) 
            && JGW_WStrCaseWStr(strUsbPidVid.c_str(),strUsbPID)
            && JGW_WStrCaseWStr(strDeviceDes.c_str(),szDeviceDes)
            && JGW_GetComportToUsbPidVid(strUsbPidVid.c_str(),nComPort)
            )
            {
            JGW::JGW_FormatWString(strComName,L"COM%d",nComPort);
            vstrComPort.push_back(strComName);
            }*/
        }
    }

    std::wstring JGW_GetNetcardDeviceNameToAdapterName(const wchar_t* strAdapterName)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection") ;
        DWORD			dwType = REG_SZ;
        TCHAR			byData [MAX_PATH ]= {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;

        JGW_FormatWString(strSubKey,L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\%s\\Connection",strAdapterName);

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return _T("");
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, L"Name", NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return _T("");
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    bool JGW_GetAdpateDHCPStatus(const wchar_t* strAdapterName)
    {
        std::wstring	strSubKey;
        DWORD dwType = REG_DWORD,byData = 0,dwSize = sizeof(DWORD);
        HKEY m_hKey = NULL;

        JGW_FormatWString(strSubKey,L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\%s",strAdapterName);

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return true;
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, L"EnableDHCP", NULL, &dwType,(LPBYTE) (&byData), &dwSize) )
        {
            RegCloseKey(m_hKey);
            return true;
        }

        RegCloseKey(m_hKey);

        return (1 == byData);
    }

    std::wstring JGW_GetIPV4AddrToAdpaterName(const wchar_t* strNetCardGUID)
    {
        std::wstring	strSubKey;
        DWORD			dwType = REG_MULTI_SZ;
        TCHAR			byData [MAX_PATH ]= {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;

        JGW_FormatWString(strSubKey,L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\%s",strNetCardGUID);
        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return _T("");
        }

        if(ERROR_SUCCESS == RegQueryValueEx(m_hKey, _T("DhcpIPAddress"), 0,&dwType, (LPBYTE) byData, &dwSize))
        {
            RegCloseKey(m_hKey);
            return std::wstring(byData);
        }
        else
        {
            if(ERROR_SUCCESS == RegQueryValueEx(m_hKey, _T("IPAddress"), 0, &dwType,(LPBYTE) byData, &dwSize))
            {
                RegCloseKey(m_hKey);
                return std::wstring(byData);
            }      
        }
#if 0
        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, L"IPAddress", NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return _T("");
        }
#endif
        RegCloseKey(m_hKey);
        return _T("");
    }

    std::wstring JGW_GetStaticIPAddrToAdpaterName(const wchar_t* strAdapterName)
    {
        std::wstring	strSubKey;
        DWORD			dwType = REG_MULTI_SZ;
        TCHAR			byData [MAX_PATH ]= {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;

        JGW_FormatWString(strSubKey,L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\%s",strAdapterName);

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return _T("");
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, L"IPAddress", NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return _T("");
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    bool JGW_SetAdpaterDHCPStatus(const wchar_t* strAdapterName,DWORD dwStatus /* = 0 */)
    {
        std::wstring strSubKey;
        std::shared_ptr<HKEY> ptr_key(new HKEY,CloseRegKey);
        DWORD dwType = REG_DWORD,dwSize = sizeof(DWORD);
        HKEY m_hKey = NULL;

        JGW_FormatWString(strSubKey,L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\%s",strAdapterName);

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE | KEY_SET_VALUE /*| KEY_WOW64_32KEY */ |KEY_WOW64_64KEY,ptr_key.get()) )
        {
            g_strErrorMsg = _T("Open Reg Fail! ERROR: ");
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return false;
        }

//         JGW_FormatWString(strTemp,L"IgnoreHWSerNum%04X%04X",vid,pid);
//         if( ERROR_SUCCESS == RegQueryValueEx(*(ptr_key.get()), strTemp.c_str(), NULL, &dwType, szInfo, &dwSize) && 0x01 == szInfo[0])
//         {
//             g_strErrorMsg = _T("RegQueryValueEx ERROR: ");
//             g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
//             return;
//         }

        if( RegSetValueEx(*(ptr_key.get()),L"EnableDHCP", NULL, dwType,(LPBYTE)(&dwStatus), dwSize) != ERROR_SUCCESS)
        {
            g_strErrorMsg = _T("RegSetValueEx ERROR: ");
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return false;
        }

        return true;
    }
}