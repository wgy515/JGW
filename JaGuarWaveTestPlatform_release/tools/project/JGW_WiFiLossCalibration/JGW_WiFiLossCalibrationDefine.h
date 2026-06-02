#pragma once
#include <string>
#include <vector>

typedef unsigned int uint;

enum WLAN_CHAIN_MASK2
{
    WLAN_CHAIN_NONE = 0,
    WLAN_CHAIN_1 = 1,
    WLAN_CHAIN_2 = 2,
    WLAN_CHAIN_12 = 3,
    WLAN_CHAIN_3 = 4,
    WLAN_CHAIN_13 = 5,
    WLAN_CHAIN_23 = 6,
    WLAN_CHAIN_123 = 7,
    WLAN_CHAIN_4 = 8,
    WLAN_CHAIN_14 = 9,
    WLAN_CHAIN_24 = 10,
    WLAN_CHAIN_124 = 11,
    WLAN_CHAIN_34 = 12,
    WLAN_CHAIN_134 = 13,
    WLAN_CHAIN_234 = 14,
    WLAN_CHAIN_1234 = 15
};

enum WLAN_UniversalDataRate
{
    WLAN_Universal_CW = -1,
    WLAN_Universal_RATE_Long_1Mbps = 0,
    WLAN_Universal_RATE_Long_2Mbps = 1,
    WLAN_Universal_RATE_Long_5_5Mbps = 2,
    WLAN_Universal_RATE_Long_11Mbps = 3,
    WLAN_Universal_RATE_Short_2Mbps = 4,
    WLAN_Universal_RATE_Short_5_5Mbps = 5,
    WLAN_Universal_RATE_Short_11Mbps = 6,
    WLAN_Universal_RATE_6Mbps = 10,
    WLAN_Universal_RATE_9Mbps = 11,
    WLAN_Universal_RATE_12Mbps = 12,
    WLAN_Universal_RATE_18Mbps = 13,
    WLAN_Universal_RATE_24Mbps = 14,
    WLAN_Universal_RATE_36Mbps = 15,
    WLAN_Universal_RATE_48Mbps = 16,
    WLAN_Universal_RATE_54Mbps = 17,
    RATE_MCS_0 = 20,
    RATE_MCS_1 = 21,
    RATE_MCS_2 = 22,
    RATE_MCS_3 = 23,
    RATE_MCS_4 = 24,
    RATE_MCS_5 = 25,
    RATE_MCS_6 = 26,
    RATE_MCS_7 = 27,
    RATE_MCS_8 = 28,
    RATE_MCS_9 = 29,
    RATE_MCS_10 = 30,
    RATE_MCS_11 = 31,
    RATE_MCS_12 = 32,
    RATE_MCS_13 = 33
};

enum WLAN_UniversalRateBW
{
    RateBW_CW = -1,
    RateBW_CCK = 0,
    RateBW_LegacyOFDM = 1,
    RateBW_11N_HT20 = 2,
    RateBW_11N_HT40 = 3,
    RateBW_11AC_VHT20 = 4,
    RateBW_11AC_VHT40 = 5,
    RateBW_11AC_VHT80 = 6,
    RateBW_11AC_VHT80P80 = 7,
    RateBW_11AX_HE20 = 8,
    RateBW_11AX_HE40 = 9,
    RateBW_11AX_HE80 = 10,
    RateBW_11AX_HE80P80 = 11,
    RateBW_11AX_OFDMA_HE20 = 12,
    RateBW_11AX_OFDMA_HE40 = 13,
    RateBW_11AX_OFDMA_HE80 = 14,
    RateBW_11AX_OFDMA_HE80P80 = 15,
    RateBW_11AX_HE160 = 16,
    RateBW_11AX_OFDMA_HE160 = 17
};

typedef struct  
{
    WLAN_UniversalRateBW meUniversalRateBW;
    WLAN_UniversalDataRate meUniversalDataRate;
    WLAN_CHAIN_MASK2 meWlanChain;
    uint mnPosIndex;
    uint mnChannel;
    double mfPower;
    std::string mstrTestProject;
}S_WIFI_TEST_INFO;

typedef struct  
{
    std::string mstrSerialNumber;
    std::vector<S_WIFI_TEST_INFO*> mvsWiFiTestInfo;
}S_SN_WIFI_TEST_INFOS;

