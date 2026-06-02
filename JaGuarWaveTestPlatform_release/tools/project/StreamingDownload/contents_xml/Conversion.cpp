#include "StdAfx.h"
#include "Conversion.h"

CConversion::CConversion(void)
{
	memset(m_buffer, 0, sizeof(m_buffer));
	m_dwDataSize = 0;
}

CConversion::~CConversion(void)
{
}

bool CConversion::ConversionEscapeChar(BYTE *pData, DWORD dwDataLen)
{
	if(NULL == pData || dwDataLen <= 0)
		return false;

	memset(m_buffer, 0, sizeof(m_buffer));
	m_dwDataSize = 0;

	BYTE *pBuff = m_buffer;

	for(DWORD i = 0; i < dwDataLen; i++)
	{
		if((0x7E == *pData))
		{
			*pBuff = 0x7D;
			pBuff++;
			*pBuff = 0x5E;
		}
		else if(0x7D == *pData)
		{
			*pBuff = 0x7D;
			pBuff++;
			*pBuff = 0x5D;
		}
		else
		{
			*pBuff = *pData;
		}

		pBuff++;
		pData++;
	}
	
	m_dwDataSize = pBuff - m_buffer;
	return true;
}

bool CConversion::GetData(BYTE *pOutData, DWORD dwBufferSize)
{
	if( NULL == pOutData || dwBufferSize <= 0 )
		return false;

	if(dwBufferSize < m_dwDataSize)
		return false;

	memcpy(pOutData, m_buffer, m_dwDataSize);
	return true;
}

DWORD CConversion::GetDataSize()
{
	return m_dwDataSize;
}