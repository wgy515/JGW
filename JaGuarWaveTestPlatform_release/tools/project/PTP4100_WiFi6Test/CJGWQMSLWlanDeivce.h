#pragma once
#include <QMSL_Core.h>
#include <string>
#include "PTP4100_QMSLConfigXml.h"
static char* QCN9224_DLLID = "QCN9224";
static unsigned int QCN9224_CHIPID = 0x40;  // This is for legacy chip only. Any value could be accepted for QCN9224.
#define WLANINSTANCE_2G (0)
#define WLANINSTANCE_6G (1)
#define WLANINSTANCE_5G (2)

enum PhyID
{
    PHY_A0 = 0,
    PHY_B = 1,
    PHY_A1 = 2,
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
    EnablingSCPC = 9,
};

enum WLAN_UniversalDataRate
{
    CW = -1,
    RATE_Long_1Mbps = 0,
    RATE_Long_2Mbps = 1,
    RATE_Long_5_5Mbps = 2,
    RATE_Long_11Mbps = 3,
    RATE_Short_2Mbps = 4,
    RATE_Short_5_5Mbps = 5,
    RATE_Short_11Mbps = 6,
    RATE_6Mbps = 10,
    RATE_9Mbps = 11,
    RATE_12Mbps = 12,
    RATE_18Mbps = 13,
    RATE_24Mbps = 14,
    RATE_36Mbps = 0xF,
    RATE_48Mbps = 0x10,
    RATE_54Mbps = 17,
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
    RATE_MCS_11 = 0x1F,
    RATE_MCS_12 = 0x20,
    RATE_MCS_13 = 33,
    RATE_MCS_14 = 34,
    RATE_MCS_15 = 35
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
    RateBW_11AX_OFDMA_HE80P80 = 0xF,
    RateBW_11AX_HE160 = 0x10,
    RateBW_11AX_OFDMA_HE160 = 17,
    RateBW_11AX_HE165 = 18,
    RateBW_11AX_OFDMA_HE165 = 19,
    RateBW_11AC_VHT160 = 20,
    RateBW_11AC_VHT165 = 21,
    RateBW_11AX_HE320 = 22,
    RateBW_11AX_OFDMA_HE320 = 23,
    RateBW_11BE_EHT20 = 30,
    RateBW_11BE_EHT40 = 0x1F,
    RateBW_11BE_EHT80 = 0x20,
    RateBW_11BE_EHT160 = 33,
    RateBW_11BE_EHT320 = 34,
    RateBW_11BE_OFDMA_EHT20 = 35,
    RateBW_11BE_OFDMA_EHT40 = 36,
    RateBW_11BE_OFDMA_EHT80 = 37,
    RateBW_11BE_OFDMA_EHT160 = 38,
    RateBW_11BE_OFDMA_EHT320 = 39,
    RateBW_NON_HT_DUP40 = 50,
    RateBW_NON_HT_DUP80 = 51,
    RateBW_NON_HT_DUP160 = 52,
    RateBW_NON_HT_DUP320 = 53
};

enum WLAN_Gen6_ChannelBondingState
{
    none = 0,
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
    BW165 = 14,
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
    Primary_3rd_20_in_BW160 = 0x1F,
    Primary_4th_20_in_BW160 = 0x20,
    Primary_5th_20_in_BW160 = 33,
    Primary_6th_20_in_BW160 = 34,
    Primary_7th_20_in_BW160 = 35,
    Primary_8th_20_in_BW160 = 36,
    Primary_1st_20_in_BW165 = 37,
    Primary_2nd_20_in_BW165 = 38,
    Primary_3rd_20_in_BW165 = 39,
    Primary_4th_20_in_BW165 = 40,
    Primary_5th_20_in_BW165 = 41,
    Primary_6th_20_in_BW165 = 42,
    Primary_7th_20_in_BW165 = 43,
    Primary_8th_20_in_BW165 = 44,
    Half_Rate = 50,
    Quarter_Rate = 51,
    BW320 = 60,
    Primary_1st_20_in_BW320 = 61,
    Primary_2nd_20_in_BW320 = 62,
    Primary_3rd_20_in_BW320 = 0x3F,
    Primary_4th_20_in_BW320 = 0x40,
    Primary_5th_20_in_BW320 = 65,
    Primary_6th_20_in_BW320 = 66,
    Primary_7th_20_in_BW320 = 67,
    Primary_8th_20_in_BW320 = 68,
    Primary_9th_20_in_BW320 = 69,
    Primary_10th_20_in_BW320 = 70,
    Primary_11th_20_in_BW320 = 71,
    Primary_12th_20_in_BW320 = 72,
    Primary_13th_20_in_BW320 = 73,
    Primary_14th_20_in_BW320 = 74,
    Primary_15th_20_in_BW320 = 75,
    Primary_16th_20_in_BW320 = 76
};

enum WLAN_CHAIN_MASK2
{
    WLAN_CHAIN_NONE = 0,
    WLAN_CHAIN_1 = 1,
    WLAN_CHAIN_2 = 2,
    WLAN_CHAIN_3 = 4,
    WLAN_CHAIN_4 = 8,
    WLAN_CHAIN_12 = 3,
    WLAN_CHAIN_123 = 7,
    WLAN_CHAIN_124 = 11,
    WLAN_CHAIN_1234 = 0xF,
    WLAN_CHAIN_13 = 5,
    WLAN_CHAIN_134 = 13,
    WLAN_CHAIN_14 = 9,
    WLAN_CHAIN_23 = 6,
    WLAN_CHAIN_234 = 14,
    WLAN_CHAIN_24 = 10,
    WLAN_CHAIN_34 = 12
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
//Broadcast/unicast
enum TCMD_TPC_TYPE
{
    TPC_TX_PWR,
    TPC_FORCED_GAIN,
    TPC_TGT_PWR,
    TPC_TX_FORCED_GAIN,
    TPC_FORCED_GAINIDX,
    TPC_FORCED_TGTPWR,
    TPC_FORCED_GLUT_INDEX,
    TPC_FORCED_PER_CHAIN
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

enum WLAN_UniversalWiFtStandard
{
    WiFiStandard_Legacy,
    WiFiStandard_Legacy11AC,
    WiFiStandard_Legacy11AX,
    WiFiStandard_OFDMA
};

enum WLAN_TLV2_OPCODE_NEW
{
    _OP2_SYNC_NEW = 100,
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
    _OP2_TX_NEW = 114,
    _OP2_TXSTATUS = 115,
    _OP2_TXSTATUSRSP = 116,
    _OP2_RX_NEW = 117,
    _OP2_RXSTATUS = 118,
    _OP2_RXSTATUSRSP = 119,
    _OP2_HWCAL = 120,
    _OP2_RXRSP = 121,
    _OP2_XTALCALPROC = 122,
    _OP2_XTALCALPROCRSP = 123,
    _OP2_READCUSTOTPSPACE = 124,
    _OP2_READCUSTOTPSPACERSP = 125,
    _OP2_WRITECUSTOTPSPACE = 126,
    _OP2_WRITECUSTOTPSPACERSP = 0x7F,
    _OP2_GETCUSTOTPSIZE = 0x80,
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
    _OP2_SETPHYRFMODE_NEW = 169,
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
    _OP2_VERQUERY = 200,
    _OP2_VERQUERYRSP = 201,
    _OP2_NOISEFLOORREAD_NEW = 202,
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
    _OP2_LMRXSTATUS = 216,
    _OP2_LMRXSTATUSRSP = 217,
    _OP2_TPCLOGSENABLE = 218,
    _OP2_TPCLOGSENABLERSP = 219,
    _OP2_NEWXTALCALPROCRSP = 220,
    _OP2_NEWXTALCALPROC = 221,
    _OP2_NOISEFLOORCALSAVE = 222,
    _OP2_NOISEFLOORCALSAVERSP = 223,
    _OP2_OFDMATONEPLAN_NEW = 224,
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
    _OP2_GETDPDLOOPBACKINFO = 249,
    _OP2_GETDPDLOOPBACKINFORSP = 250,
    _OP2_TPCGENTOOL = 251,
    _OP2_TPCGENTOOLRSP = 252,
    _OP2_INVALIDATECALDATAOTPSECTION = 253,
    _OP2_INVALIDATECALDATAOTPSECTIONRSP = 254,
    _OP2_SETPERCHAINPWRLIMITREG = 0xFF,
    _OP2_SETPERCHAINPWRLIMITREGRSP = 0x100,
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
    _OP2_BDWRITE = 269,
    _OP2_MODIFYBDFOTPFLAGS = 270,
    _OP2_OFDMAULTXPOSTINFO = 271,
    _OP2_OFDMAULTXPOSTINFORSP = 272,
    _OP2_BDGETIBFCALINFO = 273,
    _OP2_BDGETIBFCALINFORSP = 274,
    _OP2_LOWPOWER = 275,
    _OP2_EEPROMWRITE = 276,
    _OP2_EEPROMREAD = 277,
    _OP2_EEPOMREADSP = 278,
    _OP2_EEPROMGETSIZE = 279,
    _OP2_EEPROMGETSIZERSP = 280,
    _OP2_ADCCAPTURECONFIG = 281,
    _OP2_EVENTCAPTURECONFIG = 282,
    _OP2_DACPLAYBACK2 = 283,
    _OP2_PHYDBGDUMP2 = 284,
    _OP2_PHYDBGDUMP2RSP = 285,
    _OP2_RSSISELFTEST = 286,
    _OP2_RSSISELFTESTRSP = 287,
    _OP2_SETBDFVERSION = 288,
    _OP2_GETBDFVERSION = 289,
    _OP2_GETBDFVERSIONRSP = 290,
    _OP2_SETPHYONOFF = 291,
    _OP2_SETRXCHAINONOFFF = 292,
    _OP2_GETTPC_PDADC = 293,
    _OP2_GETTPC_PDADCRSP = 294,
    _OP2_CHANGTPC_FORCEDGAIN = 295,
    _OP2_SETTPC_CALFREQ = 296,
    _OP2_SENDTPC_TXMEAS = 297,
    _OP2_TPCCAL_POSTPROCESS = 298,
    _OP2_TPCCAL_POSTPROCESSRSP = 299,
    _OP2_TPCCAL_BDFUPDATE = 300,
    _OP2_EDPDCALINFO = 301,
    _OP2_EDPDCALINFORSP = 302,
    _OP2_CUST_OPCCALPROC = 303,
    _OP2_CUST_OPCCALPROCRSP = 304,
    _OP2_CUST_RXGAINCALPROC = 305,
    _OP2_CUST_RXGAINCALPROCRSP = 306,
    _OP2_GETCTLEXCEPTIONTABLESIZE = 307,
    _OP2_GETCTLEXCEPTIONTABLESIZERSP = 308,
    _OP2_GETCTLEXCEPTIONVALUE = 309,
    _OP2_GETCTLEXCEPTIONVALUERSP = 310,
    _OP2_SETCTLEXCEPTIONVALUE = 311,
    _OP2_SETCTLINFO = 312,
    _OP2_GETCTLINFO = 313,
    _OP2_GETCTLINFORSP = 314,
    _OP2_GETCTLSUPPORTEDINFO = 315,
    _OP2_GETCTLSUPPORTEDINFORSP = 316,
    _OP2_GETCTLSUPPORTEDCHANNS = 317,
    _OP2_GETCTLSUPPORTEDCHANNSRSP = 318,
    _OP2_SETCTLTPCTESTSCREEN = 319,
    _OP2_SETCTLTPCTESTSCREENRSP = 320,
    _OP2_GETCTLTPCFTMDATA = 321,
    _OP2_GETCTLTPCFTMDATARSP = 322,
    _OP2_RXGAINCALRESULT = 323,
    _OP2_RXGAINCALRESULTRSP = 324,
    _OP2_SKIPCALDBREGEN = 325,
    _OP2_SKIPCALDBREGENRSP = 326,
    _OP2_CALDBREGEN = 327,
    _OP2_CALDBREGENRSP = 328,
    _OP2_READ_FIELD = 500,
    _OP2_WRITE_FIELD = 501
};

enum WLAN_Antenna
{
    Antenna0 = 0,
    Antenna1 = 1,
    Antenna2
};



enum WLAN_RX_MODE
{
    ALL_FRAMES,
    FILTER_MATCH_MAC_BSSID
};
namespace JGW
{
    class CCJGWQMSLWlanDeivce
    {
    public:
        CCJGWQMSLWlanDeivce(void);
        ~CCJGWQMSLWlanDeivce(void);
    public:
        bool ConnectStandaloneWlanCardQUTS(const std::string& strCom);
        bool IsConnectWlanDeivce();
        void CaptureQMSLTraceLog(void);
        bool LoadDut(char* dllID = QCN9224_DLLID, char* bin_file = "", unsigned int chipID = QCN9224_CHIPID);
        void SetWlanInstance(int channel);
        bool WlanSetDutTxMacAddressBssId();

        void StopTx(int channel = 6135, PhyID phyId = PHY_B);
        void CloseDut();
        void DisconnectStandaloneWlanCard(void);
    public:
        // txChainMask broadcast : Broadcast/unicast
        // pktLen0 = payload size  def 1500
        bool StartTx(int txPower0,RFMode phyRFMode = DBS_PhyA5G_PhyB2G, PhyID phyId = PHY_B, WLAN_TX_Mode txMode = ContTx99, 
            int channel = 6135, int channel2 = 0, WLAN_UniversalDataRate rateBitIndex0 = RATE_MCS_0, 
            WLAN_Gen6_ChannelBondingState wlanMode = BW320, int pktLen0 = 64000, WLAN_CHAIN_MASK2 txChain0 = WLAN_CHAIN_1, 
            int shortGuard = 0, int numPackets = 0, 
            int broadcast = 0, int flags = 0x18, int gainIdx = 5, int dacGain = 5, int paConfig = 5, 
            TCMD_TPC_TYPE tpcm = TPC_TX_PWR, WLAN_UniversalWiFtStandard wifiStd = WiFiStandard_Legacy11AX, int bandWidth = 0, 
            WLAN_UniversalRateBW rateBw = RateBW_11BE_EHT320, WLAN_UniversalNSS nss = _1Stream, 
            WLAN_UniversalGuardInterval gI = LTF_Mode4_GI_800);
#if 0
        bool StartRx(RFMode phyRFMode = DBS_PhyA5G_PhyB2G, PhyID phyId = PHY_B, 
            int channel = 6135, int channel2 = 0, 
            WLAN_RX_MODE rxMode = ALL_FRAMES, 
            WLAN_UniversalDataRate rateBitIndex0 = RATE_MCS_0, 
            WLAN_CHAIN_MASK2 rxChain = WLAN_CHAIN_1, WLAN_Gen6_ChannelBondingState wlanMode = BW320,
            WLAN_UniversalWiFtStandard wifiStd = WiFiStandard_Legacy11AX, int bandWidth = 0, 
            WLAN_UniversalRateBW rateBw = RateBW_11BE_EHT320, WLAN_UniversalNSS nss = _1Stream);
#else
        bool StartRx(WLAN_TLV2_OPCODE_NEW wlanTlv2Opcode= _OP2_RX_NEW, PhyID phyId = PHY_B, 
            int channel = 6135, int channel2 = 0, 
            WLAN_RX_MODE rxMode = ALL_FRAMES, 
            WLAN_UniversalDataRate rateBitIndex0 = RATE_MCS_0, 
            WLAN_CHAIN_MASK2 rxChain = WLAN_CHAIN_1, WLAN_Gen6_ChannelBondingState wlanMode = BW320,
            WLAN_UniversalWiFtStandard wifiStd = WiFiStandard_Legacy11AX, int bandWidth = 0, 
            WLAN_UniversalRateBW rateBw = RateBW_11BE_EHT320, WLAN_UniversalNSS nss = _1Stream,WLAN_Antenna antenna = Antenna0);
#endif
        bool GetRxReport(size_t& uTotalPackets,size_t& uGoodPackets,int& nRssi,float& fEvm,PhyID phyId = PHY_B);
        void StopRxWithReport(PhyID phyId = PHY_B);
    public:
        bool ExecTlv2ConfigFlow(std::vector<S_TLV2_CONFIG>& vsTlv2Configs);
    private:
        bool mbLoadedDut;
        HANDLE mhResourceContext;
        std::string mstrCom;
        std::string mstrQMSL_Log_File;
    };
}


