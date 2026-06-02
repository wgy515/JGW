#pragma once
#define INSTRUMENT_TEST_COUNTS  3


#define CHECKOPC(A)		\
{\
	if (1 != (atoi((A))))\
{\
	return false;\
}\
}

enum
{
    NI_GPIB_TYPE = 0,
    AGILENT_GPIB_TYPE
};

enum
{
    E_GPIB_HIVE,//! GPIB连接 又分为NI和Agen
    E_TCP_IP_HIVE, //! TCP IP连接
    E_VISA_HIVE, //! VISA
    E_SERIAL_HIVE //! Serial连接
};


enum CMW500_PORT
{
	ERFAC = 1,	//RFAC
	ERFAO,		//RFAO
	ERFBC,		//RFBC
    ERF_UNKNOW
};
enum E_LTE_MODE
{
	E_LTE_FDD,
	E_LTE_TDD
};

const char g_szLteModeName[2][4] = {"FDD","TDD"};

enum E_SYS_NAME
{
	E_LTE_NAME = 0,
	E_WCDMA_NAME,
	E_TDSCDMA_NAME,
	E_CDMA_NAME,
	E_GSM_NAME,
    E_WLAN_NAME,
	E_MAX_SYS
};

const char g_szSysName[][8] = {"LTE","WCDMa","TDSCdma","CDMA","GSM","WLAN"};
const wchar_t g_wszSysName[E_MAX_SYS][10] = {L"LTE",L"WCDMa",L"TDSCdma",L"CDMA",L"GSM",L"WLAN"};



#define GetRFTxStrFlags(x) (ERFAC == (x))?"AC":((ERFBC == (x))?"BC":"AO")
#define GetRFRxStrFlags(x) (ERFAC == (x))?"AC":"BC"

typedef enum
{
	E_LTE_CH_BW_1_4_MHZ,
	E_LTE_CH_BW_03_MHZ,
	E_LTE_CH_BW_05_MHZ,
	E_LTE_CH_BW_10_MHZ,
	E_LTE_CH_BW_15_MHZ,
	E_LTE_CH_BW_20_MHZ,
	E_LTE_CH_BW_UNKNOWN
}E_LTE_CH_BW_TYPE;

class S_BAND_CHANNEL_INFO
{
public:
	int			m_nUpLinkChannel;//! 测试信道  配置文件读取的是上行信道
	int			m_nDownLinkChannel;//! 测试信道  配置文件读取的是上行信道
	double		m_fUpLinkFreq;//! 测试上行频率	
	double		m_fDownLinkFreq;//! 测试下行频率
	float		m_fPTxLoss; //主集发射线损信息
	float		m_fPRxLoss; //! 主集接收线损
	float		m_fDRxLoss; //! 分集接收线损
	void Clear()
	{
		m_nUpLinkChannel = 0;
		m_nDownLinkChannel = 0;
		m_fUpLinkFreq = 0;
		m_fDownLinkFreq = 0;
		m_fPTxLoss = 1.00f;
		m_fPRxLoss = 1.00f;
		m_fDRxLoss = 1.00f;
	}
};


typedef struct  
{
	int indicator;
	float evm;
	float freqError;
	float OBW;
	float ACLR[6];
	int sem;
	float maxPwr;
} LTE_NS_MaxPwrTestValue;

typedef struct  
{
	int indicator;
	float evm;
	float freqError;
	float OBW;
	float ACLR[4];
	float PCDE;
	float sem[6];
	float maxPwr;
	int pvt;
	float IQoffset;
	float IQbalance;
	float PhaseError;
}TDS_NS_MaxPwrTestValue;

typedef struct  
{
	int indicator;
	float evm;
	float freqError;
	float OBW;
	float ACLR[4];
	float PCDE;
	float PhaseError;
	float sem[8];
	float maxPwr;
	float IQoffset;
	float IQbalance;
}W_NS_MaxPwrTestValue;

typedef struct  
{
	int indicator;
	float evm;
	float freqError;
	float IQbalance;
	float PhaseError;

	float maxPwr;
	float wfQuality;

}C_NS_MaxPwrTestValue;


typedef struct  
{
	int indicator;
	float fPowerlevel;
	float fPhaseErrorPeak;
	float fPhaseErrorRMS;
	float freqError;
	bool  bPVtime;
	float fber;
	bool  bSpectrumSwitching;
	bool  bSpectrumModulation;
	union
	{
		float evm;
		float fth95Evm;
	};

	/*	float */
	// 	float IQbalance;
	// 	float wfQuality;

}GSM_NS_TestValue;

const int nOffestGsMTestValue[] = {offsetof(GSM_NS_TestValue,fPowerlevel),
	offsetof(GSM_NS_TestValue,freqError),
	offsetof(GSM_NS_TestValue,evm),
	offsetof(GSM_NS_TestValue,fPhaseErrorRMS),
	offsetof(GSM_NS_TestValue,fPhaseErrorPeak),
	offsetof(GSM_NS_TestValue,bPVtime),
	offsetof(GSM_NS_TestValue,bSpectrumSwitching),
	offsetof(GSM_NS_TestValue,bSpectrumModulation),
	offsetof(GSM_NS_TestValue,fber)};

/*
● 0 (OK):
Measurement values available, no error detected.
● 1 (Measurement Timeout):
Measurement results may be available, however, at least a part of the measurement
provides only INValid results or has not completed the full statistic count. The measurement
has been stopped after the configurable measurement timeout.
● 3 (Overdriven):
Measurement results available, however, the accuracy of some results may be
impaired because the input signal level was too high.
● 4 (Underdriven):
Measurement results available, however, the accuracy of some results may be
impaired because the input signal level was too low.
● 6 (Trigger Timeout):
No measurement results available. The measurement could not be started because
no trigger event was detected.
● 7 (Acquisition Error):
No measurement results available, e.g. because the R&S CMW500 could not properly
decode the RF input signal.
● 8 (Sync Error):
No measurement results available, because the R&S CMW500 could not synchronize
to the RF input signal.
● 15 (Reference Frequency Error):
The instrument has been configured to use an external reference signal but the reference
oscillator could not be phase locked to the external signal (e.g. signal level
too low, frequency out of range or reference signal not available at all).
*/
enum E_Reliability_Indicator
{
	E_MEAS_OK = 0,
	E_MEAS_TIME_OUT = 1,
	E_MEAS_OVERDRIVER = 3,
	E_MEAS_Underdriven = 4,
	E_MEAS_TRIGGER_TIMEOUT = 6,
	E_MEAS_Acquisition_ERROR = 7,
	E_MEAS_SYNC_ERROR = 8,
	E_MEAS_REFERENCE_FREQUENCY_ERROR = 15
};

#ifndef JGW_NIGPIB_CONTROLLIMPL_LIB_EXPORTS
#   define JGW_NIGPIB_CONTROLLIMPL_LIB_IMPORTS
#endif

#ifdef JGW_NIGPIB_CONTROLLIMPL_LIB_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_NIGPIBControllImpl_d.lib")
#   else
#       pragma comment(lib,"JGW_NIGPIBControllImpl.lib")
#   endif
#endif