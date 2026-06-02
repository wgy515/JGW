#include "StdAfx.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_ConfigIni.h"
#define MAX_INI_VALUE_SIZE	512

namespace JGW
{
	CCJGW_ConfigIni::CCJGW_ConfigIni(void):m_strIniPath(L"")
		,m_bInit(false)
	{
	}


	CCJGW_ConfigIni::~CCJGW_ConfigIni(void)
	{
	}

	bool CCJGW_ConfigIni::InitIniFilePath( const wchar_t *strIniPath )
	{
		m_strIniPath = strIniPath;
		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(strIniPath))
		{
			if ( !JGW_CreateDirectory(strIniPath, false) ) throw L"Create Directory Fail"/*JGW_GetCommonFuncErrorMsg()*/;
			HANDLE hfile = CreateFile(strIniPath,GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if ( hfile == INVALID_HANDLE_VALUE ) return false;
			CloseHandle(hfile);
		}

		return m_bInit = true;
	}

	std::wstring CCJGW_ConfigIni::GetIniKeyValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName /*= L""*/)
	{
		if(!m_bInit) return lpDefaleName;
		wchar_t strKeyValue[MAX_INI_VALUE_SIZE] = {0};
		GetPrivateProfileStringW(lpAppName,lpKeyName,lpDefaleName,strKeyValue,MAX_INI_VALUE_SIZE,m_strIniPath.c_str());
		return strKeyValue;
	}

    std::string CCJGW_ConfigIni::GetIniKeyValueA(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName /* = L"" */)
    {
        if(!m_bInit) return JGW_W2A_W(lpDefaleName);
        wchar_t strKeyValue[MAX_INI_VALUE_SIZE] = {0};
        GetPrivateProfileStringW(lpAppName,lpKeyName,lpDefaleName,strKeyValue,MAX_INI_VALUE_SIZE,m_strIniPath.c_str());
        return JGW_W2A_W(strKeyValue);
    }

	int CCJGW_ConfigIni::GetIniKeyIntValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,int nDefaleValue /* = 0 */)
	{
		if(!m_bInit) return nDefaleValue;
		return GetPrivateProfileIntW(lpAppName,lpKeyName,nDefaleValue,m_strIniPath.c_str());
	}

	float CCJGW_ConfigIni::GetIniKeyFloatValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName /* = L"0" */)
	{
		return static_cast<float>(_wtof(GetIniKeyValue(lpAppName,lpKeyName,lpDefaleName).c_str()));
	}

	bool CCJGW_ConfigIni::GetIniKeyBoolValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,int nDefaleValue/* = 0*/)
	{
		return (GetIniKeyIntValue(lpAppName,lpKeyName,nDefaleValue) == 1 ?true:false);
	}

    bool CCJGW_ConfigIni::SetIniKeyValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const std::string& strKeyValue)
    {
        std::wstring strTemp = JGW_A2W(strKeyValue);
        return WritePrivateProfileString(lpAppName,lpKeyName,strTemp.c_str(),m_strIniPath.c_str())>0?true:false;
    }

	bool CCJGW_ConfigIni::SetIniKeyValue( const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpKeyValue )
	{
		return WritePrivateProfileString(lpAppName,lpKeyName,lpKeyValue,m_strIniPath.c_str())>0?true:false;
	}

	bool CCJGW_ConfigIni::SetIniKeyValue( const wchar_t *lpAppName,const wchar_t *lpKeyName,int lpKeyValue )
	{
		wchar_t szBuffer[20] = {0};
		wsprintf(szBuffer,L"%d",lpKeyValue);
		return WritePrivateProfileString(lpAppName,lpKeyName,szBuffer,m_strIniPath.c_str())>0?true:false;
	}

}
