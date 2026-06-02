#pragma once
#include <Windows.h>
#include <stdio.h>

#define DEBUGMODEL 
#pragma warning(disable:4819)
// enum COMMUNICATIONSSYS
// {
// 	TECHNOLOGY_CDMA = 0,
// 	TECHNOLOGY_WCDMA = 1,
// 	GSM = 2,
// 	BLUETOOTH = 3,
// 	MEDIAFLO = 4,
// 	ANALOG = 5,
// 	TECHNOLOGY_LTE = 10,
// 	TECHNOLOGY_TD_SCDMA = 11,
// 	TECHNOLOGY_UNKNOW
// };

enum COMMUNICATIONSSYS
{
	TECHNOLOGY_CDMA = 0,
	TECHNOLOGY_WCDMA = 1,
	GSM = 2,
	TECHNOLOGY_BLUETOOTH,
	TECHNOLOGY_MEDIAFLO,
	TECHNOLOGY_DVBH,
	TECHNOLOGY_ANALOG,
	TECHNOLOGY_GPS,
	TECHNOLOGY_ISDBT,
	TECHNOLOGY_1xEVDO,
	TECHNOLOGY_LTE = 10,
	TECHNOLOGY_TD_SCDMA = 11,
	TECHNOLOGY_UNKNOW,
	// 	TECHNOLOGY_WLAN,
	// 	TECHNOLOGY_NFC,
	// 	TECHNOLOGY_MULTIMEDIA,
	// 	TECHNOLOGY_FM_RADIO,
	// 	//IF ADDING MORE ENUMS, ALSO ADD AN ENTRY IN TechnologyTypeToName METHOD
	// 	TECHNOLOGY_TYPE_MAX = 0xFFFFFFFF
};


#define  GetGsmBand(iBand)\
	switch(iBand)\
{\
	case 8:\
	case 900:\
	iBand = FTM_GSM_BER_EGSM_900;\
	break;\
case 5:\
case 850:\
	iBand = FTM_GSM_BER_CELL_850;\
	break;\
case 3:\
case 1800:\
	iBand = FTM_GSM_BER_DCS_1800;\
	break;\
case 2:\
case 1900:\
	iBand = FTM_GSM_BER_PCS_1900;\
	break;\
default:\
	return FALSE;\
	}

typedef struct 
{
	//COMMUNICATIONSSYS m_eSys;//制式信息
	int				  m_nUpLinkChannel;//信道信息
	int				  m_nDownloadLinkChannel;
	double			  dFreqUL;//上行频率
	double			  dFreqDL;//下行频率
	int				  m_nBand;//频段
	int				  m_iPwrIndex;
	void	InitParam()
	{
		//m_eSys = TECHNOLOGY_UNKNOW;
		dFreqDL = 0;
		dFreqUL = 0;
		m_nBand = 0;
		m_nUpLinkChannel = 0;
		m_iPwrIndex = 0;
	}
}SBAND_INFO, *LPSBAND_INFO;

#ifdef DEBUGMODEL

#define CHECKFUNRES_SUB(A)	\
{\
	if (!(A)){\
	return false;\
	}\
}

#define TESTFUNCOK_SUB(A)	\
{\
	if (!(A)){\
	return false;\
	}\
}

#else

#define CHECKFUNRES_SUB(A)	\
{\
	if (!(A)){\
	return false;\
	}\
}

#define TESTFUNCOK_SUB(A)	\
{\
	if (!(A)){\
	return false;\
	}\
}
#endif


typedef struct _LTE_CHANNEL_TABLE_
{
	double dDownloadLinkFreqLow;
	int nDownloadLinkOffset;
	double dUpLinkFreqLow;
	int nUpLinkOffset;
}LTE_CHANNEL_TABLE;

typedef struct _WCDMA_CHANNEL_TABLE_
{
	double dDownloadLinkFreqLow;
	int nDownloadLinkOffset;
	double dUpLinkFreqLow;
	int nUpLinkOffset;
	int UARFCNoffset;
}WCDMA_CHANNEL_TABLE;

const LTE_CHANNEL_TABLE g_dLteFreqTable[] = {{0, 0, 0, 0}, 
{2110, 0, 1920, 18000}, {1930, 600, 1850, 18600}, {1805, 1200, 1710, 19200}, {2110, 1950, 1710, 19950},
{869, 2400, 824, 20400}, {875, 2650, 830, 20650}, {2620, 2750, 2500, 20750}, {925, 3450, 880, 21450},
{1844.9, 3800, 1749.9, 21800}, {2110, 4150, 1710, 22150}, {1475.9, 4750, 1427.9, 22750}, {729, 5010, 699, 23010},
{746, 5180, 777, 23180}/*B13*/,{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{791,6150,832,24150}/*B20*/,
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{0,0,0,0},{0,0,0,0},{0,0,0,0},{758,9210,703,27210}/*28*/,
{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
{1900, 36000, 1900, 36000}/*B33*/, {2010, 36200, 2010, 36200}, {1850, 36350, 1850, 36350},
{1930, 36950, 1930, 36950},{1910, 37550, 1910, 37550}, {2570, 37750, 2570, 37750}, {1880, 38250, 1880, 38250},
{2300, 38650, 2300, 38650}, {2496, 39650, 2496, 39650},{3400, 41590, 3400, 41590}, {3600, 43590, 3600, 43590}, 
{703, 45590, 703, 45590}
};

const WCDMA_CHANNEL_TABLE g_dWcdmaFreqTable[12] = {
	{0,0,0,0},							//NA		
	//	 DL      DLoff UL      ULoff	UARFCNoffset	
	{2112.4, 0,    1922.4, 0		,950},			//BAND1
	{1932.4, 0,    1852.4, 0		,400},			//BAND2
	{1807.4, 1575, 1712.4, 1525		,225},		//BAND3
	{2112.4, 1805, 1712.4, 1450		,225},		//BAND4
	{871.4,  0,    826.4,  0		,225},			//BAND5
	{877.4,  0,    832.4,  0		,225},			//BAND6
	{2622.4, 2175, 2502.4, 2100		,225},		//BAND7
	{927.4,  340,  882.4,  340		,225},		//BAND8
	{1847.4, 0,    1752.4, 0		,475},			//BAND9
	{2112.4, 1490, 1712.4, 1135		,225},		//BAND10
	{1478.4, 736,  1430.4, 733		,225},		//BAND11
};
