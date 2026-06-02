#pragma once
#include <intsafe.h>
#define FALCON_FLASH_MEM_BASE 0xF8000000
#define FALCON_FLASH_MEM_BASE_STRING "F8000000"
#define MAX_SERIAL_NUMBER_FALSH_BIT 128
#define MAXCLIWRITE 224
#define MAX_WRITE_LEN 504
#define MAXCLIWRITEHEX 0xE0
#define SECTOR_SIZE 0x10000
//! SerialNumber
#define SERIALNUMBER_POS_TO_BIN 0x0d

#define MAC_ADDRESS_POS_TO_BIN 0x05

#define MAC_ADDRESS_MAX_LEN 12

#define SERIALNUMBER_MAX_LEN 14

#define MASK_32BIT 0xFFFFFFFF

#define PERSISTENT_DATABSE_1_CONFIG_HEADER "persistent_database_1_config_header.bin"
#define PERSISTENT_DATABSE_1_CONFIG "persistent_database_1_config.bin"
#define PERSISTENT_DATABSE_0_OTP_HEADER "persistent_database_0_otp_header.bin"
#define PERSISTENT_DATABSE_0_OTP "persistent_database_0_otp.bin"
#define FW_IMAGE_BIN "image.bin"

namespace JGW
{
    /*
    persistent_database_1_config_header.bin 位于FLASH最后两个sector
    persistent_database_1_config.bin  位于FLASH最后两个sector + persistent_database_1_config_header.bin的偏移32字节
    persistent_database_0_otp_header.bin 位于FALSH最后一个sector
    persistent_database_0_otp.bin 位于FLASH最后一个sector + persistent_database_1_config_header.bin的偏移32字节
    image.bin起始 Addr = 0
    */

    typedef struct 
    {
        size_t mnFileSize;
        unsigned char* mszFileBuf;
    }S_FILE_INFO,*PS_FILE_INFO;

    enum E_DONGLES_PDB_FILE_TYPE
    {
        E_FW_IMAGE = 0,
        E_CONFIG,
        E_CONFIG_HEADER,
        E_OTP,
        E_OTP_HEADER,
        E_MAX_DONGLES_PDB_FILE
    };

    const bool gvEraseFlash[E_MAX_DONGLES_PDB_FILE] = {false,true,false,true,false};

    const char gvszDonglesPDBFileNames[][40] =
    {
        FW_IMAGE_BIN,
        PERSISTENT_DATABSE_1_CONFIG,
        PERSISTENT_DATABSE_1_CONFIG_HEADER,
        PERSISTENT_DATABSE_0_OTP,
        PERSISTENT_DATABSE_0_OTP_HEADER,
        ""
    };

    struct S_DONGLES_PDB_FILES_INFO 
    {
        UINT32 muDonglesFlashPDBAddr[E_MAX_DONGLES_PDB_FILE];
        S_FILE_INFO msDonglesPDBFileInfo[E_MAX_DONGLES_PDB_FILE];
    };
}
