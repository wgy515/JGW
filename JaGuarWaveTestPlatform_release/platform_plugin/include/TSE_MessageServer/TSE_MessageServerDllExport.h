#pragma once
#ifndef TSE_MESSAGESERVER_DEFINE_H__
#define TSE_MESSAGESERVER_DEFINE_H__

#if (defined TSE_MESSAGESERVER_EXPORTS) || (defined TSE_MESSAGESERVER_IMPORTS)
#else
#define TSE_MESSAGESERVER_IMPORTS
#endif


#ifdef TSE_MESSAGESERVER_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define TSE_MESSAGESERVER_API __declspec(dllexport)
#		define TSE_MESSAGESERVER_CLASS __declspec(dllexport)
#	else
#		define JTSE_MESSAGESERVER_API 
#		define TSE_MESSAGESERVER_CLASS 
#	endif
#endif

#ifdef TSE_MESSAGESERVER_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define TSE_MESSAGESERVER_API __declspec(dllimport)
#			define TSE_MESSAGESERVER_CLASS __declspec(dllimport)
#		else
#			define TSE_MESSAGESERVER_API 
#			define TSE_MESSAGESERVER_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"TSE_MessageServer_d.lib")
#			else
#				pragma comment(lib,"TSE_MessageServer.lib")
#			endif
#endif



#ifdef __cplusplus
extern "C" 
{
#endif
    TSE_MESSAGESERVER_API void StartTSEMessageServer();

    TSE_MESSAGESERVER_API const char* GetTSEMSID();

    TSE_MESSAGESERVER_API void* GetTSELogBaseToID(const char* strID);
    /// CTSE_LogMsgBase* 

    TSE_MESSAGESERVER_API void* GetTSEControlBaseToID(const char* strID);
    /// CTSE_ControlMsgBase* 

    TSE_MESSAGESERVER_API void StopTSEMessageServer();
#ifdef __cplusplus
};
#endif

#endif