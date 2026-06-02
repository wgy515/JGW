#pragma once
#ifndef JAGUARWAVETESTPLATFORMHELP_DEFINE_H__
#define JAGUARWAVETESTPLATFORMHELP_DEFINE_H__
#include <string>
#include <vector>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
//! JAGUARWAVETESTPLATFORMHELP_EXPORTS
#if (defined JAGUARWAVETESTPLATFORMHELP_EXPORTS) || (defined JAGUARWAVETESTPLATFORMHELP_IMPORTS)
#else
#define JAGUARWAVETESTPLATFORMHELP_IMPORTS
#endif


#ifdef JAGUARWAVETESTPLATFORMHELP_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JAGUARWAVETESTPLATFORMHELP_API __declspec(dllexport)
#		define JAGUARWAVETESTPLATFORMHELP_CLASS __declspec(dllexport)
#	else
#		define JJAGUARWAVETESTPLATFORMHELP_API 
#		define JAGUARWAVETESTPLATFORMHELP_CLASS 
#	endif
#endif

#ifdef JAGUARWAVETESTPLATFORMHELP_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JAGUARWAVETESTPLATFORMHELP_API __declspec(dllimport)
#			define JAGUARWAVETESTPLATFORMHELP_CLASS __declspec(dllimport)
#		else
#			define JAGUARWAVETESTPLATFORMHELP_API 
#			define JAGUARWAVETESTPLATFORMHELP_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JaGuarWaveTestPlatformHelp_d.lib")
#			else
#				pragma comment(lib,"JaGuarWaveTestPlatformHelp.lib")
#			endif
#endif

#endif

namespace JGW
{
#ifdef __cplusplus
    extern "C" 
    {
#endif
        JAGUARWAVETESTPLATFORMHELP_API CCMessageThread* GetHelpModuleThread();

        JAGUARWAVETESTPLATFORMHELP_API void SetMainWindowsHwnd(HWND hMainHwnd);

        JAGUARWAVETESTPLATFORMHELP_API void StopHelpModuleThread();
#ifdef __cplusplus
    };
#endif
}