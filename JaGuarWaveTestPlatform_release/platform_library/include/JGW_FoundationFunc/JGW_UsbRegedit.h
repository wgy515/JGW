#ifndef JGW_FOUNDATIONFUNCLIB_USBREGEDIT_H__
#define JGW_FOUNDATIONFUNCLIB_USBREGEDIT_H__

#include "JGW_FoundationFuncLib_Define.h"
#include <vector>
#include <string>
namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        //! 检测是否开启DHCP  true: 开启动态IP  false: 开启静态IP
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_GetAdpateDHCPStatus(const wchar_t* strAdapterName);
        //!
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_SetAdpaterDHCPStatus(const wchar_t* strAdapterName,DWORD dwStatus = 0);

        //! 获取静态IP地址信息
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetStaticIPAddrToAdpaterName(const wchar_t* strAdapterName);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetIPV4AddrToAdpaterName(const wchar_t* strNetCardGUID);
        //! 更新usbflags 可以禁止驱动重复安装
        JGW_FOUNDATIONFUNC_LIB_API void	JGW_UpdateRegUsbFlags( int pid,int vid );

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetRegUsbInfoToTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag);

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetRegUsbInfoToMultiTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag);
        //! 根据USB 实例ID 获取USB FriendlyName
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetUsbFriendlyNameToPidVID(const wchar_t* strRegUsbPidVid);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_GetComportToUsbPidVid(const wchar_t* strRegUsbPidVid,int& nComPort);

        //! qcusbser 根据USB服务名获取当前插入设备数量
        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetInsertUsbDeviceCountToServicesName(const wchar_t* strServicesName);
        //! get usb enum pid vid to servicesname or enumid 根据USB服务名以及枚举ID 获取当前枚举ID所对应的USB实例ID
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetUsbEnumPidVidToServiceName(const wchar_t* strServicesName,DWORD dwEnumID);
        //! 获取当前USB服务名下所有已注册的的USB实例ID
        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetUsbEnumPidVidVectotToServiceName(const wchar_t* strServicesName,std::vector<std::wstring>& vEnumPidVid);
        //! 返回第一个端口
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetComPortToUsbServiceName(const wchar_t* strServiceName,const wchar_t* strUsbVID = L"",const wchar_t* strUsbPID = L"",const wchar_t* strDeviceDes = L"");
        //! 返回当前VID PID所有端口设备
        JGW_FOUNDATIONFUNC_LIB_API void JGW_GetAllComPortToUsbServiceName(std::vector<std::wstring>& vstrComPort,const wchar_t* strServiceName,const wchar_t* strUsbVID = L"",const wchar_t* strUsbPID = L"",const wchar_t* strDeviceDes = L"");
        //! 根据适配器名称获取网卡设备名称 AdapterName 比如：{5EDC9DE2-A000-49F1-A928-4C589078E2C9}
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetNetcardDeviceNameToAdapterName(const wchar_t* strAdapterName);
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}
#endif