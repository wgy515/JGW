/****************************************************************************

Copyright 2017 Peraso Technologies Inc. ALL RIGHTS RESERVED.

This Information is proprietary to Peraso Technologies Inc., Toronto, Ontario
(Peraso) and MAY NOT be copied by any method or incorporated into another
program without the express written consent of Peraso. This Information or any
portion thereof remains the property of Peraso. The Information contained herein
is believed to be accurate and Peraso assumes no responsibility or liability for
its use in any way and conveys no license or title under any patent or copyright
and makes no representation or warranty that this Information is free from
patent or copyright infringement.


$Id: //sw_peraso/trunk/utility/prs_download_flash/common/include/common_defs.h#2 $

****************************************************************************/

#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H 1

// Include <name.h> instead of <cname> since that
// requires ISO C++ 2011 which is experimental for G++ support
// on target Linux compile
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

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

typedef enum
{
    ePrsSectionType_FWImage = 0,
    ePrsSectionType_Config,
    ePrsSectionType_OTP,
    ePrsSectionType_Max,

} EPrsSectionType;

typedef std::string String;

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

void Usage();
uint32_t ParseCommandLine(
    int32_t             argc,
    char*               argv[],
    SPrsFlashCmdParms*  parms);
void PrintResult(uint32_t result);
int32_t GetFileSize(const char* a_pFileName);
int32_t GetFileData(
    const char      *a_pFileName,
    unsigned char   *a_pBuffer,
    uint32_t        a_bufferLen);
void ShowVersion(void);
void InitOpts(SPrsFlashCmdParms* a_pOpts);

#endif /* COMMON_DEFS_H */
