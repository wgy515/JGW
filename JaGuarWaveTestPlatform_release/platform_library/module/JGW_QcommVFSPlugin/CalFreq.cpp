#include "StdAfx.h"
#include <CTF_QCSubSysVFS/CalFreq.h>


CCalFreq::CCalFreq(void)
{
}


CCalFreq::~CCalFreq(void)
{
}

bool CCalFreq::CalFreq(COMMUNICATIONSSYS nsys,SBAND_INFO& sBandInfo)
{
	switch(nsys)
	{
	case TECHNOLOGY_LTE:
		return CalLteFreq(sBandInfo);
	case TECHNOLOGY_WCDMA:
		return CalWcdmaFreq(sBandInfo);
	case TECHNOLOGY_TD_SCDMA:
		return CalTdscdmaFreq(sBandInfo);
	case TECHNOLOGY_CDMA:
		return CalCdmaFreq(sBandInfo);
	case GSM:
		return CalGsmFreq(sBandInfo);
	default:
		throw L"不支持制式计算频段信息";
	}
	
	return true;
}

bool CCalFreq::CalLteFreq(SBAND_INFO& sBandInfo)
{
	CHECKFUNRES_SUB( sBandInfo.m_nBand >= 1 && sBandInfo.m_nBand <= 44 );

	if (sBandInfo.m_nBand >= 1 && sBandInfo.m_nBand <= 32)
	{
		sBandInfo.m_nDownloadLinkChannel = g_dLteFreqTable[sBandInfo.m_nBand].nDownloadLinkOffset + (sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[sBandInfo.m_nBand].nUpLinkOffset);
	}
	else 
	{
		sBandInfo.m_nDownloadLinkChannel = sBandInfo.m_nUpLinkChannel;
	}

	sBandInfo.dFreqDL = g_dLteFreqTable[sBandInfo.m_nBand].dDownloadLinkFreqLow + 0.1 * (sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[sBandInfo.m_nBand].nUpLinkOffset);
	sBandInfo.dFreqUL = g_dLteFreqTable[sBandInfo.m_nBand].dUpLinkFreqLow + 0.1 * (sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[sBandInfo.m_nBand].nUpLinkOffset);

	return true;
}

bool CCalFreq::CalWcdmaFreq(SBAND_INFO& sBandInfo)
{
	CHECKFUNRES_SUB(sBandInfo.m_nBand < 12 && sBandInfo.m_nBand > 0);

	sBandInfo.m_nDownloadLinkChannel = sBandInfo.m_nUpLinkChannel + g_dWcdmaFreqTable[sBandInfo.m_nBand].UARFCNoffset;
	sBandInfo.dFreqUL = ((double)sBandInfo.m_nUpLinkChannel / (double)5.0) + (double)g_dWcdmaFreqTable[sBandInfo.m_nBand].nUpLinkOffset;
	sBandInfo.dFreqDL = ((double)sBandInfo.m_nDownloadLinkChannel  / (double)5.0) + (double)g_dWcdmaFreqTable[sBandInfo.m_nBand].nDownloadLinkOffset;

	return true;
}

bool CCalFreq::CalTdscdmaFreq(SBAND_INFO& sBandInfo)
{
	sBandInfo.m_nDownloadLinkChannel = sBandInfo.m_nUpLinkChannel ;
	sBandInfo.dFreqDL = sBandInfo.dFreqUL = sBandInfo.m_nUpLinkChannel/5.00;
	return true;
}

bool CCalFreq::CalGsmFreq(SBAND_INFO& sBandInfo)
{
	switch(sBandInfo.m_nBand)
	{
	case 2://1900
	case 1900:
		sBandInfo.dFreqUL = 1850 + (sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		sBandInfo.dFreqDL = 1930 + (sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		break;
	case 3://1800
	case 1800:
		sBandInfo.dFreqUL = 1710 + (sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		sBandInfo.dFreqDL = 1805 + (sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		break;
	case 5://850
	case 850:
		sBandInfo.dFreqUL = 824 + 0.2 * (sBandInfo.m_nUpLinkChannel - 127);
		sBandInfo.dFreqDL = 869 + 0.2 * (sBandInfo.m_nUpLinkChannel - 127);;
		break;
	case 8://900
	case 900:
		if (sBandInfo.m_nUpLinkChannel >= 975)
		{
			sBandInfo.dFreqUL = 890 + 0.2 * (sBandInfo.m_nUpLinkChannel - 1024);
			sBandInfo.dFreqDL = 935 + 0.2 * (sBandInfo.m_nUpLinkChannel - 1024);;
		}
		else
		{
			sBandInfo.dFreqUL = 890 + 0.2 * sBandInfo.m_nUpLinkChannel;
			sBandInfo.dFreqDL = 935 + 0.2 * sBandInfo.m_nUpLinkChannel;;
		}
		break;
	default:
		return false;
	}

	return true;
}

bool CCalFreq::CalCdmaFreq(SBAND_INFO& sBandInfo)
{
	CHECKFUNRES_SUB( 0 == sBandInfo.m_nBand );
	sBandInfo.dFreqUL = 825 + ( sBandInfo.m_nUpLinkChannel * 0.03 );
	sBandInfo.dFreqDL = 870 + ( sBandInfo.m_nUpLinkChannel * 0.03 );
	sBandInfo.m_nDownloadLinkChannel =  sBandInfo.m_nUpLinkChannel;

	return true;
}