// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <WinBase.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <poco/String.h>



#include <poco//SingletonHolder.h>
#include "ILog.h"
// TODO: 在此处引用程序需要的其他头文件
extern Poco::SingletonHolder<JGW::ILog> gLog;
namespace JGW
{

    extern std::string LocalToUTF8(const std::wstring& localString);
    extern std::wstring UTF8ToLocal(const std::string& utf8String);

    extern std::string W2A(const std::wstring& localString,int codepage = 0);
    extern std::wstring A2W(const std::string& localString,int codepage = 0);
    extern std::string GetApplicationFolder();
    extern std::wstring GetApplicationFolderW();
    extern std::wstring GetAbsolutePath(const std::wstring& relativePath, const wchar_t* strBaseFolder = NULL);
    extern std::wstring GetModuleFolderW(HMODULE hModule = NULL);
    extern std::string FormatString(std::string& strFormat,const char *szFormat,...);
    extern void FormatWString(std::wstring& strFormat,const wchar_t* szFormat,...);
    extern std::string GetSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd = "\n",const std::string& strDef = "");

    template <class S,class _Elem>
    extern void EraseChars(S& sSource,std::vector<_Elem> vRemoveChars)
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

    template <class S>
    extern void EraseLastAndFristTwoChars(S& sSource,const char toRemoveOne = '\r',const char toRemoveTwo = '\n')
    {
        while ( !sSource.empty() && (toRemoveOne == sSource.at(0) || toRemoveTwo == sSource.at(0)) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && (toRemoveOne == sSource.at(sSource.length()-1) || toRemoveTwo == sSource.at(sSource.length()-1)) )
        {
            sSource.erase(sSource.end()-1);
        }
    }

    template <class S>
    int StrCaseStr(const S& str,const S& subStr)
    {
        S strUp = Poco::toUpper(str);
        S subStrUp = Poco::toUpper(subStr);
        return strUp.find(subStrUp);
    }

    template <class _Elem>
    bool IsFindCharVector(_Elem ch,std::vector<_Elem> vRemoveChars)
    {
        for (std::vector<_Elem>::iterator it = vRemoveChars.begin();it != vRemoveChars.end();++it)
        {
            if (ch == it[0])
            {
                return true;
            }
        }
        return false;
    }

    extern size_t GetInsertUsbDeviceCountToServicesName(const wchar_t* strServicesName);

    extern std::wstring GetUsbEnumPidVidToServiceName(const wchar_t* strServicesName,DWORD dwEnumID);

    extern std::wstring GetRegUsbInfoToTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag);
}
