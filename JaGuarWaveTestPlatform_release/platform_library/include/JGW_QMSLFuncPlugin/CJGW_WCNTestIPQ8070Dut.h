#pragma once
#include "DutTxDetailSettings2.h"
#include "QMSL_inc/QLib.h"
#include "QMSL_inc/QLib_Defines.h"
#include "CCJGW_WCNTestDefine.h"
#include <string>
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>

namespace JGW
{
    class CCJGW_WCNTestBoxInterface;
    class JGWQMSLFUNCPLUGIN_API CCJGW_WCNTestIPQ8070Dut
    {
    public:
        CCJGW_WCNTestIPQ8070Dut(void);
        ~CCJGW_WCNTestIPQ8070Dut(void);
    public:
        void SetWCNTestBoxInstance(CCJGW_WCNTestBoxInterface* pWCNTestBoxInterface);

        bool ConnectDut(QLIB_TargetType_Enum type = QLIB_TARGET_TYPE_APQ,int WlanID = 8070,const std::string& strComPort = "COM30001", bool useQPST = true);
        //! logOn: Enable log if logOn = true, Disable log if logOn = false
        //! fileName: C:\\log\\dut_log.txt
        bool DutLog(bool logOn, const std::string& fileName = "C:\\log\\dut_log.txt");
        ///
        //static bool QmslMsgCB(unsigned int type,char *strMsg);
        //! ssid: Device ID: 0x3c:QC98xx; 0x3d:AR6004; 0x3e:AR6174
        //! eepFName D:\BC\Desktop\Bin\bdwlan290.bin
        bool WlanLoadDut(const std::string& eepFName, int ssid = 0x40, const std::string& DevDLLName = "ipq8070", const std::string& refDesign = "ipq8070", NVMEM_OPTION iNVMemOption = DataFile);
        //! 
        bool WlanSetInstance(WLAN_INSTANCE instance = WLAN0);
        //!
        bool WlanSetDutTxMacAddressBssId(const std::string& rxStation = "00.03.7F.44.55.68",const std::string& txStation = "00.03.7F.44.55.69",const std::string& bssId = "00.03.7F.44.55.70",unsigned int deviceIndex = 0);

        bool WlanSetLocalMac(const std::string& macAddr1 = "000000000000", 
            const std::string& macAddr2 = "000000000000", 
            const std::string& macAddr3 = "000000000000", 
            const std::string& macAddr4 = "000000000000", 
            const std::string& macAddr5 = "000000000000", 
            const std::string& macAddr6 = "000000000000", 
            const std::string& macAddr7 = "000000000000", 
            const std::string& macAddr8 = "000000000000", 
            const std::string& macAddr9 = "000000000000");

        bool SetUpDutTxDetails2(double* txPower,
            WLAN_UniversalPhyID phyId = PHY_A0, //! phyId
            double channel = 5500, //! Center freqMHz or channel number of tx output
            double channel2 = 0, //! Center freqMHz for non-contiguous 80+80Mhz only
            WLAN_Gen6_ChannelBondingState cbState = primary20, //! Channel Bonding State/Primay Channel
            WLAN_UniversalDataRate rate = RATE_MCS_0, //! Data Rate
            WLAN_UniversalRateBW rateBw = RateBW_11AC_VHT20, //! Data rate bandwidth
            WLAN_UniversalNSS nss = _1Stream, //! Number of Spatial Stream
            TxPowerMode pwrMode = TxPowerForce_CLPC, //! Power Mode
            WLAN_TX_Mode txMode = ContTx99, //! Tx Mode
            WLAN_CHAIN_MASK2 chainMask = WLAN_CHAIN_1, //! Tx ChainMask for Phy A0, A1 or B to turn on
            WLAN_CHAIN_MASK2 chainMaskH = WLAN_CHAIN_NONE, //! Tx ChainMask for Phy A0 to turn on for both A0 and A1
            double powerLevel = 0, //! Tx power in dBm
            unsigned int gain = 0, //! Gain (RF gain for WCN) for open loop power control mode
            int digitalGain = 0, //! Digital gain for WCN for open loop power control mode
            unsigned int PAcfg = 5, //! PA configuration
            unsigned int payLoadSize = 1500, //! payload size
            bool ldpcRate = false, //! True if this is an 11n or 11ac LDPC rate
            bool stbcMode = false, //! True if this is a STBC mode
            unsigned int aggregation = 1, //! aggregation
            DPDSetting dpdMode = DPD_Disabled, //! DPD mode
            HCSetting HC_Enable = HCS_Disabled, //! Enables Heavy Clipping for more than 2 streams
            int ifs = 1, //! interframe space, currently not in use
            unsigned int dutyCycle = 10, //! 0 ~ 100%, dutyCycle percentage
            TxDataPattern nPattern = ZEROES_PATTERN, //! Used when nPattern >=6, user defined data pattern with length of nPattern, currently not in use
            char* pattern = "",  //! data frame data pattern enum. if >=6, data frame will be user defined in pattern with length of nPattern
            unsigned int nPacket = 0, //! Number of Packets, 0 = continuous
            bool broadcastMode = false, //! Broadcast/unicast
            unsigned int AIFSN = 0, //! interframe spacing value"
            int dutTxSettlingDelayMs = 500, //! Delay in ms after set power for DUT for tx settling time
            bool scramblerDisable = false, //! Enable/Disable Scrambler
            bool ackRequest = false,//! Request ACK from RX
            WLAN_UniversalGuardInterval guardInterval = GI_0, //! Guard Interval
            PPDU_TYPE ppduType = SingleUser, //! The WLAN signal PLCP Protocol Data Unit (PPDU) type
            bool dualCarrierModulation = false);

        bool GetTxReport2(WLAN_UniversalPhyID phyId/* = PHY_A0*/, 
            WLAN_CHAIN_MASK2 chainMask /*= WLAN_CHAIN_1*/, 
            WLAN_CHAIN_MASK2 chainMaskH/* = WLAN_CHAIN_NONE*/, 
            uint& TxRGdPkt, 
            uint& TxRput, 
            uint& TxRTemp0, 
            uint& TxRTemp1, 
            double& rssi, 
            uint& TxRGainIdx, 
            uint& TxRDacGain, 
            uint& TxRPAcfg, 
            uint& TxRpdadc);

        bool WlanXtalCalInit2(WLAN_UniversalPhyID phyId = PHY_A0, int dXtalTargetPPM = 0, uint dXtalTolerancePPM = 1, WLAN_XTAL_STORAGE_OTP dXtalCalOtpSaveOption = XTAL_OTP_DISABLE, uint settleTime = 1000, uint delayTime = 2000);

        bool WlanXtalCalibration2(uint& xtalCapIn,  uint& xtalCapOut,  double& freqErrPPM,WLAN_UniversalPhyID phyId = PHY_A0, uint channel = 5500, WLAN_SINGLE_CHAIN_MASK txChainMask = WLAN_SINGLE_CHAIN_1, uint gainIndex = 28, int dacGain = 0, int paCfg = 5, uint numAverages = 3,bool ppmCurveDetection = false);

        bool WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId = PHY_A0, WLAN_TpcCalScheme calScheme = ALL_POINTS, uint totalNumOfChains = 4, uint multiChainMask = 1, uint numAverages = 1, bool VsaOnePort = false, double rangeDb1ptCalUL = 3.0, double rangeDb1ptCalLL = -3.0);

        bool WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, WLAN_SYS_BAND band = WLAN_SYS_5G, WLAN_Action PromiscuousMode = PromiscuousMode, const std::string& rxMacAddress = "010000C0FFEE", const std::string& bssId = "020000C0FFEE");

        bool WlanNoiseFloorCal2(double NFCAL_Values[8],WLAN_UniversalPhyID PhyId = PHY_A0, WLAN_CHAIN_MASK2 chainMask = WLAN_CHAIN_1234, WLAN_CHAIN_MASK2 chainMaskH = WLAN_CHAIN_NONE, unsigned short channel = 5745,SYNTH_MODE Mode = Regular, const std::string& pathName = "c:\\Log\\", const std::string& fileName = "NF.txt");

        bool WlanNoiseFloorCalGenBDF(const std::string& strChannel = "5180,5320,5500,5745",WLAN_UniversalPhyID PhyId= PHY_A0, WLAN_CHAIN_MASK2 chainMask = WLAN_CHAIN_1234, WLAN_CHAIN_MASK2 chainMaskH = WLAN_CHAIN_NONE, bool updateNfToBdf = false);
        //! 
        bool BoardDataGetSize(std::string& BoardDataSize);
        //! 
        bool BoardDataRead(uint boardDataSize,const std::string& filePath);
        //!
        bool WlanSaveBDF2(BDF_OPTION saveOption = BDF_DataFlash, const std::string& bdfFileName = "bdf.bin");

        bool DisconnectWlanTester();
    private:
        std::string tlv2GetRspParam(const std::string& key);

        uint GetTgtPwr2(uint freq, uint rate, uint rateBw, WLAN_UniversalPhyID phyId);

        bool tlv2AddRspParam(const std::string& key,char data[]);

        bool tlv2AddRspParam(const std::string& key,const std::string& data);

        bool tlvAddRspParam(const std::string& key, const std::string& data);

        bool tlvAddRspParam(const std::string& key, char data[]);

        std::string tlvGetRspParam(const std::string& key);

        void SaveTxSettings2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState,WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool ackRequest, 
            WLAN_UniversalGuardInterval guardInterval);

        void SetUpTxDetail2(uint wifiStandard, uint phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint chainMask, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, uint nPacket, bool broadcastMode, uint AIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, uint ppduType, bool dualCarrierModulation, double* txPower);

        void setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval);

        void SetUpTxDefault2(uint wifiStandard, uint phyId, uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, WLAN_TX_Mode txMode, uint txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval);

        void TurnOffTX2(uint phyId, uint channel, uint txChain);

        int getChanFreqMhz(uint chanID);

        void GetTxReport(int txChain, uint& TxRGdPkt, uint& TxRTemp, uint& TxRput, double& rssiDbm, uint& TxRGainIdx, uint& TxRDacGain, uint& TxRPAcfg, uint& TxRpdadc);

        void SetPhyId(WLAN_UniversalPhyID newPhyId);

        void Xtal_Cal_Pre();

        int GetSISOChainNumber(WLAN_SINGLE_CHAIN_MASK mask);

        bool Xtal_Cal_Post(bool bDescending = true);

        void setRFSwitch(WLAN_CHAIN_MASK2 chainmask);

        void NfCal2(WLAN_UniversalPhyID phyId, uint chainMask, unsigned short freq, uint nfcal_values[8]);

        void bdRead(uint bdsize, uint offset, uint size, char data[]);

        bool ConnectToServerAutoDetect(uint comPort, uint cOMPortAutoDetectTimeout);
        //bool XtalCalibration2(uint& xtalCapIn, uint& xtalCapOut,  double& freqErrPPM,uint channel, WLAN_SINGLE_CHAIN_MASK txChainMask, uint gainIndex, int dacGain, int paCfg, uint numAverages, bool ppmCD);
    public:
        static POWER_MEAS_TYPE m_pwrMeasType;
        static WLAN_CAL_SCHEME mCalScheme;
        static uint m_rate;
        static uint m_txChainMask;
        static int m_fc_index;
        static _FREQ_CAP mFreqCap[16];
    private:
        bool mbInitialized;

        HANDLE m_hResourceContext;


        uint mXtalTolerancePPM;
        int mXtalTargetPPM;


        char mszRxStationAddr[7];
        char mszTxStationAddr[7];
        char mszTxBSSID[7];
    };
}


