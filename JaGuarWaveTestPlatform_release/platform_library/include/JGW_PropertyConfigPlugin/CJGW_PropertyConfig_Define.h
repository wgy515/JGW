#pragma once
#if (defined JGW_PROPERTYCONFIGPLUGIN_EXPORTS) || (defined JGW_PROPERTYCONFIGPLUGIN_IMPORTS)
#else
#define JGW_PROPERTYCONFIGPLUGIN_IMPORTS
#endif


#ifdef JGW_PROPERTYCONFIGPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_PROPERTYCONFIGPLUGIN_API __declspec(dllexport)
#		define JGW_PROPERTYCONFIGPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_PROPERTYCONFIGPLUGIN_API 
#		define JGW_PROPERTYCONFIGPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_PROPERTYCONFIGPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_PROPERTYCONFIGPLUGIN_API __declspec(dllimport)
#			define JGW_PROPERTYCONFIGPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_PROPERTYCONFIGPLUGIN_API 
#			define JGW_PROPERTYCONFIGPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_PropertyConfigPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_PropertyConfigPlugin.lib")
#			endif
#endif
//! global.property.config.ini
#define GLOBAL_PROPERTY_CONFIG_INI L"global.property.config.ini"