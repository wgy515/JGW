#pragma once
#ifndef JGW_CSHARPMODULE_H__
#define JGW_CSHARPMODULE_H__

#define CSHARP_MESSGAE_MAX_BUFFER  (4096 + 1024)


#if (defined JGW_CSHARPMODULE_EXPORTS) || (defined JGW_CSHARPMODULE_IMPORTS)
#else
#define JGW_CSHARPMODULE_IMPORTS
#endif


#ifdef JGW_CSHARPMODULE_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_CSHARPMODUL_API __declspec(dllexport)
#		define JGW_CSHARPMODUL_CLASS __declspec(dllexport)
#	else
#		define JJGW_CSHARPMODUL_API 
#		define JGW_CSHARPMODUL_CLASS 
#	endif
#endif

#ifdef JGW_CSHARPMODULE_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_CSHARPMODUL_API __declspec(dllimport)
#			define JGW_CSHARPMODUL_CLASS __declspec(dllimport)
#		else
#			define JGW_CSHARPMODUL_API 
#			define JGW_CSHARPMODUL_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_CSharpModule_d.lib")
#			else
#				pragma comment(lib,"JGW_CSharpModule.lib")
#			endif
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
    JGW_CSHARPMODUL_API bool StartCSharpModuleThread();

    JGW_CSHARPMODUL_API void StopCSharpModuleThread();
#ifdef __cplusplus
};
#endif

#endif