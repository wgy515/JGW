#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"

namespace JGW
{
	class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_ConfigIni
	{
	public:
		CCJGW_ConfigIni(void);
		~CCJGW_ConfigIni(void);
	public:
		bool InitIniFilePath( const wchar_t *strIniPath );
		std::wstring GetIniKeyValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName = L"");
        std::string GetIniKeyValueA(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName = L"");
		int	GetIniKeyIntValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,int nDefaleValue = 0);
		float GetIniKeyFloatValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpDefaleName = L"0");
		bool GetIniKeyBoolValue(const wchar_t *lpAppName,const wchar_t *lpKeyName,int nDefaleValue = 0);
        bool SetIniKeyValue( const wchar_t *lpAppName,const wchar_t *lpKeyName,const std::string& strKeyValue );
		bool SetIniKeyValue( const wchar_t *lpAppName,const wchar_t *lpKeyName,const wchar_t *lpKeyValue );
		bool SetIniKeyValue( const wchar_t *lpAppName,const wchar_t *lpKeyName,int lpKeyValue );
	private:
        bool m_bInit;
		std::wstring m_strIniPath;	
	};
}


