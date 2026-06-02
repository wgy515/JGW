#pragma once

#if (defined JGW_QCOMMVFSPLUGIN_EXPORTS) || (defined JGW_QCOMMVFSPLUGIN_IMPORTS)
#else
#define JGW_QCOMMVFSPLUGIN_IMPORTS
#endif


#ifdef JGW_QCOMMVFSPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGWQCOMMVFSPLUGIN_API __declspec(dllexport)
#		define JGWQCOMMVFSPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGWQCOMMVFSPLUGIN_API 
#		define JGWQCOMMVFSPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_QCOMMVFSPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGWQCOMMVFSPLUGIN_API __declspec(dllimport)
#			define JGWQCOMMVFSPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGWQCOMMVFSPLUGIN_API 
#			define JGWQCOMMVFSPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_QcommVFSPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_QcommVFSPlugin.lib")
#			endif
#endif