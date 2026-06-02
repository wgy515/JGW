#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#define MAX_XML_SIZE    (4*1024)

//! 小于等于512KB的下载文件提前缓存 524288
#define MAX_FILE_CACHE_BUFFER (1024*1024)

#define MAX_REWRITE_PROGRAM_COMMAND 3
const int setactivepartition = 1;
#define snprintf _snprintf
#define MAX_XML_FILES           8000
#define ONE_MEGABYTE            1048576
#define FIREHOSE_TX_BUFFER_SIZE (ONE_MEGABYTE)
#define XML_HEADER_LENGTH 41    // 40 does NOT include null
#define GETCWD _getcwd
#define ZLPAWAREHOST 1
#define SLASH '\\'    // defined differently below for LINUX
#define WRONGSLASH '/'
#define O_RDWR _O_RDWR
#define O_SYNC _O_SEQUENTIAL
#define sleep(x) Sleep(x*1000)

#define SIZE_T_64 long long int
#define SIZE_T_FORMAT "lld"    // Use in middle of string "Channel read "SIZE_T_FORMAT" bytes", at end of string "num_physical_partitions="SIZE_T_FORMAT
#define SIZE_T_FORMAT4 ".4lld"
#define SIZE_T_FORMAT5 ".5lld"
#define fseek _fseeki64
#define ftell _ftelli64
#define snprintf _snprintf

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif
//! 发送超时
#define TX_TIMEOUT_SECOND 10
//! 接收超时
#define RX_TIMEOUT_SECOND 10

#define SPARSE_HEADER_MAGIC 0xed26ff3a
#define CHUNK_TYPE_RAW 0xCAC1
#define CHUNK_TYPE_FILL 0xCAC2
#define CHUNK_TYPE_DONT_CARE 0xCAC3
#define SPARSE_HEADER_MAJOR_VER 1

#define MAX_STRING_SIZE 2048
#define MAX_PATH_SIZE 2048
#define MAX_STRING_PAIR_SIZE (2*4096)
#define MAX_NUMBER(a,b) (((a)>(b))?(a):(b))

enum LOG_TYPES
{
    LOG_ERROR = 1,
    LOG_ALWAYS,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,  // usually only goes to log, unless VerboseLevel==LOG_DEBUG
    LOG_ONLY  // always only goes to log, such as hex dump
};

#define MAX_READ_BUFFER_SIZE ONE_MEGABYTE
extern SIZE_T_64 MaxBytesToReadFromUSB ;  // this is changed to fh.attrs.MaxPayloadSizeFromTargetInBytes after handleConfigure()

typedef struct
{
    size_t MaxPayloadSizeToTargetInBytes;     // numeric attributes
    SIZE_T_64 MaxPayloadSizeToTargetInBytesSupported;  // DDR version would typically have this set to 1MB
    SIZE_T_64 MaxPayloadSizeFromTargetInBytes;
    SIZE_T_64 MaxDigestTableSizeInBytes;
    SIZE_T_64 AckRawDataEveryNumPackets;
    SIZE_T_64 delayinseconds;
    SIZE_T_64 address64;
    SIZE_T_64 value64;
    SIZE_T_64 storagedrive;
    SIZE_T_64 SECTOR_SIZE_IN_BYTES;
    SIZE_T_64 byte_offset;
    SIZE_T_64 physical_partition_number;
    SIZE_T_64 size_in_bytes;
    SIZE_T_64 file_sector_offset;

    SIZE_T_64 ZlpAwareHost;        // bool attributes but made SIZE_T_64.
    SIZE_T_64 SkipWrite;     // otherwise the generic function that assigns values
    SIZE_T_64 BufferWrites;
    //SIZE_T_64 AckRawData;
    //SIZE_T_64 display;
    //SIZE_T_64 read_back_verify;
    SIZE_T_64 AlwaysValidate;      // will possibly overwrite the next address in memory
    SIZE_T_64 Verbose;
    SIZE_T_64 commit;
    SIZE_T_64 trials;
    //SIZE_T_64 display;
    SIZE_T_64 TestWritePerformance;
    SIZE_T_64 TestReadPerformance;
    SIZE_T_64 TestDigestPerformance;
    //SIZE_T_64 read_back_verify;
    SIZE_T_64 SkipStorageInit;
    SIZE_T_64 SkipSettingMinClocks;
    SIZE_T_64 SkipSettingMaxClocks;
    SIZE_T_64 actual_size_in_bytes;

    //SIZE_T_64 start_sector;    // special attributes
    char start_sector[MAX_STRING_SIZE];    // special attributes
    SIZE_T_64 num_partition_sectors;
    char filename[MAX_STRING_SIZE];
    char value[1024 * 1024]; //On PC, this line handles log messages char value[MAX_STRING_SIZE];

    char MemoryName[MAX_STRING_SIZE];
    char TargetName[MAX_STRING_SIZE];
    char Commit[MAX_STRING_SIZE];       // We have some commit='true', which eventually goes to set fh.attrs.commit=1

    // these are stored into first in preParseAttributes(), then
    // parseAttributes() fills in start_sector and num_partition_sectors
    //char start_sector_value[64];
    //char num_partition_sectors_value[64];

    //hotplug_dev_type storage_type;  // attribute MemoryName={"eMMC" or "ufs"}
} firehose_attrs_t;


typedef struct
{
    firehose_attrs_t attrs;
} firehose_protocol_t;

extern firehose_protocol_t fh;

#define dbg_debug //

#define dbg(log_level,nIndex, fmt, ...) MyLog(log_level,nIndex, _T(__FUNCTION__), __LINE__, fmt, __VA_ARGS__)

//#define dbg_debug(log_level,nIndex, fmt, ...) MyLog(log_level,nIndex, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define memscpy(dest,destsize,src,srcsize) memcpy(dest,src,srcsize)
static const char xml_header[XML_HEADER_LENGTH] = "<\?xml version=\"1.0\" encoding=\"UTF-8\" \?>\n";

extern std::string g_strXMLStringTable[MAX_XML_FILES];

namespace JGW
{
    extern void MyLog (int log_level,int nIndex, const wchar_t *func_name, int line_number, const wchar_t *format, ...);

    extern SIZE_T_64 CopyString (char *Dest, char *Source, SIZE_T_64  Dstart, SIZE_T_64  Sstart, SIZE_T_64  length, SIZE_T_64 DestSize, SIZE_T_64 SourceSize);

    extern void close_file(FILE* fd);
}

#if 0
typedef struct
{
    void*       mfile_buf; //! 文件缓冲buf
    SIZE_T_64      mfile_len;
    SIZE_T_64 SectorSizeInBytes; //! 段大小字节
    std::string mstrXMLConfig; //! 发送的下载协议命令
    std::string mstrProgramFilePath; //! 文件保存路径
    
}firehose_xml_config;
#else
class firehose_xml_config
{
public:
    firehose_xml_config():mfile_buf(NULL),mfile_buf_len(0),SectorSizeInBytes(0)
    {

    }
public:
    void*       mfile_buf; //! 文件缓冲buf
    size_t      mfile_buf_len;
    size_t      SectorSizeInBytes; //! 段大小字节
    std::string mstrXMLConfig; //! 发送的下载协议命令
    std::string mstrProgramFilePath; //! 文件保存路径
};

#endif

typedef struct
{
    void*       file_buf;
    size_t      file_len;
}s_download_file_info;




// typedef struct
// {
//     const std::vector<firehose_xml_config>* mpVFirehose;
//     int mnXMLPos;
//     bool mbRawMode;
// }SV_XML_CONFIG_INFO,*LP_SV_XML_CONFIG_INFO;

struct s_firehose_command_info
{
	const char*		mszCommand;
	size_t			mpos;
	size_t			mcommand_len;
};
/*LPCSTR*/
typedef std::vector<firehose_xml_config> s_firehose_download_info;
/*typedef std::vector<firehose_xml_config>* ps_firehose_download_info;*/
/*typedef s_firehose_download_info *ps_firehose_download_info;*/

typedef struct
{
	bool mbRawMode;
	int mnXMLPos;
	const s_firehose_download_info* mpVFirehose;
	s_firehose_command_info sfirehose_info;
}s_firehose_all_command_info;

typedef struct
{
    DWORD64                         mnFileSizeCounts; //! 所有文件大小
    s_download_file_info            ms_sahara_arm_prg_file_info; //! arm prg文件信息
    s_firehose_download_info        ms_firehose_download_info;
}s_download_all_file_info,*ps_download_all_file_info;

typedef struct
{
    bool                            mbReset;//! 是否下载完成重启
    std::string                     mstrDownloadPath; //! 下载配置文件路径
    std::string                     mstrArmPrgFilePath;
    std::vector<std::string>        mvstrRawProgram;
    std::vector<std::string>        mvstrPatch;
}s_download_config_info,*ps_download_config_info;


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
    sahara_packet_header header;
    uint64_t image_id;                // ID of image to be transferred
    uint64_t data_offset;             // offset into image file to read data from
    uint64_t data_length;             // length of data segment to be retreived
    // from image file
} sahara_packet_read_data_64bit;

typedef struct
{
    boot_sahara_state sahara_state;
    int offset;
    sahara_packet_done* sahara_done;
    sahara_packet_hello* sahara_hello;
    sahara_packet_hello_resp* sahara_hello_resp;
    sahara_packet_end_image_tx* sahara_end_image_tx;
    sahara_packet_read_data* sahara_read_data;
    sahara_packet_read_data_64bit* sahara_read_data_64bit;
    sahara_packet_done_resp* sahara_done_resp;
    sahara_packet_reset* sahara_reset;
    sahara_packet_reset_resp* sahara_reset_resp;
}s_sahara_info;




#define memalign(x, y) _aligned_malloc(y, x)
#define ALIGNED_FREE _aligned_free
#define MIN_NUMBER(x,y) ((x) < (y) ? (x) : (y))

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