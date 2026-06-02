#pragma once

#if (defined JGW_QMSLFUNCPLUGIN_EXPORTS) || (defined JGW_QMSLFUNCPLUGIN_IMPORTS)
#else
#define JGW_QMSLFUNCPLUGIN_IMPORTS
#endif


#ifdef JGW_QMSLFUNCPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGWQMSLFUNCPLUGIN_API __declspec(dllexport)
#		define JGWQMSLFUNCPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGWQMSLFUNCPLUGIN_API 
#		define JGWQMSLFUNCPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_QMSLFUNCPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGWQMSLFUNCPLUGIN_API __declspec(dllimport)
#			define JGWQMSLFUNCPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGWQMSLFUNCPLUGIN_API 
#			define JGWQMSLFUNCPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_QMSLFuncPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_QMSLFuncPlugin.lib")
#			endif
#endif