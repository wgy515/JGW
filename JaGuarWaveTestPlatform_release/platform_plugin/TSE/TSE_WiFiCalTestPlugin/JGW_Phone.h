#pragma once
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#include "CJGW_DutTxDetailSettings.h"
#include "CJGW_WlanDutWrapper.h"
#include <map>
#define PHONE GetPhoneInstance()
#define COLLARS_ARRAY_SIZE 2
#define FINE_TUNING_POWER 0.05
//Fine-tuning power
namespace JGW
{
    typedef struct  
    {
        size_t mnChannel;
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        WLAN_CHAIN_MASK2 meRxChain;
        WLAN_RATE meRate;
        WLAN_Action mePromiscuousMode;
        size_t mnChannel2;
        AntennaSetting meAntennaSetting;
        /*float mfStartPower;
        float mfStopPower;*/
        //int mnCoursePktCount;
        //int mnFinePktCount;
        //double mdCoarseStepSize;
        //double mdFineStepSize;
        bool mbLDPC;
        bool mbSTBC;
    }S_RxCalTestParam,*PS_RxCalTestParam;

    typedef struct
    {
        size_t mnChannel;
        WLAN_CHAIN_MASK2 meTxChain;
        //! TX RATE LOOP
        WLAN_Gen6_ChannelBondingState meChannelBondingState;
        WLAN_DataRate meRate;
        WLAN_RATE meWlanRate;
        //! Number of Spatial Stream
        WLAN_UniversalNSS meNss;
        //double mfEvmLL;
        //double mfEvmUL;
        //! common param
        TxPowerMode mePwrMode;
        WLAN_TX_Mode meTxMode;
        double mfPowerLevel;
        uint muGain;
        int mnDigitalGain;
        uint muPASetting;
        uint muPayLoadSize;
        bool mbShort11b_nGuard;
        bool mbLdpcRate;
        bool mbSTBCmode;
        uint muAggregation;
        DPDSetting meDpdMode;
        HCSetting meHC_Enable;
        int mnIfs;
        uint mnDutyCycle;
        TxDataPattern mePattern;
        std::vector<byte> mvPattern;
        bool mbBroadcastMode;
        bool mbackRequest;
        uint muPacket;
        uint muiAIFSN;
        bool mbScramblerDisable;
        size_t mnChannel2;
        size_t muDutTxSettlingDelayMs;
        //! Guard Interval
        WLAN_UniversalGuardInterval meGuardInterval;
    }S_TxTestParam,*PS_TxTestParam;

    enum E_WLAN_TEST_Collars
    {
        E_MIN_Collars = 0,
        E_MAX_Collars,
    };

    typedef struct 
    {
        bool mbAutoDecrement;// 自增自减
        double mfGoal; //! power level
        double mfMeasPower;//! 测量功率
        double mfChangeMeasPower; //! 微调后的功率
        unsigned int mnGain;
        //! int mnChannelFreq;
        int mnTxChainMask;
    }S_XtalCalibration_Result,*LPS_XtalCalibration_Result;


    typedef struct
    {
        //! EVM
        bool mbTestEvm;
        double mvEvm[COLLARS_ARRAY_SIZE];
        //! Amplitude Imbalance Error
        bool mbTestAmplErr;
        double mvAmplErr[COLLARS_ARRAY_SIZE];
        //! Data Rate
        bool mbTestDataRateMB;
        double mvDataRateMB[COLLARS_ARRAY_SIZE];
        //! Symbol Clock Error
        bool mbTestSymbolClockError;
        double mvSymbolClockError[COLLARS_ARRAY_SIZE];
        //! Frequency Error
        bool mbTestFreqErr;
        double mvFreqErr[COLLARS_ARRAY_SIZE];
        //! Phase Imbalance Error
        bool mbTestPhaseErr;
        double mvPhaseErr[COLLARS_ARRAY_SIZE];
        //! LO Leakage 
        bool mbTestLoLeakage;
        double mvLoLeakage[COLLARS_ARRAY_SIZE];
        //! Avg Tx Power Delta
        bool mbTestAvgTxPowerDelta;
        double mvAvgTxPowerDelta[COLLARS_ARRAY_SIZE];
    }S_TX_TestingStandard,*PS_TX_TestingStandard;

    //typedef int (_cdecl *OpenUserDefinedTransportFn)(const char* resourceId);

    //typedef void (_cdecl *CloseUserDefinedFn)(const char* resourceId);
    //static unsigned long (*OpenUserDefinedTransport)(char* pIpPort );
    //static void (*CloseUserDefinedTransport)(char* pIpPort );
    //static unsigned long (*UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long* piResponseSize,   unsigned char* piResponseBytes);
    //static unsigned long (*UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
    //static unsigned long (*UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);

    typedef unsigned long (*OpenUserDefinedTransportFn)(char* pIpPort );
    typedef void (*CloseUserDefinedFn)(char* pIpPort );
    double AveragePowResult(std::vector<double> vdTestValue);
    double GetFineTuningPower(unsigned int iGain,unsigned int iFreq,double dPowerLevel,double avgVal);
    bool asyncRxGainCalDoneCB(unsigned int status);
    int asyncSGCB(int inputSignalStrength_dBm, unsigned int freq, unsigned int rate, unsigned int numPackets, unsigned int chain);
    class CJGW_Phone
    {
    public:
        CJGW_Phone(void);
        ~CJGW_Phone(void);
    public:
        //!
        bool WlanSetPhyRFMode(PhyID phyId, RFMode mode,int wlan_instance);
        //!
        void SetResourceContent(HANDLE hResourceContent);
        //! 
        void SetLibraryMode(QLIB_TargetType_Enum type);
        //! 
        bool LoadUserDefinedTransportLibraryAndConnectDUT(const std::string& resourceId, const std::string& userDefinedDll,bool reloadUserdefineDll);
        //!
        bool DisconnectDut();
        //!
        void StartLogging(const std::string& strFileName);
        //!
        void StopLogging();
        //！
        void FTM_WLAN_Atheros_Qmsl_Msg_Callback(asyncQmslMessageCB cbMsg);
        //! 
        bool FTM_WLAN_Atheros_LoadDUT(const std::string& strDLLname, const std::string& BroadDataFName, int eNvmemOption,size_t ssid);
        //!
        bool FTM_WLAN_Atheros_xtal_CAL_INIT(int iXtalTargetPPM, uint iXtalTolerancePPM, int iXtalCapDelta, WLAN_XTAL_STORAGE_OTP dXtalCalSaveOption, uint delayCapSettleMs = 1000, uint delayPostXtalMs = 2000);
        //!
        void XtalCalibration(size_t channel, WLAN_SINGLE_CHAIN_MASK txChainMask, size_t gainIndex, int dacGain, int paCfg, uint numAverages, size_t& xtalCap, double& freqErrPPM);
        //!
        bool TxCalibrationFWControlled(bool AllChainOn, bool DebugInfo, std::vector<int>& iMaxIteration, size_t numAverages,int nVsaAverages);
        //!
        bool SetUpTxDetail(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, double& txPower);
        //!
        bool TurnOffTX(uint channel, WLAN_CHAIN_MASK2 txChain);
        //! 检查设备是否连接
        bool IsMobileConnected(void);
        //! 
        bool SetMacAddress(unsigned char* aiMAC_Address);
        //!
        bool TxCalMemCommit(byte itemplate, byte iWriteOption, byte iMemSize, byte iCompress, byte iOverwrite, byte* section);
        //! 
        bool RxCalibrationFWControlled(uint radioId, uint chainToCalibrate, uint band);
    public:
        int GetSISOChainNumber(WLAN_SINGLE_CHAIN_MASK mask);
        void GetTgtPwr(size_t freq, size_t rateBitIdx, size_t userMode, PhyID phyId, size_t& tgtPwr);
        //! 
        void setUpTxDetailCommon(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable);
        //! 
        void SaveTxSettings(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN);

        //!
        void SetUpTxDefault(uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate,WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable);


        //!
        bool mapWlanMode(WLAN_DataRate wlanRate, WLAN_Gen6_ChannelBondingState cbState, TCMD_WLAN_MODE& wlanMode);

        //! RX TSET
        bool SetUpRx(const S_RxCalTestParam& msRxCalTestParam);
        //!
        void GetRxReport(int rxChain, uint& totalPktCount,  uint& totalMacPktCount, uint& crcErrPktCount,uint& secErrPktCount,double& rssiDbm);

    public: 
        /************************************************************************/
        /* QCA639x                                                              */
        /************************************************************************/
        bool AutoDetectChip(std::string& strChipInfo);
        //!
        bool SetUpTxDetail2(int nPowerLevel,double& txPower,const S_TxTestParam& sTxTestParam,PhyID ePhyID = PhyID_A,WLAN_UniversalRateBW eRateBw = RateBW_11AX_HE80,WLAN_CHAIN_MASK2 eWlanChainMask = WLAN_CHAIN_1);

        /*[QTMParameter("pwrMode", "TxPowerAuto", " ", " ", "", "Power Mode")]
        [QTMParameter("nPacket", "0", " ", " ", "", "Number of Packets, 0 = continuous")]
        [QTMParameter("AIFSN", "0", " ", " ", "", "interframe spacing value")]
        [QTMParameter("dutTxSettlingDelayMs", "500", " ", " ", "ms", "Delay in ms after set power for DUT for tx settling time")]
        [QTMParameter("scramblerDisable", "False", " ", " ", "", "Enable/Disable Scrambler")]
        [QTMParameter("ackRequest", "False", " ", " ", "", "Request ACK from RX")]
        [QTMParameter("guardInterval", "LTF_Mode0_GI_800", " ", " ", "", "Guard Interval")]
        [QTMParameter("ppduType", "0", " ", " ", "PPDU_TYPE", "The WLAN signal PLCP Protocol Data Unit (PPDU) type", DisplayHint = DisplayHints.ENUM_LIST)]
        [QTMParameter("dualCarrierModulation", "false", " ", " ", "state", "The WLAN signal dual sub-carrier modulation status flag", DisplayHint = DisplayHints.TRUEFALSE)]
        [QTMParameter("phy165Mode", "0", " ", " ", "PHY Mode for 165MHz", "Applicable to 165MHz BW only", DisplayHint = DisplayHints.ENUM_LIST)]
        [QTMParameter("puncBwMask", "0x00", " ", " ", "Bitmask for 16 20MHz BW", "11BE EHT only")]
        [QTMParameter("broadcastMode", "True", " ", " ", "", "Broadcast/unicast")]
        [QTMParameter("phyId", "0", " ", " ", "", "phyId")]
        [QTMParameter("channel", "5180", " ", " ", "", "Center freqMHz or channel number of tx output")]
        [QTMParameter("channel2", "0", " ", " ", "", "Center freqMHz for non-contiguous 80+80Mhz only")]
        [QTMParameter("cbState", "None", " ", " ", "", "Channel Bonding State/Primay Channel")]
        [QTMParameter("rate", "RATE_6Mbps", " ", " ", "", "Data Rate")]
        [QTMParameter("rateBw", "RateBW_CCK", " ", " ", "", "Data rate bandwidth")]
        [QTMParameter("nss", "1", " ", " ", "", "Number of Spatial Stream")]
        [QTMTest]
        [QTMParameter("txMode", "ContTx99", " ", " ", "", "Tx Mode")]
        [QTMParameter("chainMask", "1", " ", " ", "", "Tx ChainMask for Phy A0, A1 or B to turn on")]
        [QTMParameter("chainMaskH", "0", " ", " ", "", "Tx ChainMask for Phy A0 to turn on for both A0 and A1")]
        [QTMParameter("powerLevel", "10", " ", " ", "", "Tx power in dBm")]
        [QTMParameter("gain", "3", " ", " ", "", "Gain (RF gain for WCN) for open loop power control mode")]
        [QTMParameter("digitalGain", "0", " ", " ", "", "Digital gain for WCN for open loop power control mode")]
        [QTMParameter("PAcfg", "0", " ", " ", "", "PA configuration")]
        [QTMParameter("payLoadSize", "1500", " ", " ", "", "payload size")]
        [QTMParameter("ldpcRate", "False", " ", " ", "", "True if this is an 11n or 11ac LDPC rate")]
        [QTMParameter("stbcMode", "False", " ", " ", "", "True if this is a STBC mode")]
        [QTMParameter("aggregation", "1", " ", " ", "", "aggregation")]
        [QTMParameter("dpdMode", "Disabled", " ", " ", "", "DPD mode")]
        [QTMParameter("HC_Enable", "Disabled", " ", " ", "", "Enables Heavy Clipping for more than 2 streams")]
        [QTMParameter("ifs", "1", " ", " ", "", "interframe space, currently not in use")]
        [QTMParameter("dutyCycle", "10", " ", " ", "", "0 ~ 100%, dutyCycle percentage")]
        [QTMParameter("nPattern", "4", " ", " ", "", "data frame data pattern enum. if >=6, data frame will be user defined in pattern with length of nPattern")]
        [QTMParameter("pattern", "0", " ", " ", "", "Used when nPattern >=6, user defined data pattern with length of nPattern, currently not in use")]*/
        bool SetUpDutTxDetails2(WLAN_UniversalPhyID phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, uint nPacket, bool broadcastMode, uint AIFSN, int dutTxSettlingDelayMs, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, PPDU_TYPE ppduType, bool dualCarrierModulation,double& txPower, PHY165Mode phy165Mode = Mode_0, const std::string& puncBwMask = "0x00"/*, out double[] txPower*/);
        //!
        bool SetUpTxDetail2(uint wifiStandard, uint phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint chainMask, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, uint nPacket, bool broadcastMode, uint AIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, uint ppduType, bool dualCarrierModulation, double& txPower);
        //!
        void GetTgtPwr2PerChain(uint phyId, uint freq, uint rate, uint rateBw, uint nss, uint ppduType,  double& tgtPwr);
        //!
        bool setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval);
        //!
        void SetUpTxDefault2(uint wifiStandard, uint phyId, uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, WLAN_TX_Mode txMode, uint txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval);
        //!
        void TurnOffTX2(uint phyId, uint channel, uint txChain);
        //!
        bool ResetRxPacketCounts2(uint wifiStandard, uint phyId,uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState,WLAN_UniversalDataRate rate,WLAN_UniversalRateBW rateBw,uint nss,int rxChain,  uint phy165Mode = 0, int noisefloor = 0, int aniMode = 0, int aniDynaLevel = 0,WLAN_Action rxFilterMode = PromiscuousMode,PPDU_TYPE ppduType = SingleUser,bool dualCarrierModulation = false,bool enANI = false);
        //!
        void SetRxPhyId(PhyID newPhyId);
        //!
        void GetRxRept(int rxChain,uint& totalPktCount,uint& totalMacPktCount,uint& crcErrPktCount,uint& secErrPktCount,double& rssiDbm,int& evms,int& pilotevms,uint& chainMask,uint& bandwidth,uint& num20MHz,uint& numChain,int& rssiPerChain20Mhz,int& tPut);



    private:
        void FreeUserDefinedDll();
        bool tlvAddRspParam(const std::string& strKey,const std::string& strData);
        bool tlvAddRspParam(const std::string& strKey,size_t nData);
        std::string tlvGetRspParam(const std::string& key);


        bool Tlv2AddRspParam(const std::string& strKey,const std::string& strData);
        bool Tlv2AddRspParam(const std::string& strKey,size_t nData);
        std::string tlv2GetRspParam(const std::string& key);
        int getChanFreqMhz(uint chanID);
    private:
        HANDLE mhResourceContent;
        HMODULE mhUserDefinedDll;
        std::string mstrErrorMsg;
        OpenUserDefinedTransportFn mOpenUserDefinedTransport;
        CloseUserDefinedFn mCloseUserDefined;
        userDefinedSend muserDefinedSend;
        userDefinedReceive muserDefinedReceive;
        userDefinedFlushTxRx muserDefinedFlushTxRx;
    public:
        bool mbResult ;
        int mnpuncBwMask;
        int mXtalTolerancePPM;
        int mXtalTargetPPM;
        //! 仪器测试一次的平均值
        int mnVsaAverages;
        size_t mnFrequency;
        size_t m_txChainMask;
        size_t mnWlanRate;
        bool mbVsaSinglePort;
        WLAN_CAL_SCHEME mCalScheme;
        POWER_MEAS_TYPE m_pwrMeasType;
        CCJGW_DutTxDetailSettings txSettings;
        std::string m_override_rxStation_addr;
        std::string m_override_txStation_addr;
        std::string m_override_txBSSID;
        std::string mstrResourceId;
        std::string mstrUserDefinedDllFilePath;
        CCJGW_WlanDutWrapper mWlanDutWrapper;
        std::map<unsigned int,std::vector<S_XtalCalibration_Result>> mmapFreqS_XtalCalibration_Result;
    };
}


