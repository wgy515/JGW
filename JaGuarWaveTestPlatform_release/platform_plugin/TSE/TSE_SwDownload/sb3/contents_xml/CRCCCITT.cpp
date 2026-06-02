#include "StdAfx.h"
#include "CRCCCITT.h"

CCRCCCITT::CCRCCCITT(void)
{
}

CCRCCCITT::~CCRCCCITT(void)
{
}

unsigned short CCRCCCITT::DoCrc(unsigned char *pData, int nLen)
{
	unsigned short fcs = 0xffff;    // ³õÊ¼»¯

	while(nLen>0)
	{
		fcs = (fcs >> 8) ^ crc16_ccitt_table[(fcs ^ *pData) & 0x00ff];
		nLen--;
		pData++;
	}

	return ~fcs;
}

//Test crc sum is ok or error in data packet
bool CCRCCCITT::CheckCrcSum(unsigned char *pData, int nLen)
{
	unsigned short crc = 0;

	crc = DoCrc(pData, nLen);
	crc = ~crc;

	if(GOOD_CRC == crc)
	{
		return false;
	}

	return true;
}