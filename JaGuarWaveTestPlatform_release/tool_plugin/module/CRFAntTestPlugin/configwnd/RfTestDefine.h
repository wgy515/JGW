#pragma once
#include <map>
#include <vector>
/*#include "../TestThread.h"*/
#include <JGW_NIGPIBControllImpl/CMW500_Montior.h>
#include <JGW_QMSLFuncPlugin/SubSysVFSDefine.h>

namespace JGW
{
    typedef std::map<int,std::vector<int>>	_mChannel;
#define ISNOTNULL(x)\
    if( !(x) )\
    return false;

    const wchar_t g_szSysNameW[][20] = {L"CDMA",L"WCDMA",L"GSM",L"BLUETOOH",L"MEDIAFLO",L"ANGLOG",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"LTE",L"TDSCDMA",L"WIFI"};
    const wchar_t g_szRfConfigSecW[][20] = {L"CDMA_RF_CONFIG",L"WCDMA_RF_CONFIG",L"GSM_RF_CONFIG",L"BLUETOOH_RF_CONFIG",L"MEDIAFLO_RF_CONFIG",L"ANGLOG_RF_CONFIG",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"LTE_RF_CONFIG",L"TDSCDMA_RF_CONFIG",L"WIFI_CONFIG"};

    const wchar_t g_szRfNormSecW[][20] = {L"CDMA_TESTOPTION",L"WCDMA_TESTOPTION",L"GSM_RF_CONFIG",L"BLUETOOH_RF_CONFIG",L"MEDIAFLO_RF_CONFIG",L"ANGLOG_RF_CONFIG",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"UNKNOW",L"LTE_TESTOPTION",L"TDS_TESTOPTION",L"WIFI_1_TESTOPTION"};

    const wchar_t gszWlanModeName[][20] = {L"B",L"G",L"N",L"AC"};

    class S_BAND_INFO
    {
    public:
        int			m_nBand;
        CMW500_PORT m_ePort;
        CMW500_PORT	m_eDrxPort;
        std::vector<S_BAND_CHANNEL_INFO> m_vBandChannelInfo;

        S_BAND_INFO()
        {
            Init();
        }

        void Init()
        {
            m_nBand = -1;
            m_ePort = ERFAC;
            m_eDrxPort = ERFAO;
            m_vBandChannelInfo.clear();
        }
    };

    //1.2.3.4.5.6.7.8.9.39
    class S_SYS_INFO
    {
    public:
        std::vector<S_BAND_INFO> m_vBandInfo;

        void Clear()
        {
            m_vBandInfo.clear();
        }
        // 	std::map<int,std::vector<int>> m_mUpLinkChannel;//! 测试信道  配置文件读取的是上行信道
        // 	std::map<int,std::vector<int>> m_mDownLinkChannel;//! 测试信道  配置文件读取的是上行信道
        // 	std::map<int,std::vector<float>> m_mUpLinkFreq;//! 测试上行频率	
        // 	std::map<int,std::vector<float>> m_mDownLinkFreq;//! 测试下行频率
        // 	std::map<int,std::vector<float>> m_mfLoss; //线损信息
    };
    //ERFAC = 1,	//RFAC
    //ERFAO,		//RFAO
    //ERFBC		//RFBC
    enum
    {
        MAX_VALUE = 0,
        MIN_VALUE,
        VAULE_ARRAY_SIZE
    };

    class S_LTE_NORM_INFO
    {
    public:
        S_SYS_INFO		m_sTestBandInfo;

        bool			m_bTestMaxPwr; 
        float			m_fMaxPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestMinPwr;
        float			m_fMinPwrLim[VAULE_ARRAY_SIZE];
        //! UTRAACLR1 UTRAACLR2
        bool			m_bTestAclr;
        float			m_fEutraAclrLim[VAULE_ARRAY_SIZE];
        float			m_fUtra1AclrLim[VAULE_ARRAY_SIZE];
        float			m_fUtra2AclrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestOBW;
        float			m_fObwLim[VAULE_ARRAY_SIZE];

        bool			m_bTestFrqErr;
        float			m_fFreqErrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestSem;
        int				m_nSemLim[VAULE_ARRAY_SIZE];

        bool			m_bTestEvm;
        float			m_fEvmLim[VAULE_ARRAY_SIZE];

        // 	bool			m_bOnOffTask;
        // 	bool			m_bCwave;
        // 	bool			m_bTsq;
        bool			m_bTestRxLevel;
        float			m_fRxLevelCellPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];
        // 	float			m_fPrxLevelLoss;
        // 	float			m_fDrxLevelLoss;

        bool			m_bTestBler;
        float			m_fBlerCellPwr;
        float			m_fBerLim[VAULE_ARRAY_SIZE];
    };

    class S_WCDMA_NORM_INFO
    {
    public:
        S_SYS_INFO		m_sTestBandInfo;
        // 	float			m_fPrxLevelLoss;
        // 	float			m_fDrxLevelLoss;
        bool			m_bTestMaxPwr; 
        float			m_fMaxPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestMinPwr;
        float			m_fMinPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestAclr;
        float			m_fAclrNega16Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrNega32Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrPosi16Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrPosi32Lim[VAULE_ARRAY_SIZE];

        bool			m_bTestOBW;
        float			m_fObwLim[VAULE_ARRAY_SIZE];

        bool			m_bTestFrqErr;
        float			m_fFreqErrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestEvm;
        float			m_fEvmLim[VAULE_ARRAY_SIZE];

        bool			m_bTestSem;
        int				m_nSemABLim[VAULE_ARRAY_SIZE];
        int				m_nSemBCLim[VAULE_ARRAY_SIZE];
        int				m_nSemCDLim[VAULE_ARRAY_SIZE];
        int				m_nSemEFLim[VAULE_ARRAY_SIZE];
        int				m_nSemFELim[VAULE_ARRAY_SIZE];
        int				m_nSemDCLim[VAULE_ARRAY_SIZE];
        int				m_nSemCBLim[VAULE_ARRAY_SIZE];
        int				m_nSemBALim[VAULE_ARRAY_SIZE];

        bool			m_bTestPCde;
        float			m_fPeakCDELim[VAULE_ARRAY_SIZE];

        bool			m_bTestPhaseError;
        float			m_fPhaseErrRmsLim[VAULE_ARRAY_SIZE];

        bool			m_bTestIQOffset;
        float			m_fIQOffsetLim[VAULE_ARRAY_SIZE];

        bool			m_bTestIQBalance;
        float			m_fIQBalance[VAULE_ARRAY_SIZE];

        bool			m_bTestRxLevel;
        float			m_fRxLevelCellPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];

        bool			m_bTestBler;
        float			m_fBlerCellPwr;
        float			m_fBerLim[VAULE_ARRAY_SIZE];
    };


    class S_CDMA_NORM_INFO
    {
    public:
        S_SYS_INFO		m_sTestBandInfo;

        bool			m_bTestMaxPwr; 
        float			m_fMaxPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestMinPwr;
        float			m_fMinPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestFrqErr;
        float			m_fFreqErrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestEvm;
        float			m_fEvmLim[VAULE_ARRAY_SIZE];

        bool			m_bTestAcpr;

        bool			m_bTestWFQ;
        float			m_fWFQLim[VAULE_ARRAY_SIZE];

        bool			m_bTestIQBalance;
        float			m_fIQBalance[VAULE_ARRAY_SIZE];

        bool			m_bTestPhaseError;
        float			m_fPhaseErrRmsLim[VAULE_ARRAY_SIZE];

        bool			m_bTestRxLevel;
        float			m_fRxLevelCellPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];

        bool			m_bTestBler;
        float			m_fBlerCellPwr;
        float			m_fBerLim[VAULE_ARRAY_SIZE];
    };

    class S_TDSCDMA_NORM_INFO
    {
    public:
        S_SYS_INFO		m_sTestBandInfo;

        bool			m_bTestMaxPwr; 
        float			m_fMaxPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestMinPwr;
        float			m_fMinPwrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestAclr;
        float			m_fAclrNega16Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrNega32Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrPosi16Lim[VAULE_ARRAY_SIZE];
        float			m_fAclrPosi32Lim[VAULE_ARRAY_SIZE];

        bool			m_bTestOBW;
        float			m_fObwLim[VAULE_ARRAY_SIZE];

        bool			m_bTestFrqErr;
        float			m_fFreqErrLim[VAULE_ARRAY_SIZE];

        bool			m_bTestEvm;
        float			m_fEvmLim[VAULE_ARRAY_SIZE];

        bool			m_bTestSem;
        int				m_nSemABLim[VAULE_ARRAY_SIZE];
        int				m_nSemCDLim[VAULE_ARRAY_SIZE];
        int				m_nSemDELim[VAULE_ARRAY_SIZE];
        int				m_nSemEDLim[VAULE_ARRAY_SIZE];
        int				m_nSemDCLim[VAULE_ARRAY_SIZE];
        int				m_nSemBALim[VAULE_ARRAY_SIZE];

        bool			m_bTestPCde;
        float			m_fPeakCDELim[VAULE_ARRAY_SIZE];

        bool			m_bTestPvt;

        bool			m_bTestRxLevel;
        float			m_fRxLevelCellPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];

        bool			m_bTestBler;
        float			m_fBlerCellPwr;
        float			m_fBerLim[VAULE_ARRAY_SIZE];
    };

    enum eGsmTestProjetc
    {
        GSM_TestPower = 0,
        GSM_TestFreqError,
        GSM_TestEvm,
        GSM_TestPhaseErrorRMS,
        GSM_TestPhaseErrorPeak,
        GSM_TESTTestPVtime,
        GSM_TestSpectrumSwitching,
        GSM_TestSpectrumModulation,
        GSM_TestRxLevel,
        GSM_TESTTestBer
    };

    enum eEDGETestProjetc
    {
        EDGE_TestPower = 0,
        EDGE_TestFreqError,
        EDGE_Test95thEvm,
        EDGE_TestEvmRMS,
        EDGE_TestEvmPeak,
        EDGE_TESTTestPVtime,
        EDGE_TestSpectrumSwitching,
        EDGE_TestSpectrumModulation,
        EDGE_TestRxLevel,
        EDGE_TESTTestBer
    };

    class S_GSM_BAND_PWL_NORM_INFO
    {
    public:
        int				m_nPwrLevel;
        float			m_fPwrLim[VAULE_ARRAY_SIZE];
        float			m_fFreqErrLim[VAULE_ARRAY_SIZE];
        float			m_fEvmLim[VAULE_ARRAY_SIZE];
        float			m_fPhaseErrRmsLim[VAULE_ARRAY_SIZE];
        float			m_fPhaseErrPeakLim[VAULE_ARRAY_SIZE];
        float			m_fRxLevelPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];
        float			m_fBlerCellPwr;
        float			m_fBerLim[VAULE_ARRAY_SIZE];
    };

    class S_GSM_BAND_NORM_INFO
    {
    public:

        bool			m_bTestPwrLevel;
        bool			m_bTestPVT;
        bool			m_bTestSpectrumSwitch;
        bool			m_bTestSpectrumModulation;
        bool			m_bTestFrqErr;
        bool			m_bTestEvm;
        bool			m_bTestPhaseError;
        bool			m_bTestPhaseErrorPeak;
        bool			m_bTestRxLevel;
        bool			m_bTestBler;
        std::vector<S_GSM_BAND_PWL_NORM_INFO>	m_vGsmBandPwl;

        S_GSM_BAND_NORM_INFO()
        {
            Init();
        }

        void Init()
        {
            m_vGsmBandPwl.clear();
            m_bTestPwrLevel = true;
            m_bTestPVT = true;
            m_bTestSpectrumSwitch = true;
            m_bTestSpectrumModulation = true;
            m_bTestFrqErr = true;
            m_bTestEvm = true;
            m_bTestPhaseError = true;
            m_bTestPhaseErrorPeak = true;
            m_bTestRxLevel = true;
            m_bTestBler = true;
        }
    };

    struct S_WLAN_CHANNEL_NORM_INFO
    {
        float			m_nPwrLevel;
        float           m_fExportPower;
        float			m_fBer;
        float			m_fPwrLim[VAULE_ARRAY_SIZE];
        float			m_fRxCellPwr;
        float			m_fRxLevelLim[VAULE_ARRAY_SIZE];
        
    };

    class S_WLAN_BAND_NORM_INFO
    {
    public:
        bool            m_bTestPwrLevel;
        bool			m_bTestRxLevel;
        bool			m_bTestBler;  
        std::vector<S_WLAN_CHANNEL_NORM_INFO> mvWlanChannelInfo;

        S_WLAN_BAND_NORM_INFO()
        {
            Init();
        }

        void Init()
        {
            mvWlanChannelInfo.clear();
            m_bTestPwrLevel = true;
            m_bTestRxLevel = true;
            m_bTestBler = true;
        }
    };

    class S_WLAN_NORM_INFO
    {
    public:
        std::string mstrBoardDataFilePath;
        std::wstring mstrWlanBatFilePath;
        std::wstring mstrWlanBatDir;
        S_SYS_INFO m_sTestBandInfo;
        std::map<int,S_WLAN_BAND_NORM_INFO> m_mWlanBandNormInfo;
    };

    class S_GSM_NORM_INFO
    {
    public:
        //std::vector<S_GSM_BAND_NORM_INFO>	m_sTest
        S_SYS_INFO								m_sTestBandInfo;
        //! key band   value 频段测试标准
        std::map<int,S_GSM_BAND_NORM_INFO>		m_mGsmBandNormInfo;
        //! float			m_fPrxLevelLoss;
        //! float			m_fDrxLevelLoss;
        //std::vector<S_GSM_BAND_NORM_INFO>		m_vGsmBandNormInfo;
        //S_GSM_BAND_NORM_INFO					m_vGsmBand;
    };

    class sModeSelecter{
    public:
        sModeSelecter():lte(true)
            ,tds(true)
            ,cdma(true)
            ,wcdma(true)
            ,gsm(true)
            ,gps(true)
            ,edge(true)
            ,wlan(true)
        {

        }
        bool	lte;
        bool	tds;
        bool	cdma;
        bool	wcdma;
        bool	gsm;
        bool	gps;
        bool	edge;
        bool    wlan;
    };

    class S_GPS_NORM_INFO
    {
    public:
        CMW500_PORT		m_ePort;
        float			m_fFreq;
        float			m_fCellPwr;
        float			m_fSnrLim[VAULE_ARRAY_SIZE];
        float			m_floss;
    };
    struct S_NIGPIB_INSTRUMENT_INFO
    {
        BYTE                m_cNIGPIBType;
        DWORD				m_nGpibInterface;
        DWORD				m_nPrimaryAddress;
        DWORD               m_nSecAddress;
    };


    struct S_VISA_INSTRUMENT_INFO
    {
        std::string mstrVisaAddress;
    };

    struct S_TCPIP_INSTRUMENT_INFO
    {
        size_t mnSubInstrument;
        std::string mstrIPAddress;
    };
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    struct  S_MONITOR_INFO
    {
        BYTE                m_cInterfaceType;
        std::wstring        m_strModelName;
        S_NIGPIB_INSTRUMENT_INFO msNIGPIBInstrumentInfo;
        S_VISA_INSTRUMENT_INFO msVISAInstrumentInfo;
        S_TCPIP_INSTRUMENT_INFO msTCPIPInstrumentInfo;
    };

    //     class S_MONITOR_INFO
    //     {
    //     public:
    //         S_MONITOR_INFO():m_strModelName(L"CMW500")
    //             ,m_nGpibInterface(0)
    //             ,m_nPrimaryAddress(20)
    //         {
    // 
    //         }
    //         std::wstring	m_strModelName;
    //         int				m_nGpibInterface;
    //         int				m_nPrimaryAddress;
    //     };

    class S_RF_CONFIG_INFO
    {
    public:
        S_RF_CONFIG_INFO():m_nErrorReTestCount(3)
            ,m_nReTestTimeOut(10)
            ,m_bIsTestRx(true)
            ,m_bIsExportTestInfo(false)
        {

        }
        int					m_nErrorReTestCount;
        int					m_nReTestTimeOut;
        bool				m_bIsTestRx;
        bool				m_bIsExportTestInfo;
        S_MONITOR_INFO		m_sMonitorInfo;
        sModeSelecter		m_sModeSelecter;
        S_LTE_NORM_INFO		m_sLteInfo;
        S_WCDMA_NORM_INFO	m_sWcdmaInfo;
        S_CDMA_NORM_INFO	m_sCdmaInfo;
        S_TDSCDMA_NORM_INFO	m_sTdscdmaInfo;
        S_GSM_NORM_INFO		m_sGsmInfo;
        S_GPS_NORM_INFO		m_sGpsInfo;
        S_WLAN_NORM_INFO    m_sWlanInfo;
    };

    class S_CHANGE_BAND_INFO
    {
    public:
        std::wstring m_strLteBand;
        std::wstring m_strWcdmaBand;
        std::wstring m_strcdmaBand;
        std::wstring m_strTdscdmaBand;
        std::wstring m_strGsmBand;
        std::wstring m_strWlanBand;
    };

    class S_CHANGE_Chanel_INFO
    {
    public:
        S_CHANGE_Chanel_INFO():m_esys(TECHNOLOGY_UNKNOW)
            ,m_strChannel(L"")
            ,m_strPrx(L"1")
            ,m_strDrx(L"1")
            ,m_strLoss(L"")
        {

        }
        COMMUNICATIONSSYS m_esys;
        int				m_nBand;
        std::wstring	m_strChannel;
        std::wstring	m_strPrx;
        std::wstring	m_strDrx;
        std::wstring	m_strLoss;
    };
}