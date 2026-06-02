#pragma once
typedef unsigned int uint;
#include <string>
#define WCN_OP_TX 1
#define WCN_OP_RX 2
#define WCN_OP_RX_STATUS 11
#define INVAILD_WIFI_RESULT 99999

namespace JGW
{

    enum WLAN_TLV_OPCODE
    {
        _OP_TX = 1,
        _OP_RX = 2,
        _OP_CAL = 3,
        _OP_CAL_DONE = 4,
        _OP_TX_STATUS = 10,
        _OP_RX_STATUS = 11,
        _OP_RESET = 15,
        _OP_OTP = 16,
        _OP_CAL_INIT = 23,
        _OP_SETPHYRFMODE = 169,
    };


    enum WLAN_SYS_BAND
    {
        WLAN_SYS_5G,
        WLAN_SYS_2G
    };

    enum WLAN_STANDARD
    {
        WLAN_STANDARD_CW = -1,
        WLAN_STANDARD_RATE_11B_LONG_1_MBPS = 0,
        WLAN_STANDARD_RATE_11B_LONG_2_MBPS = 1,
        WLAN_STANDARD_RATE_11B_LONG_5_5_MBPS = 2,
        WLAN_STANDARD_RATE_11B_LONG_11_MBPS = 3,
        WLAN_STANDARD_RATE_11B_SHORT_2_MBPS = 4,
        WLAN_STANDARD_RATE_11B_SHORT_5_5_MBPS = 5,
        WLAN_STANDARD_RATE_11B_SHORT_11_MBPS = 6,
        WLAN_STANDARD_RATE_11A_6_MBPS = 7,
        WLAN_STANDARD_RATE_11A_9_MBPS = 8,
        WLAN_STANDARD_RATE_11A_12_MBPS = 9,
        WLAN_STANDARD_RATE_11A_18_MBPS = 10,
        WLAN_STANDARD_RATE_11A_24_MBPS = 11,
        WLAN_STANDARD_RATE_11A_36_MBPS = 12,
        WLAN_STANDARD_RATE_11A_48_MBPS = 13,
        WLAN_STANDARD_RATE_11A_54_MBPS = 14,
        WLAN_STANDARD_RATE_11N = 15,
        WLAN_STANDARD_RATE_11P = 16,
        WLAN_STANDARD_RATE_11AC = 17,
        WLAN_STANDARD_RATE_11AC_80P80 = 18,
        WLAN_STANDARD_RATE_11AX = 19,
        WLAN_STANDARD_RATE_11AX_80P80 = 20,
        WLAN_STANDARD_RATE_11AX_OFDMA = 21,
        WLAN_STANDARD_RATE_11AX_80P80_OFDMA = 22,
        WLAN_STANDARD_RATE_11BE = 23,
        WLAN_STANDARD_RATE_11BE_OFDMA = 24,
    };

    // Token: 0x0200000B RID: 11
    enum DiagType
    {
        // Token: 0x040000B0 RID: 176
        QLIB_DIAG_TYPE_STANDARD,
        // Token: 0x040000B1 RID: 177
        QLIB_DIAG_TYPE_LITE,
        // Token: 0x040000B2 RID: 178
        QLIB_DIAG_TYPE_MAX_INVALID
    };

    enum AntennaSetting
    {
        // Token: 0x0400026F RID: 623
        ANTENNA0,
        // Token: 0x04000270 RID: 624
        ANTENNA1
    };

    enum WLAN_Action
    {
        PromiscuousMode = 0,
        FilterMode = 1
    };

    enum SYNTH_MODE
    {
        Regular,
        DTIM
    };

    enum WLAN_XTAL_STORAGE_OTP
    {
        XTAL_OTP_DISABLE = 0,
        XTAL_OTP_ENABLE = 1
    };

    enum BDF_OPTION
    {
        BDF_DataFlash,
        BDF_DataEeprom,
        BDF_DataFile
    };

    enum NVMEM_OPTION
    {
        DataNone,
        DataFlash,
        DataEeprom,
        DataOtp,
        DataDontLoad,
        DataFile,
        DataDram
    };

    enum PhyID
    {
        PhyID_A,
        PhyID_B,
        PhyID_A1
    };

    enum RFMode
    {
        PhyA_only,
        DBS_PhyA5G_PhyB2G,
        SBS,
        PhyB_only,
        DBS_SBS,
        DBS_OR_SBS,
        DBS_PhyA2G_PhyB5G
    };

    enum WLAN_INSTANCE
    {
        WLAN0,
        WLAN1,
        WLAN2
    };

    enum WLAN_UniversalPhyID
    {
        PHY_A0 = 0,
        PHY_B = 1,
        PHY_A1 = 2
    };

    enum LINK_DIRECTION
    {
        Up = 0,
        Down = 1,
        Direct = 2,
    };

    enum WLAN_MODE
    {
        WIFI_11B = 0,
        WIFI_11AG = 1,
        WIFI_11N_HT20 = 2,
        WIFI_11N_HT40Minus = 3,
        WIFI_11N_HT40Plus = 4,
        WIFI_11AC_HT20 = 5,
        WIFI_11AC_HT40Minus = 6,
        WIFI_11AC_HT40Plus = 7,
        WIFI_11AC_HT80_40low_20low = 8,
        WIFI_11AC_HT80_40low_20high = 9,
        WIFI_11AC_HT80_40high_20low = 10,
        WIFI_11AC_HT80_40high_20high = 11,
        WIFI_11P_BW5 = 100,
        WIFI_11P_BW10 = 101,
        WIFI_11P_BW20 = 102
    };

    enum WLAN_Gen6_ChannelBondingState
    {
        WLAN_Gen6_ChannelBondingState_none = 0,
        primaryLow = 1,
        primary20 = 2,
        primaryHigh = 3,
        BW80_20_40Low_40_80Center = 4,
        BW80_20_40Center_40_80Center = 5,
        BW80_20_40High_40_80Center = 6,
        BW80_20_40Low_40_80Low = 7,
        BW80_20_40High_40_80Low = 8,
        BW80_20_40Low_40_80High = 9,
        BW80_20_40GHigh_40_80High = 10,
        BW80p80 = 12,
        BW160 = 13,
        Primary_1st_20_in_BW80p80 = 21,
        Primary_2nd_20_in_BW80p80 = 22,
        Primary_3rd_20_in_BW80p80 = 23,
        Primary_4th_20_in_BW80p80 = 24,
        Primary_5th_20_in_BW80p80 = 25,
        Primary_6th_20_in_BW80p80 = 26,
        Primary_7th_20_in_BW80p80 = 27,
        Primary_8th_20_in_BW80p80 = 28,
        Primary_1st_20_in_BW160 = 29,
        Primary_2nd_20_in_BW160 = 30,
        Primary_3rd_20_in_BW160 = 31,
        Primary_4th_20_in_BW160 = 32,
        Primary_5th_20_in_BW160 = 33,
        Primary_6th_20_in_BW160 = 34,
        Primary_7th_20_in_BW160 = 35,
        Primary_8th_20_in_BW160 = 36,
        Half_Rate = 50,
        Quarter_Rate = 51
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

    enum WLAN_UniversalNSS
    {
        _1Stream = 1,
        _2Streams = 2,
        _3Streams = 3,
        _4Streams = 4,
        _5Streams = 5,
        _6Streams = 6,
        _7Streams = 7,
        _8Streams = 8
    };

    enum TxPowerMode
    {
        ForcedGain,
        OpenLoopSearch,
        TxPowerAuto,
        TxPowerForce_CLPC,
        TxPowerForce_SCPC,
        TxPowerForce_OLPC,
        ForcedDesiredGain,
        ForcedGLUTIndex,
        TxForcedPerChain
    };

    enum WLAN_TX_Mode
    {
        DisablingContinuousMode = 0,
        EnablingContinuousUnmodulatedTX = 1,
        EnablingContinuousModulatedTX = 2,
        ContTx99 = 3,
        ContTx100 = 4,
        EnablingOffsetTone = 5,
        EnablingPSATCal = 6,
        EnablingCWTone = 7,
        EnablingCLPCPacket = 8,
        EnablingSCPC = 9
    };

    enum WLAN_TpcCalScheme
    {
        ALL_POINTS,
        ONE_POINT
    };

    enum POWER_MEAS_TYPE
    {
        POWER_MEAS_AVG,
        POWER_MEAS_PEAK
    };

    enum WLAN_CAL_SCHEME
    {
        CAL_1PBIN = 0,
        CAL_INITGainIndex = 1,
        CAL_OLPC2P = 2,
        CAL_CLPC_PDETI = 3,
        CAL_CLPC_PDETX = 4,
        CAL_SCPC = 5,
        CAL_SCPC_PSATADJUST = 6
    };

    enum AvgType
    {
        // Token: 0x040001D1 RID: 465
        Linear,
        // Token: 0x040001D2 RID: 466
        Log10,
        // Token: 0x040001D3 RID: 467
        Log20
    };

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

    enum WLAN_tlvDataRate
    {
        tlvRATE_1Mbps = 0,
        tlvRATE_2Mbps_L = 1,
        tlvRATE_2Mbps_S = 2,
        tlvRATE_5_5Mbps_L = 3,
        tlvRATE_5_5Mbps_S = 4,
        tlvRATE_11Mbps_L = 5,
        tlvRATE_11Mbps_S = 6,
        tlvRATE_6Mbps = 8,
        tlvRATE_9Mbps = 9,
        tlvRATE_12Mbps = 10,
        tlvRATE_18Mbps = 11,
        tlvRATE_24Mbps = 12,
        tlvRATE_36Mbps = 13,
        tlvRATE_48Mbps = 14,
        tlvRATE_54Mbps = 15,
        tlvRATE_MCS_0_20 = 16,
        tlvRATE_MCS_1_20 = 17,
        tlvRATE_MCS_2_20 = 18,
        tlvRATE_MCS_3_20 = 19,
        tlvRATE_MCS_4_20 = 20,
        tlvRATE_MCS_5_20 = 21,
        tlvRATE_MCS_6_20 = 22,
        tlvRATE_MCS_7_20 = 23,
        tlvRATE_MCS_0_40 = 24,
        tlvRATE_MCS_1_40 = 25,
        tlvRATE_MCS_2_40 = 26,
        tlvRATE_MCS_3_40 = 27,
        tlvRATE_MCS_4_40 = 28,
        tlvRATE_MCS_5_40 = 29,
        tlvRATE_MCS_6_40 = 30,
        tlvRATE_MCS_7_40 = 31,
        tlvRATE_MCS_8_20 = 32,
        tlvRATE_MCS_9_20 = 33,
        tlvRATE_MCS_10_20 = 34,
        tlvRATE_MCS_11_20 = 35,
        tlvRATE_MCS_12_20 = 36,
        tlvRATE_MCS_13_20 = 37,
        tlvRATE_MCS_14_20 = 38,
        tlvRATE_MCS_15_20 = 39,
        tlvRATE_MCS_8_40 = 40,
        tlvRATE_MCS_9_40 = 41,
        tlvRATE_MCS_10_40 = 42,
        tlvRATE_MCS_11_40 = 43,
        tlvRATE_MCS_12_40 = 44,
        tlvRATE_MCS_13_40 = 45,
        tlvRATE_MCS_14_40 = 46,
        tlvRATE_MCS_15_40 = 47,
        tlvRATE_MCS_16_20 = 48,
        tlvRATE_MCS_17_20 = 49,
        tlvRATE_MCS_18_20 = 50,
        tlvRATE_MCS_19_20 = 51,
        tlvRATE_MCS_20_20 = 52,
        tlvRATE_MCS_21_20 = 53,
        tlvRATE_MCS_22_20 = 54,
        tlvRATE_MCS_23_20 = 55,
        tlvRATE_MCS_16_40 = 56,
        tlvRATE_MCS_17_40 = 57,
        tlvRATE_MCS_18_40 = 58,
        tlvRATE_MCS_19_40 = 59,
        tlvRATE_MCS_20_40 = 60,
        tlvRATE_MCS_21_40 = 61,
        tlvRATE_MCS_22_40 = 62,
        tlvRATE_MCS_23_40 = 63,
        tlvRATE_AC_MCS_0_20 = 64,
        tlvRATE_AC_MCS_1_20 = 65,
        tlvRATE_AC_MCS_2_20 = 66,
        tlvRATE_AC_MCS_3_20 = 67,
        tlvRATE_AC_MCS_4_20 = 68,
        tlvRATE_AC_MCS_5_20 = 69,
        tlvRATE_AC_MCS_6_20 = 70,
        tlvRATE_AC_MCS_7_20 = 71,
        tlvRATE_AC_MCS_8_20 = 72,
        tlvRATE_AC_MCS_9_20 = 73,
        tlvRATE_AC_MCS_0_40 = 76,
        tlvRATE_AC_MCS_1_40 = 77,
        tlvRATE_AC_MCS_2_40 = 78,
        tlvRATE_AC_MCS_3_40 = 79,
        tlvRATE_AC_MCS_4_40 = 80,
        tlvRATE_AC_MCS_5_40 = 81,
        tlvRATE_AC_MCS_6_40 = 82,
        tlvRATE_AC_MCS_7_40 = 83,
        tlvRATE_AC_MCS_8_40 = 84,
        tlvRATE_AC_MCS_9_40 = 85,
        tlvRATE_AC_MCS_0_80 = 88,
        tlvRATE_AC_MCS_1_80 = 89,
        tlvRATE_AC_MCS_2_80 = 90,
        tlvRATE_AC_MCS_3_80 = 91,
        tlvRATE_AC_MCS_4_80 = 92,
        tlvRATE_AC_MCS_5_80 = 93,
        tlvRATE_AC_MCS_6_80 = 94,
        tlvRATE_AC_MCS_7_80 = 95,
        tlvRATE_AC_MCS_8_80 = 96,
        tlvRATE_AC_MCS_9_80 = 97,
        tlvRATE_AC_MCS_0_20_2S = 100,
        tlvRATE_AC_MCS_1_20_2S = 101,
        tlvRATE_AC_MCS_2_20_2S = 102,
        tlvRATE_AC_MCS_3_20_2S = 103,
        tlvRATE_AC_MCS_4_20_2S = 104,
        tlvRATE_AC_MCS_5_20_2S = 105,
        tlvRATE_AC_MCS_6_20_2S = 106,
        tlvRATE_AC_MCS_7_20_2S = 107,
        tlvRATE_AC_MCS_8_20_2S = 108,
        tlvRATE_AC_MCS_9_20_2S = 109,
        tlvRATE_AC_MCS_0_40_2S = 112,
        tlvRATE_AC_MCS_1_40_2S = 113,
        tlvRATE_AC_MCS_2_40_2S = 114,
        tlvRATE_AC_MCS_3_40_2S = 115,
        tlvRATE_AC_MCS_4_40_2S = 116,
        tlvRATE_AC_MCS_5_40_2S = 117,
        tlvRATE_AC_MCS_6_40_2S = 118,
        tlvRATE_AC_MCS_7_40_2S = 119,
        tlvRATE_AC_MCS_8_40_2S = 120,
        tlvRATE_AC_MCS_9_40_2S = 121,
        tlvRATE_AC_MCS_0_80_2S = 124,
        tlvRATE_AC_MCS_1_80_2S = 125,
        tlvRATE_AC_MCS_2_80_2S = 126,
        tlvRATE_AC_MCS_3_80_2S = 127,
        tlvRATE_AC_MCS_4_80_2S = 128,
        tlvRATE_AC_MCS_5_80_2S = 129,
        tlvRATE_AC_MCS_6_80_2S = 130,
        tlvRATE_AC_MCS_7_80_2S = 131,
        tlvRATE_AC_MCS_8_80_2S = 132,
        tlvRATE_AC_MCS_9_80_2S = 133,
        tlvRATE_AC_MCS_0_20_3S = 136,
        tlvRATE_AC_MCS_1_20_3S = 137,
        tlvRATE_AC_MCS_2_20_3S = 138,
        tlvRATE_AC_MCS_3_20_3S = 139,
        tlvRATE_AC_MCS_4_20_3S = 140,
        tlvRATE_AC_MCS_5_20_3S = 141,
        tlvRATE_AC_MCS_6_20_3S = 142,
        tlvRATE_AC_MCS_7_20_3S = 143,
        tlvRATE_AC_MCS_8_20_3S = 144,
        tlvRATE_AC_MCS_9_20_3S = 145,
        tlvRATE_AC_MCS_0_40_3S = 148,
        tlvRATE_AC_MCS_1_40_3S = 149,
        tlvRATE_AC_MCS_2_40_3S = 150,
        tlvRATE_AC_MCS_3_40_3S = 151,
        tlvRATE_AC_MCS_4_40_3S = 152,
        tlvRATE_AC_MCS_5_40_3S = 153,
        tlvRATE_AC_MCS_6_40_3S = 154,
        tlvRATE_AC_MCS_7_40_3S = 155,
        tlvRATE_AC_MCS_8_40_3S = 156,
        tlvRATE_AC_MCS_9_40_3S = 157,
        tlvRATE_AC_MCS_0_80_3S = 160,
        tlvRATE_AC_MCS_1_80_3S = 161,
        tlvRATE_AC_MCS_2_80_3S = 162,
        tlvRATE_AC_MCS_3_80_3S = 163,
        tlvRATE_AC_MCS_4_80_3S = 164,
        tlvRATE_AC_MCS_5_80_3S = 165,
        tlvRATE_AC_MCS_6_80_3S = 166,
        tlvRATE_AC_MCS_7_80_3S = 167,
        tlvRATE_AC_MCS_8_80_3S = 168,
        tlvRATE_AC_MCS_9_80_3S = 169,
        tlvRATE_AC_MCS_0_20_4S = 192,
        tlvRATE_AC_MCS_1_20_4S = 193,
        tlvRATE_AC_MCS_2_20_4S = 194,
        tlvRATE_AC_MCS_3_20_4S = 195,
        tlvRATE_AC_MCS_4_20_4S = 196,
        tlvRATE_AC_MCS_5_20_4S = 197,
        tlvRATE_AC_MCS_6_20_4S = 198,
        tlvRATE_AC_MCS_7_20_4S = 199,
        tlvRATE_AC_MCS_8_20_4S = 200,
        tlvRATE_AC_MCS_9_20_4S = 201,
        tlvRATE_AC_MCS_0_40_4S = 202,
        tlvRATE_AC_MCS_1_40_4S = 203,
        tlvRATE_AC_MCS_2_40_4S = 204,
        tlvRATE_AC_MCS_3_40_4S = 205,
        tlvRATE_AC_MCS_4_40_4S = 206,
        tlvRATE_AC_MCS_5_40_4S = 207,
        tlvRATE_AC_MCS_6_40_4S = 208,
        tlvRATE_AC_MCS_7_40_4S = 209,
        tlvRATE_AC_MCS_8_40_4S = 210,
        tlvRATE_AC_MCS_9_40_4S = 211,
        tlvRATE_AC_MCS_0_80_4S = 212,
        tlvRATE_AC_MCS_1_80_4S = 213,
        tlvRATE_AC_MCS_2_80_4S = 214,
        tlvRATE_AC_MCS_3_80_4S = 215,
        tlvRATE_AC_MCS_4_80_4S = 216,
        tlvRATE_AC_MCS_5_80_4S = 217,
        tlvRATE_AC_MCS_6_80_4S = 218,
        tlvRATE_AC_MCS_7_80_4S = 219,
        tlvRATE_AC_MCS_8_80_4S = 220,
        tlvRATE_AC_MCS_9_80_4S = 221,
        tlvRATE_AC_MCS_0_160_1S = 224,
        tlvRATE_AC_MCS_1_160_1S = 225,
        tlvRATE_AC_MCS_2_160_1S = 226,
        tlvRATE_AC_MCS_3_160_1S = 227,
        tlvRATE_AC_MCS_4_160_1S = 228,
        tlvRATE_AC_MCS_5_160_1S = 229,
        tlvRATE_AC_MCS_6_160_1S = 230,
        tlvRATE_AC_MCS_7_160_1S = 231,
        tlvRATE_AC_MCS_8_160_1S = 232,
        tlvRATE_AC_MCS_9_160_1S = 233,
        tlvRATE_AC_MCS_0_160_2S = 234,
        tlvRATE_AC_MCS_1_160_2S = 235,
        tlvRATE_AC_MCS_2_160_2S = 236,
        tlvRATE_AC_MCS_3_160_2S = 237,
        tlvRATE_AC_MCS_4_160_2S = 238,
        tlvRATE_AC_MCS_5_160_2S = 239,
        tlvRATE_AC_MCS_6_160_2S = 240,
        tlvRATE_AC_MCS_7_160_2S = 241,
        tlvRATE_AC_MCS_8_160_2S = 242,
        tlvRATE_AC_MCS_9_160_2S = 243,
        tlvRATE_MCS_24_20 = 256,
        tlvRATE_MCS_25_20 = 257,
        tlvRATE_MCS_26_20 = 258,
        tlvRATE_MCS_27_20 = 259,
        tlvRATE_MCS_28_20 = 260,
        tlvRATE_MCS_29_20 = 261,
        tlvRATE_MCS_30_20 = 262,
        tlvRATE_MCS_31_20 = 263,
        tlvRATE_MCS_24_40 = 264,
        tlvRATE_MCS_25_40 = 265,
        tlvRATE_MCS_26_40 = 266,
        tlvRATE_MCS_27_40 = 267,
        tlvRATE_MCS_28_40 = 268,
        tlvRATE_MCS_29_40 = 269,
        tlvRATE_MCS_30_40 = 270,
        tlvRATE_MCS_31_40 = 271,
        tlvRATE_P_1p5Mbps_5 = 1008,
        tlvRATE_P_2Mbps_5 = 1009,
        tlvRATE_P_3Mbps_5 = 1010,
        tlvRATE_P_4p5Mbps_5 = 1011,
        tlvRATE_P_6Mbps_5 = 1012,
        tlvRATE_P_9Mbps_5 = 1013,
        tlvRATE_P_12Mbps_5 = 1014,
        tlvRATE_P_13p5Mbps_5 = 1015,
        tlvRATE_P_3Mbps_10 = 1108,
        tlvRATE_P_4p5Mbps_10 = 1109,
        tlvRATE_P_6Mbps_10 = 1110,
        tlvRATE_P_9Mbps_10 = 1111,
        tlvRATE_P_12Mbps_10 = 1112,
        tlvRATE_P_18Mbps_10 = 1113,
        tlvRATE_P_24Mbps_10 = 1114,
        tlvRATE_P_27Mbps_10 = 1115,
        tlvRATE_P_6Mbps_20 = 1208,
        tlvRATE_P_9Mbps_20 = 1209,
        tlvRATE_P_12Mbps_20 = 1210,
        tlvRATE_P_18Mbps_20 = 1211,
        tlvRATE_P_24Mbps_20 = 1212,
        tlvRATE_P_36Mbps_20 = 1213,
        tlvRATE_P_48Mbps_20 = 1214,
        tlvRATE_P_54Mbps_20 = 1215
    };

    enum WLAN_DataRate
    {
        RATE_1Mbps = 0,
        RATE_2Mbps = 1,
        RATE_5_5Mbps = 2,
        RATE_11Mbps = 3,
        RATE_6Mbps = 4,
        RATE_9Mbps = 5,
        RATE_12Mbps = 6,
        RATE_18Mbps = 7,
        RATE_24Mbps = 8,
        RATE_36Mbps = 9,
        RATE_48Mbps = 10,
        RATE_54Mbps = 11,
        RATE_MCS_0_20 = 12,
        RATE_MCS_1_20 = 13,
        RATE_MCS_2_20 = 14,
        RATE_MCS_3_20 = 15,
        RATE_MCS_4_20 = 16,
        RATE_MCS_5_20 = 17,
        RATE_MCS_6_20 = 18,
        RATE_MCS_7_20 = 19,
        RATE_MCS_0_40 = 20,
        RATE_MCS_1_40 = 21,
        RATE_MCS_2_40 = 22,
        RATE_MCS_3_40 = 23,
        RATE_MCS_4_40 = 24,
        RATE_MCS_5_40 = 25,
        RATE_MCS_6_40 = 26,
        RATE_MCS_7_40 = 27,
        RATE_AC_MCS_0_20 = 28,
        RATE_AC_MCS_1_20 = 29,
        RATE_AC_MCS_2_20 = 30,
        RATE_AC_MCS_3_20 = 31,
        RATE_AC_MCS_4_20 = 32,
        RATE_AC_MCS_5_20 = 33,
        RATE_AC_MCS_6_20 = 34,
        RATE_AC_MCS_7_20 = 35,
        RATE_AC_MCS_8_20 = 36,
        RATE_AC_MCS_9_20 = 37,
        RATE_AC_MCS_0_40 = 38,
        RATE_AC_MCS_1_40 = 39,
        RATE_AC_MCS_2_40 = 40,
        RATE_AC_MCS_3_40 = 41,
        RATE_AC_MCS_4_40 = 42,
        RATE_AC_MCS_5_40 = 43,
        RATE_AC_MCS_6_40 = 44,
        RATE_AC_MCS_7_40 = 45,
        RATE_AC_MCS_8_40 = 46,
        RATE_AC_MCS_9_40 = 47,
        RATE_AC_MCS_0_80 = 48,
        RATE_AC_MCS_1_80 = 49,
        RATE_AC_MCS_2_80 = 50,
        RATE_AC_MCS_3_80 = 51,
        RATE_AC_MCS_4_80 = 52,
        RATE_AC_MCS_5_80 = 53,
        RATE_AC_MCS_6_80 = 54,
        RATE_AC_MCS_7_80 = 55,
        RATE_AC_MCS_8_80 = 56,
        RATE_AC_MCS_9_80 = 57,
        RATE_MCS_8_20 = 58,
        RATE_MCS_9_20 = 59,
        RATE_MCS_10_20 = 60,
        RATE_MCS_11_20 = 61,
        RATE_MCS_12_20 = 62,
        RATE_MCS_13_20 = 63,
        RATE_MCS_14_20 = 64,
        RATE_MCS_15_20 = 65,
        RATE_MCS_8_40 = 66,
        RATE_MCS_9_40 = 67,
        RATE_MCS_10_40 = 68,
        RATE_MCS_11_40 = 69,
        RATE_MCS_12_40 = 70,
        RATE_MCS_13_40 = 71,
        RATE_MCS_14_40 = 72,
        RATE_MCS_15_40 = 73,
        RATE_AC_MCS_10_20 = 74,
        RATE_AC_MCS_11_20 = 75,
        RATE_AC_MCS_12_20 = 76,
        RATE_AC_MCS_13_20 = 77,
        RATE_AC_MCS_14_20 = 78,
        RATE_AC_MCS_15_20 = 79,
        RATE_AC_MCS_16_20 = 80,
        RATE_AC_MCS_17_20 = 81,
        RATE_AC_MCS_18_20 = 82,
        RATE_AC_MCS_19_20 = 83,
        RATE_AC_MCS_10_40 = 84,
        RATE_AC_MCS_11_40 = 85,
        RATE_AC_MCS_12_40 = 86,
        RATE_AC_MCS_13_40 = 87,
        RATE_AC_MCS_14_40 = 88,
        RATE_AC_MCS_15_40 = 89,
        RATE_AC_MCS_16_40 = 90,
        RATE_AC_MCS_17_40 = 91,
        RATE_AC_MCS_18_40 = 92,
        RATE_AC_MCS_19_40 = 93,
        RATE_AC_MCS_10_80 = 94,
        RATE_AC_MCS_11_80 = 95,
        RATE_AC_MCS_12_80 = 96,
        RATE_AC_MCS_13_80 = 97,
        RATE_AC_MCS_14_80 = 98,
        RATE_AC_MCS_15_80 = 99,
        RATE_AC_MCS_16_80 = 100,
        RATE_AC_MCS_17_80 = 101,
        RATE_AC_MCS_18_80 = 102,
        RATE_AC_MCS_19_80 = 103,
        RATE_MCS_16_20 = 104,
        RATE_MCS_17_20 = 105,
        RATE_MCS_18_20 = 106,
        RATE_MCS_19_20 = 107,
        RATE_MCS_20_20 = 108,
        RATE_MCS_21_20 = 109,
        RATE_MCS_22_20 = 110,
        RATE_MCS_23_20 = 111,
        RATE_MCS_16_40 = 112,
        RATE_MCS_17_40 = 113,
        RATE_MCS_18_40 = 114,
        RATE_MCS_19_40 = 115,
        RATE_MCS_20_40 = 116,
        RATE_MCS_21_40 = 117,
        RATE_MCS_22_40 = 118,
        RATE_MCS_23_40 = 119,
        RATE_AC_MCS_20_20 = 120,
        RATE_AC_MCS_21_20 = 121,
        RATE_AC_MCS_22_20 = 122,
        RATE_AC_MCS_23_20 = 123,
        RATE_AC_MCS_24_20 = 124,
        RATE_AC_MCS_25_20 = 125,
        RATE_AC_MCS_26_20 = 126,
        RATE_AC_MCS_27_20 = 127,
        RATE_AC_MCS_28_20 = 128,
        RATE_AC_MCS_29_20 = 129,
        RATE_AC_MCS_20_40 = 130,
        RATE_AC_MCS_21_40 = 131,
        RATE_AC_MCS_22_40 = 132,
        RATE_AC_MCS_23_40 = 133,
        RATE_AC_MCS_24_40 = 134,
        RATE_AC_MCS_25_40 = 135,
        RATE_AC_MCS_26_40 = 136,
        RATE_AC_MCS_27_40 = 137,
        RATE_AC_MCS_28_40 = 138,
        RATE_AC_MCS_29_40 = 139,
        RATE_AC_MCS_20_80 = 140,
        RATE_AC_MCS_21_80 = 141,
        RATE_AC_MCS_22_80 = 142,
        RATE_AC_MCS_23_80 = 143,
        RATE_AC_MCS_24_80 = 144,
        RATE_AC_MCS_25_80 = 145,
        RATE_AC_MCS_26_80 = 146,
        RATE_AC_MCS_27_80 = 147,
        RATE_AC_MCS_28_80 = 148,
        RATE_AC_MCS_29_80 = 149,
        RATE_AC_MCS_30_20 = 150,
        RATE_AC_MCS_31_20 = 151,
        RATE_AC_MCS_32_20 = 152,
        RATE_AC_MCS_33_20 = 153,
        RATE_AC_MCS_34_20 = 154,
        RATE_AC_MCS_35_20 = 155,
        RATE_AC_MCS_36_20 = 156,
        RATE_AC_MCS_37_20 = 157,
        RATE_AC_MCS_38_20 = 158,
        RATE_AC_MCS_39_20 = 159,
        RATE_AC_MCS_30_40 = 160,
        RATE_AC_MCS_31_40 = 161,
        RATE_AC_MCS_32_40 = 162,
        RATE_AC_MCS_33_40 = 163,
        RATE_AC_MCS_34_40 = 164,
        RATE_AC_MCS_35_40 = 165,
        RATE_AC_MCS_36_40 = 166,
        RATE_AC_MCS_37_40 = 167,
        RATE_AC_MCS_38_40 = 168,
        RATE_AC_MCS_39_40 = 169,
        RATE_AC_MCS_30_80 = 170,
        RATE_AC_MCS_31_80 = 171,
        RATE_AC_MCS_32_80 = 172,
        RATE_AC_MCS_33_80 = 173,
        RATE_AC_MCS_34_80 = 174,
        RATE_AC_MCS_35_80 = 175,
        RATE_AC_MCS_36_80 = 176,
        RATE_AC_MCS_37_80 = 177,
        RATE_AC_MCS_38_80 = 178,
        RATE_AC_MCS_39_80 = 179,
        RATE_AC_MCS_0_160 = 180,
        RATE_AC_MCS_1_160 = 181,
        RATE_AC_MCS_2_160 = 182,
        RATE_AC_MCS_3_160 = 183,
        RATE_AC_MCS_4_160 = 184,
        RATE_AC_MCS_5_160 = 185,
        RATE_AC_MCS_6_160 = 186,
        RATE_AC_MCS_7_160 = 187,
        RATE_AC_MCS_8_160 = 188,
        RATE_AC_MCS_9_160 = 189,
        RATE_AC_MCS_10_160 = 190,
        RATE_AC_MCS_11_160 = 191,
        RATE_AC_MCS_12_160 = 192,
        RATE_AC_MCS_13_160 = 193,
        RATE_AC_MCS_14_160 = 194,
        RATE_AC_MCS_15_160 = 195,
        RATE_AC_MCS_16_160 = 196,
        RATE_AC_MCS_17_160 = 197,
        RATE_AC_MCS_18_160 = 198,
        RATE_AC_MCS_19_160 = 199,
        RATE_MCS_24_20 = 220,
        RATE_MCS_25_20 = 221,
        RATE_MCS_26_20 = 222,
        RATE_MCS_27_20 = 223,
        RATE_MCS_28_20 = 224,
        RATE_MCS_29_20 = 225,
        RATE_MCS_30_20 = 226,
        RATE_MCS_31_20 = 227,
        RATE_MCS_24_40 = 228,
        RATE_MCS_25_40 = 229,
        RATE_MCS_26_40 = 230,
        RATE_MCS_27_40 = 231,
        RATE_MCS_28_40 = 232,
        RATE_MCS_29_40 = 233,
        RATE_MCS_30_40 = 234,
        RATE_MCS_31_40 = 235,
        RATE_P_1p5Mbps_5 = 1004,
        RATE_P_2Mbps_5 = 1005,
        RATE_P_3Mbps_5 = 1006,
        RATE_P_4p5Mbps_5 = 1007,
        RATE_P_6Mbps_5 = 1008,
        RATE_P_9Mbps_5 = 1009,
        RATE_P_12Mbps_5 = 1010,
        RATE_P_13p5Mbps_5 = 1011,
        RATE_P_3Mbps_10 = 1104,
        RATE_P_4p5Mbps_10 = 1105,
        RATE_P_6Mbps_10 = 1106,
        RATE_P_9Mbps_10 = 1107,
        RATE_P_12Mbps_10 = 1108,
        RATE_P_18Mbps_10 = 1109,
        RATE_P_24Mbps_10 = 1110,
        RATE_P_27Mbps_10 = 1111,
        RATE_P_6Mbps_20 = 1204,
        RATE_P_9Mbps_20 = 1205,
        RATE_P_12Mbps_20 = 1206,
        RATE_P_18Mbps_20 = 1207,
        RATE_P_24Mbps_20 = 1208,
        RATE_P_36Mbps_20 = 1209,
        RATE_P_48Mbps_20 = 1210,
        RATE_P_54Mbps_20 = 1211
    };

    enum WLAN_RATE
    {
        RATE_CW = -1,
        RATE_11B_LONG_1_MBPS = 0,
        RATE_11B_LONG_2_MBPS = 1,
        RATE_11B_LONG_5_5_MBPS = 2,
        RATE_11B_LONG_11_MBPS = 3,
        RATE_11B_SHORT_2_MBPS = 4,
        RATE_11B_SHORT_5_5_MBPS = 5,
        RATE_11B_SHORT_11_MBPS = 6,
        RATE_11A_6_MBPS = 7,
        RATE_11A_9_MBPS = 8,
        RATE_11A_12_MBPS = 9,
        RATE_11A_18_MBPS = 10,
        RATE_11A_24_MBPS = 11,
        RATE_11A_36_MBPS = 12,
        RATE_11A_48_MBPS = 13,
        RATE_11A_54_MBPS = 14,
        RATE_11N_HT20_MCS0 = 15,
        RATE_11N_HT20_MCS1 = 16,
        RATE_11N_HT20_MCS2 = 17,
        RATE_11N_HT20_MCS3 = 18,
        RATE_11N_HT20_MCS4 = 19,
        RATE_11N_HT20_MCS5 = 20,
        RATE_11N_HT20_MCS6 = 21,
        RATE_11N_HT20_MCS7 = 22,
        RATE_11N_HT40_MCS0 = 23,
        RATE_11N_HT40_MCS1 = 24,
        RATE_11N_HT40_MCS2 = 25,
        RATE_11N_HT40_MCS3 = 26,
        RATE_11N_HT40_MCS4 = 27,
        RATE_11N_HT40_MCS5 = 28,
        RATE_11N_HT40_MCS6 = 29,
        RATE_11N_HT40_MCS7 = 30,
        RATE_11AC_HT20_MCS0 = 31,
        RATE_11AC_HT20_MCS1 = 32,
        RATE_11AC_HT20_MCS2 = 33,
        RATE_11AC_HT20_MCS3 = 34,
        RATE_11AC_HT20_MCS4 = 35,
        RATE_11AC_HT20_MCS5 = 36,
        RATE_11AC_HT20_MCS6 = 37,
        RATE_11AC_HT20_MCS7 = 38,
        RATE_11AC_HT20_MCS8 = 39,
        RATE_11AC_HT20_MCS9 = 40,
        RATE_11AC_HT40_MCS0 = 41,
        RATE_11AC_HT40_MCS1 = 42,
        RATE_11AC_HT40_MCS2 = 43,
        RATE_11AC_HT40_MCS3 = 44,
        RATE_11AC_HT40_MCS4 = 45,
        RATE_11AC_HT40_MCS5 = 46,
        RATE_11AC_HT40_MCS6 = 47,
        RATE_11AC_HT40_MCS7 = 48,
        RATE_11AC_HT40_MCS8 = 49,
        RATE_11AC_HT40_MCS9 = 50,
        RATE_11AC_HT80_MCS0 = 51,
        RATE_11AC_HT80_MCS1 = 52,
        RATE_11AC_HT80_MCS2 = 53,
        RATE_11AC_HT80_MCS3 = 54,
        RATE_11AC_HT80_MCS4 = 55,
        RATE_11AC_HT80_MCS5 = 56,
        RATE_11AC_HT80_MCS6 = 57,
        RATE_11AC_HT80_MCS7 = 58,
        RATE_11AC_HT80_MCS8 = 59,
        RATE_11AC_HT80_MCS9 = 60,
        RATE_11N_HT20_MCS8 = 61,
        RATE_11N_HT20_MCS9 = 62,
        RATE_11N_HT20_MCS10 = 63,
        RATE_11N_HT20_MCS11 = 64,
        RATE_11N_HT20_MCS12 = 65,
        RATE_11N_HT20_MCS13 = 66,
        RATE_11N_HT20_MCS14 = 67,
        RATE_11N_HT20_MCS15 = 68,
        RATE_11N_HT40_MCS8 = 69,
        RATE_11N_HT40_MCS9 = 70,
        RATE_11N_HT40_MCS10 = 71,
        RATE_11N_HT40_MCS11 = 72,
        RATE_11N_HT40_MCS12 = 73,
        RATE_11N_HT40_MCS13 = 74,
        RATE_11N_HT40_MCS14 = 75,
        RATE_11N_HT40_MCS15 = 76,
        RATE_11AC_HT20_MCS10 = 77,
        RATE_11AC_HT20_MCS11 = 78,
        RATE_11AC_HT20_MCS12 = 79,
        RATE_11AC_HT20_MCS13 = 80,
        RATE_11AC_HT20_MCS14 = 81,
        RATE_11AC_HT20_MCS15 = 82,
        RATE_11AC_HT20_MCS16 = 83,
        RATE_11AC_HT20_MCS17 = 84,
        RATE_11AC_HT20_MCS18 = 85,
        RATE_11AC_HT20_MCS19 = 86,
        RATE_11AC_HT40_MCS10 = 87,
        RATE_11AC_HT40_MCS11 = 88,
        RATE_11AC_HT40_MCS12 = 89,
        RATE_11AC_HT40_MCS13 = 90,
        RATE_11AC_HT40_MCS14 = 91,
        RATE_11AC_HT40_MCS15 = 92,
        RATE_11AC_HT40_MCS16 = 93,
        RATE_11AC_HT40_MCS17 = 94,
        RATE_11AC_HT40_MCS18 = 95,
        RATE_11AC_HT40_MCS19 = 96,
        RATE_11AC_HT80_MCS10 = 97,
        RATE_11AC_HT80_MCS11 = 98,
        RATE_11AC_HT80_MCS12 = 99,
        RATE_11AC_HT80_MCS13 = 100,
        RATE_11AC_HT80_MCS14 = 101,
        RATE_11AC_HT80_MCS15 = 102,
        RATE_11AC_HT80_MCS16 = 103,
        RATE_11AC_HT80_MCS17 = 104,
        RATE_11AC_HT80_MCS18 = 105,
        RATE_11AC_HT80_MCS19 = 106,
        RATE_11N_HT20_MCS16 = 107,
        RATE_11N_HT20_MCS17 = 108,
        RATE_11N_HT20_MCS18 = 109,
        RATE_11N_HT20_MCS19 = 110,
        RATE_11N_HT20_MCS20 = 111,
        RATE_11N_HT20_MCS21 = 112,
        RATE_11N_HT20_MCS22 = 113,
        RATE_11N_HT20_MCS23 = 114,
        RATE_11N_HT40_MCS16 = 115,
        RATE_11N_HT40_MCS17 = 116,
        RATE_11N_HT40_MCS18 = 117,
        RATE_11N_HT40_MCS19 = 118,
        RATE_11N_HT40_MCS20 = 119,
        RATE_11N_HT40_MCS21 = 120,
        RATE_11N_HT40_MCS22 = 121,
        RATE_11N_HT40_MCS23 = 122,
        RATE_11AC_HT20_MCS20 = 123,
        RATE_11AC_HT20_MCS21 = 124,
        RATE_11AC_HT20_MCS22 = 125,
        RATE_11AC_HT20_MCS23 = 126,
        RATE_11AC_HT20_MCS24 = 127,
        RATE_11AC_HT20_MCS25 = 128,
        RATE_11AC_HT20_MCS26 = 129,
        RATE_11AC_HT20_MCS27 = 130,
        RATE_11AC_HT20_MCS28 = 131,
        RATE_11AC_HT20_MCS29 = 132,
        RATE_11AC_HT40_MCS20 = 133,
        RATE_11AC_HT40_MCS21 = 134,
        RATE_11AC_HT40_MCS22 = 135,
        RATE_11AC_HT40_MCS23 = 136,
        RATE_11AC_HT40_MCS24 = 137,
        RATE_11AC_HT40_MCS25 = 138,
        RATE_11AC_HT40_MCS26 = 139,
        RATE_11AC_HT40_MCS27 = 140,
        RATE_11AC_HT40_MCS28 = 141,
        RATE_11AC_HT40_MCS29 = 142,
        RATE_11AC_HT80_MCS20 = 143,
        RATE_11AC_HT80_MCS21 = 144,
        RATE_11AC_HT80_MCS22 = 145,
        RATE_11AC_HT80_MCS23 = 146,
        RATE_11AC_HT80_MCS24 = 147,
        RATE_11AC_HT80_MCS25 = 148,
        RATE_11AC_HT80_MCS26 = 149,
        RATE_11AC_HT80_MCS27 = 150,
        RATE_11AC_HT80_MCS28 = 151,
        RATE_11AC_HT80_MCS29 = 152,
        RATE_11AC_HT20_MCS30 = 153,
        RATE_11AC_HT20_MCS31 = 154,
        RATE_11AC_HT20_MCS32 = 155,
        RATE_11AC_HT20_MCS33 = 156,
        RATE_11AC_HT20_MCS34 = 157,
        RATE_11AC_HT20_MCS35 = 158,
        RATE_11AC_HT20_MCS36 = 159,
        RATE_11AC_HT20_MCS37 = 160,
        RATE_11AC_HT20_MCS38 = 161,
        RATE_11AC_HT20_MCS39 = 162,
        RATE_11AC_HT40_MCS30 = 163,
        RATE_11AC_HT40_MCS31 = 164,
        RATE_11AC_HT40_MCS32 = 165,
        RATE_11AC_HT40_MCS33 = 166,
        RATE_11AC_HT40_MCS34 = 167,
        RATE_11AC_HT40_MCS35 = 168,
        RATE_11AC_HT40_MCS36 = 169,
        RATE_11AC_HT40_MCS37 = 170,
        RATE_11AC_HT40_MCS38 = 171,
        RATE_11AC_HT40_MCS39 = 172,
        RATE_11AC_HT80_MCS30 = 173,
        RATE_11AC_HT80_MCS31 = 174,
        RATE_11AC_HT80_MCS32 = 175,
        RATE_11AC_HT80_MCS33 = 176,
        RATE_11AC_HT80_MCS34 = 177,
        RATE_11AC_HT80_MCS35 = 178,
        RATE_11AC_HT80_MCS36 = 179,
        RATE_11AC_HT80_MCS37 = 180,
        RATE_11AC_HT80_MCS38 = 181,
        RATE_11AC_HT80_MCS39 = 182,
        RATE_11AC_HT160_MCS0 = 183,
        RATE_11AC_HT160_MCS1 = 184,
        RATE_11AC_HT160_MCS2 = 185,
        RATE_11AC_HT160_MCS3 = 186,
        RATE_11AC_HT160_MCS4 = 187,
        RATE_11AC_HT160_MCS5 = 188,
        RATE_11AC_HT160_MCS6 = 189,
        RATE_11AC_HT160_MCS7 = 190,
        RATE_11AC_HT160_MCS8 = 191,
        RATE_11AC_HT160_MCS9 = 192,
        RATE_11AC_HT160_MCS10 = 193,
        RATE_11AC_HT160_MCS11 = 194,
        RATE_11AC_HT160_MCS12 = 195,
        RATE_11AC_HT160_MCS13 = 196,
        RATE_11AC_HT160_MCS14 = 197,
        RATE_11AC_HT160_MCS15 = 198,
        RATE_11AC_HT160_MCS16 = 199,
        RATE_11AC_HT160_MCS17 = 200,
        RATE_11AC_HT160_MCS18 = 201,
        RATE_11AC_HT160_MCS19 = 202,
        RATE_11AC_HT160_MCS20 = 203,
        RATE_11AC_HT160_MCS21 = 204,
        RATE_11AC_HT160_MCS22 = 205,
        RATE_11AC_HT160_MCS23 = 206,
        RATE_11AC_HT160_MCS24 = 207,
        RATE_11AC_HT160_MCS25 = 208,
        RATE_11AC_HT160_MCS26 = 209,
        RATE_11AC_HT160_MCS27 = 210,
        RATE_11AC_HT160_MCS28 = 211,
        RATE_11AC_HT160_MCS29 = 212,
        RATE_11AC_HT160_MCS30 = 213,
        RATE_11AC_HT160_MCS31 = 214,
        RATE_11AC_HT160_MCS32 = 215,
        RATE_11AC_HT160_MCS33 = 216,
        RATE_11AC_HT160_MCS34 = 217,
        RATE_11AC_HT160_MCS35 = 218,
        RATE_11AC_HT160_MCS36 = 219,
        RATE_11AC_HT160_MCS37 = 220,
        RATE_11AC_HT160_MCS38 = 221,
        RATE_11AC_HT160_MCS39 = 222,
        RATE_11AC_HT80P80_MCS0 = 223,
        RATE_11AC_HT80P80_MCS1 = 224,
        RATE_11AC_HT80P80_MCS2 = 225,
        RATE_11AC_HT80P80_MCS3 = 226,
        RATE_11AC_HT80P80_MCS4 = 227,
        RATE_11AC_HT80P80_MCS5 = 228,
        RATE_11AC_HT80P80_MCS6 = 229,
        RATE_11AC_HT80P80_MCS7 = 230,
        RATE_11AC_HT80P80_MCS8 = 231,
        RATE_11AC_HT80P80_MCS9 = 232,
        RATE_11AC_HT80P80_MCS10 = 233,
        RATE_11AC_HT80P80_MCS11 = 234,
        RATE_11AC_HT80P80_MCS12 = 235,
        RATE_11AC_HT80P80_MCS13 = 236,
        RATE_11AC_HT80P80_MCS14 = 237,
        RATE_11AC_HT80P80_MCS15 = 238,
        RATE_11AC_HT80P80_MCS16 = 239,
        RATE_11AC_HT80P80_MCS17 = 240,
        RATE_11AC_HT80P80_MCS18 = 241,
        RATE_11AC_HT80P80_MCS19 = 242,
        RATE_11AC_HT80P80_MCS20 = 243,
        RATE_11AC_HT80P80_MCS21 = 244,
        RATE_11AC_HT80P80_MCS22 = 245,
        RATE_11AC_HT80P80_MCS23 = 246,
        RATE_11AC_HT80P80_MCS24 = 247,
        RATE_11AC_HT80P80_MCS25 = 248,
        RATE_11AC_HT80P80_MCS26 = 249,
        RATE_11AC_HT80P80_MCS27 = 250,
        RATE_11AC_HT80P80_MCS28 = 251,
        RATE_11AC_HT80P80_MCS29 = 252,
        RATE_11AC_HT80P80_MCS30 = 253,
        RATE_11AC_HT80P80_MCS31 = 254,
        RATE_11AC_HT80P80_MCS32 = 255,
        RATE_11AC_HT80P80_MCS33 = 256,
        RATE_11AC_HT80P80_MCS34 = 257,
        RATE_11AC_HT80P80_MCS35 = 258,
        RATE_11AC_HT80P80_MCS36 = 259,
        RATE_11AC_HT80P80_MCS37 = 260,
        RATE_11AC_HT80P80_MCS38 = 261,
        RATE_11AC_HT80P80_MCS39 = 262,
        RATE_11N_HT20_MCS24 = 263,
        RATE_11N_HT20_MCS25 = 264,
        RATE_11N_HT20_MCS26 = 265,
        RATE_11N_HT20_MCS27 = 266,
        RATE_11N_HT20_MCS28 = 267,
        RATE_11N_HT20_MCS29 = 268,
        RATE_11N_HT20_MCS30 = 269,
        RATE_11N_HT20_MCS31 = 270,
        RATE_11N_HT40_MCS24 = 271,
        RATE_11N_HT40_MCS25 = 272,
        RATE_11N_HT40_MCS26 = 273,
        RATE_11N_HT40_MCS27 = 274,
        RATE_11N_HT40_MCS28 = 275,
        RATE_11N_HT40_MCS29 = 276,
        RATE_11N_HT40_MCS30 = 277,
        RATE_11N_HT40_MCS31 = 278,
        RATE_11N = 700,
        RATE_11P = 701,
        RATE_11AC = 702,
        RATE_11AC_80P80 = 703,
        RATE_11AX = 704,
        RATE_11AX_80P80 = 705,
        RATE_11AX_OFDMA = 706,
        RATE_11AX_OFDMA_80P80 = 707,
        RATE_11AX_BW20_OFDMA = 800,
        RATE_11AX_BW20_MCS0 = 801,
        RATE_11AX_BW20_MCS1 = 802,
        RATE_11AX_BW20_MCS2 = 803,
        RATE_11AX_BW20_MCS3 = 804,
        RATE_11AX_BW20_MCS4 = 805,
        RATE_11AX_BW20_MCS5 = 806,
        RATE_11AX_BW20_MCS6 = 807,
        RATE_11AX_BW20_MCS7 = 808,
        RATE_11AX_BW20_MCS8 = 809,
        RATE_11AX_BW20_MCS9 = 810,
        RATE_11AX_BW20_MCS10 = 811,
        RATE_11AX_BW20_MCS11 = 812,
        RATE_11AX_BW40_OFDMA = 820,
        RATE_11AX_BW40_MCS0 = 821,
        RATE_11AX_BW40_MCS1 = 822,
        RATE_11AX_BW40_MCS2 = 823,
        RATE_11AX_BW40_MCS3 = 824,
        RATE_11AX_BW40_MCS4 = 825,
        RATE_11AX_BW40_MCS5 = 826,
        RATE_11AX_BW40_MCS6 = 827,
        RATE_11AX_BW40_MCS7 = 828,
        RATE_11AX_BW40_MCS8 = 829,
        RATE_11AX_BW40_MCS9 = 830,
        RATE_11AX_BW40_MCS10 = 831,
        RATE_11AX_BW40_MCS11 = 832,
        RATE_11AX_BW80_OFDMA = 840,
        RATE_11AX_BW80_MCS0 = 841,
        RATE_11AX_BW80_MCS1 = 842,
        RATE_11AX_BW80_MCS2 = 843,
        RATE_11AX_BW80_MCS3 = 844,
        RATE_11AX_BW80_MCS4 = 845,
        RATE_11AX_BW80_MCS5 = 846,
        RATE_11AX_BW80_MCS6 = 847,
        RATE_11AX_BW80_MCS7 = 848,
        RATE_11AX_BW80_MCS8 = 849,
        RATE_11AX_BW80_MCS9 = 850,
        RATE_11AX_BW80_MCS10 = 851,
        RATE_11AX_BW80_MCS11 = 852,
        RATE_11AX_BW80P80_OFDMA = 860,
        RATE_11AX_BW80P80_MCS0 = 861,
        RATE_11AX_BW80P80_MCS1 = 862,
        RATE_11AX_BW80P80_MCS2 = 863,
        RATE_11AX_BW80P80_MCS3 = 864,
        RATE_11AX_BW80P80_MCS4 = 865,
        RATE_11AX_BW80P80_MCS5 = 866,
        RATE_11AX_BW80P80_MCS6 = 867,
        RATE_11AX_BW80P80_MCS7 = 868,
        RATE_11AX_BW80P80_MCS8 = 869,
        RATE_11AX_BW80P80_MCS9 = 870,
        RATE_11AX_BW80P80_MCS10 = 871,
        RATE_11AX_BW80P80_MCS11 = 872,
        RATE_11P_BW5_1p5_MBPS = 1007,
        RATE_11P_BW5_2p25_MBPS = 1008,
        RATE_11P_BW5_3_MBPS = 1009,
        RATE_11P_BW5_4p5_MBPS = 1010,
        RATE_11P_BW5_6_MBPS = 1011,
        RATE_11P_BW5_9_MBPS = 1012,
        RATE_11P_BW5_12_MBPS = 1013,
        RATE_11P_BW5_13p5_MBPS = 1014,
        RATE_11P_BW10_3_MBPS = 1107,
        RATE_11P_BW10_4p5_MBPS = 1108,
        RATE_11P_BW10_6_MBPS = 1109,
        RATE_11P_BW10_9_MBPS = 1110,
        RATE_11P_BW10_12_MBPS = 1111,
        RATE_11P_BW10_18_MBPS = 1112,
        RATE_11P_BW10_24_MBPS = 1113,
        RATE_11P_BW10_27_MBPS = 1114,
        RATE_11P_BW20_6_MBPS = 1207,
        RATE_11P_BW20_9_MBPS = 1208,
        RATE_11P_BW20_12_MBPS = 1209,
        RATE_11P_BW20_18_MBPS = 1210,
        RATE_11P_BW20_24_MBPS = 1211,
        RATE_11P_BW20_36_MBPS = 1212,
        RATE_11P_BW20_48_MBPS = 1213,
        RATE_11P_BW20_54_MBPS = 1214
    };

    enum ChannelEstimationOption
    {
        preambleOnly = 1,
        fullpacketEstimate = 3
    };

    enum WLAN_Gen6_PHYDBG_PreambleRate
    {
        PREAMBLE_OFDM_11G = 0,
        PREAMBLE_GREENFIELD_11N = 1,
        PREAMBLE_MIXED_11N = 2,
        PREAMBLE_SHORTB_11B = 3,
        PREAMBLE_LONGB_11B = 4,
        max = 5
    };

    enum DPDSetting
    {
        DPD_Enabled,
        DPD_Disabled
    };

    enum HCSetting
    {
        HCS_Enabled,
        HCS_Disabled
    };

    enum TxDataPattern
    {
        ZEROES_PATTERN,
        ONES_PATTERN,
        REPEATING_10,
        PN7_PATTERN,
        PN9_PATTERN,
        PN15_PATTERN,
        USER_DEFINED_PATTERN
    };

    enum PHY165Mode
    {
        Mode_0,
        Mode_1,
        Mode_2
    };

    enum WLAN_UniversalGuardInterval
    {
        GI_0 = 0,
        LTF_Mode0_GI_400 = 1,
        LTF_Mode0_GI_800 = 2,
        LTF_Mode0_GI_1600 = 3,
        LTF_Mode0_GI_3200 = 4,
        LTF_Mode1_GI_400 = 17,
        LTF_Mode1_GI_800 = 18,
        LTF_Mode1_GI_1600 = 19,
        LTF_Mode1_GI_3200 = 20,
        LTF_Mode2_GI_400 = 33,
        LTF_Mode2_GI_800 = 34,
        LTF_Mode2_GI_1600 = 35,
        LTF_Mode2_GI_3200 = 36,
        LTF_Mode4_GI_400 = 49,
        LTF_Mode4_GI_800 = 50,
        LTF_Mode4_GI_1600 = 51,
        LTF_Mode4_GI_3200 = 52
    };

    enum PPDU_TYPE
    {
        SingleUser = 0,
        MultipleUsers = 1,
        ExtRangeSingleUser = 2,
        Trigger = 3
    };

    enum AniMode
    {
        Static,
        Dynamic
    };

    enum WLAN_P_TRANSMIT_POWER_CLASS
    {
        CLASS_A = 0,
        CLASS_B = 1,
        CLASS_C = 2,
        CLASS_D = 3
    };

    enum WLAN_TLV2_OPCODE
    {
        _OP2_SYNC = 100,
        _OP2_TPCCAL = 101,
        _OP2_TPCCALRSP = 102,
        _OP2_TPCCALPWR = 103,
        _OP2_TPCCALDATA = 104,
        _OP2_RXGAINCAL = 105,
        _OP2_RXGAINCALRSP = 106,
        _OP2_RXGAINCAL_SIGL_DONE = 107,
        _OP2_RXGAINCALRSP_DONE = 108,
        _OP2_REGREAD = 109,
        _OP2_REGREADRSP = 110,
        _OP2_REGWRITE = 111,
        _OP2_REGWRITERSP = 112,
        _OP2_BASICRSP = 113,
        _OP2_TX = 114,
        _OP2_TXSTATUS = 115,
        _OP2_TXSTATUSRSP = 116,
        _OP2_RX = 117,
        _OP2_RXSTATUS = 118,
        _OP2_RXSTATUSRSP = 119,
        _OP2_RXRSP = 121,
        _OP2_XTALCALPROC = 122,
        _OP2_XTALCALPROCRSP = 123,
        _OP2_READCUSTOTPSPACE = 124,
        _OP2_READCUSTOTPSPACERSP = 125,
        _OP2_WRITECUSTOTPSPACE = 126,
        _OP2_WRITECUSTOTPSPACERSP = 127,
        _OP2_GETCUSTOTPSIZE = 128,
        _OP2_GETCUSTOTPSIZERSP = 129,
        _OP2_GETDPDCOMPLETE = 130,
        _OP2_GETDPDCOMPLETERSP = 131,
        _OP2_GETTGTPWR = 132,
        _OP2_GETTGTPWRRSP = 133,
        _OP2_SETPCIECONFIGPARAMS = 134,
        _OP2_SETPCIECONFIGPARAMSRSP = 135,
        _OP2_COMMITOTPSTREAM = 136,
        _OP2_COMMITOTPSTREAMRSP = 137,
        _OP2_SETREGDMN = 138,
        _OP2_SETREGDMNRSP = 139,
        _OP2_MEMWRITE = 140,
        _OP2_MEMWRITERSP = 141,
        _OP2_MEMREAD = 142,
        _OP2_MEMREADRSP = 143,
        _OP2_CONFIG = 144,
        _OP2_DPDTUNE_LOOPBACKTIMING = 145,
        _OP2_DPDTUNE_LOOPBACKTIMINGRSP = 146,
        _OP2_DPDTUNE_LOOPBACKATTEN = 147,
        _OP2_DPDTUNE_LOOPBACKATTENRSP = 148,
        _OP2_DPDTUNE_TRAININGQUALITY = 149,
        _OP2_DPDTUNE_TRAININGQUALITYRSP = 150,
        _OP2_DPDTUNE_AGC2PWR = 151,
        _OP2_DPDTUNE_AGC2PWRRSP = 152,
        _OP2_LMHWCAL = 153,
        _OP2_LMHWCALRSP = 154,
        _OP2_LMTX = 155,
        _OP2_MORESEGMENT = 156,
        _OP2_MORESEGMENTRSP = 157,
        _OP2_LMRX = 158,
        _OP2_ADCCAPTURE = 159,
        _OP2_ADCCAPTURERSP = 160,
        _OP2_GENWAVEFORM = 161,
        _OP2_GENWAVEFORMRSP = 162,
        _OP2_ENABLEDFE = 163,
        _OP2_ENABLEDFERSP = 164,
        _OP2_CALCALTIME = 165,
        _OP2_CALCALTIMERSP = 166,
        _OP2_RXDCOGROUP = 167,
        _OP2_RXDCOGROUPRSP = 168,
        _OP2_SETPHYRFMODE = 169,
        _OP2_LMCHANNELLIST = 170,
        _OP2_LMCHANNELLISTRSP = 171,
        _OP2_LMTXINIT = 172,
        _OP2_LMTXINITRSP = 173,
        _OP2_LMGO = 174,
        _OP2_LMGORSP = 175,
        _OP2_LMQUERY = 176,
        _OP2_LMQUERYRSP = 177,
        _OP2_GENERICUTFCMD = 178,
        _OP2_GENERICUTFRSP = 179,
        _OP2_LMRXINIT = 180,
        _OP2_LMRXINITRSP = 181,
        _OP2_PHYDBGDUMP = 182,
        _OP2_PHYDBGDUMPRSP = 183,
        _OP2_DACPLYBCK = 184,
        _OP2_DACPLYBCKRSP = 185,
        _OP2_COMBCALGROUP = 186,
        _OP2_COMBCALGROUPRSP = 187,
        _OP2_NFCALGROUP = 188,
        _OP2_NFCALGROUPRSP = 189,
        _OP2_AGCHISTORYCONFIG = 190,
        _OP2_AGCHISTORYCONFIGRSP = 191,
        _OP2_AGCHISTORYDUMP = 192,
        _OP2_AGCHISTORYDUMPRSP = 193,
        _OP2_RXGAINCTRL = 194,
        _OP2_RXGAINCTRLRSP = 195,
        _OP2_EFUSEREAD = 196,
        _OP2_EFUSEREADRSP = 197,
        _OP2_EFUSEWRITE = 198,
        _OP2_EFUSEWRITERSP = 199,
        _OP2_NOISEFLOORREAD = 202,
        _OP2_NOISEFLOORREADRSP = 203,
        _OP2_STICKY = 204,
        _OP2_BDGETSIZE = 205,
        _OP2_BDGETSIZERSP = 206,
        _OP2_BDREAD = 207,
        _OP2_BDREADRSP = 208,
        _OP2_GETRSSI = 209,
        _OP2_GETRSSIRSP = 210,
        _OP2_SAR = 211,
        _OP2_SARRSP = 212,
        _OP2_TXGAINCTRL = 213,
        _OP2_TXGAINCTRLRSP = 214,
        _OP2_TONEPLAN = 215,
        _OP2_TONEPLANRSP = 216,
        _OP2_NOISEFLOORCALSAVE = 222,
        _OP2_NOISEFLOORCALSAVERSP = 223,
        _OP2_OFDMATONEPLAN = 224,
        _OP2_OFDMATONEPLANRSP = 225,
        _OP2_SETLOCALMAC = 226,
        _OP2_SETLOCALMACRSP = 227,
        _OP2_SETBOARDMCN = 228,
        _OP2_SETBOARDMCNRSP = 229,
        _OP2_GETPOWERTABLE = 230,
        _OP2_GETPOWERTABLERSP = 231,
        _OP2_RADARDETECT = 232,
        _OP2_RADARDETECTRSP = 233,
        _OP2_COEXREQ = 234,
        _OP2_COEXREQRSP = 235,
        _OP2_CONFIGURESPECTRALSCAN = 236,
        _OP2_CONFIGURESPECTRALSCANRSP = 237,
        _OP2_CAPTUREFFTREPORTS = 238,
        _OP2_CAPTUREFFTREPORTSRSP = 239,
        _OP2_CAPTUREDFSREPORTS = 240,
        _OP2_CAPTUREDFSREPORTSRSP = 241,
        _OP2_WRITECALDATATOOTP = 247,
        _OP2_WRITECALDATATOOTPRSP = 248,
        _OP2_TPCGENTOOL = 251,
        _OP2_TPCGENTOOLRSP = 252,
        _OP2_INVALIDATECALDATAOTPSECTION = 253,
        _OP2_INVALIDATECALDATAOTPSECTIONRSP = 254,
        _OP2_SETPERCHAINPWRLIMITREG = 255,
        _OP2_SETPERCHAINPWRLIMITREGRSP = 256,
        _OP2_VERIFYCALDB = 257,
        _OP2_VERIFYCALDBRSP = 258,
        _OP2_TPC1PTSETCHANPWR = 259,
        _OP2_TPC1PTSETCHANPWRRSP = 260,
        _OP2_CALDBSAVETOHOST = 261,
        _OP2_CALDBSAVETOHOSTRSP = 262,
        _OP2_TPC1PTSETPRECALDATA = 263,
        _OP2_TPC1PTSETPRECALDATARSP = 264,
        _OP2_TPCFULLPTSETPRECALDATA = 265,
        _OP2_TPCFULLPTSETPRECALDATARSP = 266,
        _OP2_TPCSETCALCHAINMASKS = 267,
        _OP2_TPCSETCALCHAINMASKSRSP = 268,
        _OP2_READ_FIELD = 500,
        _OP2_WRITE_FIELD = 501
    };

    enum WLAN_SINGLE_CHAIN_MASK
    {
        WLAN_SINGLE_CHAIN_1 = 1,
        WLAN_SINGLE_CHAIN_2 = 2,
        WLAN_SINGLE_CHAIN_3 = 4,
        WLAN_SINGLE_CHAIN_4 = 8,
        WLAN_SINGLE_CHAIN_5 = 16,
        WLAN_SINGLE_CHAIN_6 = 32,
        WLAN_SINGLE_CHAIN_7 = 64,
        WLAN_SINGLE_CHAIN_8 = 128
    };

    enum TriggerType
    {
        FreeRun = 0,
        Edge = 1
    };

    enum ChainMask
    {
        NotSpecified = 0,
        Chain_1 = 1,
        Chain_2 = 2,
        Chains_1_2 = 3,
        Chain_3 = 4,
        Chains_1_3 = 5,
        Chains_2_3 = 6,
        Chains_1_2_3 = 7,
        Chain_4 = 8,
        Chains_1_4 = 9,
        Chains_2_4 = 10,
        Chains_1_2_4 = 11,
        Chains_3_4 = 12,
        Chains_1_3_4 = 13,
        Chains_2_3_4 = 14,
        Chains_1_2_3_4 = 15,
        Chain_5 = 16
    };

    enum TCMD_WLAN_MODE
    {
        TCMD_WLAN_MODE_NOHT = 0,
        TCMD_WLAN_MODE_HT20 = 1,
        TCMD_WLAN_MODE_HT40PLUS = 2,
        TCMD_WLAN_MODE_HT40MINUS = 3,
        TCMD_WLAN_MODE_CCK = 4,
        TCMD_WLAN_MODE_VHT20 = 5,
        TCMD_WLAN_MODE_VHT40PLUS = 6,
        TCMD_WLAN_MODE_VHT40MINUS = 7,
        TCMD_WLAN_MODE_VHT80_0 = 8,
        TCMD_WLAN_MODE_VHT80_1 = 9,
        TCMD_WLAN_MODE_VHT80_2 = 10,
        TCMD_WLAN_MODE_VHT80_3 = 11,
        TCMD_WLAN_MODE_VHT80p80 = 12,
        TCMD_WLAN_MODE_VHT160 = 13,
        TCMD_WLAN_MODE_VHT80P80_20_0 = 21,
        TCMD_WLAN_MODE_VHT80P80_20_1 = 22,
        TCMD_WLAN_MODE_VHT80P80_20_2 = 23,
        TCMD_WLAN_MODE_VHT80P80_20_3 = 24,
        TCMD_WLAN_MODE_VHT80P80_20_4 = 25,
        TCMD_WLAN_MODE_VHT80P80_20_5 = 26,
        TCMD_WLAN_MODE_VHT80P80_20_6 = 27,
        TCMD_WLAN_MODE_VHT80P80_20_7 = 28,
        TCMD_WLAN_MODE_VHT160_20_0 = 29,
        TCMD_WLAN_MODE_VHT160_20_1 = 30,
        TCMD_WLAN_MODE_VHT160_20_2 = 31,
        TCMD_WLAN_MODE_VHT160_20_3 = 32,
        TCMD_WLAN_MODE_VHT160_20_4 = 33,
        TCMD_WLAN_MODE_VHT160_20_5 = 34,
        TCMD_WLAN_MODE_VHT160_20_6 = 35,
        TCMD_WLAN_MODE_VHT160_20_7 = 36
    };

    typedef struct 
    {
        double m_freqErr;

        uint m_capVal;
    }_FREQ_CAP;

    typedef struct
    {
        int StreamCount;
        std::string WifiRate;
    }S_WaveformParameter;

    typedef struct
    {
        WLAN_RATE meWlanRate;
        S_WaveformParameter msWaveformParameter;
    }S_Rate_WaveformParameter;

    const static S_Rate_WaveformParameter WaveformParameters[] =
    {
        { RATE_11B_LONG_1_MBPS,1,"WIFI_11B_LONG_1_MBPS" },
        { RATE_11B_LONG_2_MBPS,1,"WIFI_11B_LONG_2_MBPS" },
        { RATE_11B_LONG_5_5_MBPS,1,"WIFI_11B_LONG_5_5_MBPS" },
        { RATE_11B_LONG_11_MBPS,1,"WIFI_11B_LONG_11_MBPS" },
        { RATE_11B_SHORT_2_MBPS,1,"WIFI_11B_SHORT_2_MBPS" },
        {
            RATE_11B_SHORT_5_5_MBPS,   
            {
                1,"WIFI_11B_SHORT_5_5_MBPS"
            }
        },
        {
            RATE_11B_SHORT_11_MBPS,

            {
                1,
                    "WIFI_11B_SHORT_11_MBPS"
            }
        },
        {
            RATE_11A_6_MBPS,

            {
                1,
                    "WIFI_11A_6_MBPS"
            }
        },
        {
            RATE_11A_9_MBPS,

            {
                1,
                    "WIFI_11A_9_MBPS"
            }
        },
        {
            RATE_11A_12_MBPS,

            {
                1,
                    "WIFI_11A_12_MBPS"
            }
        },
        {
            RATE_11A_18_MBPS,

            {
                1,
                    "WIFI_11A_18_MBPS"
            }
        },
        {
            RATE_11A_24_MBPS,

            {
                1,
                    "WIFI_11A_24_MBPS"
            }
        },
        {
            RATE_11A_36_MBPS,

            {
                1,
                    "WIFI_11A_36_MBPS"
            }
        },
        {
            RATE_11A_48_MBPS,

            {
                1,
                    "WIFI_11A_48_MBPS"
            }
        },
        {
            RATE_11A_54_MBPS,

            {
                1,
                    "WIFI_11A_54_MBPS"
            }
        },
        {
            RATE_11N_HT20_MCS0,

            {
                1,
                    "WIFI_11N_HT20_MCS0"
            }
        },
        {
            RATE_11N_HT20_MCS1,

            {
                1,
                    "WIFI_11N_HT20_MCS1"
            }
        },
        {
            RATE_11N_HT20_MCS2,

            {
                1,
                    "WIFI_11N_HT20_MCS2"
            }
        },
        {
            RATE_11N_HT20_MCS3,

            {
                1,
                    "WIFI_11N_HT20_MCS3"
            }
        },
        {
            RATE_11N_HT20_MCS4,

            {
                1,
                    "WIFI_11N_HT20_MCS4"
            }
        },
        {
            RATE_11N_HT20_MCS5,

            {
                1,
                    "WIFI_11N_HT20_MCS5"
            }
        },
        {
            RATE_11N_HT20_MCS6,

            {
                1,
                    "WIFI_11N_HT20_MCS6"
            }
        },
        {
            RATE_11N_HT20_MCS7,

            {
                1,
                    "WIFI_11N_HT20_MCS7"
            }
        },
        {
            RATE_11N_HT20_MCS8,

            {
                2,
                    "WIFI_11N_HT20_MCS0"
            }
        },
        {
            RATE_11N_HT20_MCS9,

            {
                2,
                    "WIFI_11N_HT20_MCS1"
            }
        },
        {
            RATE_11N_HT20_MCS10,

            {
                2,
                    "WIFI_11N_HT20_MCS2"
            }
        },
        {
            RATE_11N_HT20_MCS11,

            {
                2,
                    "WIFI_11N_HT20_MCS3"
            }
        },
        {
            RATE_11N_HT20_MCS12,

            {
                2,
                    "WIFI_11N_HT20_MCS4"
            }
        },
        {
            RATE_11N_HT20_MCS13,

            {
                2,
                    "WIFI_11N_HT20_MCS5"
            }
        },
        {
            RATE_11N_HT20_MCS14,

            {
                2,
                    "WIFI_11N_HT20_MCS6"
            }
        },
        {
            RATE_11N_HT20_MCS15,

            {
                2,
                    "WIFI_11N_HT20_MCS7"
            }
        },
        {
            RATE_11N_HT20_MCS16,

            {
                3,
                    "WIFI_11N_HT20_MCS0"
            }
        },
        {
            RATE_11N_HT20_MCS17,

            {
                3,
                    "WIFI_11N_HT20_MCS1"
            }
        },
        {
            RATE_11N_HT20_MCS18,

            {
                3,
                    "WIFI_11N_HT20_MCS2"
            }
        },
        {
            RATE_11N_HT20_MCS19,

            {
                3,
                    "WIFI_11N_HT20_MCS3"
            }
        },
        {
            RATE_11N_HT20_MCS20,

            {
                3,
                    "WIFI_11N_HT20_MCS4"
            }
        },
        {
            RATE_11N_HT20_MCS21,

            {
                3,
                    "WIFI_11N_HT20_MCS5"
            }
        },
        {
            RATE_11N_HT20_MCS22,

            {
                3,
                    "WIFI_11N_HT20_MCS6"
            }
        },
        {
            RATE_11N_HT20_MCS23,

            {
                3,
                    "WIFI_11N_HT20_MCS7"
            }
        },
        {
            RATE_11N_HT20_MCS24,

            {
                4,
                    "WIFI_11N_HT20_MCS0"
            }
        },
        {
            RATE_11N_HT20_MCS25,

            {
                4,
                    "WIFI_11N_HT20_MCS1"
            }
        },
        {
            RATE_11N_HT20_MCS26,

            {
                4,
                    "WIFI_11N_HT20_MCS2"
            }
        },
        {
            RATE_11N_HT20_MCS27,

            {
                4,
                    "WIFI_11N_HT20_MCS3"
            }
        },
        {
            RATE_11N_HT20_MCS28,

            {
                4,
                    "WIFI_11N_HT20_MCS4"
            }
        },
        {
            RATE_11N_HT20_MCS29,

            {
                4,
                    "WIFI_11N_HT20_MCS5"
            }
        },
        {
            RATE_11N_HT20_MCS30,

            {
                4,
                    "WIFI_11N_HT20_MCS6"
            }
        },
        {
            RATE_11N_HT20_MCS31,

            {
                4,
                    "WIFI_11N_HT20_MCS7"
            }
        },
        {
            RATE_11N_HT40_MCS0,

            {
                1,
                    "WIFI_11N_HT40_MCS0"
            }
        },
        {
            RATE_11N_HT40_MCS1,

            {
                1,
                    "WIFI_11N_HT40_MCS1"
            }
        },
        {
            RATE_11N_HT40_MCS2,

            {
                1,
                    "WIFI_11N_HT40_MCS2"
            }
        },
        {
            RATE_11N_HT40_MCS3,

            {
                1,
                    "WIFI_11N_HT40_MCS3"
            }
        },
        {
            RATE_11N_HT40_MCS4,

            {
                1,
                    "WIFI_11N_HT40_MCS4"
            }
        },
        {
            RATE_11N_HT40_MCS5,

            {
                1,
                    "WIFI_11N_HT40_MCS5"
            }
        },
        {
            RATE_11N_HT40_MCS6,

            {
                1,
                    "WIFI_11N_HT40_MCS6"
            }
        },
        {
            RATE_11N_HT40_MCS7,

            {
                1,
                    "WIFI_11N_HT40_MCS7"
            }
        },
        {
            RATE_11N_HT40_MCS8,

            {
                2,
                    "WIFI_11N_HT40_MCS0"
            }
        },
        {
            RATE_11N_HT40_MCS9,

            {
                2,
                    "WIFI_11N_HT40_MCS1"
            }
        },
        {
            RATE_11N_HT40_MCS10,

            {
                2,
                    "WIFI_11N_HT40_MCS2"
            }
        },
        {
            RATE_11N_HT40_MCS11,

            {
                2,
                    "WIFI_11N_HT40_MCS3"
            }
        },
        {
            RATE_11N_HT40_MCS12,

            {
                2,
                    "WIFI_11N_HT40_MCS4"
            }
        },
        {
            RATE_11N_HT40_MCS13,

            {
                2,
                    "WIFI_11N_HT40_MCS5"
            }
        },
        {
            RATE_11N_HT40_MCS14,

            {
                2,
                    "WIFI_11N_HT40_MCS6"
            }
        },
        {
            RATE_11N_HT40_MCS15,

            {
                2,
                    "WIFI_11N_HT40_MCS7"
            }
        },
        {
            RATE_11N_HT40_MCS16,

            {
                3,
                    "WIFI_11N_HT40_MCS0"
            }
        },
        {
            RATE_11N_HT40_MCS17,

            {
                3,
                    "WIFI_11N_HT40_MCS1"
            }
        },
        {
            RATE_11N_HT40_MCS18,

            {
                3,
                    "WIFI_11N_HT40_MCS2"
            }
        },
        {
            RATE_11N_HT40_MCS19,

            {
                3,
                    "WIFI_11N_HT40_MCS3"
            }
        },
        {
            RATE_11N_HT40_MCS20,

            {
                3,
                    "WIFI_11N_HT40_MCS4"
            }
        },
        {
            RATE_11N_HT40_MCS21,

            {
                3,
                    "WIFI_11N_HT40_MCS5"
            }
        },
        {
            RATE_11N_HT40_MCS22,

            {
                3,
                    "WIFI_11N_HT40_MCS6"
            }
        },
        {
            RATE_11N_HT40_MCS23,

            {
                3,
                    "WIFI_11N_HT40_MCS7"
            }
        },
        {
            RATE_11N_HT40_MCS24,

            {
                4,
                    "WIFI_11N_HT40_MCS0"
            }
        },
        {
            RATE_11N_HT40_MCS25,

            {
                4,
                    "WIFI_11N_HT40_MCS1"
            }
        },
        {
            RATE_11N_HT40_MCS26,

            {
                4,
                    "WIFI_11N_HT40_MCS2"
            }
        },
        {
            RATE_11N_HT40_MCS27,

            {
                4,
                    "WIFI_11N_HT40_MCS3"
            }
        },
        {
            RATE_11N_HT40_MCS28,

            {
                4,
                    "WIFI_11N_HT40_MCS4"
            }
        },
        {
            RATE_11N_HT40_MCS29,

            {
                4,
                    "WIFI_11N_HT40_MCS5"
            }
        },
        {
            RATE_11N_HT40_MCS30,

            {
                4,
                    "WIFI_11N_HT40_MCS6"
            }
        },
        {
            RATE_11N_HT40_MCS31,

            {
                4,
                    "WIFI_11N_HT40_MCS7"
            }
        },
        {
            RATE_11AC_HT20_MCS0,

            {
                1,
                    "WIFI_11AC_HT20_MCS0"
            }
        },
        {
            RATE_11AC_HT20_MCS1,

            {
                1,
                    "WIFI_11AC_HT20_MCS1"
            }
        },
        {
            RATE_11AC_HT20_MCS2,

            {
                1,
                    "WIFI_11AC_HT20_MCS2"
            }
        },
        {
            RATE_11AC_HT20_MCS3,

            {
                1,
                    "WIFI_11AC_HT20_MCS3"
            }
        },
        {
            RATE_11AC_HT20_MCS4,

            {
                1,
                    "WIFI_11AC_HT20_MCS4"
            }
        },
        {
            RATE_11AC_HT20_MCS5,

            {
                1,
                    "WIFI_11AC_HT20_MCS5"
            }
        },
        {
            RATE_11AC_HT20_MCS6,

            {
                1,
                    "WIFI_11AC_HT20_MCS6"
            }
        },
        {
            RATE_11AC_HT20_MCS7,

            {
                1,
                    "WIFI_11AC_HT20_MCS7"
            }
        },
        {
            RATE_11AC_HT20_MCS8,

            {
                1,
                    "WIFI_11AC_HT20_MCS8"
            }
        },
        {
            RATE_11AC_HT20_MCS9,

            {
                1,
                    "WIFI_11AC_HT20_MCS9"
            }
        },
        {
            RATE_11AC_HT20_MCS10,

            {
                2,
                    "WIFI_11AC_HT20_MCS0"
            }
        },
        {
            RATE_11AC_HT20_MCS11,

            {
                2,
                    "WIFI_11AC_HT20_MCS1"
            }
        },
        {
            RATE_11AC_HT20_MCS12,

            {
                2,
                    "WIFI_11AC_HT20_MCS2"
            }
        },
        {
            RATE_11AC_HT20_MCS13,

            {
                2,
                    "WIFI_11AC_HT20_MCS3"
            }
        },
        {
            RATE_11AC_HT20_MCS14,

            {
                2,
                    "WIFI_11AC_HT20_MCS4"
            }
        },
        {
            RATE_11AC_HT20_MCS15,

            {
                2,
                    "WIFI_11AC_HT20_MCS5"
            }
        },
        {
            RATE_11AC_HT20_MCS16,

            {
                2,
                    "WIFI_11AC_HT20_MCS6"
            }
        },
        {
            RATE_11AC_HT20_MCS17,

            {
                2,
                    "WIFI_11AC_HT20_MCS7"
            }
        },
        {
            RATE_11AC_HT20_MCS18,

            {
                2,
                    "WIFI_11AC_HT20_MCS8"
            }
        },
        {
            RATE_11AC_HT20_MCS19,

            {
                2,
                    "WIFI_11AC_HT20_MCS9"
            }
        },
        {
            RATE_11AC_HT20_MCS20,

            {
                3,
                    "WIFI_11AC_HT20_MCS0"
            }
        },
        {
            RATE_11AC_HT20_MCS21,

            {
                3,
                    "WIFI_11AC_HT20_MCS1"
            }
        },
        {
            RATE_11AC_HT20_MCS22,

            {
                3,
                    "WIFI_11AC_HT20_MCS2"
            }
        },
        {
            RATE_11AC_HT20_MCS23,

            {
                3,
                    "WIFI_11AC_HT20_MCS3"
            }
        },
        {
            RATE_11AC_HT20_MCS24,

            {
                3,
                    "WIFI_11AC_HT20_MCS4"
            }
        },
        {
            RATE_11AC_HT20_MCS25,

            {
                3,
                    "WIFI_11AC_HT20_MCS5"
            }
        },
        {
            RATE_11AC_HT20_MCS26,

            {
                3,
                    "WIFI_11AC_HT20_MCS6"
            }
        },
        {
            RATE_11AC_HT20_MCS27,

            {
                3,
                    "WIFI_11AC_HT20_MCS7"
            }
        },
        {
            RATE_11AC_HT20_MCS28,

            {
                3,
                    "WIFI_11AC_HT20_MCS8"
            }
        },
        {
            RATE_11AC_HT20_MCS29,

            {
                3,
                    "WIFI_11AC_HT20_MCS9"
            }
        },
        {
            RATE_11AC_HT20_MCS30,

            {
                4,
                    "WIFI_11AC_HT20_MCS0"
            }
        },
        {
            RATE_11AC_HT20_MCS31,

            {
                4,
                    "WIFI_11AC_HT20_MCS1"
            }
        },
        {
            RATE_11AC_HT20_MCS32,

            {
                4,
                    "WIFI_11AC_HT20_MCS2"
            }
        },
        {
            RATE_11AC_HT20_MCS33,

            {
                4,
                    "WIFI_11AC_HT20_MCS3"
            }
        },
        {
            RATE_11AC_HT20_MCS34,

            {
                4,
                    "WIFI_11AC_HT20_MCS4"
            }
        },
        {
            RATE_11AC_HT20_MCS35,

            {
                4,
                    "WIFI_11AC_HT20_MCS5"
            }
        },
        {
            RATE_11AC_HT20_MCS36,

            {
                4,
                    "WIFI_11AC_HT20_MCS6"
            }
        },
        {
            RATE_11AC_HT20_MCS37,

            {
                4,
                    "WIFI_11AC_HT20_MCS7"
            }
        },
        {
            RATE_11AC_HT20_MCS38,

            {
                4,
                    "WIFI_11AC_HT20_MCS8"
            }
        },
        {
            RATE_11AC_HT20_MCS39,

            {
                4,
                    "WIFI_11AC_HT20_MCS9"
            }
        },
        {
            RATE_11AC_HT40_MCS0,

            {
                1,
                    "WIFI_11AC_HT40_MCS0"
            }
        },
        {
            RATE_11AC_HT40_MCS1,

            {
                1,
                    "WIFI_11AC_HT40_MCS1"
            }
        },
        {
            RATE_11AC_HT40_MCS2,

            {
                1,
                    "WIFI_11AC_HT40_MCS2"
            }
        },
        {
            RATE_11AC_HT40_MCS3,

            {
                1,
                    "WIFI_11AC_HT40_MCS3"
            }
        },
        {
            RATE_11AC_HT40_MCS4,

            {
                1,
                    "WIFI_11AC_HT40_MCS4"
            }
        },
        {
            RATE_11AC_HT40_MCS5,

            {
                1,
                    "WIFI_11AC_HT40_MCS5"
            }
        },
        {
            RATE_11AC_HT40_MCS6,

            {
                1,
                    "WIFI_11AC_HT40_MCS6"
            }
        },
        {
            RATE_11AC_HT40_MCS7,

            {
                1,
                    "WIFI_11AC_HT40_MCS7"
            }
        },
        {
            RATE_11AC_HT40_MCS8,

            {
                1,
                    "WIFI_11AC_HT40_MCS8"
            }
        },
        {
            RATE_11AC_HT40_MCS9,

            {
                1,
                    "WIFI_11AC_HT40_MCS9"
            }
        },
        {
            RATE_11AC_HT40_MCS10,

            {
                2,
                    "WIFI_11AC_HT40_MCS0"
            }
        },
        {
            RATE_11AC_HT40_MCS11,

            {
                2,
                    "WIFI_11AC_HT40_MCS1"
            }
        },
        {
            RATE_11AC_HT40_MCS12,

            {
                2,
                    "WIFI_11AC_HT40_MCS2"
            }
        },
        {
            RATE_11AC_HT40_MCS13,

            {
                2,
                    "WIFI_11AC_HT40_MCS3"
            }
        },
        {
            RATE_11AC_HT40_MCS14,

            {
                2,
                    "WIFI_11AC_HT40_MCS4"
            }
        },
        {
            RATE_11AC_HT40_MCS15,

            {
                2,
                    "WIFI_11AC_HT40_MCS5"
            }
        },
        {
            RATE_11AC_HT40_MCS16,

            {
                2,
                    "WIFI_11AC_HT40_MCS6"
            }
        },
        {
            RATE_11AC_HT40_MCS17,

            {
                2,
                    "WIFI_11AC_HT40_MCS7"
            }
        },
        {
            RATE_11AC_HT40_MCS18,

            {
                2,
                    "WIFI_11AC_HT40_MCS8"
            }
        },
        {
            RATE_11AC_HT40_MCS19,

            {
                2,
                    "WIFI_11AC_HT40_MCS9"
            }
        },
        {
            RATE_11AC_HT40_MCS20,

            {
                3,
                    "WIFI_11AC_HT40_MCS0"
            }
        },
        {
            RATE_11AC_HT40_MCS21,

            {
                3,
                    "WIFI_11AC_HT40_MCS1"
            }
        },
        {
            RATE_11AC_HT40_MCS22,

            {
                3,
                    "WIFI_11AC_HT40_MCS2"
            }
        },
        {
            RATE_11AC_HT40_MCS23,

            {
                3,
                    "WIFI_11AC_HT40_MCS3"
            }
        },
        {
            RATE_11AC_HT40_MCS24,

            {
                3,
                    "WIFI_11AC_HT40_MCS4"
            }
        },
        {
            RATE_11AC_HT40_MCS25,

            {
                3,
                    "WIFI_11AC_HT40_MCS5"
            }
        },
        {
            RATE_11AC_HT40_MCS26,

            {
                3,
                    "WIFI_11AC_HT40_MCS6"
            }
        },
        {
            RATE_11AC_HT40_MCS27,

            {
                3,
                    "WIFI_11AC_HT40_MCS7"
            }
        },
        {
            RATE_11AC_HT40_MCS28,

            {
                3,
                    "WIFI_11AC_HT40_MCS8"
            }
        },
        {
            RATE_11AC_HT40_MCS29,

            {
                3,
                    "WIFI_11AC_HT40_MCS9"
            }
        },
        {
            RATE_11AC_HT40_MCS30,

            {
                4,
                    "WIFI_11AC_HT40_MCS0"
            }
        },
        {
            RATE_11AC_HT40_MCS31,

            {
                4,
                    "WIFI_11AC_HT40_MCS1"
            }
        },
        {
            RATE_11AC_HT40_MCS32,

            {
                4,
                    "WIFI_11AC_HT40_MCS2"
            }
        },
        {
            RATE_11AC_HT40_MCS33,

            {
                4,
                    "WIFI_11AC_HT40_MCS3"
            }
        },
        {
            RATE_11AC_HT40_MCS34,

            {
                4,
                    "WIFI_11AC_HT40_MCS4"
            }
        },
        {
            RATE_11AC_HT40_MCS35,

            {
                4,
                    "WIFI_11AC_HT40_MCS5"
            }
        },
        {
            RATE_11AC_HT40_MCS36,

            {
                4,
                    "WIFI_11AC_HT40_MCS6"
            }
        },
        {
            RATE_11AC_HT40_MCS37,

            {
                4,
                    "WIFI_11AC_HT40_MCS7"
            }
        },
        {
            RATE_11AC_HT40_MCS38,

            {
                4,
                    "WIFI_11AC_HT40_MCS8"
            }
        },
        {
            RATE_11AC_HT40_MCS39,

            {
                4,
                    "WIFI_11AC_HT40_MCS9"
            }
        },
        {
            RATE_11AC_HT80_MCS0,

            {
                1,
                    "WIFI_11AC_HT80_MCS0"
            }
        },
        {
            RATE_11AC_HT80_MCS1,

            {
                1,
                    "WIFI_11AC_HT80_MCS1"
            }
        },
        {
            RATE_11AC_HT80_MCS2,

            {
                1,
                    "WIFI_11AC_HT80_MCS2"
            }
        },
        {
            RATE_11AC_HT80_MCS3,

            {
                1,
                    "WIFI_11AC_HT80_MCS3"
            }
        },
        {
            RATE_11AC_HT80_MCS4,

            {
                1,
                    "WIFI_11AC_HT80_MCS4"
            }
        },
        {
            RATE_11AC_HT80_MCS5,

            {
                1,
                    "WIFI_11AC_HT80_MCS5"
            }
        },
        {
            RATE_11AC_HT80_MCS6,

            {
                1,
                    "WIFI_11AC_HT80_MCS6"
            }
        },
        {
            RATE_11AC_HT80_MCS7,

            {
                1,
                    "WIFI_11AC_HT80_MCS7"
            }
        },
        {
            RATE_11AC_HT80_MCS8,

            {
                1,
                    "WIFI_11AC_HT80_MCS8"
            }
        },
        {
            RATE_11AC_HT80_MCS9,

            {
                1,
                    "WIFI_11AC_HT80_MCS9"
            }
        },
        {
            RATE_11AC_HT80_MCS10,

            {
                2,
                    "WIFI_11AC_HT80_MCS0"
            }
        },
        {
            RATE_11AC_HT80_MCS11,

            {
                2,
                    "WIFI_11AC_HT80_MCS1"
            }
        },
        {
            RATE_11AC_HT80_MCS12,

            {
                2,
                    "WIFI_11AC_HT80_MCS2"
            }
        },
        {
            RATE_11AC_HT80_MCS13,

            {
                2,
                    "WIFI_11AC_HT80_MCS3"
            }
        },
        {
            RATE_11AC_HT80_MCS14,

            {
                2,
                    "WIFI_11AC_HT80_MCS4"
            }
        },
        {
            RATE_11AC_HT80_MCS15,

            {
                2,
                    "WIFI_11AC_HT80_MCS5"
            }
        },
        {
            RATE_11AC_HT80_MCS16,

            {
                2,
                    "WIFI_11AC_HT80_MCS6"
            }
        },
        {
            RATE_11AC_HT80_MCS17,

            {
                2,
                    "WIFI_11AC_HT80_MCS7"
            }
        },
        {
            RATE_11AC_HT80_MCS18,

            {
                2,
                    "WIFI_11AC_HT80_MCS8"
            }
        },
        {
            RATE_11AC_HT80_MCS19,

            {
                2,
                    "WIFI_11AC_HT80_MCS9"
            }
        },
        {
            RATE_11AC_HT80_MCS20,

            {
                3,
                    "WIFI_11AC_HT80_MCS0"
            }
        },
        {
            RATE_11AC_HT80_MCS21,

            {
                3,
                    "WIFI_11AC_HT80_MCS1"
            }
        },
        {
            RATE_11AC_HT80_MCS22,

            {
                3,
                    "WIFI_11AC_HT80_MCS2"
            }
        },
        {
            RATE_11AC_HT80_MCS23,

            {
                3,
                    "WIFI_11AC_HT80_MCS3"
            }
        },
        {
            RATE_11AC_HT80_MCS24,

            {
                3,
                    "WIFI_11AC_HT80_MCS4"
            }
        },
        {
            RATE_11AC_HT80_MCS25,

            {
                3,
                    "WIFI_11AC_HT80_MCS5"
            }
        },
        {
            RATE_11AC_HT80_MCS26,

            {
                3,
                    "WIFI_11AC_HT80_MCS6"
            }
        },
        {
            RATE_11AC_HT80_MCS27,

            {
                3,
                    "WIFI_11AC_HT80_MCS7"
            }
        },
        {
            RATE_11AC_HT80_MCS28,

            {
                3,
                    "WIFI_11AC_HT80_MCS8"
            }
        },
        {
            RATE_11AC_HT80_MCS29,

            {
                3,
                    "WIFI_11AC_HT80_MCS9"
            }
        },
        {
            RATE_11AC_HT80_MCS30,

            {
                4,
                    "WIFI_11AC_HT80_MCS0"
            }
        },
        {
            RATE_11AC_HT80_MCS31,

            {
                4,
                    "WIFI_11AC_HT80_MCS1"
            }
        },
        {
            RATE_11AC_HT80_MCS32,

            {
                4,
                    "WIFI_11AC_HT80_MCS2"
            }
        },
        {
            RATE_11AC_HT80_MCS33,

            {
                4,
                    "WIFI_11AC_HT80_MCS3"
            }
        },
        {
            RATE_11AC_HT80_MCS34,

            {
                4,
                    "WIFI_11AC_HT80_MCS4"
            }
        },
        {
            RATE_11AC_HT80_MCS35,

            {
                4,
                    "WIFI_11AC_HT80_MCS5"
            }
        },
        {
            RATE_11AC_HT80_MCS36,

            {
                4,
                    "WIFI_11AC_HT80_MCS6"
            }
        },
        {
            RATE_11AC_HT80_MCS37,

            {
                4,
                    "WIFI_11AC_HT80_MCS7"
            }
        },
        {
            RATE_11AC_HT80_MCS38,

            {
                4,
                    "WIFI_11AC_HT80_MCS8"
            }
        },
        {
            RATE_11AC_HT80_MCS39,

            {
                4,
                    "WIFI_11AC_HT80_MCS9"
            }
        },
        {
            RATE_11AC_HT160_MCS0,

            {
                1,
                    "WIFI_11AC_HT160_MCS0"
            }
        },
        {
            RATE_11AC_HT160_MCS1,

            {
                1,
                    "WIFI_11AC_HT160_MCS1"
            }
        },
        {
            RATE_11AC_HT160_MCS2,

            {
                1,
                    "WIFI_11AC_HT160_MCS2"
            }
        },
        {
            RATE_11AC_HT160_MCS3,

            {
                1,
                    "WIFI_11AC_HT160_MCS3"
            }
        },
        {
            RATE_11AC_HT160_MCS4,

            {
                1,
                    "WIFI_11AC_HT160_MCS4"
            }
        },
        {
            RATE_11AC_HT160_MCS5,

            {
                1,
                    "WIFI_11AC_HT160_MCS5"
            }
        },
        {
            RATE_11AC_HT160_MCS6,

            {
                1,
                    "WIFI_11AC_HT160_MCS6"
            }
        },
        {
            RATE_11AC_HT160_MCS7,

            {
                1,
                    "WIFI_11AC_HT160_MCS7"
            }
        },
        {
            RATE_11AC_HT160_MCS8,

            {
                1,
                    "WIFI_11AC_HT160_MCS8"
            }
        },
        {
            RATE_11AC_HT160_MCS9,

            {
                1,
                    "WIFI_11AC_HT160_MCS9"
            }
        },
        {
            RATE_11AC_HT160_MCS10,

            {
                2,
                    "WIFI_11AC_HT160_MCS0"
            }
        },
        {
            RATE_11AC_HT160_MCS11,

            {
                2,
                    "WIFI_11AC_HT160_MCS1"
            }
        },
        {
            RATE_11AC_HT160_MCS12,

            {
                2,
                    "WIFI_11AC_HT160_MCS2"
            }
        },
        {
            RATE_11AC_HT160_MCS13,

            {
                2,
                    "WIFI_11AC_HT160_MCS3"
            }
        },
        {
            RATE_11AC_HT160_MCS14,

            {
                2,
                    "WIFI_11AC_HT160_MCS4"
            }
        },
        {
            RATE_11AC_HT160_MCS15,

            {
                2,
                    "WIFI_11AC_HT160_MCS5"
            }
        },
        {
            RATE_11AC_HT160_MCS16,

            {
                2,
                    "WIFI_11AC_HT160_MCS6"
            }
        },
        {
            RATE_11AC_HT160_MCS17,

            {
                2,
                    "WIFI_11AC_HT160_MCS7"
            }
        },
        {
            RATE_11AC_HT160_MCS18,

            {
                2,
                    "WIFI_11AC_HT160_MCS8"
            }
        },
        {
            RATE_11AC_HT160_MCS19,

            {
                2,
                    "WIFI_11AC_HT160_MCS9"
            }
        },
        {
            RATE_11AC_HT160_MCS20,

            {
                3,
                    "WIFI_11AC_HT160_MCS0"
            }
        },
        {
            RATE_11AC_HT160_MCS21,

            {
                3,
                    "WIFI_11AC_HT160_MCS1"
            }
        },
        {
            RATE_11AC_HT160_MCS22,

            {
                3,
                    "WIFI_11AC_HT160_MCS2"
            }
        },
        {
            RATE_11AC_HT160_MCS23,

            {
                3,
                    "WIFI_11AC_HT160_MCS3"
            }
        },
        {
            RATE_11AC_HT160_MCS24,

            {
                3,
                    "WIFI_11AC_HT160_MCS4"
            }
        },
        {
            RATE_11AC_HT160_MCS25,

            {
                3,
                    "WIFI_11AC_HT160_MCS5"
            }
        },
        {
            RATE_11AC_HT160_MCS26,

            {
                3,
                    "WIFI_11AC_HT160_MCS6"
            }
        },
        {
            RATE_11AC_HT160_MCS27,

            {
                3,
                    "WIFI_11AC_HT160_MCS7"
            }
        },
        {
            RATE_11AC_HT160_MCS28,

            {
                3,
                    "WIFI_11AC_HT160_MCS8"
            }
        },
        {
            RATE_11AC_HT160_MCS29,

            {
                3,
                    "WIFI_11AC_HT160_MCS9"
            }
        },
        {
            RATE_11AC_HT160_MCS30,

            {
                4,
                    "WIFI_11AC_HT160_MCS0"
            }
        },
        {
            RATE_11AC_HT160_MCS31,

            {
                4,
                    "WIFI_11AC_HT160_MCS1"
            }
        },
        {
            RATE_11AC_HT160_MCS32,

            {
                4,
                    "WIFI_11AC_HT160_MCS2"
            }
        },
        {
            RATE_11AC_HT160_MCS33,

            {
                4,
                    "WIFI_11AC_HT160_MCS3"
            }
        },
        {
            RATE_11AC_HT160_MCS34,

            {
                4,
                    "WIFI_11AC_HT160_MCS4"
            }
        },
        {
            RATE_11AC_HT160_MCS35,

            {
                4,
                    "WIFI_11AC_HT160_MCS5"
            }
        },
        {
            RATE_11AC_HT160_MCS36,

            {
                4,
                    "WIFI_11AC_HT160_MCS6"
            }
        },
        {
            RATE_11AC_HT160_MCS37,

            {
                4,
                    "WIFI_11AC_HT160_MCS7"
            }
        },
        {
            RATE_11AC_HT160_MCS38,

            {
                4,
                    "WIFI_11AC_HT160_MCS8"
            }
        },
        {
            RATE_11AC_HT160_MCS39,

            {
                4,
                    "WIFI_11AC_HT160_MCS9"
            }
        },
        {
            RATE_11AC_HT80P80_MCS0,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS0"
            }
        },
        {
            RATE_11AC_HT80P80_MCS1,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS1"
            }
        },
        {
            RATE_11AC_HT80P80_MCS2,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS2"
            }
        },
        {
            RATE_11AC_HT80P80_MCS3,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS3"
            }
        },
        {
            RATE_11AC_HT80P80_MCS4,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS4"
            }
        },
        {
            RATE_11AC_HT80P80_MCS5,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS5"
            }
        },
        {
            RATE_11AC_HT80P80_MCS6,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS6"
            }
        },
        {
            RATE_11AC_HT80P80_MCS7,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS7"
            }
        },
        {
            RATE_11AC_HT80P80_MCS8,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS8"
            }
        },
        {
            RATE_11AC_HT80P80_MCS9,

            {
                1,
                    "WIFI_11AC_HT80P80_MCS9"
            }
        },
        {
            RATE_11AC_HT80P80_MCS10,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS0"
            }
        },
        {
            RATE_11AC_HT80P80_MCS11,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS1"
            }
        },
        {
            RATE_11AC_HT80P80_MCS12,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS2"
            }
        },
        {
            RATE_11AC_HT80P80_MCS13,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS3"
            }
        },
        {
            RATE_11AC_HT80P80_MCS14,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS4"
            }
        },
        {
            RATE_11AC_HT80P80_MCS15,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS5"
            }
        },
        {
            RATE_11AC_HT80P80_MCS16,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS6"
            }
        },
        {
            RATE_11AC_HT80P80_MCS17,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS7"
            }
        },
        {
            RATE_11AC_HT80P80_MCS18,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS8"
            }
        },
        {
            RATE_11AC_HT80P80_MCS19,

            {
                2,
                    "WIFI_11AC_HT80P80_MCS9"
            }
        },
        {
            RATE_11AC_HT80P80_MCS20,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS0"
            }
        },
        {
            RATE_11AC_HT80P80_MCS21,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS1"
            }
        },
        {
            RATE_11AC_HT80P80_MCS22,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS2"
            }
        },
        {
            RATE_11AC_HT80P80_MCS23,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS3"
            }
        },
        {
            RATE_11AC_HT80P80_MCS24,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS4"
            }
        },
        {
            RATE_11AC_HT80P80_MCS25,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS5"
            }
        },
        {
            RATE_11AC_HT80P80_MCS26,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS6"
            }
        },
        {
            RATE_11AC_HT80P80_MCS27,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS7"
            }
        },
        {
            RATE_11AC_HT80P80_MCS28,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS8"
            }
        },
        {
            RATE_11AC_HT80P80_MCS29,

            {
                3,
                    "WIFI_11AC_HT80P80_MCS9"
            }
        },
        {
            RATE_11AC_HT80P80_MCS30,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS0"
            }
        },
        {
            RATE_11AC_HT80P80_MCS31,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS1"
            }
        },
        {
            RATE_11AC_HT80P80_MCS32,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS2"
            }
        },
        {
            RATE_11AC_HT80P80_MCS33,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS3"
            }
        },
        {
            RATE_11AC_HT80P80_MCS34,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS4"
            }
        },
        {
            RATE_11AC_HT80P80_MCS35,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS5"
            }
        },
        {
            RATE_11AC_HT80P80_MCS36,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS6"
            }
        },
        {
            RATE_11AC_HT80P80_MCS37,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS7"
            }
        },
        {
            RATE_11AC_HT80P80_MCS38,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS8"
            }
        },
        {
            RATE_11AC_HT80P80_MCS39,

            {
                4,
                    "WIFI_11AC_HT80P80_MCS9"
            }
        },
        {
            RATE_11P_BW5_1p5_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS0"
            }
        },
        {
            RATE_11P_BW5_2p25_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS1"
            }
        },
        {
            RATE_11P_BW5_3_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS2"
            }
        },
        {
            RATE_11P_BW5_4p5_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS3"
            }
        },
        {
            RATE_11P_BW5_6_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS4"
            }
        },
        {
            RATE_11P_BW5_9_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS5"
            }
        },
        {
            RATE_11P_BW5_12_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS6"
            }
        },
        {
            RATE_11P_BW5_13p5_MBPS,

            {
                1,
                    "WIFI_11P_BW5_MCS7"
            }
        },
        {
            RATE_11P_BW10_3_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS0"
            }
        },
        {
            RATE_11P_BW10_4p5_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS1"
            }
        },
        {
            RATE_11P_BW10_6_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS2"
            }
        },
        {
            RATE_11P_BW10_9_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS3"
            }
        },
        {
            RATE_11P_BW10_12_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS4"
            }
        },
        {
            RATE_11P_BW10_18_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS5"
            }
        },
        {
            RATE_11P_BW10_24_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS6"
            }
        },
        {
            RATE_11P_BW10_27_MBPS,

            {
                1,
                    "WIFI_11P_BW10_MCS7"
            }
        },
        {
            RATE_11P_BW20_6_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS0"
            }
        },
        {
            RATE_11P_BW20_9_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS1"
            }
        },
        {
            RATE_11P_BW20_12_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS2"
            }
        },
        {
            RATE_11P_BW20_18_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS3"
            }
        },
        {
            RATE_11P_BW20_24_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS4"
            }
        },
        {
            RATE_11P_BW20_36_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS5"
            }
        },
        {
            RATE_11P_BW20_48_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS6"
            }
        },
        {
            RATE_11P_BW20_54_MBPS,

            {
                1,
                    "WIFI_11P_BW20_MCS7"
            }
        },
        {
            RATE_11N,

            {
                1,
                    "WIFI_11N"
            }
        },
        {
            RATE_11P,

            {
                1,
                    "WIFI_11P"
            }
        },
        {
            RATE_11AC,

            {
                1,
                    "WIFI_11AC"
            }
        },
        {
            RATE_11AC_80P80,

            {
                1,
                    "WIFI_11AC_80P80"
            }
        },
        {
            RATE_11AX,

            {
                1,
                    "WIFI_11AX"
            }
        },
        {
            RATE_11AX_80P80,

            {
                1,
                    "WIFI_11AX_80P80"
            }
        },
        {
            RATE_11AX_OFDMA,

            {
                1,
                    "WIFI_11AX_OFDMA"
            }
        },
        {
            RATE_11AX_OFDMA_80P80,

            {
                1,
                    "WIFI_11AX_OFDMA_80P80"
            }
        },
        {
            RATE_11AX_BW20_OFDMA,

            {
                1,
                    "WIFI_11AX_BW20_OFDMA"
            }
        },
        {
            RATE_11AX_BW20_MCS0,

            {
                1,
                    "WIFI_11AX_BW20_MCS0"
            }
        },
        {
            RATE_11AX_BW20_MCS1,

            {
                1,
                    "WIFI_11AX_BW20_MCS1"
            }
        },
        {
            RATE_11AX_BW20_MCS2,

            {
                1,
                    "WIFI_11AX_BW20_MCS2"
            }
        },
        {
            RATE_11AX_BW20_MCS3,

            {
                1,
                    "WIFI_11AX_BW20_MCS3"
            }
        },
        {
            RATE_11AX_BW20_MCS4,

            {
                1,
                    "WIFI_11AX_BW20_MCS4"
            }
        },
        {
            RATE_11AX_BW20_MCS5,

            {
                1,
                    "WIFI_11AX_BW20_MCS5"
            }
        },
        {
            RATE_11AX_BW20_MCS6,

            {
                1,
                    "WIFI_11AX_BW20_MCS6"
            }
        },
        {
            RATE_11AX_BW20_MCS7,

            {
                1,
                    "WIFI_11AX_BW20_MCS7"
            }
        },
        {
            RATE_11AX_BW20_MCS8,

            {
                1,
                    "WIFI_11AX_BW20_MCS8"
            }
        },
        {
            RATE_11AX_BW20_MCS9,

            {
                1,
                    "WIFI_11AX_BW20_MCS9"
            }
        },
        {
            RATE_11AX_BW20_MCS10,

            {
                1,
                    "WIFI_11AX_BW20_MCS10"
            }
        },
        {
            RATE_11AX_BW20_MCS11,

            {
                1,
                    "WIFI_11AX_BW20_MCS11"
            }
        },
        {
            RATE_11AX_BW40_OFDMA,

            {
                1,
                    "WIFI_11AX_BW40_OFDMA"
            }
        },
        {
            RATE_11AX_BW40_MCS0,

            {
                1,
                    "WIFI_11AX_BW40_MCS0"
            }
        },
        {
            RATE_11AX_BW40_MCS1,

            {
                1,
                    "WIFI_11AX_BW40_MCS1"
            }
        },
        {
            RATE_11AX_BW40_MCS2,

            {
                1,
                    "WIFI_11AX_BW40_MCS2"
            }
        },
        {
            RATE_11AX_BW40_MCS3,

            {
                1,
                    "WIFI_11AX_BW40_MCS3"
            }
        },
        {
            RATE_11AX_BW40_MCS4,

            {
                1,
                    "WIFI_11AX_BW40_MCS4"
            }
        },
        {
            RATE_11AX_BW40_MCS5,

            {
                1,
                    "WIFI_11AX_BW40_MCS5"
            }
        },
        {
            RATE_11AX_BW40_MCS6,

            {
                1,
                    "WIFI_11AX_BW40_MCS6"
            }
        },
        {
            RATE_11AX_BW40_MCS7,

            {
                1,
                    "WIFI_11AX_BW40_MCS7"
            }
        },
        {
            RATE_11AX_BW40_MCS8,

            {
                1,
                    "WIFI_11AX_BW40_MCS8"
            }
        },
        {
            RATE_11AX_BW40_MCS9,

            {
                1,
                    "WIFI_11AX_BW40_MCS9"
            }
        },
        {
            RATE_11AX_BW40_MCS10,

            {
                1,
                    "WIFI_11AX_BW40_MCS10"
            }
        },
        {
            RATE_11AX_BW40_MCS11,

            {
                1,
                    "WIFI_11AX_BW40_MCS11"
            }
        },
        {
            RATE_11AX_BW80_OFDMA,

            {
                1,
                    "WIFI_11AX_BW80_OFDMA"
            }
        },
        {
            RATE_11AX_BW80_MCS0,

            {
                1,
                    "WIFI_11AX_BW80_MCS0"
            }
        },
        {
            RATE_11AX_BW80_MCS1,

            {
                1,
                    "WIFI_11AX_BW80_MCS1"
            }
        },
        {
            RATE_11AX_BW80_MCS2,

            {
                1,
                    "WIFI_11AX_BW80_MCS2"
            }
        },
        {
            RATE_11AX_BW80_MCS3,

            {
                1,
                    "WIFI_11AX_BW80_MCS3"
            }
        },
        {
            RATE_11AX_BW80_MCS4,

            {
                1,
                    "WIFI_11AX_BW80_MCS4"
            }
        },
        {
            RATE_11AX_BW80_MCS5,

            {
                1,
                    "WIFI_11AX_BW80_MCS5"
            }
        },
        {
            RATE_11AX_BW80_MCS6,

            {
                1,
                    "WIFI_11AX_BW80_MCS6"
            }
        },
        {
            RATE_11AX_BW80_MCS7,

            {
                1,
                    "WIFI_11AX_BW80_MCS7"
            }
        },
        {
            RATE_11AX_BW80_MCS8,

            {
                1,
                    "WIFI_11AX_BW80_MCS8"
            }
        },
        {
            RATE_11AX_BW80_MCS9,

            {
                1,
                    "WIFI_11AX_BW80_MCS9"
            }
        },
        {
            RATE_11AX_BW80_MCS10,

            {
                1,
                    "WIFI_11AX_BW80_MCS10"
            }
        },
        {
            RATE_11AX_BW80_MCS11,

            {
                1,
                    "WIFI_11AX_BW80_MCS11"
            }
        },
        {
            RATE_11AX_BW80P80_OFDMA,

            {
                1,
                    "WIFI_11AX_BW80P80_OFDMA"
            }
        },
        {
            RATE_11AX_BW80P80_MCS0,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS0"
            }
        },
        {
            RATE_11AX_BW80P80_MCS1,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS1"
            }
        },
        {
            RATE_11AX_BW80P80_MCS2,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS2"
            }
        },
        {
            RATE_11AX_BW80P80_MCS3,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS3"
            }
        },
        {
            RATE_11AX_BW80P80_MCS4,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS4"
            }
        },
        {
            RATE_11AX_BW80P80_MCS5,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS5"
            }
        },
        {
            RATE_11AX_BW80P80_MCS6,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS6"
            }
        },
        {
            RATE_11AX_BW80P80_MCS7,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS7"
            }
        },
        {
            RATE_11AX_BW80P80_MCS8,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS8"
            }
        },
        {
            RATE_11AX_BW80P80_MCS9,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS9"
            }
        },
        {
            RATE_11AX_BW80P80_MCS10,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS10"
            }
        },
        {
            RATE_11AX_BW80P80_MCS11,

            {
                1,
                    "WIFI_11AX_BW80P80_MCS11"
            }
        },
        {
            RATE_CW,

            {
                1,
                    "CW"
            }
        }
    };
}