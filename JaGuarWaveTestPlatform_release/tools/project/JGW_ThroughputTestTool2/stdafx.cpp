// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_ThroughputTestTool2.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
//#include <Poco/UnicodeConverter.h>
#include <poco/Path.h>
#include <Poco/Buffer.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

#if 1
namespace JGW
{
    std::wstring UTF8ToLocal(const std::string& utf8String)
    {
        return A2W(utf8String,CP_UTF8);
    }

    std::wstring A2W(const std::string& localString,int codepage /* = CP_ACP */)
    {
        std::wstring wstr;
        if ( localString.length() > 0)
        {
            int wlen = MultiByteToWideChar(codepage, 0, localString.c_str(), localString.size(), NULL, 0);
            wstr.resize(wlen);
            MultiByteToWideChar(codepage, 0, localString.c_str(), localString.size(),const_cast<wchar_t*>(wstr.data()), wlen);
        }
        return wstr;
    }

    std::string LocalToUTF8(const std::wstring& localString)
    {
        return W2A(localString,CP_UTF8);
    }

    std::string W2A(const std::wstring& localString,int codepage /* = CP_ACP */)
    {
        std::string str;

        if ( localString.length() > 0)
        {
            int len = WideCharToMultiByte(codepage, 0, localString.c_str(), localString.length(), NULL, 0, NULL, NULL);
            str.resize(len);
            WideCharToMultiByte(codepage, 0, localString.c_str(), localString.length(),const_cast<char*>(str.data()), len, NULL, NULL);
        }

        return str;
    }

    std::wstring GetModuleFolderW(HMODULE hModule)
    {
        DWORD len = MAX_PATH + 1;
        std::wstring buffer;
        buffer.resize(len);
        DWORD n = GetModuleFileNameW(hModule, const_cast<wchar_t*>(buffer.data()),len);
        if (n > 0 && n <= len)
        {
            while (!buffer.empty() && buffer[buffer.size() - 1] != '\\') buffer.erase(buffer.size() - 1);
            return buffer;
        }
        return L"";
    }

    std::wstring GetApplicationFolderW()
    {
        return GetModuleFolderW(NULL);
    }

    std::string GetApplicationFolder()
    {
        return W2A(GetApplicationFolderW());
    }

    std::wstring GetAbsolutePath(const std::wstring& relativePath,const wchar_t* strBaseFolder)
    {
        std::string relativePathUTF8;
        std::wstring relativePathGB2312 (relativePath);
        relativePathUTF8 = LocalToUTF8(relativePathGB2312);
        //Poco::UnicodeConverter::convert(relativePathGB2312, relativePathUTF8);
        Poco::Path path(relativePathUTF8);
        if (path.isRelative())
        {
            relativePathGB2312 = NULL == strBaseFolder ? GetApplicationFolderW() : strBaseFolder;
            relativePathGB2312 += relativePath;
        }
        return relativePathGB2312;
    }

    void EraseChars(std::string& sSource,std::vector<char> vRemoveChars)
    {
        while ( !sSource.empty() && IsFindCharVector(sSource[0],vRemoveChars) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && IsFindCharVector(sSource[sSource.length() - 1],vRemoveChars))
        {
            sSource.erase(sSource.end()-1);
        }
    }

    inline std::string FormatString(std::string& strFormat,const char *szFormat,...)
    {
        int nListCount = 0;

        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnprintf_s(const_cast<char *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) return "buffer too small";
            va_end(pArgList);
        }

        strFormat.resize(nListCount);

        return strFormat;
    }

    inline void FormatWString(std::wstring& strFormat,const wchar_t *szFormat,...)
    {
        int nListCount = 0;

        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscwprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnwprintf_s(const_cast<wchar_t *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) throw L"buffer too small";

            va_end(pArgList);
        }

        strFormat.resize(nListCount);
    }


    size_t GetInsertUsbDeviceCountToServicesName(const wchar_t* strServicesName)
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

    std::wstring GetUsbEnumPidVidToServiceName(const wchar_t* strServicesName,DWORD dwEnumID)
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

    std::wstring GetRegUsbInfoToTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag)
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

    std::string GetSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = L"" */,const std::string& strDef /* = L"" */)
    {
        std::string strRsp = strDef;
        size_t index = strSource.find(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::string::npos != index && std::string::npos != endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
        }
        return strRsp;
    }
}
#endif