#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#define FLAG_FRAME			                                        0x7E
#define FLAG_FRAME_ESCAPE								            0x7D
#define PACKET_SIZE													2048
#define STREAM_MAX_BUF_SIZE                                         4096
#define SA_PACKET_SIZE											    4096
#define MAX_STRING_SIZE                                             2048
#define ONE_MEGABYTE                                                1048576
#define FIREHOSE_TX_BUFFER_SIZE (ONE_MEGABYTE)
#define memalign(x, y) _aligned_malloc(y, x)
#define ALIGNED_FREE _aligned_free
#define MIN_NUMBER(x,y) ((x) < (y) ? (x) : (y))
//! 发送超时
#define TX_TIMEOUT_SECOND 10
//! 接收超时
#define RX_TIMEOUT_SECOND 10

typedef struct
{
    std::string strPartitionName;
    std::string strPartitionImageName;
}S_PARTITION_INFO,*LPS_PARTITION_INFO;

typedef struct 
{
    std::string mstrFileName;
    std::string mstrFilePath;
}S_CONTENTS_FILES,*LPS_CONTENTS_FILES;
//! sahara 
typedef struct
{
    //! 命令长度
    size_t      mcommand_len;
    //! 命令缓冲区
    BYTE*       mcommand_buf;
    //! 分区名称
    std::string mstrPartitionName;
    //! 分区下载路径
    std::string mstrPartitionMBNPath;
}s_stream_protocol_command;

typedef struct 
{
    size_t      mfile_len;
    BYTE*       mfile_buf;
}s_file_info;

//typedef struct
//{
//    s_file_info ms_EnArmPrgFileInfo; //! image_id :13  紧急下载ARM PRG
//    s_file_info ms_NArmPrgFileInfo;//! image_id :7
//}s_sahara_protocol_armprg_file_info,*ps_sahara_protocol_armprg_file_info;

typedef std::vector<s_stream_protocol_command> s_streaming_download_info;

typedef struct
{
    DWORD64                                 mnFileSizeCounts;
    //s_sahara_protocol_armprg_file_info      msSaharahProtocolArmPrgFile; //! sahara协议
    std::vector<s_stream_protocol_command>  mvStreamProtCommand; //! 流协议下载命令
}s_download_all_command,*ps_download_all_command;

typedef struct
{
    size_t mnBuf_Pos; //! 文件buf 定位
    size_t mnBuf_len; //! 文件buf 剩余长度
    size_t mnFile_Pos; //! 文件定位
    char*  mnFileTempBuf; //! 文件临时buf
}s_file_temp_buf_info,*ps_file_temp_buf_info;

typedef struct  
{
    bool                mbReset;
    std::string         mstrDownloadPath; //! 下载文件路径
    std::string         mstrContentXMLFilePath;
}s_download_config_info,*ps_download_config_info;

typedef struct  
{
    size_t                                  mnPos;
    DWORD64                                 mnFileSizeCounts;
    //ps_sahara_protocol_armprg_file_info     mpsSahara; //! sahara协议
    s_streaming_download_info*              mpsStreaming;
}s_ss_download_asio_config_info,*ps_ss_download_asio_config_info;


//! 下载状态
enum E_DOWNLOAD_STATUS
{
    //! E_INIT_DOWNLOAD = 0;
    E_IDLE_DOWNLOAD,            //! 下载初始化状态
    E_BUSY_DOWNLOAD,           //! 下载中
    E_ERROR_DOWNLOAD,           //! 下载错误
    E_SUCCESSFUL_DOWNLOAD       //! 下载成功
};

struct S_DOWNLOAD_PROGRESS_INFO
{
    E_DOWNLOAD_STATUS       meDownloadStatus;
    size_t					mnIndex;
    DWORD64                 mdw64SendCounts;
    std::string             mstrComPort;
};

enum LOG_TYPES
{
    LOG_ERROR = 1,
    LOG_ALWAYS,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,  // usually only goes to log, unless VerboseLevel==LOG_DEBUG
    LOG_ONLY  // always only goes to log, such as hex dump
};

#define dbg(log_level,nIndex, fmt, ...) MyLog(log_level,__FUNCTION__, __LINE__, fmt, __VA_ARGS__)

extern void MyLog (int log_level, const char *func_name, int line_number, const char *format, ...);



/*===========================================================================
 *
 *                          INCLUDE FILES
 *
 *  ===========================================================================*/

/*===========================================================================
 *
 *                     PUBLIC DATA DECLARATIONS
 *
 *  ===========================================================================*/
// Sahara Protocol Version
#define SAHARA_VERSION 2
#define SAHARA_VERSION_SUPPORTED 4

/*Maximum 1 megabyte tx buffer size*/
#define SAHARA_RAW_BUFFER_SIZE 0x100000

// Sahara command IDs
typedef enum
{
  SAHARA_NO_CMD_ID          = 0x00,
  SAHARA_HELLO_ID           = 0x01, // sent from target to host
  SAHARA_HELLO_RESP_ID      = 0x02, // sent from host to target
  SAHARA_READ_DATA_ID       = 0x03, // sent from target to host
  SAHARA_END_IMAGE_TX_ID    = 0x04, // sent from target to host
  SAHARA_DONE_ID            = 0x05, // sent from host to target
  SAHARA_DONE_RESP_ID       = 0x06, // sent from target to host
  SAHARA_RESET_ID           = 0x07, // sent from host to target
  SAHARA_RESET_RESP_ID      = 0x08, // sent from target to host
  SAHARA_MEMORY_DEBUG_ID    = 0x09, // sent from target to host
  SAHARA_MEMORY_READ_ID     = 0x0A, // sent from host to target
  SAHARA_CMD_READY_ID       = 0x0B, // sent from target to host
  SAHARA_CMD_SWITCH_MODE_ID = 0x0C, // sent from host to target
  SAHARA_CMD_EXEC_ID        = 0x0D, // sent from host to target
  SAHARA_CMD_EXEC_RESP_ID   = 0x0E, // sent from target to host
  SAHARA_CMD_EXEC_DATA_ID   = 0x0F, // sent from host to target
  SAHARA_64_BITS_MEMORY_DEBUG_ID	= 0x10, // sent from target to host
  SAHARA_64_BITS_MEMORY_READ_ID		= 0x11, // sent from host to target
  SAHARA_64_BITS_READ_DATA_ID		= 0x12,
  // place all new commands above this
  SAHARA_LAST_CMD_ID,
  SAHARA_MAX_CMD_ID             = 0x7FFFFFFF // To ensure 32-bits wide
} boot_sahara_cmd_id;

typedef enum {
    SAHARA_IMAGE_TYPE_BINARY = 0,               /* Binary format */
    SAHARA_IMAGE_TYPE_ELF,                      /* ELF format */
    SAHARA_IMAGE_UNKNOWN = 0x7FFFFFFF           /* To ensure 32-bits wide */
} boot_sahara_image;

// Status codes for Sahara
typedef enum
{
  // Success
  SAHARA_STATUS_SUCCESS =                     0x00,

  // Invalid command received in current state
  SAHARA_NAK_INVALID_CMD =                    0x01,

  // Protocol mismatch between host and target
  SAHARA_NAK_PROTOCOL_MISMATCH =              0x02,

  // Invalid target protocol version
  SAHARA_NAK_INVALID_TARGET_PROTOCOL =        0x03,

  // Invalid host protocol version
  SAHARA_NAK_INVALID_HOST_PROTOCOL =          0x04,

  // Invalid packet size received
  SAHARA_NAK_INVALID_PACKET_SIZE =            0x05,

  // Unexpected image ID received
  SAHARA_NAK_UNEXPECTED_IMAGE_ID =            0x06,

  // Invalid image header size received
  SAHARA_NAK_INVALID_HEADER_SIZE =            0x07,

  // Invalid image data size received
  SAHARA_NAK_INVALID_DATA_SIZE =              0x08,

  // Invalid image type received
  SAHARA_NAK_INVALID_IMAGE_TYPE =             0x09,

  // Invalid tranmission length
  SAHARA_NAK_INVALID_TX_LENGTH =              0x0A,

  // Invalid reception length
  SAHARA_NAK_INVALID_RX_LENGTH =              0x0B,

  // General transmission or reception error
  SAHARA_NAK_GENERAL_TX_RX_ERROR =            0x0C,

  // Error while transmitting READ_DATA packet
  SAHARA_NAK_READ_DATA_ERROR =                0x0D,

  // Cannot receive specified number of program headers
  SAHARA_NAK_UNSUPPORTED_NUM_PHDRS =          0x0E,

  // Invalid data length received for program headers
  SAHARA_NAK_INVALID_PDHR_SIZE =              0x0F,

  // Multiple shared segments found in ELF image
  SAHARA_NAK_MULTIPLE_SHARED_SEG =            0x10,

  // Uninitialized program header location
  SAHARA_NAK_UNINIT_PHDR_LOC =                0x11,

  // Invalid destination address
  SAHARA_NAK_INVALID_DEST_ADDR =              0x12,

  // Invalid data size receieved in image header
  SAHARA_NAK_INVALID_IMG_HDR_DATA_SIZE =      0x13,

  // Invalid ELF header received
  SAHARA_NAK_INVALID_ELF_HDR =                0x14,

  // Unknown host error received in HELLO_RESP
  SAHARA_NAK_UNKNOWN_HOST_ERROR =             0x15,

  // Timeout while receiving data
  SAHARA_NAK_TIMEOUT_RX =                     0x16,

  // Timeout while transmitting data
  SAHARA_NAK_TIMEOUT_TX =                     0x17,

  // Invalid mode received from host
  SAHARA_NAK_INVALID_HOST_MODE =              0x18,

  // Invalid memory read access
  SAHARA_NAK_INVALID_MEMORY_READ =            0x19,

  // Host cannot handle read data size requested
  SAHARA_NAK_INVALID_DATA_SIZE_REQUEST =      0x1A,

  // Memory debug not supported
  SAHARA_NAK_MEMORY_DEBUG_NOT_SUPPORTED =     0x1B,

  // Invalid mode switch
  SAHARA_NAK_INVALID_MODE_SWITCH =            0x1C,

  // Failed to execute command
  SAHARA_NAK_CMD_EXEC_FAILURE =               0x1D,

  // Invalid parameter passed to command execution
  SAHARA_NAK_EXEC_CMD_INVALID_PARAM =         0x1E,

  // Unsupported client command received
  SAHARA_NAK_EXEC_CMD_UNSUPPORTED =           0x1F,

  // Invalid client command received for data response
  SAHARA_NAK_EXEC_DATA_INVALID_CLIENT_CMD =   0x20,

  // Failed to authenticate hash table
  SAHARA_NAK_HASH_TABLE_AUTH_FAILURE =        0x21,

  // Failed to verify hash for a given segment of ELF image
  SAHARA_NAK_HASH_VERIFICATION_FAILURE =      0x22,

  // Failed to find hash table in ELF image
  SAHARA_NAK_HASH_TABLE_NOT_FOUND =           0x23,

  // Place all new error codes above this
  SAHARA_NAK_LAST_CODE,

  SAHARA_NAK_MAX_CODE = 0x7FFFFFFF // To ensure 32-bits wide
} boot_sahara_status;

// Status of all image transfers
typedef enum
{
  SAHARA_MODE_IMAGE_TX_PENDING  = 0x0,
  SAHARA_MODE_IMAGE_TX_COMPLETE = 0x1,
  SAHARA_MODE_MEMORY_DEBUG      = 0x2,
  SAHARA_MODE_COMMAND           = 0x3,

  // place all new commands above this
  SAHARA_MODE_LAST,
  SAHARA_MODE_MAX = 0x7FFFFFFF
} boot_sahara_mode;

// Executable commands when target is in command mode
typedef enum
{
  SAHARA_EXEC_CMD_NOP              = 0x00,
  SAHARA_EXEC_CMD_SERIAL_NUM_READ  = 0x01,
  SAHARA_EXEC_CMD_MSM_HW_ID_READ   = 0x02,
  SAHARA_EXEC_CMD_OEM_PK_HASH_READ = 0x03,
  SAHARA_EXEC_CMD_SWITCH_DMSS      = 0x04,
  SAHARA_EXEC_CMD_SWITCH_STREAMING = 0x05,
  SAHARA_EXEC_CMD_READ_DEBUG_DATA  = 0x06,

  // place all new commands above this
  SAHARA_EXEC_CMD_LAST,
  SAHARA_EXEC_CMD_MAX = 0x7FFFFFFF
} boot_sahara_exec_cmd_id;

/* Sahara Protocol states */
typedef enum {
    SAHARA_WAIT_HELLO,
    SAHARA_WAIT_COMMAND,
    SAHARA_WAIT_RESET_RESP,
    SAHARA_WAIT_DONE_RESP,
    SAHARA_WAIT_MEMORY_READ,
    SAHARA_WAIT_CMD_EXEC_RESP,
    SAHARA_WAIT_MEMORY_TABLE,
    SAHARA_WAIT_MEMORY_REGION,
    //! Command Switch Mode packet
    SAHARA_WAIT_COMMAND_SWITCH_MODE
} boot_sahara_state;

#include <stdint.h>
typedef struct
{
    uint32_t command;                 // command ID
    uint32_t length;                  // packet length incl command and length
} sahara_packet_header;

// HELLO command packet type - sent from target to host
//   indicates start of protocol on target side
typedef struct
{
    sahara_packet_header header;
    uint32_t version;                 // target protocol version number
    uint32_t version_supported;       // minimum protocol version number supported
    // on target
    uint32_t cmd_packet_length;       // maximum packet size supported for command
    // packets
    uint32_t mode;                    // expected mode of target operation
    uint32_t reserved0;               // reserved field
    uint32_t reserved1;               // reserved field
    uint32_t reserved2;               // reserved field
    uint32_t reserved3;               // reserved field
    uint32_t reserved4;               // reserved field
    uint32_t reserved5;               // reserved field
} sahara_packet_hello;

typedef struct
{
    sahara_packet_header header;
    uint32_t version;                 // host protocol version number
    uint32_t version_supported;       // minimum protocol version number supported
    // on host
    uint32_t status;                  // OK or error condition
    uint32_t mode;                    // mode of operation for target to execute
    uint32_t reserved0;               // reserved field
    uint32_t reserved1;               // reserved field
    uint32_t reserved2;               // reserved field
    uint32_t reserved3;               // reserved field
    uint32_t reserved4;               // reserved field
    uint32_t reserved5;               // reserved field
} sahara_packet_hello_resp;

// CMD_SWITCH_MODE packet type - sent from host to target
//   indicates to target to switch modes
typedef struct
{
    sahara_packet_header header;
    uint32_t mode;                    // mode of operation for target to execute
} sahara_packet_cmd_switch_mode;

typedef struct
{
    sahara_packet_header header;
    uint32_t image_id;                // ID of image to be transferred
    uint32_t data_offset;             // offset into image file to read data from
    uint32_t data_length;             // length of data segment to be retreived
    // from image file
} sahara_packet_read_data;

// END_IMAGE_TX command packet type - sent from target to host
//   indicates end of a single image transfer and status of transfer
typedef struct
{
    sahara_packet_header header;
    uint32_t image_id;                // ID of image to be transferred
    uint32_t status;                  // OK or error condition
} sahara_packet_end_image_tx;

// DONE packet type - sent from host to target
//   indicates end of single image transfer
typedef struct
{
    sahara_packet_header header;
} sahara_packet_done;

typedef struct
{
    sahara_packet_header header;
    uint32_t mode;//! Mode of operation for target to execute
}sahara_switch_mode_packet;

// DONE_RESP packet type - sent from target to host
//   indicates end of all image transfers
typedef struct
{
    sahara_packet_header header;
    uint32_t image_tx_status;         // indicates if all images have been
    // transferred;
    // 0 = IMAGE_TX_PENDING
    // 1 = IMAGE_TX_COMPLETE
} sahara_packet_done_resp;

// RESET_RESP packet type - sent from target to host
//   indicates to host that target has reset
typedef struct
{
    sahara_packet_header header;
} sahara_packet_reset_resp;

// RESET packet type - sent from host to target
//   indicates to target to reset
typedef struct
{
    sahara_packet_header header;
} sahara_packet_reset;

typedef struct
{
    boot_sahara_state sahara_state;
    int offset;
    sahara_packet_done* sahara_done;
    sahara_packet_hello* sahara_hello;
    sahara_packet_hello_resp* sahara_hello_resp;
    sahara_packet_end_image_tx* sahara_end_image_tx;
    sahara_packet_read_data* sahara_read_data;
    sahara_packet_done_resp* sahara_done_resp;
    sahara_switch_mode_packet* sahara_switch_mode;
    sahara_packet_reset* sahara_reset;
    sahara_packet_reset_resp* sahara_reset_resp;
}s_sahara_info;








/******************************************************************************
SW Download - Streaming Download subsystem

For complete command list, please refer to 80-V5348-1

*******************************************************************************/
/**
definitions of legal streaming download protocol commands
*/
typedef enum
{
    HELLO_CMD =          0x1,                //!< ' Hello command                 - sent */
    HELLO_RSP_CMD =      0x2,                //!< ' Hello command response        - recv */
    READ_CMD =           0x3,                //!< ' Read command                  - sent */
    READ_RSP_CMD =       0x4,                //!< ' Read command response         - recv */
    S_WRITE_CMD =        0x5,                //!< ' Simple write command          - sent */
    S_WRITE_RSP_CMD =    0x6,                //!< ' Simple write command response - recv */
    STRM_WRITE_CMD =     0x7,                //!< ' Stream write command          - sent */
    STRM_WRITE_RSP_CMD = 0x8,                //!< ' Stream write command response - recv */
    STRM_NOP_CMD =            0x9,               //!< ' NOP command                   - sent */
    STRM_NOP_RSP_CMD =        0xa,               //!< ' NOP command response          - recv */
    STRM_RESET_CMD =          0xb,               //!< ' Reset command                 - sent */
    STRM_RESET_RSP_CMD =      0xc,               //!< ' Reset command response        - recv */
    ERROR_CMD =          0xd,                //!< ' Error response                - recv */
    LOG_CMD =            0xe,                //!< ' Log response                  - recv */
    STRM_UNLOCK_CMD =         0xf,               //!< ' Unlock command                - sent */
    UNLOCK_RSP_CMD =    0x10,                //!< ' Unlock command response       - recv */
    POWER_OFF_CMD =     0x11,                //!< ' Power off command             - sent */
    POWER_OFF_RSP_CMD = 0x12,                //!< ' Power off command response    - recv */
    DWNLDR_OPEN_CMD =   0x13,                //!< ' Open downloader               - sent */
    DWNLDR_OPEN_RSP_CMD = 0x14,              //!< ' Open downloader response      - recv */
    DWNLDR_CLOSE_CMD =  0x15,                //!< ' Close downloader              - sent */
    DWNLDR_CLOSE_RSP_CMD = 0x16,         //!< ' Close downloader response     - recv */
    DWNLDR_SECURITY_MODE_CMD = 0x17,     //!< ' Multi-image security mode          - sent
    DWNLDR_SECURITY_MODE_RSP_CMD = 0x18, //!< ' Multi-image security mode response - recv
    DWNLDR_PRTN_TBL_CMD = 0x19,              //!< ' Multi-image partition command      - sent
    DWNLDR_PRTN_TBL_RSP_CMD = 0x1A,          //!< ' Multi-image partition response     - recv
    DWNLDR_MULTI_IMG_OPEN_CMD = 0x1B,        //!< ' Multi-image open command           - sent
    DWNLDR_MULTI_IMG_OPEN_RSP_CMD = 0x1C,    //!< ' Multi-image open response          - recv
    DWNLDR_FLASH_ERASE_CMD = 0x1D,           //!< ' Erase flash (multi- to single- d/l) cmd
    DWNLDR_FLASH_ERASE_RSP_CMD = 0x1E,       //!< ' Erase flash response
    UNFRAMED_STREAM_WRITE_CMD = 0x30,        //!< ' Unframed Stream Write command      - sent
    UNFRAMED_STREAM_WRITE_RSP_CMD = 0x31,    //!< ' Unframed Stream Write response command  - recv
    QFPROM_WRITE_CMD         = 0x32,         //!< ' QFPROM write command - sent
    QFPROM_WRITE_RSP_CMD     = 0x33,         //!< ' QFPROM write response = recv
    QFPROM_READ_CMD          = 0x34,         //!< ' QFPROM read command - sent
    QFPROM_READ_RSP_CMD      = 0x35          //!< ' QFPROM read response = recv
} Streaming_Download_Cmd_Enum;

static const char szStreamingDownloadCmdEnumString[][60] = {"",
    "Hello command                 - sent",//!0x01
    "Hello command response        - recv",//! 0x02
    "Read command                  - sent",//! 0x03
    "Read command response         - recv",//! 0x04
    "Simple write command          - sent",//! 0x05
    "Simple write command response - recv",//! 0x06
    "Stream write command          - sent",//! 0x07
    "Stream write command response - recv",//! 0x08
    "NOP command                   - sent",//! 0x09
    "NOP command response          - recv",//! 0x0A
    "Reset command                 - sent",//! 0x0B
    "Reset command response        - recv",//! 0x0C
    "Error response                - recv",//! 0x0D
    "Log response                  - recv",//! 0x0E
    "Unlock command                - sent",//! 0x0F
    "Unlock command response       - recv",//! 0x10
    "Power off command             - sent",//! 0x11
    "Power off command response    - recv",//! 0x12
    "Open downloader               - sent",//! 0x13
    "Open downloader response      - recv",//! 0x14
    "Close downloader              - sent",//! 0x15
    "Close downloader response     - recv",//! 0x16
    "Multi-image security mode          - sent",//! 0x17
    "Multi-image security mode response - recv",//! 0x18
    "Multi-image partition command      - sent",//! 0x19
    "Multi-image partition response     - recv",//! 0x1A
    "Multi-image open command           - sent",//! 0x1B
    "Multi-image open response          - recv",//! 0x1C
    "Erase flash (multi- to single- d/l) cmd",//! 0x1D
    "Erase flash response",//! 0x1E
    "","","","","","","","","","",//!0x1F~ 0x28
    "","","","","","","","Unframed Stream Write command      - sent","Unframed Stream Write response command  - recv",//! 0x29  ~ 0x31
    "QFPROM write command - sent",//! 0x32
    "QFPROM write response = recv",//! 0x33
    "QFPROM read command - sent",//! 0x34
    "QFPROM read response = recv",//! 0x35
};

enum CMD_ERROR_CODE
{
    E_STCMD_IIEGAL = 0x00,
    E_STCMD_RESERVED,
    E_STCMD_INV_DEST_ADDRESS,
    E_STCMD_INV_LEN,
    E_STCMD_UNEXPECTED,
    E_STCMD_INV_CMD,
    E_STCMD_RESERVERD_SIX,
    E_STCMD_OPERATION,
    E_STCMD_WRONG_FLAS,
    E_STCMD_BAD_PROGRAM,
    E_STCMD_WRITE_VERIFY,
    E_STCMD_RESERVED_B,
    E_STCMD_INCORRECT_SECURITY,
    E_STCMD_CANNOT_POWER_DOWN,
    E_STCMD_NAND_FLASH_RPOGRAM_NOT_SUPPORT,
    E_STCMD_COMMAND_OUT_OF_SEQ,
    E_STCMD_CLOSE_DID_NOT_SUCCEED,
    E_STCMD_INCOMPATIBLE_FEATURE_BITS,
    E_STCMD_OUT_OF_SPACE,
    E_STCMD_INVALID_SEC_MODE,
    E_STCMD_MULTI_IMAGE_NAND_NOT_SUPPORT,
    E_STCMD_POWER_OFF
};

// enum CMD_SAHARA_DWONLOAD
// {
//     SACMD_UNKNOW = 0x00,
//     SACMD_HELLO = 0x01,
//     SACMD_HELLORESPONSE = 0x02,
//     SACMD_READ_DATA = 0x03,
//     SACMD_END_OF_IMAGE_TRANSFER = 0x04,
//     SACMD_DONE = 0x05,
//     SACMD_DONE_REPONSE = 0x06,
// };
// 
// enum CMD_DMSS_DOWNLOAD
// {
//     CMD_DMSS_UNKNOW = 0x00,
//     CMD_ACK = 0x02,
//     CMD_NAK_RESPONSE = 0x03,
//     CMD_GO = 0x05,
//     CMD_NOP = 0x06,
//     CMD_PARAM_REQUEST = 0x07,
//     CMD_PARAM_RESPONSE = 0x08,
//     CMD_WRITE32 = 0x0F,
//     CMD_FLAG_END = 0x7E,
// };

// enum CMD_STREAMING_DOWNLOAD
// {
//     CMD_STREAMING_UNKNOW = 0x00,
//     CMD_HELLO	= 0x01,
//     CMD_HELL_RESPONSE = 0x02,
//     CMD_STREAMING_WRITE = 0x07,
//     CMD_BLOCK_WRITTEN_RESPONSE = 0x08,
//     CMD_RESET = 0x0B,
//     CMD_RESET_ACK_RESPONSE = 0x0C
//     CMD_LOG_PACKET = 0x0E,
//     CMD_ERROR = 0x0D,
//     CMD_CLOSE_PACKET	= 0x15,
//     CMD_CLOSE_PACKET_RESPONSE = 0x16,
//     CMD_SECURITY_MODE = 0x17,
//     CMD_SECURITY_MODE_RESPONSE = 0x18,
//     CMD_PARTITION_TABLE = 0x19,
//     CMD_PARTITION_TABLE_RESPONSE = 0x1A,
//     CMD_OPEN_MULTIIMAGE = 0x1B,
//     CMD_OPEN_MULTIIMAGE_RESPONSE = 0x1C, 
// };

#pragma pack(push, 1)

typedef struct  
{
    BYTE frame_header; //! FLAG_FRAME
}s_stream_frame;

typedef struct  
{
    BYTE frame_header; //! FLAG_FRAME
    BYTE command;
}s_stream_frame_header,*ps_stream_frame_header;
//! packet
typedef struct  
{
    BYTE fcs[2];
    BYTE flag; //! FLAG_FRAME
}s_stream_frame_end;

typedef struct
{
    BYTE command;
}s_stream_command,*ps_stream_command;

typedef struct  
{
    BYTE    command; //! 0x01
    BYTE    magic_number[32]; //! "QCOM fast download protocol host"
    BYTE    version_number; //!
    BYTE    compatible_version; //! 
    BYTE    feature_bits;//! (N LENGTH)
}s_stream_hello_packet;

typedef struct
{
    BYTE    command; //! 0x02
    BYTE    magic_number[32]; //! "QCOM fast download protocol host"
    BYTE    version_number; //!
    BYTE    compatible_version; //! 
    UINT32  maximum;
    UINT32  base_address_of_flash;
    UINT32  flash_id_length;
    BYTE    flash_id_lenth;
    BYTE    flash_identifier[1]; //! N* flash_id_lenth
    UINT16  window_size;
    UINT16  number_of_sectors;
    BYTE    sectors[1];//! N* number_of_sectors
    BYTE    feature_bits[1];//! (N LENGTH)
}s_stream_hello_response_packet,*ps_stream_hello_response_packet;

typedef struct  
{
    BYTE    command; //! 0x17
    BYTE    mode; //! 0x01  0x0 – Nontrusted 0x1 – Trusted
}s_stream_security_mode_packet;

//! CMD_SECURITY_MODE_RESPONSE
typedef struct  
{
    BYTE    command; //! 返回非0x18则代表有错误
}s_stream_security_mode_response_packet;

typedef struct
{
    BYTE command; //! 0x19
    BYTE override; //! 0x0 no override 0x01 override existing table  default 0x00
    BYTE partition_table[1]; //! partition buf (max length 512)
}s_stream_partition_table_packet;

/*
0x0——分区表接受
0x1——分区表不同，覆盖被接受
0x2——分区表格式不被识别，不接受覆盖
0x3——擦除操作失败
*/
typedef struct
{
    BYTE command;
    BYTE status;//! 0x0 – Partition table accepted 0x1 – Partition table differs, override is accepted 0x2 – Partition table format not recognized, does not accept override 0x3 – Erase operation failed
}s_stream_partition_table_response_packet;

typedef struct  
{
    BYTE command; //! 0x1b
    BYTE type; //! 0x0E
    BYTE data[1];//! max 512 bytes 分区名称 + 1
}s_stream_multi_image_packet,*ps_stream_multi_image_packet;
/*
 0x0 – Open successful
 0x1 – Payload length exceeded, fail
 0x2 – No payload expected, fail
 0x3 – Payload required, fail
 0x4 – Block 0 write protected, fail
*/
typedef struct
{
    BYTE command; //! 0x0C
    BYTE status; //! 0x0 – Open successful 0x1 – Payload length exceeded, fail 0x2 – No payload expected, fail 0x3 – Payload required, fail 0x4 – Block 0 write protected, fail
}s_stream_multi_image_response_packet;

typedef struct  
{
    BYTE command;//! 0x1D
}s_stream_erase_flash_packet;

typedef struct  
{
    BYTE command; //! 0x15 
}s_stream_close_packet;

typedef struct  
{
    BYTE command; //! 0x16
}s_stream_close_response_packet;

typedef struct  
{
    BYTE command; //! 0x0B
}s_stream_reset_packet;

typedef struct  
{
    BYTE command; //! 0x0C
}s_stream_reset_response_packet;

typedef struct  
{
    BYTE command; //! 0x07
    UINT32 address; //! 文件地址
    BYTE data[1];
}s_stream_write_packet,*ps_stream_write_packet;

typedef struct  
{
    BYTE command; //! 0x07
    UINT32 address; //! 文件地址
}s_stream_write_response_packet,*ps_stream_write_response_packet;

typedef struct  
{
    BYTE command; //! 0x0D
    UINT32 error_code; //! 
    BYTE text_data[1];
}s_streaming_error_packet,*ps_streaming_error_packet;

// typedef struct  
// {
//     BYTE command; //! 0x0E
//     BYTE text[1];
// }s_streaming_log_packet,*ps_streaming_log_packet;

/**********************PACKET struct *****************/
// typedef struct _PACKET_HEADER_
// {
//     BYTE FrameHeader;
//     BYTE Command;
// }PACKET_HEADER;
// 
// typedef struct _PACKET_END_
// {
//     BYTE FCS[2];
//     BYTE Flag;
// }PACKET_END;
// 
// typedef struct _PACKET_DATA_
// {
//     BYTE *pData;
//     PACKET_END packetend;
// }PACKET_DATA;

typedef struct 
{
    BYTE data[1024];
}s_stream_debug_info,*ps_stream_debug_info;

typedef struct
{
    BYTE    command;
    BYTE    log[1];
}s_streaming_log_info,*ps_streaming_log_info;

typedef struct  
{
    ps_streaming_error_packet mpsStreamingErrorCode;
    ps_streaming_log_info mpsStreamingLoginfo;
    ps_stream_command mpsStreamCommand;
    ps_stream_hello_response_packet mpStreamHelloResponsePacket;
}s_streaming_info;

typedef struct  
{
    BYTE command;//! 0x30
    BYTE padding_bits[3]; //! 0x00
    UINT32 address;
    UINT32 length;
    BYTE data[1];
}s_streaming_unframe_stream_write_packet,*ps_streaming_unframe_stream_write_packet;

typedef struct  
{
    BYTE command;//! 0x31
    UINT32 address;
}s_streaming_unframe_stream_write_response_packet;

#pragma pack(pop)