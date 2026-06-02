#pragma once

#if (defined JGW_TESTLOGGINGPLUGIN_EXPORTS) || (defined JGW_TESTLOGGINGPLUGIN_IMPORTS)
#else
#define JGW_TESTLOGGINGPLUGIN_IMPORTS
#endif


#ifdef JGW_TESTLOGGINGPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_TESTLOGGINGPLUGIN_API __declspec(dllexport)
#		define JGW_TESTLOGGINGPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_TESTLOGGINGPLUGIN_API 
#		define JGW_TESTLOGGINGPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_TESTLOGGINGPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_TESTLOGGINGPLUGIN_API __declspec(dllimport)
#			define JGW_TESTLOGGINGPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_TESTLOGGINGPLUGIN_API 
#			define JGW_TESTLOGGINGPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_TestLoggingPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_TestLoggingPlugin.lib")
#			endif
#endif