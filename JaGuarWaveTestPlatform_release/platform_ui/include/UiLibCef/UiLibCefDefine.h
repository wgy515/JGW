#pragma once
#ifndef UILIB_CEF_H__
#define UILIB_CEF_H__

#if (defined UILIBCEF_EXPORTS) || (defined UILIBCEF_IMPORTS)
#else
#define UILIBCEF_IMPORTS
#endif


#ifdef UILIBCEF_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define UILIBCEF_API __declspec(dllexport)
#		define UILIBCEF_CLASS __declspec(dllexport)
#	else
#		define UILIBCEF_API 
#		define UILIBCEF_CLASS 
#	endif
#endif

#ifdef UILIBCEF_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define UILIBCEF_API __declspec(dllimport)
#			define UILIBCEF_CLASS __declspec(dllimport)
#		else
#			define UILIBCEF_API 
#			define UILIBCEF_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"UiLibCef_d.lib")
#			else
#				pragma comment(lib,"UiLibCef.lib")
#			endif
#endif



#endif