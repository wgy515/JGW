#pragma once
#include "CJGW_ReportDeviceInfoDefine.h"

#ifdef JGW_REPORTDEVICEINFO_API
#undef JGW_REPORTDEVICEINFO_API
#endif

#ifdef JGW_REPORTDEVICEINFO_EXPORTS
#define JGW_REPORTDEVICEINFO_API __declspec(dllexport)
#else
#define JGW_REPORTDEVICEINFO_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"JGW_ReportDeviceInfo_d.lib")
#else
#pragma comment(lib,"JGW_ReportDeviceInfo.lib")
#endif
#endif // JGW_REPORTDEVICEINFO_EXPORTS

#ifdef __cplusplus
extern "C" {
#endif
    //! 开启网卡设备线程 true: 已开启，false: 开启失败
    JGW_REPORTDEVICEINFO_API bool StartNetCardDeviceThread(int nIntervalTimeMS = 200);
    //! 添加注册消息
    JGW_REPORTDEVICEINFO_API bool RegNetCardDeviceMsg(HWND hWindow,const wchar_t* strIPAddressSegment = L"192.168.1.");
    // 
    JGW_REPORTDEVICEINFO_API bool RegNetCardDeviceThreadMsg(HANDLE hThread,const wchar_t* strIPAddressSegment = L"192.168.1.");
    //!
    JGW_REPORTDEVICEINFO_API void ClearNetCardSortConfig();
    //! 移除注册消息
    JGW_REPORTDEVICEINFO_API void RemoveNetCardDeviceMsg(HWND hWindow);
    //! 关闭网卡设备线程池
    JGW_REPORTDEVICEINFO_API void CloseNetCardDeviceThread();
#ifdef __cplusplus
}
#endif