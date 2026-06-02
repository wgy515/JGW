/******************************************************************************
 * $Header: //depot/HTE/QDART/QMSL6.1/QMSL_Projects/QLibDemo_WLAN/QLibDemo_WLAN/QLibDemo_WLAN_Core.h#22 $
 * $DateTime: 2022/06/08 07:30:22 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2018-2021 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */


#include "windows.h"
#include <vector>
#include "stdio.h"
#include "stdlib.h"

// Data storage
enum NVMEM_OPTION
{
	DataNone = 0,
	DataFlash,
	DataEeprom,
	DataOtp,
	DataDontLoad,
	DataFile,
	DataDram
};

enum CONNECTION_TYPE
{
	TYPE_IP = 0,
	TYPE_COMPORT = 1,
	TYPE_QPST = 2,
	TYPE_TCPSERVER = 3,
	TYPE_QPST_MDM = 4,
	TYPE_QPST_APQ = 5,
	TYPE_QUTS = 6,
};

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

enum WLAN_TLV2_OPCODE
{
   _OP2_TX = 1,
   _OP2_RX = 2,
   _OP2_SYNC = 100,
   _OP2_SETPHYRFMODE = 169,
   _OP2_NOISEFLOORREAD = 202,
   _OP2_OFDMATONEPLAN = 224,
};

typedef bool( *asyncSetChainMessageCB )
(
  int chainMask
);

typedef bool( *asyncRFSMessageCB )
(
  int chainMask
);

typedef double( *asyncPMMessageCB )
   (
   unsigned int iGain,
   unsigned int iFreq,
   double dPowerLevel,
   unsigned int iNumAvg,
   unsigned int iTriggerType,
   int iTriggerLevel      // for measure CW tone, set iTriggerLevel = -1000
   );

typedef double( *asyncCWFreqErrMessageCB  )
   (
   unsigned int iCapValue,
   unsigned int iCwFreqMHz,
   double dPowerLevel,
   unsigned int iNumAvg
   );

typedef double(*asyncCWFreqErrMessageCB2)
(
	unsigned int iCapInValue,
	unsigned int iCapOutValue,
	unsigned int iCwFreqMHz,
	double dPowerLevel,
	unsigned int iNumAvg
	);

typedef int(*asyncHandlerSG_CB)
(
	int inputSignalStrength_dBm,
	unsigned int freq,
	unsigned int rate,
	unsigned int numPackets,
	unsigned int chain
);

typedef bool(*asyncHandlerRxGainCalDoneCB)
(
	unsigned int status
);

// WLAN_TX_Mode
//{
//    DisablingContinuousMode = 0,
//    EnablingContinuousUnmodulatedTX = 1,
//    EnablingContinuousModulatedTX = 2,
//    ContTx99 = 3,
//    ContTx100 = 4,
//    EnablingOffsetTone = 5,
//    EnablingPSATCal = 6,
//    EnablingCWTone = 7,
//    EnablingCLPCPacket = 8,
//    EnablingSCPC = 9,
//}

// TxPowerMode
//{
//    ForcedGain,              // Operate in a forced gain (open loop) scenario, using an analog and digital gain value
//    OpenLoopSearch,          // Use test equipment to close the loop to determine gain settings for power level
//    TxPowerAuto,             // Transmit power mode - calibration scheme determined by how device was calibrated
//    TxPowerForce_CLPC,       // Transmit power mode - Force cal scheme to operate in CLPC mode only
//    TxPowerForce_SCPC,       // Transmit power mode - Force cal scheme to operate in SCPC mode only
//    TxPowerForce_OLPC,       // Transmit power mode - Force cal scheme to operate in OLPC mode only
//    ForcedDesiredGain,        // Operate in a forced gain (open loop) scenario, using desired gain (pcdac) value
//    ForcedGLUTIndex          //Operate in a forced GLUT scenario
//};


// qca9984_qca9994 Data Rates TX=========
// tlvRATE_AC_MCS_0_160_1S = 224
// tlvRATE_AC_MCS_1_160_1S = 225
// tlvRATE_AC_MCS_2_160_1S = 226
// tlvRATE_AC_MCS_3_160_1S = 227
// tlvRATE_AC_MCS_4_160_1S = 228
// tlvRATE_AC_MCS_5_160_1S = 229
// tlvRATE_AC_MCS_6_160_1S = 230
// tlvRATE_AC_MCS_7_160_1S = 231
// tlvRATE_AC_MCS_8_160_1S = 232
// tlvRATE_AC_MCS_9_160_1S = 233
//
// tlvRATE_AC_MCS_0_160_2S = 234
// tlvRATE_AC_MCS_1_160_2S = 235
// tlvRATE_AC_MCS_2_160_2S = 236
// tlvRATE_AC_MCS_3_160_2S = 237
// tlvRATE_AC_MCS_4_160_2S = 238
// tlvRATE_AC_MCS_5_160_2S = 239
// tlvRATE_AC_MCS_6_160_2S = 240
// tlvRATE_AC_MCS_7_160_2S = 241
// tlvRATE_AC_MCS_8_160_2S = 242
// tlvRATE_AC_MCS_9_160_2S = 243

// qca9984_qca9994 Data Rate RX========
// RATE_11AC_160_MCS0     = 183
// RATE_11AC_160_MCS1     = 184
// RATE_11AC_160_MCS2     = 185
// RATE_11AC_160_MCS3     = 186
// RATE_11AC_160_MCS4     = 187
// RATE_11AC_160_MCS5     = 188
// RATE_11AC_160_MCS6     = 189
// RATE_11AC_160_MCS7     = 190
// RATE_11AC_160_MCS8     = 191
// RATE_11AC_160_MCS9     = 192
// RATE_11AC_160_MCS10    = 193
// RATE_11AC_160_MCS11    = 194
// RATE_11AC_160_MCS12    = 195
// RATE_11AC_160_MCS13    = 196
// RATE_11AC_160_MCS14    = 197
// RATE_11AC_160_MCS15    = 198
// RATE_11AC_160_MCS16    = 199
// RATE_11AC_160_MCS17    = 200
// RATE_11AC_160_MCS18    = 201
// RATE_11AC_160_MCS19    = 202

// qca9984_qca9994 cbState/wlanMode========
// BW80p80 = 12,
// Primary_1st_20_in_BW80p80 = 21
// Primary_2nd_20_in_BW80p80 = 22
// Primary_3rd_20_in_BW80p80 = 23
// Primary_4th_20_in_BW80p80 = 24
// Primary_5th_20_in_BW80p80 = 25
// Primary_6th_20_in_BW80p80 = 26
// Primary_7th_20_in_BW80p80 = 27
// Primary_8th_20_in_BW80p80 = 28

// ====================================
// IPQ807x DataRate
// {
//     CW = -1,
//     RATE_Long_1Mbps = 0,
//     RATE_Long_2Mbps = 1,
//     RATE_Long_5_5Mbps = 2,
//     RATE_Long_11Mbps = 3,
//     RATE_Short_2Mbps = 4,
//     RATE_Short_5_5Mbps = 5,
//     RATE_Short_11Mbps = 6,
//     RATE_6Mbps = 10,
//     RATE_9Mbps = 11,
//     RATE_12Mbps = 12,
//     RATE_18Mbps = 13,
//     RATE_24Mbps = 14,
//     RATE_36Mbps = 15,
//     RATE_48Mbps = 16,
//     RATE_54Mbps = 17,
//     RATE_MCS_0 = 20,
//     RATE_MCS_1 = 21,
//     RATE_MCS_2 = 22,
//     RATE_MCS_3 = 23,
//     RATE_MCS_4 = 24,
//     RATE_MCS_5 = 25,
//     RATE_MCS_6 = 26,
//     RATE_MCS_7 = 27,
//     RATE_MCS_8 = 28,
//     RATE_MCS_9 = 29,
//     RATE_MCS_10 = 30,
//     RATE_MCS_11 = 31,
// }
// 
// IPQ807x GuardInterval
// {
//     GI_0 = 0,
//     LTF_Mode0_GI_400 = 1,
//     LTF_Mode0_GI_800 = 2,
//     LTF_Mode0_GI_1600 = 3,
//     LTF_Mode0_GI_3200 = 4,
//     LTF_Mode1_GI_400 = 17,
//     LTF_Mode1_GI_800 = 18,
//     LTF_Mode1_GI_1600 = 19,
//     LTF_Mode1_GI_3200 = 20,
//     LTF_Mode2_GI_400 = 33,
//     LTF_Mode2_GI_800 = 34,
//     LTF_Mode2_GI_1600 = 35,
//     LTF_Mode2_GI_3200 = 36,
//     LTF_Mode4_GI_400 = 49,
//     LTF_Mode4_GI_800 = 50,
//     LTF_Mode4_GI_1600 = 51,
//     LTF_Mode4_GI_3200 = 52,
// }
// 
// IPQ807x NSS
// {
//     _1Stream = 1,
//     _2Streams = 2,
//     _3Streams = 3,
//     _4Streams = 4,
//     _5Streams = 5,
//     _6Streams = 6,
//     _7Streams = 7,
//     _8Streams = 8,
// }
// 
// IPQ807x PhyID
// {
//     PHY_A0 = 0,
//     PHY_B = 1,
//     PHY_A1 = 2,
// }
// 
// IPQ807x RateBW
// {
//     RateBW_CW = -1,
//     RateBW_CCK = 0,
//     RateBW_LegacyOFDM = 1,
//     RateBW_11N_HT20 = 2,
//     RateBW_11N_HT40 = 3,
//     RateBW_11AC_VHT20 = 4,
//     RateBW_11AC_VHT40 = 5,
//     RateBW_11AC_VHT80 = 6,
//     RateBW_11AC_VHT80P80 = 7,
//     RateBW_11AX_HE20 = 8,
//     RateBW_11AX_HE40 = 9,
//     RateBW_11AX_HE80 = 10,
//     RateBW_11AX_HE80P80 = 11,
//     RateBW_11AX_OFDMA_HE20 = 12,
//     RateBW_11AX_OFDMA_HE40 = 13,
//     RateBW_11AX_OFDMA_HE80 = 14,
//     RateBW_11AX_OFDMA_HE80P80 = 15,
// }
// 
// IPQ807x WiFtStandard
// {
//     WiFiStandard_Legacy = 0,
//     WiFiStandard_Legacy11AC = 1,
//     WiFiStandard_Legacy11AX = 2,
//     WiFiStandard_OFDMA = 3,
// }

#define USER_HANDLE   1

// QLib.h
#define QLIB_LIB_MODE_QPHONEMS false    // Internal engine, QPHONEMS, also used for user defined transport
#define QLIB_LIB_MODE_QPST     true     // Use QPST for packet transport
#define QLIB_LIB_MODE_QUTS     2

#define QLIB_TARGET_TYPE_MSM_MDM    0       // The target supports WWAN modem.
#define QLIB_TARGET_TYPE_APQ        1       // Application processor only type target (APQ).  No WWAN modem

#define QLIB_COM_AUTO_DETECT ((unsigned int)0xFFFF)

#define QLIB_DIAG_TYPE_STANDARD	0
#define QLIB_DIAG_TYPE_LITE		1

//Filenames for OTP Dump and compare functionality
#define OTP_DUMP_FILE       "C:\\log\\otpRawContents.txt"
#define OTP_RESULTS_FILE    "C:\\log\\compareResults.txt"

//  ---------------------------------------------------------
//  defines for phone logging settings
//
//  ---------------------------------------------------------
#define LOG_NOTHING     0x0000  //!<' log nothing

#define LOG_C_HIGH_LEVEL_START  0x0200  //!<' High level C function start, indicates the begining of a high level C function, which
//!<' calls other low level C functions internal to the library

#define LOG_C_HIGH_LEVEL_STOP   0x4000  //!<' High level C function stop

#define LOG_IO          0x0001  //!<' data IO (data bytes)
#define LOG_FN          0x0002  //!<' function calls with parameters
#define LOG_RET         0x0004  //!<' function return data

#define LOG_INF         0x0008  //!<' general information (nice to know)--do not use this one, as
//!<' this space needs to be reserved for async messages

#define LOG_ASYNC       0x0008  //!<' asynchronous messages

#define LOG_ERR         0x0010  //!<' critical error information

#define LOG_IO_AHDLC    0x0020  //!<' HDLC IO tracing (data bytes)
#define LOG_FN_AHDLC    0x0040  //!<' HDLC layer function calls
#define LOG_RET_AHDLC   0x0080  //!<' HDLC function return data
#define LOG_INF_AHDLC   0x0100  //!<' HDLC general information
#define LOG_ERR_AHDLC   LOG_INF_AHDLC   //!<' HDLC Error info merged with LOG_INF_AHDLC, to free up the log bit

#define LOG_IO_DEV      0x0400  //!<' device IO tracing (data bytes)
#define LOG_FN_DEV      0x0800  //!<' device layer function calls
#define LOG_RET_DEV     0x1000  //!<' device function return data
#define LOG_INF_DEV     0x2000  //!<' device general information
#define LOG_ERR_DEV     LOG_INF_DEV     //!<' device error information, merged with LOG_INF_DEV to free up the log bit

#define LOG_DEFAULT     (LOG_C_HIGH_LEVEL_START|LOG_C_HIGH_LEVEL_STOP|LOG_FN|LOG_IO|LOG_RET|LOG_ERR|LOG_ASYNC) //!<'  default settings

#define LOG_ALL         0xFFFF  //!<' everything
//  ---------------------------------------------------------

extern "C" {
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_LoadDUT(HANDLE hResourceContext,unsigned char *DevDLLname, unsigned char *boardDataFName, int iNVMem, unsigned int ssid);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_UNLoadDUT(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV_Create(HANDLE hResourceContext,unsigned char opCode);
   QLIB_API char QLIB_FTM_WLAN_TLV_AddParam(HANDLE hResourceContext, char *pKey, char *pData);
   QLIB_API char QLIB_FTM_WLAN_TLV_Complete(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV_GetRspParam(HANDLE hResourceContext,char *pKey, char *pData);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx_CALIBRATE_INIT(HANDLE hResourceContext, unsigned char *calDLLname, int iCalPoints, int iCalGainMode);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx_CAL(HANDLE hResourceContext, asyncPMMessageCB pAsyncPMMessageCB, unsigned int measAvg);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Tx_FW_CAL(HANDLE hResourceContext, asyncSetChainMessageCB pAsyncSetChainMessageCB, asyncPMMessageCB pAsyncPMMessageCB, unsigned int measAvg, bool AllChainOn, asyncRFSMessageCB pAsyncRFSMessageCB, bool DebugInfo);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_Rx_FW_CAL(HANDLE hResourceContext, asyncHandlerSG_CB pAsyncHandlerSigGenCB, asyncHandlerRxGainCalDoneCB pAsyncHandlerRxGainCalDoneCB, unsigned int radioId, unsigned int chainToCalibrate, unsigned int band);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(HANDLE hResourceContext, int iXtalTargetPPM, unsigned int iXtalTolerancePPM, int iXtalCapDelta, unsigned int iXtalCalSaveOption, unsigned int delayCapSettleMs, unsigned int delayPostXtalMs);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_xtal_CAL(HANDLE hResourceContext, asyncCWFreqErrMessageCB pAsyncCWFreqErrMessageCB, unsigned int measAvg);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_xtal_CAL2(HANDLE hResourceContext, asyncCWFreqErrMessageCB2 pAsyncCWFreqErrMessageCB2, unsigned int measAvg, bool ppmCD, bool pmic);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_BD_ADDRESS_FROM_OTP(HANDLE hResourceContext, unsigned char *aiBD_Address);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_BD_ADDRESS_IN_OTP(HANDLE hResourceContext, unsigned char aiBD_Address[6]);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_TOP_CONFIG_IN_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_TOP_CONFIG_FROM_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_BT_CONFIG_IN_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_BT_CONFIG_FROM_OTP(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_SET_PCIe_CONFIG_IN_OTP_STREAM(HANDLE g_hResourceContext, unsigned int iAction,unsigned int iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_GET_PCIe_CONFIG_FROM_OTP_STREAM(HANDLE hResourceContext, unsigned int iAction,unsigned int *iValue);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_SET_OTP_REG_DOMAIN( HANDLE hResourceContext, unsigned int aiRegDmn[2]);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_GET_OTP_REG_DOMAIN( HANDLE hResourceContext, unsigned int *aiRegDmn);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_DumpOTPToFile(HANDLE hResourceContext, char *piFilename);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_CompareOTPAgainstFile(HANDLE hResourceContext, char *piFilename, char *piResultsFilename, unsigned int *iResults);
   QLIB_API unsigned char QLIB_FTM_WLAN_Atheros_MEM_COMMIT(HANDLE hResourceContext, unsigned char templateNum, unsigned char iWriteOption, unsigned long iMemSize, unsigned char iCompress, unsigned char iOverwrite, unsigned long iSectionMask);   
   QLIB_API char QLIB_FTM_WLAN_TLV2_Create(HANDLE hResourceContext,unsigned char opCode);
   QLIB_API char QLIB_FTM_WLAN_TLV2_Create16(HANDLE hResourceContext,unsigned short opCode);
   QLIB_API char QLIB_FTM_WLAN_TLV2_AddParam(HANDLE hResourceContext, char *pKey, char *pData);
   QLIB_API char QLIB_FTM_WLAN_TLV2_Complete(HANDLE hResourceContext);
   QLIB_API char QLIB_FTM_WLAN_TLV2_GetRspParam(HANDLE hResourceContext,char *pKey, char *pData);
   QLIB_API char QLIB_FTM_WLAN_Atheros_Tx_FW_CAL2(HANDLE hResourceContext, int phyId, int calScheme, double _1ptRangeDbUL, double _1ptRangeDbLL, int totalNumOfChains, int ultiChainMask, int band, int numAverages, void *pAsyncSetChainHandlerCB, void *pAsyncPMHandlerCB2, void *pAsyncRFSHandlerCB);
   QLIB_API char QLIB_FTM_WLAN_Atheros_Save_BDF(HANDLE hResourceContext,unsigned int option, char *fname);
   QLIB_API void QLIB_SetLibraryMode(unsigned char mode);
   QLIB_API void QLIB_SetTargetType(unsigned char type); 
   QLIB_API HANDLE QLIB_ConnectServer(unsigned int comNum);
   unsigned char QLIB_GetAvailablePhonesHandleIDList(
	   QUTS_PROTOCOL_TYPE protocol_type /* for future usase. Set 0 for PROT_DIAG */,
	   unsigned int* iNumListSize,
	   QUTS_DEVICE_HANDLE_ID_TYPE* iDeviceHandleList,
	   QUTS_PROTOCOL_ID_TYPE* iProtocolHandleList,
	   unsigned int* iDescriptionLength, char** pDescriptionList,
	   unsigned int* iErrorCode);
   HANDLE QLIB_ConnectServerWithWaitByHandleID(
	   QUTS_PROTOCOL_TYPE protocol_type,
	   QUTS_DEVICE_HANDLE_ID_TYPE device_handle_id,
	   QUTS_PROTOCOL_ID_TYPE protocol_handle_id,
	   unsigned long iWait_ms);
   QLIB_API unsigned char QLIB_IsPhoneConnected(HANDLE hResourceContext);
   QLIB_API void QLIB_DisconnectAllServers(void);
   QLIB_API HANDLE QLIB_QPHONEMS_ConnectTCPIP(
	           int iComPort, 
			   unsigned char bConnection_direction  /* 0 = CONNECTION_INCOMING, 1 = CONNECTION_IP_OUTGOING}. */,  
               unsigned int resourceStringLength, 
               char* resourceString, 
               unsigned char bCheckConnection, 
               int retryTimeoutSec, 
               unsigned int* errorCode);   
   QLIB_API void QLIB_GetLibraryVersion( char* psVersion );
   QLIB_API unsigned char QLIB_StartLogging( HANDLE hResourceContext, char* sLogFile );
   QLIB_API unsigned char QLIB_StopLogging( HANDLE hResourceContext );  
   QLIB_API unsigned char QLIB_SetLogFlags( HANDLE hResourceContext, unsigned int uiLogFlags );   
   // WCN36xx
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_START(HANDLE g_hResourceContext, int wlanID);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_NV_VERSION(HANDLE g_hResourceContext, unsigned char *version);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_STOP(HANDLE g_hResourceContext);
   QLIB_API unsigned char QLIB_FTM_WLAN_WCN_ANTENNA_SELECT(HANDLE g_hResourceContext, unsigned int antenna);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_ENABLE_CHAINS(HANDLE g_hResourceContext, unsigned int chain);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_TX_PKT_START_STOP(HANDLE g_hResourceContext, unsigned char iStart);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_CHANNEL_V2(HANDLE g_hResourceContext, unsigned int iChannel, unsigned int channelBondingState);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_ENABLE_DPD(HANDLE g_hResourceContext, unsigned char enable);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_FRAME(HANDLE g_hResourceContext, 
		   unsigned long iNumTestFrame,
		   unsigned short iPayloadSize,
		   unsigned long iFrameSpacing,
		   unsigned char iFCSCal,
		   unsigned long iRate,
		   unsigned long iPreamble );
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_FRAME_V2(HANDLE g_hResourceContext, 
		   unsigned long iNumTestFrame,
		   unsigned short iPayloadSize,
		   unsigned char iPayloadType, 
		   unsigned char iPayloadFillByte, 
		   unsigned long iFrameSpacing,
		   unsigned char iFCSCal,
		   unsigned long iRate,
		   unsigned long iPreamble );
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_TX_WAVEFORM_GAIN_V2(HANDLE g_hResourceContext, unsigned int gain);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_CLOSE_TPC_LOOP_V2(HANDLE g_hResourceContext, unsigned char mode);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_CLOSED_LOOP_POWER(HANDLE g_hResourceContext, double dPowerIndBm);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_PWR_INDEX_SOURCE(HANDLE g_hResourceContext, unsigned long source);
   // Enable/Disable the receiver capabilities.
   // mask: bit 0: disable B packets, 
   //		bit 1: disable A/G packets bit, 
   //		bit 2: disable N 40 MHz packets, 
   //		bit 3: disable AC 80 MHz packets
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_SET_RX_DISABLE_MODE_V2(HANDLE g_hResourceContext, unsigned long mask);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_RESET_RX_PACKET_STATISTICS(HANDLE g_hResourceContext);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_RX_PACKET_COUNTS(HANDLE g_hResourceContext, 
	   unsigned long *rxFrameCounter, unsigned long *totalMacRxPackets, unsigned long *totalMacFcsErrPackets);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_GET_RX_RSSI_VAL(HANDLE g_hResourceContext, 
	   short *rssiVals, unsigned short *numRxChains);
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_NV_WRITE_XML(HANDLE g_hResourceContext, char *fileName);	
   QLIB_API unsigned char QLIB_FTM_WLAN_GEN6_READ_XML_WRITE_NV(HANDLE g_hResourceContext, char *fileName);
}

// UserDefinedTransport - Function protocol
typedef unsigned long (*_OpenUserDefinedTransport)(char* pIpPort );
typedef void (*_CloseUserDefinedTransport)(char* pIpPort );
typedef unsigned long (*_UserDefinedReceive)(HANDLE hQMSL_ContextID, HANDLE hUserContextID,   unsigned long* piResponseSize,   unsigned char* piResponseBytes);
typedef unsigned long (*_UserDefinedSend)(HANDLE hQMSL_ContextID, HANDLE hUserContextID, unsigned long iRequestSize, unsigned char* piRequestBytes,   unsigned long* piActualWriteSize);
typedef unsigned long (*_UserDefinedFlushTxRx)(HANDLE hQMSL_ContextID, HANDLE hUserContextID);


extern char *chip_select;
extern HINSTANCE hUDT;
//extern HANDLE g_hResourceContext;

extern void Testqca998x_qca999x(void);
extern void Testqca988x(void);
extern void Testqca9984_qca9994(void);
extern void TestIPQ40xx(void);
extern void TestIPQ807x(void);
extern void TestWCN39x0(void);
extern void TestWCN36XX(void);
extern void TestQCA40xx(void);
extern void Testqca61x4_qca65x4_qca93xx(void);
extern void TestAr93xx(void);
extern void TestQC6390(void);
extern void TestWCN7850(void);
extern void TestQCN9224(void);

extern char (*pQLIB_FTM_WLAN_TLV_Create)(HANDLE hResourceContext,unsigned char opCode);
extern char (*pQLIB_FTM_WLAN_TLV_Create16)(HANDLE hResourceContext,unsigned short opCode);
extern char (*pQLIB_FTM_WLAN_TLV_AddParam)(HANDLE hResourceContext, char *pKey, char *pData);
extern char (*pQLIB_FTM_WLAN_TLV_Complete)(HANDLE hResourceContext);
extern char (*pQLIB_FTM_WLAN_TLV_GetRspParam)(HANDLE hResourceContext,char *pKey, char *pData);
extern char (*pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2)(HANDLE hResourceContext, int phyId, int calScheme, double _1ptRangeDbUL, double _1ptRangeDbLL, int totalNumOfChains, int ultiChainMask, int band, int numAverages, void *pAsyncSetChainHandlerCB, void *pAsyncPMHandlerCB2, void *pAsyncRFSHandlerCB);
extern char (*pQLIB_FTM_WLAN_Atheros_Save_BDF)(HANDLE hResourceContext,unsigned int option, char *fname);

extern void SetTLV1(void);
extern void SetTLV2(void);

extern bool ConnectStandaloneWlanCard(unsigned int connectionType, std::string strCom);
extern void DisconnectStandaloneWlanCard(unsigned int connectionType);
extern bool ConnectDiagTypeLiteWlanCard(unsigned int connectType);
extern bool ConnectQPSTWithComPortNum(unsigned int comNum, HANDLE* phResourceContext);
extern void DisconnectQPSTServer(HANDLE hResourceContext);
extern void Set_UDT_IP(char *newIP);
extern void Set_QPST_ComNum(unsigned int comNum);
extern bool Test_FTM_QCA_WLAN_LoadTonePlan();
extern bool Test_FTM_QCA_WLAN_SendTonePlan(int phyId);
