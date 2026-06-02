#pragma once
#include <cstdint>
#include <vector>
#include <string>

/*#define FLASH_ENABLE_ARRAY */
//! 发送超时
#define TX_TIMEOUT_SECOND 10
//! 接收超时
#define RX_TIMEOUT_SECOND 10

#define memalign(x, y) _aligned_malloc(y, x)
#define ALIGNED_FREE _aligned_free

#define MAX_PUMA_RSP_BUF_SIZE 1024

#define RESP_TERMINATOR '>'
// Flash Specific parameters
#define FLASH_PAGE_SIZE         (256)                   // in # of byte
#define FLASH_PAGE_PER_SECTOR   (256)                   // in # of page
#define FLASH_SECTOR_SIZE       (FLASH_PAGE_PER_SECTOR * FLASH_PAGE_SIZE)
#define OPCODE_PAGE_PROGRAM     '2'
#define OPCODE_READ_DATA        '3'
#define FLASH_READ_SIZE         (256)
#define HEX_DISPLAY_LINE_LEN    (16)

/// @brief Default size of flash device
#define PRS_FLASH_DEFAULT_SIZE_KB                       (1024)

/// @brief Peraso flashing and read-back procedure for "OTP" and "Config" sections
///        differentiates flash devices into two range of sizes
#define SIZE_BYTES(size_kb)                             ((size_kb * 1024))
#define PRS_SMALL_FLASH_SIZE                            (0x80000)
#define PRS_LARGE_FLASH(total_size_kb)                  (SIZE_BYTES(total_size_kb) > PRS_SMALL_FLASH_SIZE)
#define PRS_SMALL_FLASH(total_size_kb)                  (!(PRS_LARGE_FLASH(total_size_hex)))

/// @brief Flash size offsets (UMAC dependencies)
#define PRS_FW_IMAGE_OFFSET(total_size_kb)              (0)
#define PRS_FW_IMAGE_SECTION_END(total_size_kb)         (PRS_CONFIG_HEADER_OFFSET(total_size_kb))

#define PRS_CONFIG_HEADER_OFFSET(total_size_kb)         (PRS_LARGE_FLASH(total_size_kb) ? (SIZE_BYTES(total_size_kb) - (2*0x10000)) : 0x7E000)
#define PRS_CONFIG_BODY_OFFSET(total_size_kb)           (PRS_LARGE_FLASH(total_size_kb) ? (SIZE_BYTES(total_size_kb) - (2*0x10000) + 0x20) : 0x7E020)
#define PRS_CONFIG_SECTION_END(total_size_kb)           (PRS_OTP_BODY_OFFSET(total_size_kb))

#define PRS_OTP_HEADER_OFFSET(total_size_kb)            (PRS_LARGE_FLASH(total_size_kb) ? (SIZE_BYTES(total_size_kb) - (1*0x10000)) : 0x7F000)
#define PRS_OTP_BODY_OFFSET(total_size_kb)              (PRS_LARGE_FLASH(total_size_kb) ? (SIZE_BYTES(total_size_kb) - (1*0x10000) + 0x20) : 0x7F020)
#define PRS_OTP_SECTION_END(total_size_kb)              (SIZE_BYTES(total_size_kb))

/// @brief Standard (expected) file names
#define PRS_CONFIG_BODY_FILE_NAME                       "persistent_database_1_config.bin"
#define PRS_CONFIG_HEADER_FILE_NAME                     "persistent_database_1_config_header.bin"

/// @brief Standard (expected) OTP file names
#define PRS_OTP_BODY_FILE_NAME                          "persistent_database_0_otp.bin"
#define PRS_OTP_HEADER_FILE_NAME                        "persistent_database_0_otp_header.bin"

/// @brief Default name of the FW image file
#define DEFAULT_IMAGE_BINARY_FILENAME                   "image.bin"

#define BOOTROM_COMMAND_ERROR_CHAR                      (0x21)

//! 检查error code
#define CHECK_ERRORCODE_RETURN(ec,msg,ret_val) \
    if (ec)\
    {\
    dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());\
    return ret_val;\
    }
//! 检查error code
#define CHECK_ERRORCODE(ec,msg) \
    if (ec)\
    {\
    dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());\
    return;\
    }
//! 打印错误消息
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) dbg(LOG_ERROR,msDownloadProgressInfo.mnIndex,msg,ec.message().c_str());

#define dbg_debug(log_level,nIndex, fmt, ...) //! MyLog(log_level,nIndex, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)

#define dbg(log_level,nIndex, fmt, ...) MyLog(log_level,nIndex, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)

namespace JGW
{
    const char scTblHex2Ascii[] = "0123456789ABCDEF";

    const char spiEnable[]        = "se 0\r";
    const size_t spiEnableLen   = strlen(spiEnable);

    const char spiSetBaud[]       = "sb 4\r";
    const size_t spiSetBaudLen = strlen(spiSetBaud);

    const char scSpiWriteCmd[]    = "sw b 0 ";

    const char scSpiReadCmd[]     = "sr b 0 ";

    const char flashErase[]       = "sw b 0 1 C7\r";

    const char flashIdentity[]    = "sr b 0 1 9f 10\r";
    const size_t flashIdentityLen   = strlen(flashIdentity);

    const char flashEnable[]      = "sw b 0 1 6\r";
    const size_t flashEnableLen   = strlen(flashEnable);

    const char flashReadStatus[]  = "sr b 0 1 05 1\r";

    const char resetcmd[]   = "reset\r";
    const size_t resetcmdLen   = strlen(resetcmd);

    typedef enum _eCmdOption
    {
        eCmdOption_Help = 0,
        eCmdOption_Size,
        eCmdOption_ComPort,
        eCmdOption_Baud,
        eCmdOption_Image,
        eCmdOption_ConfigDatabase,
        eCmdOption_OTPDatabase,
        eCmdOption_InputFolder,
        eCmdOption_PrintMemory,
        eCmdOption_Erase,
        eCmdOption_Version,
        eCmdOption_Reset,
        eCmdOption_All,
        eCmdOption_Max,
    } ECmdOption;

#define RESULT_OK(result) (result < ((1 << eDownloadResult_maxWarning) << 1))

    enum EDownloadResult {
        eDownloadResult_success,
        eDownloadResult_setUsbLatencyWarning,
        eDownloadResult_maxWarning = eDownloadResult_setUsbLatencyWarning,
        eDownloadResult_invalidArgsFailure,
        eDownloadResult_findBootromPortFailure,
        eDownloadResult_openBootromPortFailure,
        eDownloadResult_setBootloaderBaudFailure,
        eDownloadResult_downloadImageFailure,
        eDownloadResult_downloadConfigFailure,
        eDownloadResult_downloadOTPFailure,
        eDownloadResult_invalidImage,
        eDownloadResult_resources,
    };

    enum FlashValues {
        // stating address for flash memory
        FlashMemoryBase = 0xF8000000,
        // mask for flash offset value
        FlashOffsetMask = 0x00FFFFFF,
        // config header offset
        FlashConfigHeaderOffset = 0xE0000,
        // config offset
        FlashConfigOffset = 0xE0020,
        // OTP header offset
        FlashOtpHeaderOffset = 0xF0000,
        // OTP offset
        FlashOtpOffset = 0xF0020,
        // image offset
        FlashImageOffset = 0
    };

    typedef enum
    {
        ePrsSectionType_FWImage = 0,
        ePrsSectionType_Config,
        ePrsSectionType_OTP,
        ePrsSectionType_Max,

    } EPrsSectionType;

    enum EDownloadStep
    {
        E_SPI_START,
        E_SPI_Enable,
        E_SPI_SetBaud,
        E_FLASH_Identity,
        E_Donwload_ImageDataInfo,
        E_Validate_DownloadDataInfo,
        E_Reset_Cmd
    };

    typedef std::string String;

    typedef unsigned __int64 DWORD64;

    typedef struct _tagSPrsFlashCmdParms
    {
        String              imagesFolder;
        String              imageBinName;
        String              baudRate;
        String              serialDevice;
        uint8_t             bFlashOTP;
        uint8_t             bFlashConfig;
        uint8_t             bFlashImage;
        uint8_t             bPrintMemory;
        uint8_t             bErase;
        uint32_t            address;
        uint32_t            len;
        uint32_t            flashSizeKb;
        uint8_t             bReset;
        uint32_t            bVersion;
    } SPrsFlashCmdParms;

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

    typedef struct  
    {
        char* mpFmtBuffer; //! 数据信息
        char* mpDownloadCmd; //! 下载命令
        size_t mAddress; //! 地址
        size_t mDataSize;//! 数据大小，未格式化的长度
        size_t mFmtBufferSize;//! 格式化的大小 
    }S_PUMA_IMAGE_DATA_INFO,*PS_PUMA_IMAGE_DATA_INFO;

    typedef struct  
    {
        bool mbValidateImageDataInfo; //! 是否验证下载镜像信息
        const std::vector<S_PUMA_IMAGE_DATA_INFO>* mpPumaImageDataInfo;
        const std::vector<S_PUMA_IMAGE_DATA_INFO>* mpPumaValidateImageDataInfo;
    }S_PUMA_DOWNLOAD_INFO,*PS_PUMA_DOWNLOAD_INFO;

    enum LOG_TYPES
    {
        LOG_ERROR = 1,
        LOG_ALWAYS,
        LOG_WARN,
        LOG_INFO,
        LOG_DEBUG,  // usually only goes to log, unless VerboseLevel==LOG_DEBUG
        LOG_ONLY  // always only goes to log, such as hex dump
    };
    extern void MyLog (int log_level,int nIndex, const char *func_name, int line_number, const char *format, ...);
}
