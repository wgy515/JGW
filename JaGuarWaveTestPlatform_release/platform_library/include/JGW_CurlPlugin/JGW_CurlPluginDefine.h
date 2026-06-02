#pragma once
#ifndef JGW_CURLPLUGIN_H__
#define JGW_CURLPLUGIN_H__
#include <string>
#include <vector>

#if (defined JGW_CURLPLUGIN_EXPORTS) || (defined JGW_CURLPLUGIN_IMPORTS)
#else
#define JGW_CURLPLUGIN_IMPORTS
#endif


#ifdef JGW_CURLPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_CURLPLUGIN_API __declspec(dllexport)
#		define JGW_CURLPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_CURLPLUGIN_API 
#		define JGW_CURLPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_CURLPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_CURLPLUGIN_API __declspec(dllimport)
#			define JGW_CURLPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_CURLPLUGIN_API 
#			define JGW_CURLPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_CurlPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_CurlPlugin.lib")
#			endif
#endif
#endif
