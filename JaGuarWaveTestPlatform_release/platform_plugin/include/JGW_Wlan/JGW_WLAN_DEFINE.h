#pragma once
#ifndef JGW_WLAN_DEFINE_H__
#define JGW_WLAN_DEFINE_H__

#if (defined JGW_WLAN_EXPORTS) || (defined JGW_WLAN_IMPORTS)
#else
#define JGW_WLAN_IMPORTS
#endif


#ifdef JGW_WLAN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_WLAN_API __declspec(dllexport)
#		define JGW_WLAN_CLASS __declspec(dllexport)
#	else
#		define JJGW_WLAN_API 
#		define JGW_WLAN_CLASS 
#	endif
#endif

#ifdef JGW_WLAN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_WLAN_API __declspec(dllimport)
#			define JGW_WLAN_CLASS __declspec(dllimport)
#		else
#			define JGW_WLAN_API 
#			define JGW_WLAN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_Wlan_d.lib")
#			else
#				pragma comment(lib,"JGW_Wlan.lib")
#			endif
#endif

#endif