#include "StdAfx.h"
#include "CJGW_PrsDownloadFlash.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_PrsDownloadFlash::CCJGW_PrsDownloadFlash(CCJGW_CSerialComPort& seialComPort) : mcPrsSerial(seialComPort),muSerialNumberPosToBin(0x0d),muMacAddressPosToBin(0x05)
    {
        mRecvBuf = (char*)calloc(0x01,RSP_RECV_MAX_BUF_SIZE);
		msPrsFlashAddress.mnFlashSizeKb = PRS_FLASH_DEFAULT_SIZE_KB;
        InitializeFlashAddresses(msPrsFlashAddress.mnFlashSizeKb);
    }


    CCJGW_PrsDownloadFlash::~CCJGW_PrsDownloadFlash(void)
    {
        free((void*)mRecvBuf);
        FreeDownloadFlashImage();
    }
#if 1
    void CCJGW_PrsDownloadFlash::InitializeFlashAddresses(uint32_t a_flashSizeKb /* = PRS_FLASH_DEFAULT_SIZE_KB */)
    {
        for (int i = 0;i < E_PRS_MAX_DONGLES_PDB_FILE;i ++)
        {
            msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mnFlashOffset = 0;
            msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mnFileSize = 0;
            msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mszFileBuf = NULL;
        }

		msPrsFlashAddress.mnbaudRate = 115200;
		//! msPrsFlashAddress.mnFlashSizeKb = PRS_FLASH_DEFAULT_SIZE_KB;

        msPrsFlashAddress.flashMemoryBase = 0xF8000000;
        msPrsFlashAddress.flashOffsetMask = 0x00FFFFFF;
        msPrsFlashAddress.flashImageOffset = PRS_FW_IMAGE_OFFSET;

        // Variable locations based on flash size
        msPrsFlashAddress.flashConfigHeaderOffset = PRS_CONFIG_HEADER_OFFSET(a_flashSizeKb);
        msPrsFlashAddress.flashConfigOffset = PRS_CONFIG_BODY_OFFSET(a_flashSizeKb);
        msPrsFlashAddress.flashOtpHeaderOffset = PRS_OTP_HEADER_OFFSET(a_flashSizeKb);
        msPrsFlashAddress.flashOtpOffset = PRS_OTP_BODY_OFFSET(a_flashSizeKb);

        msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mnFlashOffset = msPrsFlashAddress.flashImageOffset;
        msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mnFlashOffset = msPrsFlashAddress.flashConfigHeaderOffset;
        msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mnFlashOffset = msPrsFlashAddress.flashOtpHeaderOffset;
    }

    void CCJGW_PrsDownloadFlash::FreeDownloadFlashImage()
    {
        for (int i = 0;i < E_PRS_MAX_DONGLES_PDB_FILE;i ++)
        {
            if (CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[i]))
            {
                free(msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mszFileBuf);
                msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mszFileBuf = NULL;
                msSPrsFlashCmdParms.msPrsPDBFileInfo[i].mnFileSize = 0;
            }
        }
    }

    void ScanDownloadImageFile(const char* filepath,const char* filename,void* lpData)
    {
        std::vector<std::string>* vstrFiles = (std::vector<std::string>*)lpData;
        std::string strFilePathTemp(filepath);
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        vstrFiles->push_back(strFilePathTemp);
    }

    bool CCJGW_PrsDownloadFlash::InitDownloadFlashImage(const std::string& strFolder,bool bInitFireware)
    {
        int numFiles = 0;
        std::vector<std::string> vstrFiles;
        std::string folder = strFolder;
        FreeDownloadFlashImage();
        //! 扫描下载bin文件
        JGW_TraverseFolder_C(folder,numFiles,"\\*.bin",ScanDownloadImageFile,&vstrFiles);
        if (0 == numFiles)
        {
            return false;
        }
        ///
        for (size_t i = 0;i < vstrFiles.size();i ++)
        {
            /// persistent_database_0_otp.bin
            if (NULL != JGW_StrCaseStr(vstrFiles[i].c_str(),"otp.bin"))
            {
                msSPrsFlashCmdParms.mstrOtpBobyFilePath = vstrFiles[i];
            }
            else if (NULL != JGW_StrCaseStr(vstrFiles[i].c_str(),"otp_header.bin"))
            {
                msSPrsFlashCmdParms.mstrOtpHeaderFilePath = vstrFiles[i];
            }
            else if (NULL != JGW_StrCaseStr(vstrFiles[i].c_str(),"config.bin"))
            {
                msSPrsFlashCmdParms.mstrConfigBobyFilePath = vstrFiles[i];
            }
            else if (NULL != JGW_StrCaseStr(vstrFiles[i].c_str(),"config_header.bin"))
            {
                msSPrsFlashCmdParms.mstrConfigHeaderFilePath = vstrFiles[i];
            }
            else if (NULL != JGW_StrCaseStr(vstrFiles[i].c_str(),"image"))
            {
                msSPrsFlashCmdParms.mstrFirewareFilePath = vstrFiles[i];
            }
        }

        if (bInitFireware)
        {
            msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mszFileBuf = JGW_GetFileBufToPath_C(msSPrsFlashCmdParms.mstrFirewareFilePath.c_str(), msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mnFileSize);
            if (!CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE])) 
            {
                return false;
            }
        }

        return GetPrsFileInfo(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP],msSPrsFlashCmdParms.mstrOtpHeaderFilePath,msSPrsFlashCmdParms.mstrOtpBobyFilePath) 
            && GetPrsFileInfo(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG],msSPrsFlashCmdParms.mstrConfigHeaderFilePath,msSPrsFlashCmdParms.mstrConfigBobyFilePath) 
            && GetSerialNumberMacAddressPosToOtpBinBuf(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP]);
    }

    bool CCJGW_PrsDownloadFlash::GetPrsFileInfo(S_PRS_FILE_INFO& sFileInfo,const std::string& strPDBHeaderFilePath,const std::string& strPDBBobyFilePath)
    {
        if (strPDBBobyFilePath.empty() && strPDBHeaderFilePath.empty())
        {
            return true;
        }
        //! load prs pdb header file
        S_PRS_FILE_INFO sHeaderFileInfo = {0},sBobyFileInfo = {0};
        if (!strPDBHeaderFilePath.empty())
        {
            sHeaderFileInfo.mszFileBuf = JGW_GetFileBufToPath_C(strPDBHeaderFilePath.c_str(),sHeaderFileInfo.mnFileSize);
            if (!CheckPrsFileInfoIsValid(sHeaderFileInfo))
            {
                return false;
            }
        }
        //! load prs pdb boby file
        if (!strPDBBobyFilePath.empty())
        {
            sBobyFileInfo.mszFileBuf = JGW_GetFileBufToPath_C(strPDBBobyFilePath.c_str(),sBobyFileInfo.mnFileSize);
            if (!CheckPrsFileInfoIsValid(sBobyFileInfo))
            {
                return false;
            }
        }

        sFileInfo.mszFileBuf = (char*)calloc(sHeaderFileInfo.mnFileSize + sBobyFileInfo.mnFileSize + 1, 1);
        sFileInfo.mnFileSize = sHeaderFileInfo.mnFileSize + sBobyFileInfo.mnFileSize;

        if (CheckPrsFileInfoIsValid(sHeaderFileInfo))
        {
            for (size_t i = 0;i < sHeaderFileInfo.mnFileSize;i ++)
            {
                sFileInfo.mszFileBuf[i] = sHeaderFileInfo.mszFileBuf[i];
            }
            free(sHeaderFileInfo.mszFileBuf);
        }

        if (CheckPrsFileInfoIsValid(sBobyFileInfo))
        {
            for (size_t i = 0;i < sBobyFileInfo.mnFileSize;i ++)
            {
                sFileInfo.mszFileBuf[sHeaderFileInfo.mnFileSize + i] = sBobyFileInfo.mszFileBuf[i];
            } 
            free(sBobyFileInfo.mszFileBuf);
        }

        return true;
    }

    bool CCJGW_PrsDownloadFlash::BuildPDBFileToPDBBuf(const std::string& saveFolder,bool bDownFireware /* = true */,bool bDownloadOtp /* = true */,bool bDownloadConfig /* = true */)
    {
        ///! 
        if (!JGW_PathFileExistsA(saveFolder.c_str()))
        {
            JGW_CreateDirectoryA(saveFolder.c_str(),true);
        }

        if (bDownFireware && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE]))
        {
            std::string strFirewareFilePath ("");
            JGW_FormatString(strFirewareFilePath,"%s\\image%s.bin",saveFolder.c_str(),JGW_GetCurrentTimeStampYMDHMS().c_str());
            JGW_WriteFileToBuf(strFirewareFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mszFileBuf,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mnFileSize);
        }

        if (bDownloadOtp && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP]))
        {
            std::string strOtpFilePath ("");
            JGW_FormatString(strOtpFilePath,"%s\\persistent_database_%s_otp.bin",saveFolder.c_str(),JGW_GetCurrentTimeStampYMDHMSA().c_str());
            JGW_WriteFileToBuf(strOtpFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mnFileSize);
        }

        if (bDownloadConfig && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG]))
        {
            std::string strConfigFilePath ("");
            JGW_FormatString(strConfigFilePath,"%s\\persistent_database_%s_config.bin",saveFolder.c_str(),JGW_GetCurrentTimeStampYMDHMSA().c_str());
            JGW_WriteFileToBuf(strConfigFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mszFileBuf,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mnFileSize);
        }

        return true;
    }

    bool CCJGW_PrsDownloadFlash::BuildSrcPDBFileToPDBBuf(const std::string& saveFolder,bool bDownFireware /* = true */,bool bDownloadOtp /* = true */,bool bDownloadConfig /* = true */)
    {

        ///! 
        if (!JGW_PathFileExistsA(saveFolder.c_str()))
        {
            JGW_CreateDirectoryA(saveFolder.c_str(),true);
        }

        if (bDownFireware && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE]))
        {
            std::string strFirewareFilePath ("");
            JGW_FormatString(strFirewareFilePath,"%s\\image%s.bin",saveFolder.c_str(),JGW_GetCurrentTimeStampYMDHMS().c_str());
            JGW_WriteFileToBuf(strFirewareFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mszFileBuf,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE].mnFileSize);
        }

        if (bDownloadOtp && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP]))
        {
            std::string strOtpFilePath ("");
            JGW_FormatString(strOtpFilePath,"%s\\%s",saveFolder.c_str(),PRS_OTP_HEADER_FILE_NAME);
            JGW_WriteFileToBuf(strOtpFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf,/*msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mnFileSize -*/ 32);

            JGW_FormatString(strOtpFilePath,"%s\\%s",saveFolder.c_str(),PRS_OTP_BODY_FILE_NAME);
            JGW_WriteFileToBuf(strOtpFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf + 32,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mnFileSize -32);
        }

        if (bDownloadConfig && CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG]))
        {
            std::string strConfigFilePath ("");
            JGW_FormatString(strConfigFilePath,"%s\\%s",saveFolder.c_str(),PRS_CONFIG_HEADER_FILE_NAME);
            JGW_WriteFileToBuf(strConfigFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mszFileBuf,32/*msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mnFileSize*/);

            JGW_FormatString(strConfigFilePath,"%s\\%s",saveFolder.c_str(),PRS_CONFIG_BODY_FILE_NAME);
            JGW_WriteFileToBuf(strConfigFilePath.c_str(),(const char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mszFileBuf + 32,msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG].mnFileSize - 32);
        }

        return true;
    }

    bool CCJGW_PrsDownloadFlash::CheckPrsFileInfoIsValid(S_PRS_FILE_INFO& sFileInfo)
    {
        return NULL != sFileInfo.mszFileBuf && sFileInfo.mnFileSize > 0;
    }

    eDeviceBootMode CCJGW_PrsDownloadFlash::CheckDeviceIsBootloaderMode()
    {
        char* newline = "\r";
        char* versioncmd = "version\r";
        std::string resp = "";

        mcPrsSerial.ReadRespCommandToWriteCommand(newline,resp);
        int status = mcPrsSerial.ReadRespCommandToWriteCommand(versioncmd,resp);
        if (status <= 0)
        {
            return eDeviceBootMode_Unknown;
        }

        if (std::string::npos != resp.find(UMAC_VERSION_TEST_STRING))
        {
            return eDeviceBootMode_UMAC;
        }

        if (std::string::npos != resp.find(BL_VERSION_TEST_STRING))
        {
            return eDeviceBootMode_BootLoader;
        }

        return eDeviceBootMode_Unknown;
    }

    bool CCJGW_PrsDownloadFlash::DonwloadAllPDBImageFileToDongleFlash(bool bDownFireware /* = true */,bool bDownloadOtp /* = true */,bool bDownloadConfig /* = true */)
    {
        if (bDownFireware)
        {
            if (!DownloadFlashWriteImage(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_FW_IMAGE],true)) 
            {
				Log4AE("Download Fireware Fail");
                return false;
            }
        }

        if (bDownloadOtp)
        {
            if (!DownloadFlashWriteImage(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP],false)) 
            {
				Log4AE("Download Otp Fail");
                ///return false;
            }
        }

        if (bDownloadConfig)
        {
            if (!DownloadFlashWriteImage(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_CONFIG],false)) 
            {
				Log4AE("Download Config Fail");
                ///return false;
            }
        }

        return true;
    }

    bool CCJGW_PrsDownloadFlash::DownloadFlashWriteImage(S_PRS_FILE_INFO& psPrsFileInfo,bool isFireware /* = false */)
    {
        if (!CheckPrsFileInfoIsValid(psPrsFileInfo))
        {
            return false;
        }
		//ValidateFlashDataEqualToPrsFileBuf(psPrsFileInfo)?0:1;
        //! init spi
        std::string resp = "";
        mcPrsSerial.ReadRespCommandToWriteCommand(spiEnable, resp);
        mcPrsSerial.ReadRespCommandToWriteCommand(spiSetBaud, resp);
        if (isFireware) mcPrsSerial.ReadRespCommandToWriteCommand(flashIdentity, resp);


        //! Check Flash is blank
        // for speed up process, just blank check the area that we are going to write 
        // and align it to nearest page boundary
        if (!isFireware || !FlashDataValidate(NULL, psPrsFileInfo.mnFlashOffset, (psPrsFileInfo.mnFileSize + (FLASH_PAGE_SIZE - 1)) & (~(FLASH_PAGE_SIZE - 1))))
        {
            FlashEraseData(psPrsFileInfo.mnFlashOffset, psPrsFileInfo.mnFileSize/*isFireware?psPrsFileInfo.mnFileSize:32*/);
        }

        char* pData = psPrsFileInfo.mszFileBuf;
        int status = 1;
        unsigned int address = psPrsFileInfo.mnFlashOffset,dataLen = 0,bufferLen = psPrsFileInfo.mnFileSize;
        while (bufferLen && status)
        {
            // the bootloader SPI limited 256 characters, we take half sector at a time to avioid
            // cross the sector boundary
            dataLen = (bufferLen > 128) ? 128 : bufferLen;
            //! 内存对其校验
            /*if ((address & 0x7F)&&(dataLen >= 128))
            {
            dataLen -= (address&0x7f);
            }*/
            status = FlashWriteData(pData, address, dataLen);
            pData += dataLen;
            address += dataLen;
            bufferLen -= dataLen;
        }

        if (status)
        {
            //pData = (char*)psPrsFileInfo.mszFileBuf;
            //address = psPrsFileInfo.mnFlashOffset;
            //bufferLen = psPrsFileInfo.mnFileSize;
            //status = FlashDataValidate(pData, address, bufferLen);
			status = ValidateFlashDataEqualToPrsFileBuf(psPrsFileInfo)?0:1;
        }
        return (status ? false : true);
    }

    int CCJGW_PrsDownloadFlash::FlashWriteData(char *a_pData, unsigned int a_address, unsigned int a_length)
    {
        int status = 0;
        char *pFmtBuffer = NULL;
        // get a buffer big enough to hold 'sw b 0 count opcode address (3*a_length)'
        if (NULL == (pFmtBuffer = (char *)malloc(32 + (a_length * 3))))
        {
            return 0;
        }

        FlashSendReceiveIsError(flashEnable);
        JGW_PrsFormatSendBuffer(pFmtBuffer, a_address, a_pData, a_length);
        status = FlashSendReceiveIsError(pFmtBuffer);
        free(pFmtBuffer);

        return ((status == 0) ? 1 : 0);
    }

    int CCJGW_PrsDownloadFlash::FlashEraseData(unsigned int a_address, unsigned int a_length)
    {
        char pCmd[32] = {0};
        int status = 0;

        if (a_length > 0x0fffff)
        {
            // do a bulk erase
            FlashSendReceiveIsError(flashErase);
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

                status = FlashSendReceiveIsError(pCmd);
                if (status)
                {
                    break;
                }
                a_address += FLASH_SECTOR_SIZE;
            }
        }
        return status;  
    }

    int CCJGW_PrsDownloadFlash::FlashReadData(char *a_pData, unsigned int a_address, unsigned int a_length)
    {
        int nbrOfLine = (a_length/16) + ((a_length % 16) ? 1 : 0);
        int recvLength = (nbrOfLine * 2) + ((a_length * 3) - nbrOfLine) + 3;
        char cmd[32] = {0};

        JGW_GetPrsFlashReadDataCmd(cmd,a_address, a_length);
        /// memcpy(a_pData,mRecvBuf,)
        //mcPrsSerial.SendPrsSerial(cmd,strlen(cmd));
        //mcPrsSerial.RecvPrsSerial(a_pData,recvLength);
        return FlashSendReceiveIsError(cmd,a_pData,recvLength);
    }

    int CCJGW_PrsDownloadFlash::FlashDataValidate(char *a_pData, unsigned int a_address, unsigned int a_length)
    {
        int status = 1, i, length;
        unsigned int mask = 0xffffffff, offset = 0;
        char *pFmtBuffer;
        unsigned char blankChar = 0xff;
        int nbrOfLine = (FLASH_READ_SIZE/16);
        int rcvLength = (nbrOfLine * 2) + ((FLASH_READ_SIZE * 3) - nbrOfLine) + 3;

        // get a buffer big enough to hold 'sw b 0 count opcode address (3*read_length)'
        if (NULL == (pFmtBuffer = (char *)malloc(rcvLength + 1)))
        {
            return 0;
        }

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
            JGW_PrsUnformatReceiveBuffer(pFmtBuffer, pFmtBuffer, length);
            for (i = 0; i < length; i++)
            {
                if (pFmtBuffer[i] != 0xFF/*a_pData[offset & mask]*/)
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

        free(pFmtBuffer);
        return (status);
    }

    bool CCJGW_PrsDownloadFlash::ValidateFlashDataEqualToPrsFileBuf(S_PRS_FILE_INFO& sFileInfo)
    {
        int nbrOfLine = (FLASH_READ_SIZE/16);
        int rcvLength = (nbrOfLine * 2) + ((FLASH_READ_SIZE * 3) - nbrOfLine) + 3;
        size_t a_length = sFileInfo.mnFileSize,a_address = sFileInfo.mnFlashOffset,readLen = 0;
		char pFmtBuffer[1024] = {0};

        while (a_length)
        {
            readLen = (a_length > FLASH_READ_SIZE) ? FLASH_READ_SIZE : a_length;
            FlashReadData(pFmtBuffer, a_address, readLen);
            JGW_PrsUnformatReceiveBuffer(pFmtBuffer, pFmtBuffer, readLen);
            for (size_t i = 0;i < readLen;i ++)
            {
                if (sFileInfo.mszFileBuf[sFileInfo.mnFileSize - a_length + i] != pFmtBuffer[i])
                {
                    return false;
                }
            }
            a_length -= readLen;
            a_address += readLen;
        }

        return true;
    }

    int CCJGW_PrsDownloadFlash::FlashSendReceiveIsError(const char* a_pData, char* a_pResp, unsigned int a_length)
    {
#if 1
        if (mcPrsSerial.WriteAsyncCommand(a_pData,strlen(a_pData)) != strlen(a_pData))
        {
            return 1;
        }

        int len = mcPrsSerial.ReadAsyncCommand(mRecvBuf,RSP_RECV_MAX_BUF_SIZE);
        mRecvBuf[len] = '\0';
        for (size_t i = 0;i < a_length && i < (size_t)len;i ++)
        {
            a_pResp[i] = mRecvBuf[i];
        }
        return  (len > 0 && len < RSP_RECV_MAX_BUF_SIZE) ? 0 : 1;
#else
        if (mcPrsSerial.WriteAsyncCommand(a_pData,strlen(a_pData)) != strlen(a_pData))
        {
            return 1;
        }

        int len = mcPrsSerial.ReadAsyncCommand(a_pResp,a_length);
        mRecvBuf[len] = '\0';
        return  (len > 0 && len < RSP_RECV_MAX_BUF_SIZE) ? 0 : 1;
#endif;
    }

    int CCJGW_PrsDownloadFlash::FlashSendReceiveIsError(const char* a_pData/*, char *a_pResp, unsigned int a_length*/)
    {
        if (mcPrsSerial.WriteAsyncCommand(a_pData,strlen(a_pData)) != strlen(a_pData))
        {
            return 1;
        }
#if 0
        bool bReadTerm = false;
        for (int i = 0;i < 3;i ++)
        {
            int len = mcPrsSerial.ReadAsyncCommand(a_pResp,a_length);
            a_pResp[len] = '\0';
            for (int index = 0;index < len;index ++)
            {
                if ('>' == a_pResp[index])
                {
                    bReadTerm = true;
                }
            }
            if (bReadTerm)
            {
                break;
            }
            //Sleep(10);
            printf("%s,len:%d",a_pResp,len);
        }
#else
        int len = mcPrsSerial.ReadAsyncCommand(mRecvBuf,RSP_RECV_MAX_BUF_SIZE);
        mRecvBuf[len] = '\0';
#endif
        return  (len > 0 && len < RSP_RECV_MAX_BUF_SIZE) ? 0 : 1;
    }

    /* uint32_t CCJGW_PrsDownloadFlash::CheckPDBFileTypeGetFlashOffset(unsigned char* szFileBuf,size_t fileSize)
    {
    if (0 == fileSize || NULL == szFileBuf)
    {
    return 0;
    }
    ///
    if (fileSize < 32)
    {
    free(szFileBuf);
    return 0;
    }
    /// DB 01 OTP HEADER
    if (szFileBuf[0] == 0xDB && szFileBuf[1] == 0x01)
    {
    return msPrsFlashAddress.flashOtpHeaderOffset;
    }
    /// DB 02 CONFIG HERADER
    if (szFileBuf[0] == 0xDB && szFileBuf[1] == 0x02)
    {
    return msPrsFlashAddress.flashConfigHeaderOffset;
    }
    /// 0B 00 flash image offset
    if (szFileBuf[0] == 0x0B && szFileBuf[1] == 0x00)
    {
    return msPrsFlashAddress.flashImageOffset;
    }
    /// 60 01 01 01 06 => mac 标识 OTP
    if (fileSize < 2 * 1024)
    {
    for (size_t i = 0;i < fileSize - 5;i ++)
    {
    if (szFileBuf[i] == 0x60 
    && szFileBuf[i + 1] == 0x01
    && szFileBuf[i + 2] == 0x01
    && szFileBuf[i + 3] == 0x01
    && szFileBuf[i + 4] == 0x06)
    {
    return msPrsFlashAddress.flashOtpOffset;
    }
    }
    }

    return msPrsFlashAddress.flashConfigOffset; 
    }*/

    bool CCJGW_PrsDownloadFlash::GetSerialNumberMacAddressPosToOtpBinBuf(S_PRS_FILE_INFO& sFileInfo)
    {
        if (!CheckPrsFileInfoIsValid(sFileInfo))
        {
            return true;
        }
        /// mac 60 01 01 01 06  => sn 02 80
        unsigned char* szBuf = (unsigned char*)sFileInfo.mszFileBuf;
        for (size_t i = 0;i < sFileInfo.mnFileSize - 5 - 8;i ++)
        {
            /// mac
            if (0x60 == szBuf[i] 
            && 0x01 == szBuf[i + 1]
            && 0x01 == szBuf[i + 2]
            && 0x01 == szBuf[i + 3]
            && 0x06 == szBuf[i + 4]
            )
            {
                muMacAddressPosToBin = i + 4 + 1;
                if (0x02 == szBuf[muMacAddressPosToBin + 6]
                && 0x80 == szBuf[muMacAddressPosToBin + 7])
                {
                    muSerialNumberPosToBin = muMacAddressPosToBin + 7 + 1;
                    Log4AI_F("Mac Address Pos To Bin:%d,Serial Number Pos To Bin:%d",muMacAddressPosToBin,muSerialNumberPosToBin);
                    return true;
                }
            }
        }
        return false;
    }

    bool CCJGW_PrsDownloadFlash::SetSerailNumberMacAddressToOtpFile(const std::string& serailNumber,const std::string& macAddress)
    {
        if (!CheckPrsFileInfoIsValid(msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP]))
        {
            Log4AE("prs otp bin buf is empty");
            return false;
        }

        if (!serailNumber.empty())
        {
            for (int i = 0;i < SERIALNUMBER_MAX_BITS;i ++)
            {
                msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf[muSerialNumberPosToBin + i] = '\0';
            }
            for (size_t i = 0;i < serailNumber.length();i ++)
            {
                msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf[muSerialNumberPosToBin + i * 2] = serailNumber[i];
            }
        }

        if (!macAddress.empty())
        {
            std::string strMac(macAddress);
            JGW_ReplaceStringA(strMac,":","");
            if (MAC_ADDRESS_LENGTH != strMac.length())
            {
                Log4AE_F("MAC（%s） len is out of range(The default length:12)",strMac.c_str());
                return false;
            }

            unsigned char szMacAddr[14] = {0};
            sscanf_s(strMac.c_str(),"%02X%02X%02X%02X%02X%02X",&szMacAddr[0],&szMacAddr[1],&szMacAddr[2],&szMacAddr[3],&szMacAddr[4],&szMacAddr[5]);
            for (int i = 0;i < MAC_ADDRESS_LENGTH / 2;i ++)
            {
                msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf[muMacAddressPosToBin + i] = szMacAddr[i];
            }
        }

        return true;
    }

    bool CCJGW_PrsDownloadFlash::BuildOTPCRC32()
    {
        return JGW_PrsGeneratedCRC32ToImageBuf(
            (unsigned char*)msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mszFileBuf,
            msSPrsFlashCmdParms.msPrsPDBFileInfo[E_PRS_OTP].mnFileSize);
    }
#endif
}


