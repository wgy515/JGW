#pragma once

#ifdef AUTO_ID_USB_API
#undef AUTO_ID_USB_API
#endif

#ifdef JGW_DEVICEPOOL_EXPORTS
#define AUTO_ID_USB_API __declspec(dllexport)
#else
#define AUTO_ID_USB_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
	/*****************************************************************************
	根据设备范例ID 获取USB排序索引，索引下标从0开始计数

	*******************************************************************************/
	AUTO_ID_USB_API bool  GetInstanceIDSortIndex(int &nSortIndex,const wchar_t *strInstanceID);

	/*****************************************************************************
	根据设备范例ID 获取USB排序索引,以及SerialNumber，索引下标从0开始计数

	*******************************************************************************/
	AUTO_ID_USB_API bool  GetInstanceIDSortIndexAndSerialNumber(int &nSortIndex,const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize);

	/*****************************************************************************
	清楚USB排序索引
	*******************************************************************************/
	AUTO_ID_USB_API void  ClearSortIndex();
	/*****************************************************************************
	获取ADB 端口的 SerialNumber
	******************************************************************************/
	AUTO_ID_USB_API void  GetSerialNumber(const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize);
    //! 
    AUTO_ID_USB_API unsigned short GetUsbProtocolVersionToInstance(const wchar_t *strInstanceID);

#ifdef __cplusplus
}
#endif
