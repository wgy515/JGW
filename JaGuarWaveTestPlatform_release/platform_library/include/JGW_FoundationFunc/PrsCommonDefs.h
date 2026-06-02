#pragma once
typedef unsigned int uint32_t;
/// @brief Signature for recognizing UMAC from version response
#define UMAC_VERSION_TEST_STRING  "Falcon UMAC"
///
#define RSP_RECV_MAX_BUF_SIZE 1024 * 2
///
#define RESP_TERMINATOR '>'

/// @brief Signature for recognizing Bootloader from version response
#define BL_VERSION_TEST_STRING  "Bootloader"

#define MAC_ADDRESS_LENGTH 12

#define SERIALNUMBER_MAX_BITS 128

#define MASK_32BIT 0xFFFFFFFF

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

#define PRS_FW_IMAGE_OFFSET                             (0)
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

namespace JGW
{
    typedef struct _PrsFlashAddress
    {
        uint32_t    mnbaudRate;
        uint32_t    mnFlashSizeKb;
        uint32_t    flashMemoryBase;
        uint32_t    flashOffsetMask;
        uint32_t    flashImageOffset;
        uint32_t    flashConfigHeaderOffset;
        uint32_t    flashConfigOffset;
        uint32_t    flashOtpHeaderOffset;
        uint32_t    flashOtpOffset;
    } PrsFlashAddress;

    //typedef struct _PrsFlashFlieInfo
    //{
    //    uint32_t    flashImageOffset;

    //}PrsFlashFlieInfo;

    typedef struct 
    {
        //! flash offeset
        size_t mnFlashOffset; 
        //! 文件大小
        size_t mnFileSize;
        //! 文件内容缓冲区
        char* mszFileBuf;
    }S_PRS_FILE_INFO,*PS_PRS_FILE_INFO;

    enum E_PRS_PDB_FILE_TYPE
    {
        E_PRS_FW_IMAGE = 0,
        E_PRS_CONFIG,
        E_PRS_OTP,
        E_PRS_MAX_DONGLES_PDB_FILE
    };

    typedef struct _tagSPrsFlashCmdParms
    {
        //bool mbFlashOTP;
        //bool mbFlashConfig;
        //bool mbFlashImage;
        std::string mstrImagesFolder;
        std::string mstrImageBinName;
        std::string mstrPDBFolder;

        std::string mstrFirewareFilePath;

        std::string mstrOtpHeaderFilePath;
        std::string mstrOtpBobyFilePath;

        std::string mstrConfigHeaderFilePath;
        std::string mstrConfigBobyFilePath;
        S_PRS_FILE_INFO msPrsPDBFileInfo[E_PRS_MAX_DONGLES_PDB_FILE];
        //String              imagesFolder;
        //String              imageBinName;

        //// For specific image names (no folder,
        //// no reliance on default image names)
        //String              fwFlashFile;
        //String              otpFlashFile;
        //String              configFlashFile;

        //String              baudRate;
        //String              serialDevice;
        //uint8_t             bFlashOTP;
        //uint8_t             bFlashConfig;
        //uint8_t             bFlashImage;
        //uint8_t             bPrintMemory;
        //uint8_t             bErase;
        //uint32_t            address;
        //uint32_t            len;
        //uint32_t            flashSizeKb;
        //uint8_t             bReset;
        //uint32_t            bVersion;
    } SPrsFlashCmdParms;

    enum eDeviceBootMode
    {
        eDeviceBootMode_Unknown = -1,        // undetermined
        eDeviceBootMode_BootLoader = 0,     // device in bootloader mode
        eDeviceBootMode_UMAC = 1,           // device in UMAC mode
    };

    const char scTblHex2Ascii[] = "0123456789ABCDEF";
    const char spiEnable[]        = "se 0\r";
    const char spiSetBaud[]       = "sb 4\r";

    // Flash command compatible to Micron M25P16 or equivalent
    const char scSpiWriteCmd[]    = "sw b 0 ";
    const char scSpiReadCmd[]     = "sr b 0 ";
    const char flashErase[]     = "sw b 0 1 C7\r";
    const char flashIdentity[]    = "sr b 0 1 9f 10\r";
    const char flashEnable[]     = "sw b 0 1 6\r";
    const char flashReadStatus[]  = "sr b 0 1 05 1\r";
}