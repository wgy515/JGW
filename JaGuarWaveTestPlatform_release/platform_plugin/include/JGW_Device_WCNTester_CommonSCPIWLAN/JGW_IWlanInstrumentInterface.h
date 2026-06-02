#pragma once
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#define INVAILD_WLAN_RESULT -999.99
typedef unsigned long DWORD;
namespace JGW
{
    enum E_GPIB_CONNECT_TYPE
    {
        NI_GPIB_TYPE = 0,
        AGILENT_GPIB_TYPE
    };

    enum E_INSTRUMENT_CONNECT_TYPE
    {
        E_GPIB_HIVE,//! GPIB连接 又分为NI和Agen
        E_TCP_IP_HIVE, //! TCP IP连接
        E_VISA_HIVE, //! VISA
        E_SERIAL_HIVE //! Serial连接
    };

    struct S_NIGPIB_INSTRUMENT_INFO
    {
        E_GPIB_CONNECT_TYPE  m_cNIGPIBType;
        DWORD m_nGpibInterface;
        DWORD m_nPrimaryAddress;
        DWORD m_nSecAddress;
    };

    struct S_VISA_INSTRUMENT_INFO
    {
        std::string mstrVisaAddress;
    };

    struct S_TCPIP_INSTRUMENT_INFO
    {
        size_t mnSubInstrument;
        size_t mnSocketPort;
        std::string mstrSourceAddress;
        std::string mstrIPAddress;
    };
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    typedef struct 
    {
        E_INSTRUMENT_CONNECT_TYPE m_cInterfaceType;
        std::wstring m_strModelName;
        S_NIGPIB_INSTRUMENT_INFO msNIGPIBInstrumentInfo;
        S_VISA_INSTRUMENT_INFO msVISAInstrumentInfo;
        S_TCPIP_INSTRUMENT_INFO msTCPIPInstrumentInfo;
    }S_MONITOR_INFO,*PS_MONITOR_INFO;

    typedef struct  
    {
        uint mnChannel;
        float mfLoss;
        float mfDelta;
    }S_LOSS_Data;

    typedef struct
    {
        std::string mstrPathName;
        std::vector<S_LOSS_Data> mvsLossData;
    }S_STATION_CAL_PATH;

    enum CMW500_PORT
    {
        ERFAC = 1,	//RFAC
        ERFAO,		//RFAO
        ERFBC,		//RFBC
        ERF_UNKNOW
    };

    class CJGW_IWlanInstrumentInterface
    {
    public:
        //! GPS
        virtual bool CMW_GPS_InitBSE( int nTxNumb = ERFAC,float fCellPower = -130,double dFreqDL = 1575.42) = 0;
        //! 
        virtual bool CMW_GPS_FinalizeBSE() = 0;

		virtual void SwitchLossPath(bool tx,size_t chainMask) = 0;
        //! connect wlan instrument
        virtual bool ConnectWlanInstrument(PS_MONITOR_INFO psMonitorInfo) = 0;
        //!
        virtual void DisConnectWlanInstrument() = 0;
        //!
        virtual bool InitWlanInstrument(const std::wstring& strTRxChainPort,std::vector<S_STATION_CAL_PATH>& vstxStationCalPath,std::vector<S_STATION_CAL_PATH>& vsRxStationCalPath) = 0;
        //!
        virtual bool MeasureCWFreqErrPower(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz,double& cwPower) = 0;
        //!
        virtual bool MeasureCWFreqErr(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz) = 0;
        //!
        virtual double GetAvgPower(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz,double powerLevelDBm,int iTriggerLevel,int vsaAverages = 1) = 0;
		virtual double GetCurrentConfigAvgPower() = 0;
        //! 
        virtual void MeasureEVM(uint dutPortMask,WLAN_RATE wlan_rate,uint chFreqMHz,double powerLevelDBm,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,bool bTestEvm,bool bTestMask,int avgTestCount = 3) = 0;
        //! 
        virtual void GetEVMTxQuality(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg) = 0;
        //!
        virtual void GetMeasureMask(bool& maskPass,double& marginsViolationPct /*,double& avgTxPowerDbm,std::vector<double>& marginsDbr,std::vector<double>&marginsFreqHz,bool wlanRemoveInBandSegments = true, bool returnActivePorts = false*/) = 0;
        //! 
        virtual void GetEVMTxQuality2(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg) = 0;
        //! 
        virtual void GetMeasureMask2(bool& maskPass,double& marginsViolationPct) = 0;

        //! RX 
        //!
        virtual void InitRxCalibration(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,uint numPackets) = 0;
        //!
        virtual void InitVsgModulation(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,bool LDPC, bool STBC) = 0;
        //！=> 默认参数
        virtual void SetVsgFreqLevel(uint dutPortMask, uint chFreqMHz, double signalLeveldBm) = 0;
        //!
        virtual void SetVsgRfEnable(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous) = 0;
        //! 
        virtual bool CheckVSGSentDone(uint timeMaxSec = 10,uint intervalMsec = 250) = 0;

        //! virtual bool Stop
        //virtual void SetVsgRfEnable(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous) = 0;

        //virtual void SetVsgPortMask(uint dutPortMask) = 0;

        /***************************QCA639x****************************************/
        virtual void SetDutIndex(int nDutIndex) = 0;

        virtual void TxWlanMeasurementConfiguration(int dutPortMask,int channel,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,int nBandWidth = 80,int mcs = 11,int nss = 1,float gint = 0.8,int ltft = 4,const std::string& link = "DOWN",const std::string& strPPDU = "SU",const std::string& strFEC = "LDPC",int dcm = 0,int nRefPower = 20,int trigLev = -20,int averCount = 3,const std::string& strWlanStand = "WIFI_11AX",bool modulationEnabled = true, bool spectralPowerEnabled = true, bool spectralMaskEnabled = true,bool errorFrequencyEnabled = false) = 0;

        virtual void RxWlanMeasurementConfiguration(int dutPortMask,int channel,int nBandWidth = 80,int mcs = 11,const std::string& strWlanStand = "WIFI_11AX",int nss = 1,float signalLeveldBm = -55.00,int numPktCount = 100,int ltft = 4,float gint = 0.8,const std::string& link = "UP",const std::string& strPPDU = "SU",const std::string& strFEC = "LDPC",int dcm = 0,int stbc = 0) = 0;

        virtual void StopRx(int dutPortMask) = 0;
    };

#ifdef JGW_DEVICE_WCNTESTER_COMMONSCPIWLAN_EXPORTS
#define WCNTESTER_COMMONSCPIWLAN_API __declspec(dllexport)
#else
#define WCNTESTER_COMMONSCPIWLAN_API __declspec(dllimport)
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_Device_WCNTester_CommonSCPIWLAN_d.lib")
#			else
#				pragma comment(lib,"JGW_Device_WCNTester_CommonSCPIWLAN.lib")
#			endif
#endif

#ifdef __cplusplus
    extern "C" {
#endif
        //WCNTESTER_COMMONSCPIWLAN_API CJGW_IWlanInstrumentInterface* GetWlanInstrumentInterface();
#ifdef __cplusplus
    }
#endif
}