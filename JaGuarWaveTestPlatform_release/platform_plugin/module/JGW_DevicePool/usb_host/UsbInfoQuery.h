#pragma once


class CUsbInfoQuery
{
public:
	CUsbInfoQuery(void );
	~ CUsbInfoQuery(void );

	bool								GetInstanceIDSortIndex(int &nSortIndex,LPCTSTR strInstanceID);
	bool								GetInstanceIDSortIndexAndSerialNumber(int &nSortIndex,const wchar_t *strInstanceID,wchar_t *strSerialNumber,size_t &nBufferSize);
	size_t                               GetUsbSortCounts();// 获取USB排序数目
};