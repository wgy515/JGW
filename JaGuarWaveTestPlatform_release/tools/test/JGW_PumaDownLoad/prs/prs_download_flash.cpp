/*
 * Copyright (c) 2014 Peraso Technologies Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * File contents: Serial CLI test under Linux, include register read, status command, fli command and flash programming.
 */

#include "stdafx.h"
#include <stdio.h>
#include <sys/stat.h>
/*#include <stdbool.h>*/

#include "serial.h"
#include "prs_version.h"
#include "../common/include/common_defs.h"
#include <Windows.h>

#define MAX_BUF_SIZE 1024

/// @brief Signature for recognizing UMAC from version response
#define UMAC_VERSION_TEST_STRING  "Falcon UMAC"

/// @brief Signature for recognizing Bootloader from version response
#define BL_VERSION_TEST_STRING  "Bootloader"

bool WriteVerify = false;
#ifdef _MSC_VER
const char *default_device = "\\\\.\\COM33";
const char *second_device  = "\\\\.\\COM28";
#else
const char *default_device = "/dev/ttyACM0";
const char *second_device  = "/dev/ttyACM1";
#endif

char resp[MAX_BUF_SIZE];	//this is the receiver buffer for public usage.

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

enum eDeviceBootMode
{
    eDeviceBootMode_Unknown,        // undetermined
    eDeviceBootMode_BootLoader,     // device in bootloader mode
    eDeviceBootMode_UMAC,           // device in UMAC mode
};

enum MemoryValues {
    // byte count limit for memory read and write bytes
    MemoryReadWriteBytesLimit = 128,
    // maximum number of bytes for a CLI command
    CLIBufferSize = 512
};



bool program_flash(SPrsFlashCmdParms* program_option, const char* dir, int deviceMode);
int FlashReadData(char *a_pData, unsigned int a_address, unsigned int a_length);
int unformatReceiveBuffer(char *a_pBuf, char *a_pData, int a_length);
int FlashEraseData(unsigned int a_address, unsigned int a_length);
bool FlashWriteImagefile(const char* a_file);
bool FlashWriteConfigFiles(const char* a_headerFile, const char* a_configFile);
bool FlashWriteOTPFiles(const char* a_headerFile, const char* a_otpFile);
bool FlashWriteFiles(const char* a_headerFile, const char* a_file, long a_group);
bool FlashWriteStream(FILE* a_stream, long a_count, long a_offset);
long GetFlashAddress(long a_offset);
bool FlashEraseAll(void);
bool FlashEraseBlock(long a_offset, long a_size);
bool FlashEraseSector(long a_offset, long a_size);
bool FlashErase(char a_area, long a_offset, long a_size);
static int32_t FlashDump(uint32_t a_address, uint32_t a_length);
static void PrintMemory(unsigned char *a_buf, uint32_t a_address, uint32_t a_length);
bool MemoryWriteByte(long a_address, char a_value, char a_mac);
bool MemoryWriteBytes(long a_address, const unsigned char* a_pBytes, long a_count);
bool MemoryWriteBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count);
bool MemoryWriteVerifyBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count);
bool MemoryReadBytes(long a_address, unsigned char* a_pBytes, long a_count);
bool MemoryReadBytesLimit(long a_address, unsigned char* a_pBytes, long a_count);
bool MemoryVerifyBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count);
long BytesToHexString(const unsigned char* a_pBytes, long a_count, char* a_string);

int DeviceGetMode(char *a_pVersion);
int DownloadFlashWriteImage(const char *a_pFileName);
int DownloadFlashWritePDB(const char *a_pHeaderFile, const char *a_pDataFile, int a_group);
int AutoselectDevice(String& a_selectedDevice);
int CheckDevice(String& a_selectedDevice);

int main(int argc, char *argv[])
{
    char resetcmd[]   = "reset\r";

    int                     actual_resp_size = 0;
    int                     status = 0;
    int32_t                 comStatus = 0;
    uint32_t                result = eDownloadResult_success;
    int                     deviceMode;
    SPrsFlashCmdParms       parms;
    char*                   cmd = NULL;

    

    result = ParseCommandLine(argc, argv, &parms);
    if (!RESULT_OK(result))
    {
        // Remove warnings from exit code
        result &= ~(((1 << eDownloadResult_maxWarning) << 1) - 1);
        printf("\nERROR: Invalid input. Check command line arguments\n\n");
        Usage();
        return result;
    }

    if (parms.bVersion)
    {

        ShowVersion();
    }
/*    parms.serialDevice = "\\\\.\\COM28";*/
    init_ser();
    if (parms.serialDevice != "") 
    {
        comStatus = CheckDevice(parms.serialDevice);
    }
    else
    {
        comStatus = AutoselectDevice(parms.serialDevice);
    }

    if (-1 == comStatus)
    {
        printf("Failed to open device.\n");
        result = (1 << eDownloadResult_openBootromPortFailure);
        return result;
    }

    printf("Device found: %s\n", parms.serialDevice.c_str());

    switch (deviceMode = DeviceGetMode(resp))
    {
        case eDeviceBootMode_BootLoader:
            printf("Device Mode In Bootloader\n");
            break;
            
        case eDeviceBootMode_UMAC:
            printf("Device Mode In UMAC\n");
            break;
                
        case eDeviceBootMode_Unknown:
        default:
            printf("Device Mode Unknown\n");
            break;
    }

    if (parms.bPrintMemory)
    {
        if (deviceMode == eDeviceBootMode_BootLoader)
        {
            status = FlashDump(parms.address, parms.len);
        }
        else
        {
            printf("Print memory supported with Bootrom connection\n");
        }
    } 
    else if (parms.bErase)
    {
        if (deviceMode == eDeviceBootMode_BootLoader)
        {
            status = FlashEraseData(parms.address, parms.len);
        }
        else
        {
            printf("Erase memory supported with Bootrom connection\n");
        }
    }
    else
    {
        printf("Using flash files from \"%s\"\n", parms.imagesFolder.c_str());
        printf("Start to erase and programming flash.....\n");

        status = program_flash(&parms, parms.imagesFolder.c_str(), deviceMode);
        if(status)
        {
            printf("Programming flash failed!\n");
        }
        else
        {
            printf("Programming flash successful!\n");
            if (parms.bFlashImage && parms.bReset)
            {
                cmd=resetcmd;
                status = send_ser(cmd, strlen(cmd), resp, MAX_BUF_SIZE, &actual_resp_size);
                printf("Sent Reset to Device, Flash Process Completed\n\n");
            }
        }
    }
    close_ser();
    return result;
}

// Deteremine the device mode to use the correct commands to program the flash content
// - Bootloader mode, or UMAC mode
int DeviceGetMode(char *a_pVersion)
{
    int i;
    char *p = a_pVersion;

    // determine the device mode based on the version string reponse
    for (i=0; i < 5; i++, p++)
    {
        if (*p == 'F')  // Falcon UMAC?
        {
            if (memcmp(p, "Falcon UMAC", strlen("Falcon UMAC")) == 0)
            {
                return eDeviceBootMode_UMAC;
            }
        }
        else if (*p == 'P') // PRS4x01 Bootloader?
        {
            if (memcmp(p+8, "Bootloader", strlen("Bootloader")) == 0)
            {
                return eDeviceBootMode_BootLoader;
            }
        }
    }
    return eDeviceBootMode_Unknown;
}

static int32_t FlashDump(
    uint32_t    a_address,
    uint32_t    a_length    )
{
    int32_t             status = 0;
    uint32_t            length = 0;
    unsigned char       *pFmtBuffer;
    int32_t             nbrOfLine = (FLASH_READ_SIZE/16);
    int32_t             rcvLength = (nbrOfLine * 2) + ((FLASH_READ_SIZE * 3) - nbrOfLine) + 3;

    // get a buffer big enough to hold 'sw b 0 count opcode address (3*a_length)'
    if (NULL == (pFmtBuffer = (unsigned char*)malloc(rcvLength + 1)))
    {
        return 1;
    }

    // There is some issue with first read. Without this artificial delay the first
    // read contains errant (though consistent) bytes in the return.
    Sleep(1);
    while (a_length != 0 && status == 0)
    {
        memset(pFmtBuffer, 0, rcvLength + 1);
        length = (a_length > FLASH_READ_SIZE) ? FLASH_READ_SIZE : a_length;
        status = FlashReadData((char*)pFmtBuffer, a_address, length);

        unformatReceiveBuffer((char*)pFmtBuffer, (char*)pFmtBuffer, length);
        PrintMemory(pFmtBuffer, a_address, length);
        a_length -= length;
        a_address += length;
    }

    free(pFmtBuffer);
    return (status);
}

static void PrintMemory(
    unsigned char   *a_buf,
    uint32_t        a_address,
    uint32_t        a_length    )
{
    uint32_t lineCount = 0;
    char str[HEX_DISPLAY_LINE_LEN + 1];
    uint32_t i = 0;

    while (i < a_length)
    {
        printf("[%08X] ", a_address);
        lineCount = 0;
        str[0] = 0;
        while (i < a_length && lineCount < HEX_DISPLAY_LINE_LEN)
        {
            printf("%02X ", a_buf[i]);
            str[lineCount] = (a_buf[i] > 0x1F && a_buf[i] < 0x7F) ? a_buf[i] : '.';
            i++;
            a_address++;
            lineCount++;
        }
        while (lineCount < HEX_DISPLAY_LINE_LEN)
        {
            printf("   ");
            lineCount++;
        }
        str[lineCount] = 0;
        printf("    %s\n", str);
    }
}

bool program_flash(
    SPrsFlashCmdParms*  options,
    const char*         dir,
    int                 deviceMode)
{
    bool result;

	String config_hdr;
	String config_bd;
	String otp_hdr;
	String otp_bd;
	String image_path;

	// Actual database names are fixed, same as in Windows implementation.
	config_hdr += dir;
	config_hdr += "/";
	config_hdr += PRS_CONFIG_HEADER_FILE_NAME;

	config_bd += dir;
	config_bd += "/";
	config_bd += PRS_CONFIG_BODY_FILE_NAME;

	otp_hdr += dir;
	otp_hdr += "/";
	otp_hdr += PRS_OTP_HEADER_FILE_NAME;

	otp_bd += dir;
	otp_bd += "/";
	otp_bd += PRS_OTP_BODY_FILE_NAME;

	image_path += dir;
	image_path += "/";
	image_path += options->imageBinName;

    const char *PdbMap[5] = {
		config_bd.c_str(),
		config_hdr.c_str(),
		otp_bd.c_str(),
		otp_hdr.c_str(),
		image_path.c_str()
    };

    if (options->bFlashImage)
    {
        if (deviceMode == eDeviceBootMode_BootLoader)
        {
            result = DownloadFlashWriteImage(PdbMap[4]);
        }
        else
        {
            result = FlashWriteImagefile(PdbMap[4]);//image file
        }
        if(result == 1)
        {

    		printf("Programming %s failed.\n",PdbMap[4]);
    		return 1;
    	}
    	else
    	{
    		printf("Programming %s successfully.\n",PdbMap[4]);
        }
    }
    
	if (options->bFlashConfig)
	{
        if (deviceMode == eDeviceBootMode_BootLoader)
        {
            result = DownloadFlashWritePDB(PdbMap[1], PdbMap[0], 0);
        }
        else
        {
    		result = FlashWriteConfigFiles(PdbMap[1], PdbMap[0]);//config_header, config
        }
		if(result == 1)
		{	
			printf("Programming %s and %s failed.\n",PdbMap[1],PdbMap[0]);
			return 1;
		}
		else
		{
			printf("Programming %s and %s successfully.\n",PdbMap[1],PdbMap[0]);
        }
    }

    if (options->bFlashOTP)
    {
        if (deviceMode == eDeviceBootMode_BootLoader)
        {
            result = DownloadFlashWritePDB(PdbMap[3], PdbMap[2], 1);
        }
        else
        {
		    result = FlashWriteOTPFiles(PdbMap[3], PdbMap[2]);//opt_header,opt
        }
		if(result == 1)
		{	
			printf("Programming %s and %s failed.\n",PdbMap[3],PdbMap[2]);
			return 1;
		}
		else
		{
		    printf("Programming %s and %s successfully.\n",PdbMap[3],PdbMap[2]);
		}
    }
	return result;
}

bool FlashWriteImagefile(const char* a_file)
{
    FILE *file;
    long filelength = 0;
    bool result;

    file = fopen(a_file, "rb");
    if(file != NULL) {
        // no built in method to get the length of a file?
        fseek(file, 0L, SEEK_END);
        filelength = ftell(file);
        fseek(file, 0L, SEEK_SET);
        if(filelength > 0) {
            result = FlashEraseBlock(GetFlashAddress(FlashImageOffset), filelength);
            if(result == 1) {
                fclose(file);
                return 1;
            }
            result = FlashWriteStream(file, filelength, FlashImageOffset);
            if(result == 1) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
	else	
	{
		return 1;
	}
    return 0;
}

bool FlashWriteConfigFiles(const char* a_headerFile, const char* a_configFile)
{
    bool result;
    result = FlashWriteFiles(a_headerFile, a_configFile, 0);
    return result;
}

bool FlashWriteOTPFiles(const char* a_headerFile, const char* a_otpFile)
{
    bool result;
    result = FlashWriteFiles(a_headerFile, a_otpFile, 1);
    return result;
}

  
bool FlashWriteFiles(const char* a_headerFile, const char* a_file, long a_group)
{
    bool result;
    long headerOffset;
    long offset;
    FILE* headerFile;
    FILE* file;
    long headerLength;
    long length;

    if(a_group == 1) {
		headerOffset=FlashOtpHeaderOffset;
		offset= FlashOtpOffset;
    } else {
		headerOffset=FlashConfigHeaderOffset;
		offset= FlashConfigOffset;
    }

    file = fopen(a_file,"rb");
    headerFile = fopen(a_headerFile, "rb");

    if((file!=NULL) && (headerFile!=NULL)) {
        fseek(file,0L, SEEK_END);
        length = ftell(file);
        fseek(file, 0L, SEEK_SET);

        fseek(headerFile,0L, SEEK_END);
        headerLength = ftell(headerFile);
        fseek(headerFile, 0L, SEEK_SET);	
		
		
        // make sure the header appears valid and the other file isn't NULL
        if(headerLength > 0 && headerLength <= 0x20 && length > 0) {
            // erase the block (the header is in the same block as the main file)
            result = FlashEraseBlock(offset, (long)length);
            if(result == 1) {
                fclose(file);
                fclose(headerFile);
                return 1;
            }
            // write header and main files
            result = FlashWriteStream(headerFile, (long)headerLength, headerOffset);
            if(result == 1) {
                fclose(file);
                fclose(headerFile);
                return 1;
            }
            result = FlashWriteStream(file, (long)length, offset);
            if(result == 1) {
                fclose(file);
                fclose(headerFile);
                return 1;
            }
        }
    }
else
{
return 1;
}
    fclose(file);
    fclose(headerFile);
    return 0;
}

bool FlashWriteStream(FILE* a_stream, long a_count, long a_offset)
{
    bool result = 0;
    int readbytes;
    long writeCount;

    unsigned char buffer[MemoryReadWriteBytesLimit];
    while(a_count > 0) {
        if(a_count > (long)MemoryReadWriteBytesLimit)
            writeCount= (long)MemoryReadWriteBytesLimit;
		else
			writeCount= a_count;
        readbytes=fread(buffer, writeCount,1,a_stream);
        // if the stream read fails then don't write any more
        if(readbytes<=0) {
            result=1;
            break;
        }
        result = MemoryWriteVerifyBytesLimit(GetFlashAddress(a_offset), buffer, writeCount);
        if(result == 1)
                break;
        a_count -= writeCount;
        a_offset += writeCount;
    }
    return result;
}


long GetFlashAddress(long a_offset)
{
    return (long)FlashMemoryBase | (a_offset & (long)FlashOffsetMask);
}

bool FlashEraseAll(void)
{
    bool status;
    status = FlashErase('a', 0, 0);
    return status;
}

bool FlashEraseBlock(long a_offset, long a_size)
{
    bool status;
    status = FlashErase('b', a_offset, a_size);
    return status;
}

bool FlashEraseSector(long a_offset, long a_size)
{
    bool status;
    status = FlashErase('s', a_offset, a_size);
    return status;
}

bool FlashErase(char a_area, long a_offset, long a_size)
{
    bool status;
    int actual_resp_size;
    char cmd[CLIBufferSize];
    if(a_area != 'a')
        sprintf(cmd, "erase %c %lx %lx\n", a_area, GetFlashAddress(a_offset), a_size);
    else
        sprintf(cmd, "erase a %lx\n", GetFlashAddress(a_offset));
		
    // send and receive
    status = send_ser(cmd, strlen(cmd), resp, MAX_BUF_SIZE, &actual_resp_size);
    return status;
}

bool MemoryWriteByte(long a_address, char a_value, char a_mac)
{
    char cmd[CLIBufferSize];
	bool status;
	int actual_resp_size;	
	// if a valid mac is specified then use it
	if(a_mac == 'l' || a_mac == 'u')
        sprintf(cmd,"%c mw b %lx 1 %x\n", a_mac, a_address, a_value);
    else
        sprintf(cmd,"mw b %lx 1 %x\n", a_address, a_value);
    status = send_ser(cmd, strlen(cmd),resp,MAX_BUF_SIZE,&actual_resp_size);
	return status;
}

bool MemoryWriteBytes(long a_address, const unsigned char* a_pBytes, long a_count)
{
    bool status = 1;
    while(a_count > 0) {
        // limit the write count to MemoryReadWriteBytesLimit
        long writeCount = a_count > (long)MemoryReadWriteBytesLimit ? (long)MemoryReadWriteBytesLimit : a_count;
        status = MemoryWriteBytesLimit(a_address, a_pBytes, writeCount);
        a_count -= writeCount;
        a_pBytes += writeCount;
        a_address += writeCount;
    }
    return status;
}

bool MemoryWriteBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count)
{
    char cmd[CLIBufferSize];
    bool status = 1;
    int actual_resp_size;
    int len;
    if(a_count == 0 || a_count > MemoryReadWriteBytesLimit)
        return false;
    len = sprintf(cmd,"mw b %lx %lx ", a_address, a_count);
    if(len > 0) {
        //cmd << "mw b " << std::hex << a_address << ' ' << std::hex << (long)a_count << ' ';
        len += BytesToHexString(a_pBytes, a_count, cmd + len);
        len += sprintf(cmd + len,"\n");
        // that writing to flash is expected to take less than the serial port timeout
        // which at the time of this writing is 400ms
        status = send_ser(cmd, len, resp, MAX_BUF_SIZE, &actual_resp_size);
    }
    return status;
}

bool MemoryWriteVerifyBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count)
{
    bool success = MemoryWriteBytesLimit(a_address, a_pBytes, a_count);
    if(success && WriteVerify)
        success = MemoryVerifyBytesLimit(a_address, a_pBytes, a_count);
    return success;
}

bool MemoryReadBytes(long a_address, unsigned char* a_pBytes, long a_count)
{
    bool status = 1;
    long count;
    while(a_count > 0) {
        // limit the write count to MemoryReadWriteBytesLimit
        if(a_count > (long)MemoryReadWriteBytesLimit)
            count= (long)MemoryReadWriteBytesLimit;
        else
            count= a_count;
        status = MemoryReadBytesLimit(a_address, a_pBytes, count);
        a_count -= count;
        a_pBytes += count;
        a_address += count;
    }
    return status;
}

bool MemoryReadBytesLimit(long a_address, unsigned char* a_pBytes, long a_count)
{
    int actual_resp_size;
    bool status;

    if(a_count == 0 || a_count > MemoryReadWriteBytesLimit)
        return false;
    char cmd[CLIBufferSize];
    sprintf(cmd,"mr b %lx %lx\n", a_address, a_count);
    //cmd << "mr b " << std::hex << a_address << ' ' << std::hex << (long)a_count << '\r';
    //std::string response = SendCommandReadResponse(cmd.str());
    status = send_ser(cmd, strlen(cmd), resp, MAX_BUF_SIZE, &actual_resp_size);
    if((actual_resp_size == a_count)&&(status==0))
        return 0;
    else
        return 1;
}

bool MemoryVerifyBytesLimit(long a_address, const unsigned char* a_pBytes, long a_count)
{
    long i;
    if(a_count == 0 || a_count > MemoryReadWriteBytesLimit)
        return false;
    unsigned char verify[MemoryReadWriteBytesLimit];
    if(MemoryReadBytesLimit(a_address, verify, a_count)) {
        for(i = 0; i < a_count; i++) {
            if(a_pBytes[i] != verify[i]) {
                // maybe the mismatch address should be returned?
                return false;
            }
        }
        return true;
    }
    return false;
}

long BytesToHexString(const unsigned char* a_pBytes, long a_count, char* a_string)
{
    long i, len;
    long totalLength = 0;
    if(a_pBytes && a_count > 0 && a_string) {
        for(i = 0; i < a_count; i++, a_pBytes++) {
            if(i > 0) {
                *a_string++ = ' ';
                totalLength++;
            }
            len = sprintf(a_string, "%x", (unsigned int)*a_pBytes);
            a_string += len;
            totalLength += len;
        }
    }
    return totalLength;
}

char scTblHex2Ascii[] = "0123456789ABCDEF";
char spiEnable[]        = "se 0\r";
char spiSetBaud[]       = "sb 4\r";

// Flash Specific parameters
#define FLASH_PAGE_SIZE         (256)                   // in # of byte
#define FLASH_PAGE_PER_SECTOR   (256)                   // in # of page   
#define FLASH_SECTOR_SIZE       (FLASH_PAGE_PER_SECTOR * FLASH_PAGE_SIZE)   
#define OPCODE_PAGE_PROGRAM     '2'
#define OPCODE_READ_DATA        '3'

// Flash command compatible to Micron M25P16 or equivalent
char scSpiWriteCmd[]    = "sw b 0 ";
char scSpiReadCmd[]     = "sr b 0 ";
char flashErase[]       = "sw b 0 1 C7\r";
char flashIdentity[]    = "sr b 0 1 9f 10\r";
char flashEnable[]      = "sw b 0 1 6\r";
char flashReadStatus[]  = "sr b 0 1 05 1\r";

int formatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length)
{
    int i, index;
    memcpy(a_pBuf, scSpiWriteCmd, sizeof(scSpiWriteCmd));    // spi command for page program (2)
    index = sizeof(scSpiWriteCmd) - 1;
    
    // calculate and format the count of SPI data
    i = a_length + 4;   // opcode(1) + address(3) + a_length
    a_pBuf[index++] = scTblHex2Ascii[0x0f & (i >> 8)];
    a_pBuf[index++] = scTblHex2Ascii[0x0f & (i >> 4)];
    a_pBuf[index++] = scTblHex2Ascii[0x0f & i];
    a_pBuf[index++] = 0x20;
    a_pBuf[index++] = OPCODE_PAGE_PROGRAM;
    a_pBuf[index++] = 0x20;

    for (i=3; i>0; i--)
    {
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
        a_pBuf[index++] = 0x20;
    }

    for (i=0; i<a_length; i++)
    {
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData >> 4)];
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData++)];
        a_pBuf[index++] = 0x20;
    }
    
    a_pBuf[index++] = '\r';
    a_pBuf[index++] = '\0';
    return index;
}

int unformatReceiveBuffer(char *a_pBuf, char *a_pData, int a_length)
{
    int i = 0;
    unsigned char data, byteHi, byteLo;
    unsigned char * pData;

    // The loops repacks ASCII data string from Falcon-B bootloader into hex string
    // The data returns from Falcon-B bootloader has a specific pattern:
    // [\n\r,[byte1,byte2,space]x15,[byte1,byte2]]x N_lines, ends with \n\r>
    while (a_length)
    {
        int length = a_length > 16 ? 16 : a_length;
        pData = (unsigned char *)(a_pData + 2);
        a_pData += (2 + (length * 3)-1);     // move buffer head to next line
        a_length -= length;

        // pack 2 ASCII data to 1 hex value
        for (i = 1; i <= length; i++)
        {
            data = *pData++;
            byteHi = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);
            data = *pData++;
            byteLo = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);        
            *a_pBuf++ = (0xf0 & (byteHi << 4)) | (0x0f & (byteLo));
            pData++;  // skip space
        }
    }
    return i;
}

int FlashSendRecevie (char *a_pData, char *a_pResp, unsigned int a_length)
{
    bool status = 1;
    int  actual_resp_size;
    int  cmdLen = strlen(a_pData);
    
    if (a_pResp)
    {
        *a_pResp = 0;
    }

    ///printf("INFO: Send %d bytes, (%s)\n", cmdLen, a_pData);
    status = send_ser(a_pData, cmdLen, a_pResp, MAX_BUF_SIZE, &actual_resp_size);

    a_pResp[actual_resp_size] = '\0';
    ///printf("INFO: Receive %d bytes, (%s) \n", actual_resp_size, a_pResp);

    return status;
}

int FlashEraseData(unsigned int a_address, unsigned int a_length)
{
    char pResp[16], pCmd[32];
    int status = 0;

    if (a_length > 0x0fffff)
    {
        // do a bulk erase
        FlashSendRecevie(flashErase, pResp, sizeof(pResp));
    }
    else
    {
        unsigned int nbrOfSectors = (a_length/FLASH_SECTOR_SIZE) + ((a_length % FLASH_SECTOR_SIZE) ? 1 : 0);
        unsigned int i, index, offset;
        a_address &= 0xff0000;          // align to sector boundary
        index = sizeof(scSpiWriteCmd) - 1;
        memcpy(pCmd, scSpiWriteCmd, index);
        pCmd[index++] = '4';
        pCmd[index++] = 0x20;
        pCmd[index++] = 'D';
        pCmd[index++] = '8';
        pCmd[index++] = 0x20;

        for ( ; nbrOfSectors; nbrOfSectors--)
        {
            offset = index;
            for (i=3; i>0; i--)
            {
                pCmd[offset++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
                pCmd[offset++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
                pCmd[offset++] = 0x20;
            }
            pCmd[offset++] = '\r';
            pCmd[offset++] = 0;

            status = FlashSendRecevie(pCmd, pResp, sizeof(pResp));
            if (status)
            {
                 break;
            }
            a_address += FLASH_SECTOR_SIZE;
        }
    }
    return status;   
}

int FlashWriteData(char *a_pData, unsigned int a_address, unsigned int a_length)
{
    int status = 0;
    char pResp[16];
    char *pFmtBuffer;

    // get a buffer big enough to hold 'sw b 0 count opcode address (3*a_length)'
    if (NULL == (pFmtBuffer = (char *)malloc(32 + (a_length * 3))))
    {
        return 0;
    }

    FlashSendRecevie(flashEnable, pResp, 3);
    formatSendBuffer(pFmtBuffer, a_address, a_pData, a_length);
    status = FlashSendRecevie(pFmtBuffer, pResp, 16);
    free(pFmtBuffer);

    return ((status == 0) ? 1 : 0);
}

int FlashReadData(char *a_pData, unsigned int a_address, unsigned int a_length)
{
    int i, index;
    char cmd[32];
    int nbrOfLine = (a_length/16) + ((a_length % 16) ? 1 : 0);
    int rcvLength = (nbrOfLine * 2) + ((a_length * 3) - nbrOfLine) + 3;

    memcpy(cmd, scSpiReadCmd, sizeof(scSpiReadCmd));    // spi command for read 
    index = sizeof(scSpiReadCmd) - 1;
    cmd[index++] = '4';     // count of read command byte to follow
    cmd[index++] = ' ';
    cmd[index++] = OPCODE_READ_DATA;
    cmd[index++] = ' ';

    // calculate and format the count of SPI data
    for (i=3; i > 0; i--)
    {
        cmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
        cmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
        cmd[index++] = 0x20;
    }

    cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 12)]; 
    cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 8)]; 
    cmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 4)]; 
    cmd[index++] = scTblHex2Ascii[0x0f & (a_length)]; 
    cmd[index++] = '\r';
    cmd[index++] = 0;
            
    return FlashSendRecevie(cmd, a_pData, rcvLength);
}

int FlashDataValidate(char *a_pData, unsigned int a_address, unsigned int a_length)
{
    int status = 1, i, length;
    unsigned int mask = 0xffffffff, offset = 0;
    //char *pFmtBuffer;
    unsigned char blankChar = 0xff;
    int nbrOfLine = (FLASH_READ_SIZE/16);
    int rcvLength = (nbrOfLine * 2) + ((FLASH_READ_SIZE * 3) - nbrOfLine) + 3;

    char pFmtBuffer[1024] = {0};
    // get a buffer big enough to hold 'sw b 0 count opcode address (3*read_length)'
//     if (NULL == (pFmtBuffer = (char *)malloc(rcvLength + 1)))
//     {
//         return 0;
//     }

    // if no source data, assume blank check
    if (a_pData == NULL)
    {
        a_pData = (char *)&blankChar;
        mask = 0x0;
    }

    while (a_length && status)
    {
        length = (a_length > FLASH_READ_SIZE) ? FLASH_READ_SIZE : a_length;
        FlashReadData(pFmtBuffer, a_address, length);
        unformatReceiveBuffer(pFmtBuffer, pFmtBuffer, length);
        for (i = 0; i < length; i++)
        {
            if (pFmtBuffer[i] != a_pData[offset & mask])
            { 
                ///printf("DEBUG: Fail at i = (%d)\n", i);
                status = 0;
                break;
            }
            offset++;
        }
        a_length -= length;
        a_address += length;
    }
    
   // free(pFmtBuffer);
    return (status);
}

int DownloadFlashWriteImage(const char *a_pFileName)
{
    unsigned int bufferLen, fileSize, address;
    char *pBuffer, *pData;
    unsigned int dataLen;
    char pResp[128];
    int status = 1;
    FILE *file;

    clock_t t_start = clock();

    if (NULL == (file = fopen(a_pFileName, "rb")))
    {
        printf("ERROR: Unable to find file %s\n", a_pFileName);
        return 0;
    }
    // determine the size
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if (!(pBuffer = (char *)malloc(fileSize+1)))
    {
        printf("Error: not able to malloc(%d)", fileSize+1);
        fclose(file);
        return 0;
    }
    bufferLen = (int)fread(pBuffer, sizeof(unsigned char), fileSize, file);
    fclose(file);
    
    if (bufferLen != fileSize)
    {
        printf("Error: data read from file (%d) != file size (%d)\n", bufferLen, fileSize);
        free(pBuffer);
        return 0;
    }
	FlashSendRecevie(spiEnable, pResp, (unsigned int)strlen(spiEnable));
	FlashSendRecevie(spiSetBaud, pResp, (unsigned int)strlen(spiSetBaud));
    FlashSendRecevie(flashIdentity, pResp, (unsigned int)strlen(flashIdentity));

    printf("[-] Checking For Blank Flash: %f\n", ((float)(clock() - t_start))/CLOCKS_PER_SEC);

    // for speed up process, just blank check the area that we are going to write 
    // and align it to nearest page boundary
    if (!FlashDataValidate(NULL, FlashImageOffset, (bufferLen+(FLASH_PAGE_SIZE-1)) & (~(FLASH_PAGE_SIZE-1))))
    {
        printf("[-] Erasing Flash Start: %f\n", ((float)(clock() - t_start))/CLOCKS_PER_SEC);
        FlashEraseData(FlashImageOffset, bufferLen);
    }
    else
    {
        printf("[-] Flash is blank\n");
    }

    printf("[-] Write Image To Flash, buffer len (%d): %f\n", bufferLen, ((float)(clock() - t_start))/CLOCKS_PER_SEC);

    pData = pBuffer;
    address = FlashImageOffset;
    while (bufferLen && status)
    {
        // the bootloader SPI limited 256 characters, we take half sector at a time to avioid
        // cross the sector boundary
        dataLen = (bufferLen > 128) ? 128 : bufferLen;
        status = FlashWriteData(pData, address, dataLen);
        pData += dataLen;
        address += dataLen;
        bufferLen -= dataLen;
    }

    if (status)
    {
        pData = pBuffer;
        address = FlashImageOffset;
        bufferLen = fileSize;

        printf("[-] Verify Flash Content: %f\n", ((float)(clock() - t_start))/CLOCKS_PER_SEC);
        status = FlashDataValidate(pData, address, bufferLen);
    }
    else
    {
    printf("[-] Write Image Failed: %f\n", ((float)(clock() - t_start))/CLOCKS_PER_SEC);
    }
    free(pBuffer);

    return (status ? 0 : 1);
}

int DownloadFlashWritePDB(const char *a_pHeaderFile, const char* a_pDataFile, int a_group)
{
    long headerOffset, dataOffset;
    FILE* file;
    int fileSize;
    int headerFileLen, dataFileLen;
    int dataLen;
    
    int address, status=1;
    char pResp[128];
    char *pHeaderBuffer, *pDataBuffer, *pData;
    
    if(a_group == 1)
    {
        headerOffset=FlashOtpHeaderOffset;
        dataOffset= FlashOtpOffset;
    }
    else
    {
        headerOffset=FlashConfigHeaderOffset;
        dataOffset= FlashConfigOffset;
    }

    // open header file and get data into buffer
    if (NULL == (file = fopen(a_pHeaderFile, "rb")))
    {
        printf("Error: not able to open file (%s)", a_pHeaderFile);
        return 0;
    }
    // determine the size
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    if (!(pHeaderBuffer = (char *)malloc(fileSize+1)))
    {
        printf("Error: not able to malloc (%d)", fileSize+1);
        fclose(file);
        return 0;
    }
    headerFileLen = (int)fread(pHeaderBuffer, sizeof(unsigned char), fileSize, file);
    fclose(file);
    
    if (headerFileLen != fileSize)
    {
        printf("Error: data read from file (%d) != file size (%d)\n", headerFileLen, fileSize);
        free(pHeaderBuffer);
        return 0;
    }

    // open header file and get data into buffer
    if (NULL == (file = fopen(a_pDataFile, "rb")))
    {
        printf("Error: not able to open file (%s)", a_pDataFile);
        free(pHeaderBuffer);
        return 0;
    }
    // determine the size
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    if (!(pDataBuffer = (char *)malloc(fileSize+1)))
    {
        printf("Error: not able to malloc(%d)", fileSize+1);
        fclose(file);
        free(pHeaderBuffer);
        return 0;
    }
    dataFileLen = (int)fread(pDataBuffer, sizeof(unsigned char), fileSize, file);
    fclose(file);
    
    if (dataFileLen != fileSize)
    {
        printf("Error: data read from file (%d) != file size (%d)\n", dataFileLen, fileSize);
        free(pHeaderBuffer);
        free(pDataBuffer);
        return 0;
    }

    // Enable flash access on SPI
    FlashSendRecevie(spiEnable, pResp, (unsigned int)strlen(spiEnable));
    FlashSendRecevie(spiSetBaud, pResp, (unsigned int)strlen(spiSetBaud));

    // Erase the block (the header is in the same block as the main file)
    FlashEraseData(headerOffset, headerFileLen);

    // Program the Header file 
    pData = pHeaderBuffer;
    address = headerOffset;
    while (headerFileLen && status)
    {
        // the bootloader SPI limited 256 characters, we take half sector at a time to avioid
        // cross the sector boundary
        dataLen = (headerFileLen > 128) ? 128 : headerFileLen;
        status = FlashWriteData(pData, address, dataLen);
        pData += dataLen;
        address += dataLen;
        headerFileLen -= dataLen;
    }
    
    if (status)
    {
        headerFileLen = pData - pHeaderBuffer;
        pData = pHeaderBuffer;
        address = headerOffset;
        status = FlashDataValidate(pData, address, headerFileLen);
        if (!status)
        {
            printf("Error: PDB Header File Verify Failed\n");
        }
    }
    else
    {
        printf("Error: Write PDB Header File Failed\n");
    }

    // Program the Data file 
    pData = pDataBuffer;
    address = dataOffset;
    while (dataFileLen && status)
    {
        // the bootloader SPI limited 256 characters, we take half sector at a time to avioid
        // cross the sector boundary
        dataLen = (dataFileLen > 128) ? 128 : dataFileLen;

        // adjust the length to align page boundary
        if ((address & 0x7f) && (dataLen >= 128))
        {
            dataLen -= (address & 0x7f);
        }
        status = FlashWriteData(pData, address, dataLen);
        pData += dataLen;
        address += dataLen;
        dataFileLen -= dataLen;
    }
    
    if (status)
    {
        dataFileLen = pData - pDataBuffer;
        pData = pDataBuffer;
        address = dataOffset;
        status = FlashDataValidate(pData, address, dataFileLen);
        if (!status)
        {
            printf("Error: PDB Data File Verify Failed\n");
        }
    }
    else
    {
        printf("Error: Write PDB Data File Failed\n");
    }

    free(pHeaderBuffer);
    free(pDataBuffer);
    
    return (status ? 0 : 1);
}

void InitOpts(SPrsFlashCmdParms* a_pOpts)
{
    // Initialize defaults
    a_pOpts->imagesFolder = ".";
    a_pOpts->imageBinName = DEFAULT_IMAGE_BINARY_FILENAME;
    a_pOpts->baudRate = "115200";
    a_pOpts->serialDevice = "";
    a_pOpts->bFlashOTP = 0;
    a_pOpts->bFlashConfig = 0;
    a_pOpts->bFlashImage = 0;
    a_pOpts->bPrintMemory = 0;
    a_pOpts->address = 0;
    a_pOpts->len = 0;
    a_pOpts->flashSizeKb = PRS_FLASH_DEFAULT_SIZE_KB;
    a_pOpts->bErase = 0;
    a_pOpts->bVersion = 0;
    a_pOpts->bReset = 1;
}

int CheckDevice(String& a_selectedDevice)
{
    int32_t             comStatus = -1;
    char*               cmd = NULL;
    char                newline[]    = "\r";
    char                versioncmd[] = "version\r";
    int                 actual_resp_size = 0;
    bool                status = 0;
    
    comStatus = open_ser(a_selectedDevice.c_str());
    if (comStatus != -1)
    {
        cmd=newline;
        status = send_ser(cmd, strlen(cmd), resp, MAX_BUF_SIZE, &actual_resp_size);
        if (status == 0)
            resp[actual_resp_size] = '\0';
        cmd=versioncmd;
        status = (status == 0) && 
                 send_ser(cmd, strlen(cmd), resp, MAX_BUF_SIZE, &actual_resp_size);
        if (status == 0)
            resp[actual_resp_size] = '\0';

        if (status == 0 &&
             (strstr(resp, UMAC_VERSION_TEST_STRING) != NULL ||
              strstr(resp, BL_VERSION_TEST_STRING) != NULL))
        {
            comStatus = 0;
            printf("received:%d bytes, --------%s\n", actual_resp_size, resp);
        }
        else
        {
            close_ser();
            comStatus = -1;
        }
    }
    return comStatus;
}

int AutoselectDevice(String& a_selectedDevice)
{
    const char*         devices[2] = {default_device, second_device};
    uint32_t            deviceNum = 0;
    int32_t             comStatus = -1;

    while (deviceNum < 2)
    {
        printf("Trying device %s\n", devices[deviceNum]);
        a_selectedDevice = devices[deviceNum];
        
        comStatus = CheckDevice(a_selectedDevice);
        if (comStatus == 0)
        {
            break;
        }
        deviceNum++;
    }
    return comStatus;
}

