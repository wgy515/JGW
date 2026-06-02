#pragma once
//! 程序提权操作
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	extern "C" {
#endif
		JGW_FOUNDATIONFUNC_LIB_API bool JGW_SetPrivilege(const wchar_t *lpszPrivilege = SE_DEBUG_NAME, bool bEnablePrivilege = true);

		JGW_FOUNDATIONFUNC_LIB_API DWORD JGW_GetProcessID2Name(const std::wstring& strName);

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetProcessNameToProcessID(DWORD dwProcessID);

        JGW_FOUNDATIONFUNC_LIB_API DWORD JGW_GetProcessModuleIDToProcessID(DWORD dwProcessID);

		JGW_FOUNDATIONFUNC_LIB_API void JGW_KillProcessID(DWORD dwProcessID);

		JGW_FOUNDATIONFUNC_LIB_API bool JGW_GetWindowsTypes(EWINDOWS_VERSION &eVersion);

		JGW_FOUNDATIONFUNC_LIB_API void JGW_GetPrivilege( const wchar_t *lpszPrivilege = SE_DEBUG_NAME, bool bEnablePrivilege = true );

        JGW_FOUNDATIONFUNC_LIB_API bool JGW_CheckProcessIsRun(HANDLE hProcess);

        JGW_FOUNDATIONFUNC_LIB_API bool JGW_KillProcessToName(const std::wstring& strName);

        JGW_FOUNDATIONFUNC_LIB_API HANDLE JGW_CreateProcess(const std::wstring& strProcessFilePath,const wchar_t*  strCurrentProcessPath = NULL,DWORD dwFlags = STARTF_USESHOWWINDOW,WORD wShowWindow = SW_NORMAL);

        JGW_FOUNDATIONFUNC_LIB_API bool JGW_TerminateProcess(HANDLE hProcess);

        JGW_FOUNDATIONFUNC_LIB_API void JGW_GetProcessPath(DWORD dwProcessID, std::wstring& strProcessPath);
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	}
#endif
}