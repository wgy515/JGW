/* Rockchip Test Equipment API
 *
 */
#ifndef __DEVICES_API_H__
#define __DEVICES_API_H__
#include <windows.h>
#include "RKDefinesHeader.h"

#define RKT_API_EXPORTS
#ifdef RKT_API_EXPORTS
#ifdef __cplusplus
	#define RKD_API extern "C" __declspec(dllexport)
#else
	#define RKD_API __declspec(dllimport)
#endif
#else
#ifdef __cplusplus
	#define RKD_API extern "C" __declspec(dllimport)
#else
	#define RKD_API __declspec(dllimport)
#endif
#endif
typedef void *RKDContextHandle; /* Currently is adb devices */
typedef void *RKDADBDevHandle; /* Currently is adb devices */
RKD_API int _stdcall RKDVersion(char *pVer, int len);
RKD_API RKDContextHandle _stdcall RKDInit();
RKD_API void _stdcall RKDSetLogEn(RKDContextHandle pContext, PFNLOGLISTENER plistener, void *userdata);
RKD_API void _stdcall RKDSetLogLevel(RKDContextHandle pContext, int level);
RKD_API void _stdcall RKDDeInit(RKDContextHandle pContext);

RKD_API RKDADBDevHandle _stdcall RKDOpenA(RKDContextHandle pContext, const char *devInfo);
RKD_API RKDADBDevHandle _stdcall RKDOpenW(RKDContextHandle pContext, const wchar_t *devInfo);
RKD_API BOOL _stdcall RKDCmdW(RKDADBDevHandle pDeviceHanle, wchar_t **pszRet, int *count, const wchar_t *cmd,...);
RKD_API BOOL _stdcall RKDCmdA(RKDADBDevHandle pDeviceHanle, char **pszRet, int *count, const char *cmd,...);
RKD_API BOOL _stdcall RKDQueryA(RKDADBDevHandle pDeviceHanle, char *pszRet, int count, const char *cmd,...);
RKD_API BOOL _stdcall RKDQueryW(RKDADBDevHandle pDeviceHanle, wchar_t *pszRet, int count, const wchar_t *cmd,...);
RKD_API void _stdcall RKDClose(RKDADBDevHandle pContext);

#endif
