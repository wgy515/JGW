#pragma once
#include "JGW_Device_WCNTester_CommonSCPIWLAN_Define.h"
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestBoxInterface.h>
#include <JGW_NIGPIBControllImpl/EquipCommtl_TcpClient.h>
#include <map>
#include "JGW_IWlanInstrumentInterface.h"
#define scpiConcatenationToken ";"
namespace JGW
{
    class JGW_DEVICE_WCNTESTER_COMMONSCPIWLAN_CLASS WCNTester_CommonSCPIWLAN : public CJGW_IWlanInstrumentInterface
    {
    public:
        WCNTester_CommonSCPIWLAN(void);
        ~WCNTester_CommonSCPIWLAN(void);
    public:
        //!
        //! bool CMW_GPS_ResetMode();
        //! 
        bool CMW_GPS_InitBSE( int nTxNumb = ERFAC,float fCellPower = -130,double dFreqDL = 1575.42);
        //！
        bool CheckGPRFGenStateIsOn();
        //! 
        bool CMW_GPS_FinalizeBSE();
        //! 
		virtual void SwitchLossPath(bool tx,size_t chainMask);
        //! connect wlan instrument
        virtual bool ConnectWlanInstrument(PS_MONITOR_INFO psMonitorInfo);
        //!
        virtual void DisConnectWlanInstrument();
        //!
        virtual bool InitWlanInstrument(const std::wstring& strTRxChainPort,std::vector<S_STATION_CAL_PATH>& vstxStationCalPath,std::vector<S_STATION_CAL_PATH>& vsRxStationCalPath);
        //!
        virtual bool MeasureCWFreqErr(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz);
        //!
        virtual bool MeasureCWFreqErrPower(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz,double& cwPower);
        //!
        virtual double GetAvgPower(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz,double powerLevelDBm,int iTriggerLevel,int vsaAverages = 1);
        
		virtual double GetCurrentConfigAvgPower();
		//! TX
        virtual void MeasureEVM(uint dutPortMask,WLAN_RATE wlan_rate,uint chFreqMHz,double powerLevelDBm,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,bool bTestEvm,bool bTestMask,int avgTestCount = 3);
        virtual void GetEVMTxQuality(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg);
        virtual void GetMeasureMask(bool& maskPass,double& marginsViolationPct/*,double& avgTxPowerDbm,std::vector<double>& marginsDbr,std::vector<double>& marginsFreqHz,bool wlanRemoveInBandSegments = true, bool returnActivePorts = false*/);
        //! RX
        bool GetWaveformParameters(uint rate,int& streamCount,std::string& strWifiRate);

        virtual void InitRxCalibration(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,uint numPackets);

        virtual void InitVsgModulation(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,bool LDPC, bool STBC);

        virtual void SetVsgFreqLevel(uint dutPortMask, uint chFreqMHz, double signalLeveldBm);

        virtual void SetVsgRfEnable(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous);

        virtual void SetVsgModulationMode(uint dutPortMask, bool LDPC, bool STBC);

        virtual bool CheckVSGSentDone(uint timeMaxSec = 10,uint intervalMsec = 250);

    public:
        /***************************QCA639x****************************************/
        virtual void SetDutIndex(int nDutIndex);
        //!
        virtual void TxWlanMeasurementConfiguration(int dutPortMask,int channel,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,int nBandWidth = 80,int mcs = 11,int nss = 1,float gint = 0.8,int ltft = 4,const std::string& link = "DOWN",const std::string& strPPDU = "SU",const std::string& strFEC = "LDPC",int dcm = 0,int nRefPower = 20,int trigLev = -20,int averCount = 3,const std::string& strWlanStand = "WIFI_11AX",bool modulationEnabled = true, bool spectralPowerEnabled = true, bool spectralMaskEnabled = true,bool errorFrequencyEnabled = false);

        void SetVsaPortMask2(uint dutPortMask);
        //virtual void ConvertDutToScpiChannelWaveformParameters();
        //!
        virtual void GetEVMTxQuality2(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg);
        //!
        virtual void GetMeasureMask2(bool& maskPass,double& marginsViolationPct);
        //! 
        virtual void RxWlanMeasurementConfiguration(int dutPortMask,int channel,int nBandWidth = 80,int mcs = 11,const std::string& strWlanStand = "WIFI_11AX",int nss = 1,float signalLeveldBm = -55.00f,int numPktCount = 100,int ltft = 4,float gint = 0.8,const std::string& link = "UP",const std::string& strPPDU = "SU",const std::string& strFEC = "LDPC",int dcm = 0,int stbc = 0);
        virtual void StopRx(int dutPortMask);
    public:
        bool ConnectWCNTestBox(const std::string& host,uint port,const std::string& source);
        /// portMask 15 则设置4个端口 CPE项目默认4X4 15 1111(二进制) values 默认使用端口3 3 3 3
        bool InitWCNVSATestBox(int values[],uint portMask = 15);
        //! 设置仪器线损
        bool SetTRxLoss(bool dutTxTable,std::vector<S_STATION_CAL_PATH>& vsStationCalPath/*,const std::string& trxStationCalPathName = "BH0_LP"*/,int dutIndex = 1);
        //! 
        //bool SetRxLoss(const std::map<std::string,std::vector<S_LOSS_Data>>& mapLossDataList);
        //! 
        void DisConnectWCNTestBox();
    public:
        virtual void getCWFreqErrPower(double chFreqMHz, double powerLevelDBm,double& cwFreqErrorHz,double& cwPower, uint dutPortMask  = 1);

        virtual double getCWFreqErr(double chFreqMHz, double powerLevelDBm, uint dutPortMask  = 1 );

        virtual bool GetPower2(double chFreqMHz, uint chainMask, WLAN_DataRate rate, double* powerLevelDBm, uint numAvg, double* oPower);

        virtual uint sigGenTransmit(int inputSignalStrength_dBm, uint freq, WLAN_DataRate rate, uint numPackets, uint chain);
        //! 
        virtual bool MeasurePower(WLAN_RATE rate, double channelFreqMhz, std::vector<double>& vPowerLevelDbm, ChainMask txChainMask, TriggerType triggerType, double* pathLossDb, std::vector<double>& vsaTriggerLevelOffsetDbr, bool shortGuardInterval, double* powerAvgEachBurst, double* powerPkEachBurst, int vsaAverages = 1); 

        virtual void mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2  = 0u);

    private:
        bool ClearErrorQueue();
        bool CheckForListModeError();
        //! 初始化WLAN SYSTEM
        void WlanSystemPreset(long timeMaxSec = 120L);
        //! 发送命令下去并且等待命令下发后是否返回成功
        bool BlockReturnUntilOperationCompleted(const std::string& command,uint timeMaxSec = 60,uint intervalMsec = 250);
        //! 信号接收 仪器RX
        void SetVsaPortMask(uint dutPortMask);
        //! TX => 仪器TX
        virtual void SetVsgPortMask(uint dutPortMask);
        //!
        void CreatePathLossCorrectionTable(const std::string& correctionTableName,uint frequencyCount, uint* frequencyMHz, double* pathLossDb);
        //!
        void SetDutTxPathLossCorrectionTable(std::vector<std::string>& correctionTableName, std::vector<int>& dutIndex,std::vector<int>& rfPortIndex);
        //! 
        void SetDutRxPathLossCorrectionTable(std::vector<std::string>& correctionTableName, std::vector<int>& dutIndex,std::vector<int>& rfPortIndex);
        //! 
        double GetLossToLossData(uint freq,std::vector<S_LOSS_Data>& sLossData);
        //! 
        bool QueryResultWlanActiveMask(const std::string& query,std::vector<double>& vResult);
        //!
        bool QueryResultWlanActiveMask(const std::string& query,std::vector<double>& vResult,int& dutPortMask);
        //! 
        void AppendActiveMaskParameters(std::string& commands, uint portMask, int value);
        //! 
        void AppendWlanTestMethodologyConfiguration(uint portMask, bool modulationEnabled, bool spectralPowerEnabled, bool spectralMaskEnabled, char* errorFrequencyEnabled/*, params string[] maskLimitTableNames*/);
        //!
        void ConfigureVsaWlanSingleUserMeasurement(/*StringBuilder commands, StringBuilder additionalCommands, */
            uint dutPortMask,
            std::vector<WLAN_RATE>& vWlanRate,  //! WLAN_RATE[] rate, 
            std::vector<uint>& vPortBonding, 
            std::vector<__int64>& vVhannelFreqHz,
            std::vector<double>& vPowerLevelDbm,
            std::vector<double>& vVsaTriggerLevelOffsetDbr, 
            std::vector<uint>& vVsaAverages, 
            std::vector<WLAN_P_TRANSMIT_POWER_CLASS>& vPowerClass11p, 
            std::vector<std::string>& vWlanType);

        bool CheckMaskVsStreamCountError(uint dutPortMask, WLAN_RATE rate,std::string& strWifiRate);

        void AppendBitMaskWaveformParameters(std::string& commands, uint portMask, const std::vector<WLAN_RATE>& vWlanRate);
        //!
        int MaskActiveCount(uint portMask, size_t& totalPortCount);
        //! 
        int MaskActiveCount(uint portMask);
        //!
        std::string GetWaveformParameters(WLAN_RATE rate, int& streamCount);
        //!
        void AppendActiveMaskParameters(std::string& commands, uint portMask, const std::vector<uint>& values);
        //! 
        void AppendActiveMaskParameters(std::string& commands, uint portMask, const std::vector<double>& values);
        //!
        void AppendBitMaskStateMask(std::string& commands, uint bitMask, uint statePortMask);
        //!
        void AppendBitMaskParameters(std::string& commands, uint portMask, const std::string& value);
        //!
        bool InitiateFetchVsaData();
        //!
        void GetWlanPowerAvg(double* avgTxPowerDbm, std::vector<double>& avgTxPowerMinDbm, std::vector<double>& avgTxPowerMaxDbm, std::vector<std::string>& wlanType);
        //!
        void GetWlanPowerPeak(double* pkTxPowerDbm, std::vector<double>& pkTxPowerMinDbm, std::vector<double>& pkTxPowerMaxDbm, std::vector<std::string>& wlanType);
        //!
        void SetVsgModulationPortMask(uint dutPortMask, WLAN_RATE rate, std::vector<uint>& portBonding);
        //!
        void VsgEnableRfPortMask(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous);
        //!
        void VsgSetPortMask(uint dutPortMask, double frequencyMHz, double signalLeveldBm);
    private:
        IEquipCommTL* mpEqipCommtl;
        bool mbIsRSCWM;
        int mnnDutIndex;
    };
}


