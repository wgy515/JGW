#pragma once
#ifndef JGW_WIFI_SETTING_DEFINE_H__
#define JGW_WIFI_SETTING_DEFINE_H__
#include <string>
#include <vector>

#if (defined JGW_WIFISETTING_EXPORTS) || (defined JGW_WIFISETTING_IMPORTS)
#else
#define JGW_WIFISETTING_IMPORTS
#endif


#ifdef JGW_WIFISETTING_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_WIFISETTING_API __declspec(dllexport)
#		define JGW_WIFISETTING_CLASS __declspec(dllexport)
#	else
#		define JJGW_WIFISETTING_API 
#		define JGW_WIFISETTING_CLASS 
#	endif
#endif

#ifdef JGW_WIFISETTING_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_WIFISETTING_API __declspec(dllimport)
#			define JGW_WIFISETTING_CLASS __declspec(dllimport)
#		else
#			define JGW_WIFISETTING_API 
#			define JGW_WIFISETTING_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_WiFiSetting_d.lib")
#			else
#				pragma comment(lib,"JGW_WiFiSetting.lib")
#			endif
#endif

#endif