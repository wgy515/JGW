#pragma once
#include <string>
#ifdef __cplusplus
extern "C" 
{
#endif
    __declspec(dllexport) size_t StartUsbDevicePool(HWND hMainWnd);

    __declspec(dllexport) void OnDeviceChange(WPARAM wParam,LPARAM lParam);

    __declspec(dllexport) void RegUsbDevicePoolMsg(UINT dwThreadID,UINT nMsgID,HWND hwnd,GUID guid,short eDeviceModeEnum);

    __declspec(dllexport) void* GetDevicePoolDevInfoToWParam(WPARAM wParam,LPARAM lParam);

    __declspec(dllexport) void StopUsbDevicePool();

#ifdef __cplusplus
};
#endif