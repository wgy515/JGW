#pragma once
#ifndef JGW_GLOBALRESOURCES_DEFINE_H__
#define JGW_GLOBALRESOURCES_DEFINE_H__
#include <string>
#include <vector>

#if (defined JGW_GLOBALRESOURCESPLUGIN_EXPORTS) || (defined JGW_GLOBALRESOURCES_IMPORTS)
#else
#define JGW_GLOBALRESOURCES_IMPORTS
#endif


#ifdef JGW_GLOBALRESOURCESPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_GLOBALRESOURCES_API __declspec(dllexport)
#		define JGW_GLOBALRESOURCES_CLASS __declspec(dllexport)
#	else
#		define JJGW_GLOBALRESOURCES_API 
#		define JGW_GLOBALRESOURCES_CLASS 
#	endif
#endif

#ifdef JGW_GLOBALRESOURCES_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_GLOBALRESOURCES_API __declspec(dllimport)
#			define JGW_GLOBALRESOURCES_CLASS __declspec(dllimport)
#		else
#			define JGW_GLOBALRESOURCES_API 
#			define JGW_GLOBALRESOURCES_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_GlobalResourcesPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_GlobalResourcesPlugin.lib")
#			endif
#endif

#endif