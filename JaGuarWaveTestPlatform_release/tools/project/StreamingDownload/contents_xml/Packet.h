#pragma once
#include "9x25_download_define.h"
#include "CRCCCITT.h"
#include "Conversion.h"

class CPacket
{
public:
	CPacket(void);
	virtual ~CPacket(void);

public :
	BOOL SaharaBuildCmdPacket(BYTE *pCmdData, DWORD dwDataLen);
	BOOL SaharaBuildDataPacket();
	BOOL GetSaharaCmdPacket(BYTE *pPacketDataOut, DWORD dwLen);
	DWORD GetSaharaPacketLen();

	BOOL BuildCmdPacket(BYTE cmd, void *cmdParam = NULL, DWORD cmdParamLen = 0);
	BOOL BuildDataPacket(BYTE cmd, BYTE* pPacketData, DWORD dwInputDataLength, void *cmdParam = NULL, DWORD cmdParamLen = 0);
	VOID FillPacketCmdParameter(BYTE cmd, void *cmdParam, DWORD &cmdParamLen);
	BOOL GetPacket(BYTE *pPacketDataOut, DWORD dwLen = PACKET_SIZE);
	DWORD GetPacketCountLength();

protected:
	CConversion	m_conversion;
	CCRCCCITT m_crcCCITT;
	BYTE m_packetdata[PACKET_SIZE];
	BYTE m_saharaPacketData[SA_PACKET_SIZE];

	DWORD m_dwSaharaPacketDataLen;
	DWORD m_dwPacketDataCountLength;
};
