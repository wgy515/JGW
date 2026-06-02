#pragma once
#include <string>
#define LOSS CJGW_WiFiLossConfig::GetWiFiLossConfig()
#define MAX_WLAN_CHAIN 4
namespace JGW
{
    enum E_WLAN_PATHID
    {
        E_2_4G_PATHID = 0,
        E_5G_PATHID,
        E_6G_PATHID
    };

    typedef struct
    {
        int mnFrequency;
        float mfLoss;
        ///float mfDelta;
    }S_WLAN_FREQCONFIG,*LPS_WLAN_FREQCONFIG;

    typedef struct
    {
        //! 设备TX端口配置
        int mDutPortVsas[E_6G_PATHID + 1];
        //! 设备RX端口配置
        int mDutPortVsgs[E_6G_PATHID + 1];
        //! 频率对应线损配置
        std::vector<S_WLAN_FREQCONFIG> mvsWlanFreqConfig;
    }S_WLANCHAIN_EQUIP_DATA;

    class CJGW_WiFiLossConfig
    {
    private:
        CJGW_WiFiLossConfig(void);
        ~CJGW_WiFiLossConfig(void);
    public:
        static CJGW_WiFiLossConfig& GetWiFiLossConfig();
    public:
        bool LoadWiFiLossConfig(const std::string& strWiFiLossConfig);
        void GetLossVsaVsgPortToFreq(WLAN_CHAIN_MASK2 chainMask,int chFreqMHz,float& fLoss,int& vsaPort,int& vsgPort);
    private:
        void InitWlanChainEquipData();
    private:
        S_WLANCHAIN_EQUIP_DATA mvsWlanChainEquipData[MAX_WLAN_CHAIN];
    };
}


