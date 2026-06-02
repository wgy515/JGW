#pragma once
#include "..\sw_sb3_download_define.h"
#include <Windows.h>
class CConversion
{
public:
	CConversion(void);
	virtual ~CConversion(void);

public:
	bool ConversionEscapeChar(BYTE *pData, DWORD dwDataLen);
	bool GetData(BYTE *pOutData, DWORD dwBufferSize = PACKET_SIZE);
	DWORD GetDataSize();

protected:
	BYTE m_buffer[PACKET_SIZE];
	DWORD m_dwDataSize;
};
