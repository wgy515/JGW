#pragma once
#include "..\9x25_download_define.h"
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
