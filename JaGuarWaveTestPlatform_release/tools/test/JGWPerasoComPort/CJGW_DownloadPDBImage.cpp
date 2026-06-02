#include "StdAfx.h"
#include "CJGW_DownloadPDBImage.h"

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_DownloadPDBImage::CCJGW_DownloadPDBImage(void):muFlashTotalSize(0),mnManufacturerID(255)
    {
        for (int i = 0;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            msOTPConfigFiles.muDonglesFlashPDBAddr[i] = 0;
            msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf = NULL;
            msOTPConfigFiles.msDonglesPDBFileInfo[i].mnFileSize = NULL;
        }
        mszWriteBuf = (char*)calloc(MAX_WRITE_LEN + 1, 1);
        mszReadBuf = (char*)calloc(MAX_WRITE_LEN + 1, 1);
        memset(mstrChangeSN,0x00,128);
    }


    CCJGW_DownloadPDBImage::~CCJGW_DownloadPDBImage(void)
    {
        for (int i = 0;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            if (msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf)
            {
                free(msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf);
                msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf = NULL;
            }
        }

        if (mszWriteBuf) free(mszWriteBuf);
        if (mszReadBuf) free (mszReadBuf);
    }

    void CCJGW_DownloadPDBImage::UnLoadPDBImageFile()
    {
        for (int i = 0;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            if (msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf)
            {
                free(msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf);
                msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf = NULL;
            }
        }
    }
    void CCJGW_DownloadPDBImage::LoadPDBImageFile(std::string& strFloder)
    {
        std::string strTempFilePath;
        UnLoadPDBImageFile();

        JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),FW_IMAGE_BIN);
        msOTPConfigFiles.msDonglesPDBFileInfo[E_FW_IMAGE].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[E_FW_IMAGE].mnFileSize);
        //!
        JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),PERSISTENT_DATABSE_1_CONFIG_HEADER);
        msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG_HEADER].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG_HEADER].mnFileSize);

        JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),PERSISTENT_DATABSE_1_CONFIG);
        msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mnFileSize);

        JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),PERSISTENT_DATABSE_0_OTP_HEADER);
        msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mnFileSize);

        JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),PERSISTENT_DATABSE_0_OTP);
        msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize);


       /* return msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf 
            && msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mszFileBuf
            && msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mszFileBuf
            && msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG_HEADER].mszFileBuf;*/
    }

    bool CCJGW_DownloadPDBImage::OpenDongleComPort(std::wstring strComName)
    {
        return mscp.OpenSerialComPort(strComName.c_str(),115200);
    }

    void CCJGW_DownloadPDBImage::CloseDongleComPort()
    {
        mscp.CloseSerialComPort();
    }

    bool CCJGW_DownloadPDBImage::InitPDBInfoToDongle()
    {
        std::string strRead;
        const char* szTemp = NULL;

        if (!mscp.ReadRespCommandToWriteCommand("fli\n",strRead))
        {
            std::cout << strRead.c_str() << std::endl; 
            return false;
        }
        std::cout << strRead.c_str() << std::endl; 
        szTemp = strstr(strRead.c_str(),"manufacturer_id");
        if (NULL == szTemp) return false;
        sscanf_s(szTemp,"manufacturer_id:%X",&mnManufacturerID);
        std::cout << "manufacturer_id: " << mnManufacturerID << std::endl;

        szTemp = strstr(strRead.c_str(),"device_size");
        if (NULL == szTemp) return false;
        sscanf_s(szTemp,"device_size:%X",&muFlashTotalSize);
        std::cout << "device_size: " << muFlashTotalSize << std::endl;

        if (mnManufacturerID <= 0 || 255 == mnManufacturerID) return false;

        GetPDBAddrToFlash(muFlashTotalSize);
        //! disable auto calibration incase enabled - Calibration will affect timing
        if (!IsCliReady() || !mscp.ReadRespCommandToWriteCommand("mib s 11-42-1-9 00 00 00 00\n",strRead))
        {
            return false;
        }
        std::cout << strRead.c_str() << std::endl; 
        return CheckComportResponce(strRead);
    }

    bool CCJGW_DownloadPDBImage::BuildOTPCRC32()
    {
        return GeneratedCRC32ToImageBuf(
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf,
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize - 4);
    }

    bool CCJGW_DownloadPDBImage::BuildConfigCRC32()
    {
        return GeneratedCRC32ToImageBuf(
            msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mszFileBuf,
            msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mnFileSize - 4);
    }

    bool CCJGW_DownloadPDBImage::BuildSNMacToFile(const char* szSN,const char* macAddr)
    {
        if (!msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf) return false;
        ChangeSNToImageFileBuf(szSN);
        ChangeMacToImageFileBuf(macAddr);

        if (!GeneratedCRC32ToImageBuf(
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf,
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize - 4))
        {
            return false;
        }

        std::string strOTPFile(PERSISTENT_DATABSE_0_OTP),strOTPHeaderFile(PERSISTENT_DATABSE_0_OTP_HEADER);
        JGW_WriteFileToBuf(strOTPFile.c_str(),(const char*)msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf,msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize);
        JGW_WriteFileToBuf(strOTPHeaderFile.c_str(),(const char*)msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mszFileBuf,msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mnFileSize);
        return true;
    }

    bool CCJGW_DownloadPDBImage::DownloadSNMacToDongleFlash(const char* szSN,const char* macAddr)
    {
        if (!msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf) return false;
        ChangeSNToImageFileBuf(szSN);
        ChangeMacToImageFileBuf(macAddr);
        
        if (!GeneratedCRC32ToImageBuf(
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf,
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize - 4))
        {
            return false;
        }
#if 1
        mscp.SetEndString();

        std::cout << "start erase and write " << PERSISTENT_DATABSE_0_OTP << std::endl;
        if (!EraseAndDonwloadImageFileToFlash(gvEraseFlash[E_OTP],msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP],msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP]))
            return false;
        std::cout << "Flash write complete" <<std::endl;


        std::cout << "start write " << PERSISTENT_DATABSE_0_OTP_HEADER << std::endl;
        if (!EraseAndDonwloadImageFileToFlash(gvEraseFlash[E_OTP_HEADER],msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP_HEADER],msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER]))
            return false;
        std::cout << "Flash write complete" <<std::endl;

#endif
        return true;
    }

    bool CCJGW_DownloadPDBImage::ReadSNMacToDongleFlash(std::string& strSN,std::string& strMAC)
    {
        std::string strRead;
        strSN = "";
        //! 读取FLASH信息
        if (!FlashReadFormattedBufferBlock(strRead,57,msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP]))
        {
            return false;
        }
        std::vector<size_t> vtemp;
        JGW_ParserIntAToHex(strRead.c_str()," ",vtemp);
        if (vtemp.size() < 57) return false;

        JGW_FormatString(strMAC,"%02X%02X%02X%02X%02X%02X",vtemp.at(MAC_ADDRESS_POS_TO_BIN)
            ,vtemp.at(MAC_ADDRESS_POS_TO_BIN + 1)
            ,vtemp.at(MAC_ADDRESS_POS_TO_BIN + 2)
            ,vtemp.at(MAC_ADDRESS_POS_TO_BIN + 3)
            ,vtemp.at(MAC_ADDRESS_POS_TO_BIN + 4)
            ,vtemp.at(MAC_ADDRESS_POS_TO_BIN + 5));
        
        if ('\0' == mstrChangeSN[0])
        {
            for (int i = 0;i < SERIALNUMBER_MAX_LEN ;i ++)
            {
                strSN += vtemp.at(SERIALNUMBER_POS_TO_BIN + i * 2);
            }
        }
        else
        {
            strSN = mstrChangeSN;
        }
        

        return true;
    }

    void CCJGW_DownloadPDBImage::ResetDongleDevice()
    {
        std::string strRead;
        mscp.ReadRespCommandToWriteCommand("reset\n",strRead);
    }

    bool CCJGW_DownloadPDBImage::GeneratedCRC32ToImageBuf(/*const*/ unsigned char* szBuf,size_t buf_len)
    {
        //! 计算当前文件缓冲区CRC 0xE11F1735
        UINT32 ucrc = CalcChecksum32ToImageBuf(szBuf,buf_len);
        if (0 == ucrc) return false;
        char szBuffer[9] = {0};
        unsigned char szCRC[10] = {0};

        sprintf_s(szBuffer,9,"%08X",ucrc);
        sscanf_s(szBuffer,"%02X%02X%02X%02X",&szCRC[0],&szCRC[1],&szCRC[2],&szCRC[3]);
        //! 后四位为CRC值，按照小端存储
        szBuf[buf_len] = szCRC[3];
        szBuf[buf_len + 1] = szCRC[2];
        szBuf[buf_len + 2] = szCRC[1];
        szBuf[buf_len + 3] = szCRC[0];
        printf("%02X%02X%02X%02X",szCRC[3],szCRC[2],szCRC[1],szCRC[0]);


        return true;
    }

    void CCJGW_DownloadPDBImage::ChangeSNToImageFileBuf(const char* szSN)
    {
        if (!szSN || !msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf) return;

        for (size_t i = 0;i < strlen(szSN);i ++)
        {
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf[SERIALNUMBER_POS_TO_BIN + i * 2] = szSN[i];
        }
        for (size_t i = strlen(szSN) * 2 - 1;i < SERIALNUMBER_MAX_BITS;i ++)
        {
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf[SERIALNUMBER_POS_TO_BIN + i] = '\0';
        }

        strcpy_s(mstrChangeSN,128,szSN);
    }
    //! 20CEC4001611
    void CCJGW_DownloadPDBImage::ChangeMacToImageFileBuf(const char* macAddr)
    {
        if (!macAddr || strlen(macAddr) != 12 || !msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf) return;

        unsigned char szMacAddr[14] = {0};
        sscanf_s(macAddr,"%02X%02X%02X%02X%02X%02X",&szMacAddr[0],&szMacAddr[1],&szMacAddr[2],&szMacAddr[3],&szMacAddr[4],&szMacAddr[5]);

        for (int i = 0;i < 6;i ++)
        {
            msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf[MAC_ADDRESS_POS_TO_BIN + i] = szMacAddr[i];
        }
    }

    bool CCJGW_DownloadPDBImage::EraseAndDonwloadImageFileToFlash(bool bErase,const UINT32 uFlashAddr,S_FILE_INFO& sFileInfo)
    {
        if (!sFileInfo.mszFileBuf) return false;
        bool bResult = true;

        if (bErase) bResult &= FlashErase(uFlashAddr,sFileInfo.mnFileSize);
        if (!bResult) return false;
        bResult &= FlashWriteVerify(&sFileInfo,uFlashAddr);

        return bResult;
    }

    bool CCJGW_DownloadPDBImage::DownloadPDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileTypes)
    {
        if (!msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileTypes].mszFileBuf 
            || !msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileTypes + 1].mszFileBuf)
        {
            return false;
        }
        if (E_OTP == eDonglesPDBFileTypes) mstrChangeSN[0] = '\0';
        mscp.SetEndString();
        bool bErase = true;

        for (int i = eDonglesPDBFileTypes;i < eDonglesPDBFileTypes + 2;i ++)
        {
            std::cout << "start erase and write " << gvszDonglesPDBFileNames[i] << std::endl;
            if (!EraseAndDonwloadImageFileToFlash(gvEraseFlash[i],msOTPConfigFiles.muDonglesFlashPDBAddr[i],msOTPConfigFiles.msDonglesPDBFileInfo[i]))
                return false;
            std::cout << "Flash write complete" <<std::endl;
        }

        return true;
    }

    bool CCJGW_DownloadPDBImage::DonwloadAllPDBImageFileToDongleFlash()
    {
        if (!msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mszFileBuf 
            || !msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP_HEADER].mszFileBuf
            || !msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mszFileBuf
            || !msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG_HEADER].mszFileBuf)
        {
            return false;
        }

        mscp.SetEndString();
        bool bErase = true;

        for (int i = E_CONFIG;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            std::cout << "start erase and write " << gvszDonglesPDBFileNames[i] << std::endl;
            if (!EraseAndDonwloadImageFileToFlash(gvEraseFlash[i],msOTPConfigFiles.muDonglesFlashPDBAddr[i],msOTPConfigFiles.msDonglesPDBFileInfo[i]))
                return false;
            std::cout << "Flash write complete" <<std::endl;
        }

        return true;
    }

    bool CCJGW_DownloadPDBImage::IsCliReady()
    {
        std::string strRead;
        return (mscp.ReadRespCommandToWriteCommand(" \n",strRead) && strRead.compare(">"));
    }

    void CCJGW_DownloadPDBImage::GetPDBAddrToFlash(UINT32 uFlashTotalSize)
    {
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP_HEADER] = uFlashTotalSize - SECTOR_SIZE;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP] = uFlashTotalSize - SECTOR_SIZE + 0x20;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG_HEADER] = uFlashTotalSize - SECTOR_SIZE * 2;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG] = uFlashTotalSize - SECTOR_SIZE * 2 + 0x20;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_FW_IMAGE] = 0x00;
    }

    bool CCJGW_DownloadPDBImage::FlashErase(UINT32 addr,UINT32 erase_len)
    {
        std::string command,strRead,strTemp = "a "FALCON_FLASH_MEM_BASE_STRING;
        //! erase all
        if (addr >= 0)
        {//! #round up to the nearest 256 
            if (erase_len > 0) erase_len += erase_len % 256;
            JGW_FormatString(strTemp,"b %X %04x",FALCON_FLASH_MEM_BASE + addr,erase_len);
        }
        JGW_FormatString(command,"erase %s\n",strTemp.c_str());
        std::cout << command.c_str() << std::endl;
#ifdef DEBUG_OPERATION_PROCEDURE
        if (!mscp.ReadRespCommandToWriteCommand(command.c_str(),strRead))
        {
            std::cout << "FlashErase: write erase command fail" << std::endl;
            return false;
        }

        if (!CheckComportResponce(strRead))
        {
            std::cout << "FlashErase: responce command fail:" << strRead.c_str() << std::endl;
            return false;
        }

        if (!IsCliReady())
        {
            std::cout << "Flash erase fail" << std::endl;
            return false;
        }

        std::cout << "Flash erase complete" << std::endl;

        return true;
#else
        return true;
#endif
    }

    bool CCJGW_DownloadPDBImage::CheckComportResponce(std::string& strRead)
    {
        if (atoi(strRead.c_str()) > 0) return false;

        if (strRead.empty())
        {
            return true;
        }

        if ('\n' == strRead.at(0))
        {
            return true;
        }    

        return false;
    }

    void CCJGW_DownloadPDBImage::VerifyWriteSizeDivisibleFour(size_t& write_size)
    {
        size_t write_size_temp = write_size;
        if (write_size < MAXCLIWRITE)
        {
            write_size = write_size - write_size % 4;
            if (0 == write_size)  write_size = write_size_temp;
        }
    }

    bool CCJGW_DownloadPDBImage::FlashWriteVerify(PS_FILE_INFO psFileInfo,UINT32 addr)
    {
        size_t file_size = psFileInfo->mnFileSize,write_size = 0, file_pos = 0,current_offset = addr;
        VerifyWriteSizeDivisibleFour(file_size);

        while (file_size > 0)
        {
            write_size = (file_size > MAXCLIWRITE)?MAXCLIWRITE:file_size;
            VerifyWriteSizeDivisibleFour(write_size);
            if (!FlashWriteBlock(psFileInfo->mszFileBuf + file_pos,write_size,current_offset)) return false;
            file_pos += write_size;
            file_size -= write_size;
            current_offset += write_size;
        }
        return true;
    }

    bool CCJGW_DownloadPDBImage::FlashReadFormattedBufferBlock(std::string& strRead,size_t read_len,UINT32 offest)
    {
        std::string strCommand;
        size_t addr = FALCON_FLASH_MEM_BASE + offest;
        //! 如果缓冲区长度整除4 则按4个字节读  否则按照按个字节读
        if (read_len % 4)
        {
            JGW_FormatString(strCommand,"mr b %08X %X\n",addr,read_len);
        }
        else
        {
            JGW_FormatString(strCommand,"mr l %08X %04X\n",addr,read_len / 4);
        }

        return mscp.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead);
    }

    bool CCJGW_DownloadPDBImage::FlashWriteBlock(const unsigned char* szFileBuf,size_t buf_len,size_t current_offset)
    {
        std::string strCommand,strRead;
        size_t addr = FALCON_FLASH_MEM_BASE + current_offset;
        //! 如果缓冲区长度整除4 则按4个字节写  否则按照按个字节写
        if (buf_len % 4)
        {
            JGW_FormatString(strCommand,"mw b %08X %X\n",addr,buf_len);
        }
        else
        {
            JGW_FormatString(strCommand,"mw l %08X %04X\n",addr,buf_len / 4);
        }

        std::cout << strCommand.c_str() << std::endl;
#ifdef DEBUG_OPERATION_PROCEDURE
        //! check out put command
        if (!mscp.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead) || !CheckComportResponce(strRead))
        {
            return false;
        }

        size_t nFormatBufLen =  Flash_Write_Formatted_Buffer(szFileBuf,buf_len);
        std::cout << mszWriteBuf << std::endl;
        std::cout << "write len :" << nFormatBufLen << std::endl;
        if (!mscp.WriteBinaryFileBufferToComPort(mszWriteBuf,nFormatBufLen) || !mscp.ReadResponseCommandToComPort(strRead) || !CheckComportResponce(strRead))
        {
            return false;
        }
#endif

        return true;
    }

    size_t CCJGW_DownloadPDBImage::Flash_Write_Formatted_Buffer(const unsigned char* buf,size_t buf_len)
    {
        memset(mszWriteBuf,0x00,MAX_WRITE_LEN + 1);
        size_t useBytes = buf_len % 4,nFormattedLen = 0;
        char szFormat[20] = {0};
        if (useBytes)
        {
            size_t nTemp = (buf_len / 4) * 4;
            for (size_t i = 0;i < nTemp;i += 4)
            {
                sprintf_s(szFormat,20,"%02X%02X%02X%02X ",buf[i + 3],buf[i + 2],buf[i + 1],buf[i + 0]);
                nFormattedLen += 9;
                strcat_s(mszWriteBuf,MAX_WRITE_LEN + 1,szFormat);
            }
            for (size_t i = buf_len;i > nTemp;i --)
            {
                sprintf_s(szFormat,20,"%02X ",buf[i - 1]);
                nFormattedLen += 3;
                strcat_s(mszWriteBuf,MAX_WRITE_LEN + 1,szFormat);
            }
        }
        else
        {
            for (size_t i = 0;i < buf_len;i += 4)
            {
                sprintf_s(szFormat,20,"%02X%02X%02X%02X ",buf[i + 3],buf[i + 2],buf[i + 1],buf[i + 0]);
                nFormattedLen += 9;
                strcat_s(mszWriteBuf,MAX_WRITE_LEN + 1,szFormat);
            }
        }

        return nFormattedLen;
    }

    UINT32 CCJGW_DownloadPDBImage::FormattedLittleEndianInt(const unsigned char* szBuf)
    {
        UINT32 word = 0;
        unsigned char* szBufUINT32 = (unsigned char *)&word;
        memcpy(szBufUINT32,szBuf,4);
        return word;
    }

    UINT32 CCJGW_DownloadPDBImage::CalcChecksum32ToImageBuf (const unsigned char* szBuf,size_t buf_len)
    {
        UINT64 sum = 0;
        size_t pos = 0;
        if (buf_len % 4 ) return 0;

        while (pos < buf_len)
        {
            sum += FormattedLittleEndianInt(szBuf + pos);
            pos += 4;
        }

        while (sum >> 32)
            sum = (sum & MASK_32BIT) + (sum >> 32);

        return ~sum & MASK_32BIT;
    }
}
