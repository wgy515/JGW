#pragma once

#if (defined JGW_QMSLWLANPLUGIN_EXPORTS) || (defined JGW_QMSLWLANPLUGIN_IMPORTS)
#else
#define JGW_QMSLWLANPLUGIN_IMPORTS
#endif


#ifdef JGW_QMSLWLANPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_QMSLWLANPLUGIN_API __declspec(dllexport)
#		define JGW_QMSLWLANPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_QMSLWLANPLUGIN_API 
#		define JGW_QMSLWLANPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_QMSLWLANPLUGIN_IMPORTS
#		define JGW_QMSLWLANPLUGIN_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_QMSLWLANPLUGIN_API __declspec(dllimport)
#			define JGW_QMSLWLANPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_QMSLWLANPLUGIN_API 
#			define JGW_QMSLWLANPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_QMSLWlanPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_QMSLWlanPlugin.lib")
#			endif
#endif