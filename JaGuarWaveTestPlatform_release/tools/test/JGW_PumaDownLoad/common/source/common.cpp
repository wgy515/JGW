#include "stdafx.h"
#include "../include/common_defs.h"

const char* g_cmdOptions[eCmdOption_Max] =
{
        "-h",   // eCmdOption_Help
        "-s",   // eCmdOption_Size
        "-p",   // eCmdOption_ComPort
        "-b",   // eCmdOption_Baud
        "-i",   // eCmdOption_Image
        "-c",   // eCmdOption_ConfigDatabase
        "-o",   // eCmdOption_OTPDatabase
        "-d",   // eCmdOption_InputFolder
        "-q",   // eCmdOption_PrintMemory
        "-e",   // eCmdOption_Erase
        "-v",   // eCmdOption_Version
        "-r",   // eCmdOption_Reset
        "-a"    // eCmdOption_All
};

static uint32_t GetAddressParms(
    char        *a_dumpStr,
    uint32_t    *address,
    uint32_t    *len    );
static void Examples();

void Usage()
{
    printf("prs_download_flash [optional flags]\n\n");
    printf("Optional flags:\n---------------\n");

    printf("    %s: Prints this usage information\n",
                g_cmdOptions[eCmdOption_Help]);

    printf("    %s: Size of the flash device in units of 1K (default 1024)\n",
                g_cmdOptions[eCmdOption_Size]);

    printf("    %s: Preferred serial port/device.\n",
                g_cmdOptions[eCmdOption_ComPort]);

    printf("    %s: Baud rate (default: 115200)\n",
                g_cmdOptions[eCmdOption_Baud]);

    printf("    %s <image_name>: Flashes the image file specified\n",
                g_cmdOptions[eCmdOption_Image]);

    printf("    %s: Flashes the config database sections\n",
                g_cmdOptions[eCmdOption_ConfigDatabase]);

    printf("    %s: Flashes the OTP database sections\n",
                g_cmdOptions[eCmdOption_OTPDatabase]);

    printf("    %s: Specifies images location folder (default \".\" if not specified)\n",
                g_cmdOptions[eCmdOption_InputFolder]);

    printf("    %s: Prints len bytes of data from offset, specified as 0x<start_address>:0x<len>\n",
                g_cmdOptions[eCmdOption_PrintMemory]);

    printf("    %s: Erases section specified by 0x<start_address>:0x<len>\n",
                g_cmdOptions[eCmdOption_Erase]);

    printf("    %s: Prints the tool version\n",
                g_cmdOptions[eCmdOption_Version]);

    printf("    %s: Resets the device after flash image: 0/1\n",
                g_cmdOptions[eCmdOption_Reset]);

    printf("    %s: Flashes all of the image (%s), OTP (%s) and Config (%s) using default names from\n",
                g_cmdOptions[eCmdOption_All],
                g_cmdOptions[eCmdOption_Image],
                g_cmdOptions[eCmdOption_OTPDatabase],
                g_cmdOptions[eCmdOption_ConfigDatabase]);
    printf("        specified (%s) or default directory\n",
                g_cmdOptions[eCmdOption_InputFolder]);

    printf("\n");
    printf("    This utility programs the firmware image, config and OTP sections.\n");
    printf("\n");
    printf("    If no options are provided, the default baud rate is used, and the suitable serial port is\n");
    printf("    auto-detected. The images folder is by default \".\" unless otherwise specified via %s.\n",
                g_cmdOptions[eCmdOption_InputFolder]);
    printf("    If no options are given the assumed action is to flash \"image.bin\" from the current directory.\n");
    printf("\n");
    printf("    As OTP and Config databases are comprised of multiple files (header/body) the file names of these\n");
    printf("    inputs are assumed to be the same as the Peraso build output. They are expected to be found at the\n");
    printf("    location specified by %s option (or default if not specified).\n",
                g_cmdOptions[eCmdOption_InputFolder]);
    printf("\n");

    Examples();
}

void Examples()
{
    printf("Example uses:\n-------------\n");

    printf("\n");
    printf("To load the image.bin file from local folder:\n\n");
    printf("> ./prs_download_flash\n");

    printf("\n");
    printf("To load the specified image from local folder:\n\n");
    printf("> ./prs_download_flash -i image_sec.bin\n");

    printf("\n");
    printf("To load the specified image from the specified directory\n\n");
    printf("> ./prs_download_flash -i image.bin -d ./PER0791553010001__PER0791553010001\n");

    printf("\n");
    printf("To load the OTP and Config from the specified directory:\n\n");
    printf("> ./prs_download_flash -o -c -d ./PER0791553010001__PER0791553010001\n");

    printf("\n");
    printf("To load all image, OTP and Config from specified directory:\n\n");
    printf("> ./prs_download_flash -o -c -i image.bin ./PER0791553010001__PER0791553010001\n");

    printf("\n");
    printf("To load all from the current directory:\n\n");
    printf("> ./prs_download_flash -a\n");

    printf("\n");
    printf("To display a section of flash starting at 0xF0000:\n\n");
    printf("> ./prs_download_flash -q 0xF0000:0x328\n");

    printf("\n");
    printf("To erase a sector of flash starting at 0xF0000 (note that minimum\n");
    printf("erase size is one 64K sector):\n\n");
    printf("> ./prs_download_flash -e 0xF0000:0xFFFF\n");

}

uint32_t ParseCommandLine(
    int32_t                 argc,
    char*                   argv[],
    SPrsFlashCmdParms*      parms)
{
    uint32_t result = eDownloadResult_success;
    uint8_t resetOption = 0;

    InitOpts(parms);

    for(int32_t i = 1; i < argc; i++)
    {
        if(result)
            break;

        // Process command switches
        if (strlen(argv[i]) > 1 && strncmp(argv[i], "-", 1) == 0)
        {
            // eCmdOption_Help
            if(0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Help]))
            {
                Usage();
                exit(0);
            }

            // eCmdOption_Baud
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Baud]))
            {
                i++;
                if(i < argc)
                    parms->baudRate = argv[i];
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_ConfigDatabase
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_ConfigDatabase]))
            {
                parms->bFlashConfig = 1;
            }

            // eCmdOption_OTPDatabase
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_OTPDatabase]))
            {
                parms->bFlashOTP = 1;
            }

            // eCmdOption_ComPort
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_ComPort]))
            {
                i++;
                if(i < argc)
                    parms->serialDevice = argv[i];
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_Image
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Image]))
            {
                parms->bFlashImage = 1;
                i++;
                if(i < argc)
                    parms->imageBinName = argv[i];
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_OutputFoler
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_InputFolder]))
            {
                i++;
                if(i < argc)
                    parms->imagesFolder = argv[i];
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_PrintMemory
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_PrintMemory]))
            {
                i++;
                if(i < argc)
                {
                    parms->bPrintMemory = 1;
                    if (!GetAddressParms((char*)argv[i], &parms->address, &parms->len))
                        result |= (1 << eDownloadResult_invalidArgsFailure);
                }
                else
                {
                    result |= (1<< eDownloadResult_invalidArgsFailure);
                }
            }

            // eCmdOption_Erase
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Erase]))
            {
                i++;
                if(i < argc)
                {
                    parms->bErase = 1;
                    if (!GetAddressParms((char*)argv[i], &parms->address, &parms->len))
                        result |= (1 << eDownloadResult_invalidArgsFailure);
                }
                else
                {
                    result |= (1<< eDownloadResult_invalidArgsFailure);
                }
            }

            // eCmdOption_Size
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Size]))
            {
                i++;
                if(i < argc)
                    parms->flashSizeKb = atoi(argv[i]);
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_Version
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Version]))
            {
                parms->bVersion = 1;
            }

            // eCmdOption_PrintMemory
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_Reset]))
            {
                resetOption = 1;
                i++;
                if(i < argc)
                    parms->bReset = atoi(argv[i]) > 0 ? 1 : 0;
                else
                    result |= (1<< eDownloadResult_invalidArgsFailure);
            }

            // eCmdOption_All
            else if (0 == strcmp(argv[i], g_cmdOptions[eCmdOption_All]))
            {
                // With this option all the default names are assumed, and
                // the input folder may be defaulted, or if eCmdOption_InputFolder
                // option has been specified then the folder is taken from that.
                parms->bFlashImage = 1;
                parms->bFlashOTP = 1;
                parms->bFlashConfig = 1;
            }

            else
            {
                result |= (1<< eDownloadResult_invalidArgsFailure);
            }
        }
        else
        {
            result |= (1<< eDownloadResult_invalidArgsFailure);
        }
    }

    // For backward compatibility and uses that provide no command line options
    // at all, assume download image with default name and path
    if (RESULT_OK (result) && (argc < 2 || (argc < 4 && resetOption)))
    {
        parms->bFlashImage = 1;
    }
    return result;
}

void PrintResult(uint32_t result)
{
    if (result & (1 << eDownloadResult_setUsbLatencyWarning))     printf("WARNING: Set USB Latency failure.\n");
    if (result & (1 << eDownloadResult_invalidArgsFailure))       printf("ERROR: Invalid arguments.\n");
    if (result & (1 << eDownloadResult_findBootromPortFailure))   printf("ERROR: Could not find bootrom COM port.\n");
    if (result & (1 << eDownloadResult_openBootromPortFailure))   printf("ERROR: Could not open bootrom COM port.\n");
    if (result & (1 << eDownloadResult_setBootloaderBaudFailure)) printf("ERROR: Failure setting boot rom baud rate.\n");
    if (result & (1 << eDownloadResult_downloadImageFailure))     printf("ERROR: Failure downloading Image COM port.\n");
    if (result & (1 << eDownloadResult_downloadConfigFailure))    printf("ERROR: Failure downloading Config database.\n");
    if (result & (1 << eDownloadResult_downloadOTPFailure))       printf("ERROR: Failure downloading OTP database.\n");
    if (result & (1 << eDownloadResult_invalidImage))             printf("ERROR: Invalid image or image not found.\n");
    if (result & (1 << eDownloadResult_resources))                printf("ERROR: Unable to allocate buffer.\n");
    if (RESULT_OK (result)) printf("SUCCESS\n");
}

static uint32_t GetAddressParms(
    char            *a_dumpStr,
    uint32_t        *address,
    uint32_t        *len)
{
    char*       tok = NULL;
    uint32_t    rc = 0;

    if (a_dumpStr != NULL && strlen(a_dumpStr) > 0)
    {
        tok = strtok(a_dumpStr, ":");
        if (tok)
        {
            *address = strtoul(tok, NULL, 16);
            if ((tok = strtok(NULL, ":")) != NULL)
            {
                *len = strtoul(tok, NULL, 16);
                rc = 1;
            }
        }
    }
    if (!rc)
    {
        printf("ERROR: Dump address of the format 0x<start_address>:0x<len>\n");
    }
    return rc;
}

int32_t GetFileSize(const char* a_pFileName)
{
    int32_t size;

    FILE * file = fopen(a_pFileName, "rb");
    if (file == NULL)
    {
        return 0;
    }
    // determine the size
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind (file);
    fclose(file);

    return size;
}

int32_t GetFileData(
    const char      *a_pFileName,
    unsigned char   *a_pBuffer,
    uint32_t        a_bufferLen)

{
    int32_t size;

    FILE * file = fopen(a_pFileName, "rb");
    if (file == NULL)
    {
        return 0;
    }
    size = (int32_t)fread(a_pBuffer, sizeof(unsigned char), a_bufferLen, file);
    fclose(file);

    return size;
}

void ShowVersion(void) {
#if 0
    printf(PRS_FORMAT_VERSION_INFO " " PRS_FORMAT_TAG_INFO PRS_FORMAT_EXTRA_INFO,
            PRS_FLASH_BIN_COMPONENT_STRING, PRS_BUILD_VERSION_STRING, PRS_REV_NUM, PRS_BUILD_TAG,
            PRS_BUILD_DATE, PRS_BUILD_TIME, PRS_BUILD_USER);
#endif
    printf ("prs_download_flash - v0.04 2017.04.01\n");
}
