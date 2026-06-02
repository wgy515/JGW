#pragma once
#include <JGW_FoundationFunc/CJGW_NumberRule.h>

#include <string>
#define JGW_CLOSEHANDLE(hHandle)\
	if( hHandle && hHandle != INVALID_HANDLE_VALUE ){\
	CloseHandle(hHandle);\
	hHandle = NULL;\
	}

typedef unsigned int uint;

namespace JGW
{
//     enum E_NUMBER_TYPE
//     {
//         E_SN = 0,
//         E_IMEI,
//         E_IMEI2,
//         E_WIFI_MAC,
//         E_ETH0_MAC,
//         E_BT_MAC,
//         E_2_4_SSID,
//         E_2_4_PASSWORD,
//         E_5_SSID,
//         E_5_PASSWORD,
//         E_AD_SSID,
//         E_AD_PASSWORD,
//         E_UNKNOWN_NUMBER
//     };

//     const wchar_t gszNumberName[][20] = {
//         L"SN",
//         L"IMEI",
//         L"IMEI2",
//         L"WiFi Mac",
//         L"eth0 Mac",
//         L"Bluetooth Address",
//         L"2.4G SSID",
//         L"2.4G PassWord",
//         L"5G SSID",
//         L"5G PassWord",
//         L"11AD SSID",
//         L"11AD PassWord",
//         L"Unknown Number",
//     }; 
	//! #define SNNVINDEX 		2498
	struct s_sn_nv_info
	{
		char sn[25];
		char clientsn[25];
	};
	
	//! #define LYNX_WIFI_NV_INFO 2499
    struct s_wifi_nv_info 
    {
        char ssid_2_4G[21]; //! 以 '\0'为结束符
        char password_2_4G[21]; //! 以 '\0'为结束符
        char ssid_5G[21]; //! 以 '\0'为结束符
        char password_5G[21]; //! 以 '\0'为结束符
        char ssid_ad[21]; //! 以 '\0'为结束符
        char password_ad[21]; //! 以 '\0'为结束符
    };
}






/*************mcwill at info ***********************************/
#define MCWILL_MAC_LENGTH 12
#define MCWLL_AT_COMMAND_TIME_OUT 1000*3
#define MCWILL_DATA_LENGTH 120

enum E_MCWILL_USB_MSG
{
	E_SWITCH_QCOMM_USB = 0,//! 切换到QCOMM USB
	E_SWITCH_MCWILL_USB,//! 切换MCWILL USB
	E_MCWILL_MODE_RESET,//! 重启M网模块
	E_SWITCH_RESET_MCWILL_USB,//! 切换M网USB 并且重启M网模块
	E_READ_SUBSYS_VERSION,//! 读取子版本号
	E_READ_MCWILL_CSQ,//! 读取MCWILL CSQ 信号强度
	E_MCWILL_WRITE_PID,
	E_MCWILL_READ_PID,
	E_MCWILL_WRITE_USBMAC,
	E_MCWILL_READ_USBMAC,
	E_MCWILL_WRITE_USER_DEFINED_AT_COMMAND,
	E_MCWILL_READ_HW_TYPE
};

// AMSS types
typedef  unsigned long long int  uint64;      //!< Unsigned 64 bit value
typedef  unsigned long int       uint32;      //!< Unsigned 32 bit value
typedef  unsigned short          uint16;      //!< Unsigned 16 bit value
typedef  unsigned char           uint8;       //!< Unsigned 8  bit value
typedef  signed long int         int32;       //!< Signed 32 bit value
typedef  signed short            int16;       //!< Signed 16 bit value
typedef  signed char             int8;        //!< Signed 8  bit value

typedef  unsigned long int       uint4;       //!< Unsigned 32 bit value
typedef  unsigned short          uint2;       //!< Unsigned 16 bit value
typedef  unsigned char           uint1;       //!< Unsigned 8  bit value

typedef  signed short            int2;        //!< Signed 16 bit value
typedef  signed char             int1;        //!< Signed 8  bit value
typedef unsigned char			byte;
typedef uint8					diagpkt_cmd_code_type;
typedef uint8					diagpkt_subsys_id_type;
typedef uint16					diagpkt_subsys_cmd_code_type;
//! 以下结构体默认就是对齐的
//#pragma pack(1)
//! 字节对齐
#pragma pack(push, 1)
//! 请求结构体
typedef struct
{
	diagpkt_cmd_code_type              cmd_code;
	diagpkt_subsys_id_type             subsys_id;
	diagpkt_subsys_cmd_code_type       subsys_cmd_code;
	uint16                             cmd_id; /* command id (required) */
	uint16                             cmd_data_len;
	uint16                             cmd_rsp_pkt_size;
	uint16							   cmd_ms_time_out;
	byte                               data[MCWILL_DATA_LENGTH];
}ftm_McWill_pkt_type;
//#pragma pack()
typedef struct {
	uint8 cmd_code;
	uint8 sub_sys_id;
	uint16 sub_sys_cmd_code;
	uint16 ftm_cmd_id;
	uint16 ftm_data_len;
	uint16 ftm_rsp_pkt_size;
} ftm_header;
//! 返回结构体
typedef  struct
{
	ftm_header	ftm;
	uint8		result;/*result*/
	union
	{
		uint8 	    platform_subtype_id;
		uint8 	    csq;
	}value;
	byte        data[MCWILL_DATA_LENGTH];
}ftm_McWill_generic_res;
#pragma pack(pop)

#define MAIN_SIM		0
#define SECOND_SIM		1
#define IMEINVINDEX		550
//#define SNNVINDEX		2498
//MAC  0xFE  FE SIZE 6
#define WIFIMACNVINDEX	4678
//BT Address 反过来读写  0xFE  EF SIZE 6
#define BTADDRESSNVINDEX 447
//MEID 0xA1234567890123   写入3210987654321A    SIZE 7
#define MEIDNVINDEX		1943
//req_packet.data[i] = strPSN[i];
#define PSNNVINDEX		6853
//req_packet.data[i] = strSN[i];
//一个字节 分为8位  前4位 后4位？
//#define SNNVINDEX 		2824
#define SNNVINDEX 		2498

#define LYNX_WIFI_NV_INFO 2499
#define LYNX_WIFI_NV_INFO_LEN 128

#define MAX_SN_LENGTH			25
#define MAX_CLIENT_SN_LENGTH	25
#define IMEI_LENGTH				15
#define WIFI_MAC_LENGTH			12
#define BT_LENGTH				12
#define MEID_LENGTH				15


//PIN 123456  写入010203040506 SIZE 64
/*
req_packet.item_id = 3033;
rsp_packet.item_id = 3033;
for(int i = 0; i < 4; i++)
{
req_packet.data[2*i+1] = 0;
req_packet.data[2*i] = strPinCode[i];
}
*/
#define PINNVINDEX		3033

// extern const wchar_t* GetJGWMsvcLibError();
// 

enum
{
	eQPhoneMS = 0,
	eQPSTMode = 1,
	eUserDefinedTransport = 2
};

enum
{
	RFNV_READ_SUCCESS  = 0,
	RFNV_READ_FAILD    = 1,
	RFNV_WRITE_SUCCESS = 2,
	RFNV_WRITE_FAILED  = 3,
	RFNV_INACTIVE_ITEM = 4,
};

/*--------------------------------------------------------------------------

Command Codes between the Diagnostic Monitor and the mobile. Packets
travelling in each direction are defined here, while the packet templates
for requests and responses are distinct.  Note that the same packet id
value can be used for both a request and a response.  These values
are used to index a dispatch table in diag.c, so

DON'T CHANGE THE NUMBERS ( REPLACE UNUSED IDS WITH FILLERS ). NEW IDs
MUST BE ASSIGNED AT THE END.

This list is originally taken from the file DiagDefines.h, then reduced
to only include items necessary for this library.

If more items are desired or if new diag commands are added then this list
should be resync'd.

Note: The prefix _ is used here because the function names are exactly
the enumeration ID, as used in the embedded code

----------------------------------------------------------------------------*/

#define _DIAG_VERNO_F           0   //!<' Version Number Request/Response
#define _DIAG_ESN_F             1   //!<' Mobile Station ESN Request/Response
#define _DIAG_MEMORY_PEEK_BYTE_F 2  //!<' Memory peek request/response (8-bit)
#define _DIAG_MEMORY_PEEK_WORD_F 3  //!<' Memory peek request/response (16-bit)
#define _DIAG_MEMORY_PEEK_DWORD_F 4 //!<' Memory peek request/response (32-bit)
#define _DIAG_MEMORY_POKE_BYTE_F 5  //!<' Memory poke request/response (8-bit)
#define _DIAG_MEMORY_POKE_WORD_F 6  //!<' Memory poke request/response (16-bit)
#define _DIAG_MEMORY_POKE_DWORD_F 7 //!<' Memory poke request/response (32-bit)
#define _DIAG_STATUS_REQUEST_F  12  //!<'Status Request/Response
#define _DIAG_STATUS_F          14  //!<' Phone status
#define _DIAG_EFS2_DIAG_STAT    15  //!<' status
#define _DIAG_LOG_F             16  //!<' Log packet Request/Response

#define _DIAG_BAD_CMD_F         19  //!<' Invalid Command Response
#define _DIAG_BAD_PARM_F        20  //!<' Invalid parmaeter Response
#define _DIAG_BAD_LEN_F         21  //!<' Invalid packet length Response
#define _DIAG_BAD_MODE_F        24  //!<' Packet not allowed in this mode

/* 22-23 Reserved */

/* Packet not allowed in this mode
( online vs offline )                      */
#define DIAG_BAD_MODE_F     24

#define _DIAG_MSG_F             31  //!<' Request for msg report
#define _DIAG_HS_KEY_F          32  //!<' Handset Emulation -- keypress
#define _DIAG_SUBSYS_CMD_CODE   36  //!<' Subsystem command code for get group info
#define _DIAG_NV_READ_F         38  //!<' Read NV item
#define _DIAG_NV_WRITE_F        39  //!<' Write NV item
#define _DIAG_CONTROL_F         41  //!<' Mode change request
#define _DIAG_ERR_READ_F        42  //!<' Read error list
#define _DIAG_ERR_CLEAR_F       43  //!<' Clear error list
#define _DIAG_GET_DIPSW_F       47  //!<' Retreive dipswitch
#define _DIAG_SET_DIPSW_F       48  //!<' Set dipswitch
#define _DIAG_VOC_PCM_LB_F      49  //!<' Start/Stop Vocoder PCM loopback
#define _DIAG_VOC_PKT_LB_F      50  //!<' Start/Stop Vocoder PKT loopback
#define _DIAG_CALL_ORIGINATION      53  //!<' Start a Mobile Origination call, e.g., SO55 IS2000 loopback
#define _DIAG_DLOAD_F           58  //!<' Switch to download mode
#define _DIAG_SPC_F             65  //!<' Send the Service Prog. Code to allow SP
#define _DIAG_SERIAL_MODE_CHANGE 68 //!<' Switch mode from diagnostic to data
#define _DIAG_EXT_LOGMASK_F     93  //!<' Extended logmask for > 32 bits.
#define _DIAG_EVENT_REPORT_F    96  //!<' Static Event reporting.
#define _DIAG_PRL_WRITE         72  //!<' Single PRL Write
#define _DIAG_PRL_READ          73  //!<' Single PRL Read
#define _DIAG_SUBSYS_CMD_F      75  //!<' Subssytem dispatcher (extended diag cmd)
#define _DIAG_NV_WRITE_ONLINE_F 76  //!<' Write to NV location without going Offline
#define _DIAG_FEATURE_QUERY     81
#define _DIAG_IS2000_STATUS_F   112 //!<' IS-2000 Status
#define _DIAG_LOG_CONFIG_F      115 //!<' Logging configuration packet
#define _DIAG_EXT_MSG_F         121 //!<' Extended msg report
#define _DIAG_PROTOCOL_LOOPBACK_F  123 //!<' Diagnostics protocol loopback.
#define _DIAG_EXT_BUILD_ID_F    124 //!<' Extended build ID
#define _DIAG_EXT_MSG_CONFIG_F  125 //!<' Request for extended msg report
#define _DIAG_SECURITY_FREEZE_F 0xff    //!<' Request for Sirius security freeze (not defined yet)
#define _DIAG_MAX_F             126 //!<' Number of packets defined.
#define _DIAG_SUBSYS_CMD_VER_2_F    128
#define _DIAG_EVENT_MASK_GET_F      129 //!<' Get event mask
#define _DIAG_EVENT_MASK_SET_F      130 //!<' Set event mask
#define _DIAG_OPTIMIZED_EXT_MSG_F   146 //!<' Optimized extended message (QShrink)
#define _DIAG_LOG_MULTI_SIM   152 //!< ' MultiSim Wrapper

#ifdef RECORDLOG
#include "log/JGW_LogFile.h"
#endif


#ifdef RECORDLOG
#ifndef WLOAERROR
#define WLOAERROR(msg) JGW_CLogFile::GetInstance()->dbg2file("error.log","a+",msg)
#endif

#ifndef WLOGDEBUG
#define WLOGDEBUG(msg) JGW_CLogFile::GetInstance()->dbg2file("debug.log","a+",msg)
#endif

#ifndef WLOGLOG
#define WLOGLOG(msg)   JGW_CLogFile::GetInstance()->dbg2file("Callog.log","a+",msg)
#endif

#ifndef WLOGF
#define WLOGF(fileName,msg) JGW_CLogFile::GetInstance()->dbg2file(fileName,"a+",msg)
#endif

#ifndef WLOGA
#define WLOGA JGW_CLogFile::GetInstance()->WriteLog
#endif

#ifndef WLOGT
#define WLOGT JGW_CLogFile::GetInstance()->WriteLog
#endif

#else
#define WLOGF(fileName,msg)
#define WLOGLOG(msg) 
#define WLOGDEBUG(msg)
#define WLOAERROR(msg)
#define WLOGA
#define WLOGT
#define WLOGDEBUG(msg)
#endif


#ifdef STATRTDEBUGMODEL
#define CHECKFUNRES(A)	\
{\
	if (!(A)){\
	WLOGA("%s---%d---%s\n",__FILE__,__LINE__,__FUNCTION__);\
	return false;\
	}\
	}

#define CHECKFUNRESFALSE(A)	\
{\
	if (!(A)){\
	WLOGA("%s---%d---%s\n",__FILE__,__LINE__,__FUNCTION__);\
	return FALSE;\
	}\
	}

#define TESTFUNCOK(A)	\
{\
	if (!(A)){\
	WLOGA("%s---%d---%s\n",__FILE__,__LINE__,__FUNCTION__);\
	return false;\
	}\
	}

#define CHECKBOOLRETURN(x) \
	if(!(x))\
{\
	WLOGA("%s---%d---%s\n",__FILE__,__LINE__,__FUNCTION__);\
	return;\
	}


#define TESTCONNECT(x)\
	if(!(x))\
{\
	WLOGA("%s---%d---%s\n",__FILE__,__LINE__,__FUNCTION__);\
	LPCDaoConn->ExitConn ();\
	return false;\
	}

#define TESTFAIL TESTFUNCOK
#else
#define CHECKFUNRES(A)	\
{\
	if (!(A)){\
	return false;\
	}\
	}

#define TESTFUNCOK(A)	\
{\
	if (!(A)){\
	return false;\
	}\
	}

#define CHECKBOOLRETURN(x) \
	if(!(x))\
	return 

#define CHECKFUNRESFALSE(A)	\
	if (!(A))\
	return FALSE;


#define TESTCONNECT(x)\
	if(!(x))\
{\
	LPCDaoConn->ExitConn ();\
	return false;\
	}
#define TESTFAIL TESTFUNCOK
#endif

#ifdef RECORDERADBLOG
#define RECORDERADBLOGINFO(A)	\
{\
	WLOGF("C://SendAdb.log",A);\
	}

#else
#define RECORDERADBLOGINFO(A)	

#endif 
