#pragma once
#include <string>

#include "configwnd/RfTestDefine.h"

#include <JGW_Device_RohdeSchwarz_CMW500/JGW_CMW500EquipmentControll.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_QMSLFuncPlugin/SubSysVFS.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_QMSLFuncPlugin/CJGWWiFiQMSL.h>
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>
// #include <JGW_FoundationFunc/ctf_wr_nv_api.h>

//#define GSP_NS_SNR
#define LTE_NS_TEST_
#define WCDMA_NS_TEST_
#define TDSCDMA_NS_TEST_
// #define CDMA_NS_TEST_
#define GSM_NS_TEST_
#define WLAN_NS_TEST_
#define TSE_TEST_

namespace JGW
{
    class CCRFAntTestThread:public CCMessageThread,public CTSE_TestSequenceExec
    {
    public:
        CCRFAntTestThread(void);
        ~CCRFAntTestThread(void);
        //! 获取SN
		std::wstring GetSerialNumber();
        //! 异步消息发送
        void    SetAsyncMessageClassInstance(CCJGW_AsyncWndMessage* pAsyncWndMessage) { mpAsyncWndMessage = pAsyncWndMessage; }
        //! 当前初始化仪器接口与仪器类型不一致返回fase
        bool    CheckMonitorConfigIsSame();
        //!
        void    SetTestSuiteXmlFilePath(const std::wstring& strConfigXmlPath);
    private:
        void    OnInitWnd( WPARAM wParam,LPARAM lParam );
        void    OnRunThread( WPARAM wParam,LPARAM lParam );
        void    OnRunRetest( WPARAM wParam,LPARAM lParam );
    private: //! CTSE_LogListener
        virtual void OnInfoLog(const wchar_t* strInfoLog);

        virtual void OnErrorLog(const wchar_t* strErrorLog);

        virtual void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
    private: //! CTSE_TestStatusListener
        virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
    private:    //! MES
        //! bool GetSerialNumberTest();
        //! MES 检查SN当前状态
        //! bool CheckMESSNStatus();
        //!
        //! bool SerialNumberNextStationMES();
        //!
        bool MESSaveTestRecords(bool bTestStatus);
    private:
        bool	SetFTMModeView(void);
        bool	LTE_NS_TestSequence(void);
        bool	WCDMA_NS_TestSequence(void);
        bool	TDSCDMA_NS_TestSequence(void);
        bool	CDMA_NS_TestSequence(void);
        bool	GSM_NS_TestSequence(void);
        bool	UnSetFTMMode(void);
        bool	GPS_NS_TestSequence(void);
#ifdef GSP_NS_SNR
        //! 设置仪器端口
        bool	GPS_InitMonitor();
        bool	GPS_NS_StartTest();
        bool	GPS_NS_CheckCN(const double pdCNoDBHz);
        bool	GPS_NS_EndCall();
        bool	GPS_NS_StartCall();
#endif

#ifdef LTE_NS_TEST_
        bool	LTE_NS_Test();
        bool	LTE_NS_InitMonitor();
        bool	LTE_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel);
        bool	LTE_NS_StartTest();
        bool	LTE_NS_TxRfNoSignalTest();
        bool	LTE_NS_RxRfNoSignalTest();
        bool	LTE_NS_MeasuresAll();
        bool	LTE_NS_EndCall();

        bool	LTE_NS_CheckMaxPwr( float maxPwr );
#if 0
        bool	LTE_NS_CheckACLR( float ACLR[6] );
        bool	LTE_NS_CheckSEM( int sem );
        bool	LTE_NS_CheckEvm( float evm );
        bool	LTE_NS_CheckFreqError( float freqError );
        bool	LTE_NS_CheckOBW( float OBW );
        bool	LTE_NS_TestMinPwr();
#endif
        bool	LTE_NS_TestFTMBler(int nChain,wchar_t* szMsg);
        bool	LTE_NS_TestRxLevel(int nChain);
        bool	LTE_NS_CheckRxLevel(short iRxLeveldBm10);
#endif

#ifdef TDSCDMA_NS_TEST_
        bool	TDSCDMA_NS_Test();
        bool	TDSCDMA_InitMonitor();
        bool	TDSCDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel);
        bool	TDSCDMA_NS_StartTest();
        bool	TDSCDMA_NS_TxRfNoSignalTest();
        bool	TDSCDMA_NS_RxRfNoSignalTest();

        bool	TDSCDMA_NS_TestRxLevel(int nChain);
        bool	TDSCDMA_NS_CheckRxLevel(short iRxLeveldBm10);

        bool	TDSCDMA_NS_TestMinPwr();
        bool	TDSCDMA_NS_MeasuresAll();
        bool	TDSCDMA_NS_CheckMaxPwr(float fMaxPwr);
#if 0     
        bool	TDSCDMA_NS_CheckACLR(float fACLR[4]);
        bool	TDSCDMA_NS_CheckSEM(float fsem[6]);
        bool	TDSCDMA_NS_CheckOBW(float fOBWValue);
        bool	TDSCDMA_NS_CheckEVM(float fEVM);
        bool	TDSCDMA_NS_CheckFreqError(float dFreqError);
        bool	TDSCDMA_NS_CheckPeakCDE(float fPCDE);
        bool	TDSCDMA_NS_CheckPVTime(int status);
#endif
#endif

#ifdef WCDMA_NS_TEST_
        bool	WCDMA_NS_Test();
        bool	WCDMA_InitMonitor();
        bool	WCDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel);
        bool	WCDMA_NS_StartTest();
        bool	WCDMA_NS_TxRfNoSignalTest();
        bool	WCDMA_NS_RxRfNoSignalTest();

        bool	WCDMA_NS_MeasuresAll();
        bool	WCDMA_NS_CheckMaxPwr( float fChannalPwr );
#if 0
        bool	WCDMA_NS_TestMinPwr();
        bool	WCDMA_NS_CheckACLR(float fACLRValue[4]);
        bool	WCDMA_NS_CheckSEM(float fSEMValue[8]);
        bool	WCDMA_NS_CheckEvm(float fEvmValue);
        bool	WCDMA_NS_CheckFreqError(float fFreqError);
        bool	WCDMA_NS_CheckOBW(float fOBWValue);
        bool	WCDMA_NS_CheckPCDE(float fPCDEValue);
        bool	WCDMA_NS_CheckPhaseError(float fPhaseError);
        bool	WCDMA_NS_CheckIQoffset(float fIQoffset);
        bool	WCDMA_NS_CheckIQBalance(float fIQBalance);
#endif
        bool	WCDMA_NS_TestRxLevel(int nChain);
        bool	WCDMA_NS_CheckRxLevel(short iRxLeveldBm10);
#endif

#ifdef CDMA_NS_TEST_
        bool	CDMA_NS_Test();
        bool	CDMA_InitMonitor();
        bool	CDMA_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel);
        bool	CDMA_NS_StartCall();
        bool	CDMA_NS_HandOver();
        bool	CDMA_NS_StartTest();
        bool	CDMA_NS_TxRfNoSignalTest();
        bool	CDMA_NS_RxRfNoSignalTest();

        bool	CDMA_NS_TestMinPwr();
        bool	CDMA_NS_MeasuresAll();
        bool	CDMA_2000_NS_TestACPR();
        bool	CDMA_2000_NS_CheckMaxPwr( float fChannalPwr );
        bool	CDMA_2000_NS_CheckEvm( float fEvmValue );
        bool	CDMA_2000_NS_CheckFreqError( float fFreqError );
        bool	CDMA_2000_NS_CheckWFQ(float wfq);
        bool	CDMA_2000_NS_TestBER(void);
        bool	CDMA_2000_NS_CheckPhaseError( float fPhaseError );
        bool	CDMA_2000_NS_CheckIQBalance( float fIQBalance );
#endif
        //GSM_TestPower = 0,
        //GSM_TestFreqError,
        //GSM_TestEvm,
        //GSM_TestPhaseErrorRMS,
        //GSM_TestPhaseErrorPeak,
        //GSM_TESTTestPVtime,
        //GSM_TestSpectrumSwitching,
        //GSM_TestSpectrumModulation,
#ifdef GSM_NS_TEST_
        bool	GSM_NS_Test();
        bool	GSM_InitMonitor();
        bool	GSM_NS_SetBandInfo(std::vector<S_BAND_CHANNEL_INFO>::iterator& itChannel);
        bool	GSM_NS_StartCall();
        bool	GSM_NS_HandOver();
        bool	GSM_NS_StartTest();
        bool	GSM_NS_TxRfNoSignalTest();
        bool	GSM_NS_RxRfNoSignalTest();
        bool	GSM_NS_MeasuresAll( int nPowerLevel );
        bool	GSM_NS_MeasuresBer( int nPowerLevel );
        bool	GSM_NS_MeasRxLevel( int nPowerLevel );
        bool	GSM_NS_GetRxLevel(float &fRxLevel,float fRxLevelup,float fRxLevelLow);
        bool	GSM_NS_CheckBer(float fTestValue,float fupLimit,const wchar_t * szBuffer);

        bool	GSM_NS_CheckPower(float fPwr);
#if 0
        bool	GSM_NS_CheckFreqError(float fFreqErr);
        bool	GSM_NS_CheckEvm(float fEvm);
        bool	GSM_NS_CheckPhaseErrorRMS(float fPhaseError);
        bool	GSM_NS_CheckPhaseErrorPeak(float fPhasePeak);
        bool	GSM_NS_CheckPVTime(bool bPvt);
        bool	GSM_NS_CheckSpectrumSwitching(bool bSpmSwitch);
        bool	GSM_NS_CheckSpectrumModulation(bool bSpmModulation);
#endif
#endif
#ifdef WLAN_NS_TEST_
        bool InitWlanModuleView();
        bool InitWlanEquipment();
        bool InitWlanInfoToBand(int band);
        bool LoadWiFiDut();
        void CloseWlanBatProcess();

        bool WLAN_NS_TestSequence();
        bool WLAN_NS_Test();


        bool RunWiFiTest();

        bool WiFiFrequencyTest();

        bool WiFiFrequencyTestToChain(int nTxChain);

        bool WiFiTxTestChain();
        bool WiFiTxTestBandWidth(int nBandWidth );
        bool WLAN_Frequency_Tx_Test();
        bool WLAN_Frequency_Rx_Test( );


        bool InitWiFiTxTestEquipment( );
        bool InitWiFiTxTestDevice( );
        bool CheckWiFiTxTestData( );
        bool DisableWiFiTxTestDevice();

        bool CheckTxPower(float fMinPower,float fMaxPower,float fPower);

        bool CheckWiFiRxRssi(float value,float minValue,float maxValue);

        bool CheckWiFiPER(int txbGoodPackets,int txbTotalPackets,float fPercentValue);

        HINSTANCE           mhWlanBatInstance;
        E_WLAN_MODE         meWlanMode;
        E_WLAN_HT_MODE      meWlanHtMode;
        E_WLAN_TX_DATA_RATE meWlanTxDataRate;
        E_WLAN_RX_DATA_RATE meWlanRxDataRate;
        S_WLAN_BAND_NORM_INFO* mpsWlanBandNormInfo;
        S_WLAN_CHANNEL_NORM_INFO* mpsWlanChannelNormInfo; 
        CMW500_PORT         meWlanPort;
        float               mfWlanLoss;
        int                 mnFrequency;
        int                 mnTxChain;
        CCJGWWiFiQMSL       mWiFiQmsl;
#endif
    private:
        bool	InitGBIPAndCheckMonitor(void);
        bool	ConnectDiag(int nComPort);
        bool	ReConnectDiag(void);//重连机制 防止USB状态不稳定
        bool	SetFTMMode(void);
        bool	EnterFTMMode(void);
        bool	InitSubsysVFS(COMMUNICATIONSSYS sys);
        bool	RecyFtmNVView();

        /*		读取设备SN并检查SN号段		*/
        bool	ReadSNView();
        bool	CheckDbSNView();
        bool	UpdateDeviceSnStationInfoView();

        void SendMsgToList(const wchar_t* szBuffer,bool bInsertItem = false);
        //! void	SendMsgToDisplay(const wchar_t* szBuffer,bool bInsertItem = true);
        //! void	SendErrMsgToDisplay(const wchar_t* szBuffer,bool bInsertItem = true);
        void	DisConnectDevices();

        bool    CheckTestResult(bool bIsTest,const wchar_t* strTestContent,float fMinValue,float fTestValue,float fMaxValue);
        bool    SYS_NS_StartCall(const std::wstring& strSysName);

        bool    SYS_NS_HandOver(const std::wstring& strSysName);
    public:
        LPCTSTR GetErrorMsg(){ return m_strErrorCode.c_str(); }

    private:
        bool                m_bInitEquipment;
        bool				m_bTDSCdmaStartCall;
        bool				m_bGsmStartCall;
        bool				m_bCdmaStartCall;
        bool				m_bWcdmaStartCall;
        bool                m_bExecTSETest;
        bool                m_bNotNextStation;;

        CMW500_PORT			m_eDrxPort;
        CMW500_PORT			m_eTxPort;
        COMMUNICATIONSSYS	m_eSys;

        int					m_nBand;
        int					m_nGsmPowerLevelIndex;

        float		        m_fPTxLoss; //主集发射线损信息
        float		        m_fPRxLoss; //! 主集接收线损
        float		        m_fDRxLoss; //! 分集接收线损

        HANDLE				m_hResourceContext;
		HANDLE              mhWlanBatProcess;

        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        S_RF_CONFIG_INFO*	m_psConfigInfo;
        S_BAND_INFO*		m_lpBandInfo;
        S_GSM_BAND_NORM_INFO* m_lpGsmBandPwlInfo;
        JGW_CMW500EquipmentControll* m_pCMW500EquipmentControll;
        HWND				m_hMainWnd;

        std::wstring        m_strTestContent;
        std::wstring		m_strErrorCode;
        std::wstring		m_strSN;
        std::string         mstrStartTestTime;
        std::string         mstrEndTestTime;
        std::wstring        mstrConfigXmlPath;
        jgw_wr_nv_api		m_cRwnv;
        CSubSysVFS			m_subSysVfs;
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    };
}
