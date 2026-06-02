#include "StdAfx.h"
#include "Packet.h"

CPacket::CPacket(void)
{
	memset(m_saharaPacketData, 0, sizeof(m_saharaPacketData));
	memset(m_packetdata, 0, sizeof(m_packetdata));
	m_dwPacketDataCountLength = 0;
	m_dwSaharaPacketDataLen = 0;
}

CPacket::~CPacket(void)
{
}

BOOL CPacket::BuildDataPacket(BYTE cmd, BYTE* pPacketData, DWORD dwDataLength, void *cmdParam, DWORD cmdParamLen)
{
	PACKET_HEADER *pPacketHeader = (PACKET_HEADER*)m_packetdata;
	PACKET_END	*pPacketEnd = NULL;
	BYTE	*pCur = m_packetdata;

	if(NULL == pPacketData || dwDataLength <= 0)
	{
		return FALSE;
	}

	memset(m_packetdata, 0, sizeof(m_packetdata));
	m_dwPacketDataCountLength = 0;

	pPacketHeader->FrameHeader = FLAG_FRAME;
	pPacketHeader->Command = cmd;

	FillPacketCmdParameter(cmd, cmdParam, cmdParamLen);

	//copy src data to packet for sum crc 
	pCur = m_packetdata + cmdParamLen + sizeof(PACKET_HEADER);
	memcpy(pCur, pPacketData, dwDataLength);
	pCur = pCur + dwDataLength;

	//crc sum
	unsigned short crc;
	crc = m_crcCCITT.DoCrc(m_packetdata + sizeof(BYTE), pCur - m_packetdata - sizeof(BYTE));
	//pPacketEnd->FCS[0] = crc &0x00ff;
	//pPacketEnd->FCS[1] = (crc >> 8)&0x00ff;

	//conversion escape char 
	BYTE szBuffer[PACKET_SIZE] = {0};
	DWORD dwCmdParamLen = 0;

	m_conversion.ConversionEscapeChar((BYTE*)cmdParam, cmdParamLen);
	m_conversion.GetData(szBuffer);
	dwCmdParamLen = m_conversion.GetDataSize();

	if (dwCmdParamLen != cmdParamLen)
	{
		pCur = m_packetdata + sizeof(PACKET_HEADER);
		memcpy(pCur, szBuffer, dwDataLength);
	}
	m_conversion.ConversionEscapeChar(pPacketData, dwDataLength);
	m_conversion.GetData(szBuffer);
	dwDataLength = m_conversion.GetDataSize();

	//copy data to packet
	pCur = m_packetdata + dwCmdParamLen + sizeof(PACKET_HEADER);
	memcpy(pCur, szBuffer, dwDataLength);
	pCur = pCur + dwDataLength;

	pPacketEnd = (PACKET_END *)pCur;

	if( 0x7D == (crc &0x00ff) )
	{
		*pCur++ = 0x7D;
		*pCur++ = 0x5D; 
	}
	else if( 0x7E == (crc &0x00ff) )
	{
		*pCur++ = 0x7D;
		*pCur++ = 0x5E;
	}
	else
	{
		*pCur++ =  crc &0x00ff;
	}

	if(0x7D == ((crc >> 8)&0x00ff))
	{
		*pCur++ = 0x7D;
		*pCur++ = 0x5D; 
	}
	else if(0x7E == ((crc >> 8)&0x00ff))
	{
		*pCur++ = 0x7D;
		*pCur++ = 0x5E;
	}
	else
	{
		*pCur++ = (crc >> 8)&0x00ff;
	}

	*pCur++= FLAG_FRAME;

	//count packet size 
	m_dwPacketDataCountLength = pCur - m_packetdata;
	return TRUE;
}

BOOL CPacket::GetPacket(BYTE *pPacketDataOut, DWORD dwLen)
{
	if(NULL == pPacketDataOut || dwLen <= 0)
		return FALSE;

	if(dwLen < m_dwPacketDataCountLength)
		return FALSE;

	memcpy(pPacketDataOut, m_packetdata, m_dwPacketDataCountLength);
	return TRUE;
}

DWORD CPacket::GetPacketCountLength()
{
	return m_dwPacketDataCountLength;
}

VOID CPacket::FillPacketCmdParameter(BYTE cmd, void *cmdParam, DWORD &cmdParamLen)
{
	BYTE *pCur = m_packetdata;

	pCur = pCur + sizeof(PACKET_HEADER);
	memcpy(pCur, (BYTE*)cmdParam, cmdParamLen);
}

BOOL CPacket::BuildCmdPacket(BYTE cmd, void *cmdParam, DWORD cmdParamLen)
{
	PACKET_HEADER *pPacketHeader = (PACKET_HEADER*)m_packetdata;
	PACKET_END	*pPacketEnd = NULL;
	BYTE	*pCur = m_packetdata;

	memset(m_packetdata, 0, sizeof(m_packetdata));
	m_dwPacketDataCountLength = 0;

	pPacketHeader->FrameHeader = FLAG_FRAME;
	pPacketHeader->Command = cmd;

	pCur = m_packetdata  + sizeof(PACKET_HEADER);

	if( cmdParam != NULL )
	{
		pCur = m_packetdata  + sizeof(PACKET_HEADER);
		memcpy(pCur, (BYTE*)cmdParam, cmdParamLen);
		pCur = pCur + cmdParamLen;
	}

	pPacketEnd = (PACKET_END *)pCur;
	pPacketEnd->Flag = FLAG_FRAME;

	//crc sum
	unsigned short crc;
	crc = m_crcCCITT.DoCrc(m_packetdata + sizeof(BYTE), pCur - m_packetdata - sizeof(BYTE));
	pPacketEnd->FCS[0] = crc &0x00ff;
	pPacketEnd->FCS[1] = (crc >> 8)&0x00ff;

	//count packet size 
	m_dwPacketDataCountLength = pCur - m_packetdata + sizeof(PACKET_END);
	return TRUE;
}

BOOL CPacket::SaharaBuildCmdPacket(BYTE *pCmdData, DWORD dwDataLen)
{
	memset(m_saharaPacketData, 0, sizeof(m_saharaPacketData));

	memcpy(m_saharaPacketData, pCmdData, dwDataLen);
	m_dwSaharaPacketDataLen = dwDataLen;
	return TRUE;
}

BOOL CPacket::SaharaBuildDataPacket()
{
	return TRUE;
}

BOOL CPacket::GetSaharaCmdPacket(BYTE *pPacketDataOut, DWORD dwLen)
{
	if (m_dwSaharaPacketDataLen > dwLen)
	{
		return FALSE;
	}

	memcpy(pPacketDataOut, m_saharaPacketData, m_dwSaharaPacketDataLen);
	return TRUE;
}

DWORD CPacket::GetSaharaPacketLen()
{
	return m_dwSaharaPacketDataLen;
}
