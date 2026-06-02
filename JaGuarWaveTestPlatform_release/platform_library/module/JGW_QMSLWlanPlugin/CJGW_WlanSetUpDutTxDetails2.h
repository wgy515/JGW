#pragma once
#include <vector>
#include <map>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
typedef unsigned int uint;
namespace JGW
{
    class CCJGW_WlanSetUpDutTxDetails2
    {
    public:
        CCJGW_WlanSetUpDutTxDetails2(void);
        ~CCJGW_WlanSetUpDutTxDetails2(void);
    public:
        void SetResourceContent(HANDLE hResourceContent);
        bool SetUpDutTxDetails2ToParam(std::map<std::string,std::string>& mapParam,double* txPower);
        void TurnOffTX2ToParam(std::map<std::string,std::string>& mapParam);

        bool TurnOnRX2ToParam(std::map<std::string,std::string>& mapParam);
        bool GetRxResultToParam(std::map<std::string,std::string>& mapParam,uint* totalPktCount, uint* totalMacPktCount, uint* crcErrPktCount,  uint* secErrPktCount,  double* rssiDbm);
    private:
        bool SetUpDutTxDetails2(WLAN_UniversalPhyID phyId, 
            double channel, double channel2, 
            WLAN_Gen6_ChannelBondingState cbState, 
            WLAN_UniversalDataRate rate, 
            WLAN_UniversalRateBW rateBw, 
            WLAN_UniversalNSS nss, 
            TxPowerMode pwrMode, 
            WLAN_TX_Mode txMode, 
            WLAN_CHAIN_MASK2 chainMask, 
            WLAN_CHAIN_MASK2 chainMaskH, 
            double powerLevel, uint gain, 
            int digitalGain, uint PAcfg, 
            uint payLoadSize, bool ldpcRate, 
            bool stbcMode, uint aggregation, 
            DPDSetting dpdMode, 
            HCSetting HC_Enable, 
            int ifs, uint dutyCycle, 
            TxDataPattern nPattern, std::vector<unsigned char> pattern, 
            uint nPacket, 
            bool broadcastMode, 
            uint AIFSN, 
            int dutTxSettlingDelayMs, 
            bool scramblerDisable, 
            bool ackRequest, 
            WLAN_UniversalGuardInterval guardInterval, 
            PPDU_TYPE ppduType, 
            bool dualCarrierModulation, 
            PHY165Mode phy165Mode, 
            const std::string& puncBwMask,  
            double* txPower);
        bool TurnOnRX2(WLAN_UniversalPhyID phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, AntennaSetting antenna, bool LDPC, bool STBC,  WLAN_Action rxFilterMode, const std::string& rxMacAddress, WLAN_UniversalGuardInterval vsgGuardInterval,PPDU_TYPE ppduType, bool dualCarrierModulation = false, bool enANI = false, PHY165Mode phy165Mode = Mode_0, AniMode aniMode = Static, int aniDynaLevel = 0, const std::string& puncBwMask = "0x00");
    private:
        uint GetTgtPwr2(uint phyId, uint freq, uint rate, uint rateBw, uint nss, uint ppduType);
        void TurnOffTX2(uint phyId, uint channel, uint txChain);
        int getChanFreqMhz(uint chanID);
        bool setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, std::vector<unsigned char> pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval);

         void GetRxReport(int rxChain, uint* totalPktCount, uint* totalMacPktCount, uint* crcErrPktCount,  uint* secErrPktCount,  double* rssiDbm);
    private:
        bool tlvAddRspParam(const std::string& strKey,const std::string& strData);
        bool tlvAddRspParam(const std::string& strKey,size_t nData);
        std::string tlvGetRspParam(const std::string& key);

        bool tlv2AddRspParam(const std::string& strKey,const std::string& strData);
        bool tlv2AddRspParam(const std::string& strKey,size_t nData);
        bool tlv2AddRspParam(const std::string& strKey,int nData);
        std::string tlv2GetRspParam(const std::string& key);
    private:
        HANDLE mhResourceContent;
        std::string mstrErrorMsg;
        std::string m_override_rxStation_addr;
        std::string m_override_txStation_addr;
        std::string m_override_txBSSID;
    };

}

