#include "StdAfx.h"
#include "CJGW_ExportPDBFile.h"
#include "PerasoDefine.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <fstream>

namespace JGW
{
    CCJGW_ExportPDBFile::CCJGW_ExportPDBFile(void)
    {
        for (int i = 0;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            msOTPConfigFiles.muDonglesFlashPDBAddr[i] = 0;
            msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf = NULL;
            msOTPConfigFiles.msDonglesPDBFileInfo[i].mnFileSize = NULL;
        }
        mszWriteBuf = (char*)calloc(MAX_WRITE_LEN + 1, 1);
        mszReadBuf = (char*)calloc(MAX_WRITE_LEN + 1, 1);
    }


    CCJGW_ExportPDBFile::~CCJGW_ExportPDBFile(void)
    {
        UnLoadPDBImageFile();
        if (mszWriteBuf) free(mszWriteBuf);
        if (mszReadBuf) free (mszReadBuf);
    }

    bool CCJGW_ExportPDBFile::LoadPDBImageFile(std::string& strFloder,std::string& strExportFolder)
    {
        std::string strTempFilePath;
        mstrExportFolder = strExportFolder;
        if (mstrExportFolder.empty()) return false;

        if (!JGW_IsPathSlashA(mstrExportFolder.at(mstrExportFolder.size() - 1)))
            mstrExportFolder += "\\";
        JGW_CreateDirectoryA(strExportFolder.c_str(),true);

        UnLoadPDBImageFile();
#if 0
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
#else
        for (size_t i = 0;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            JGW_FormatString(strTempFilePath,"%s%s",strFloder.c_str(),gvszDonglesPDBFileNames[i]);
            msOTPConfigFiles.msDonglesPDBFileInfo[i].mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strTempFilePath.c_str(),msOTPConfigFiles.msDonglesPDBFileInfo[i].mnFileSize);
        }
#endif

        return true;
    }

    void CCJGW_ExportPDBFile::UnLoadPDBImageFile()
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

    bool CCJGW_ExportPDBFile::OpenDongleComPort(std::wstring strComName)
    {
        return mscp.OpenSerialComPort(strComName.c_str(),115200);
    }

    void CCJGW_ExportPDBFile::CloseDongleComPort()
    {
        mscp.CloseSerialComPort();
    }

    bool CCJGW_ExportPDBFile::InitPDBInfoToDongle()
    {
        std::string strRead;
        const char* szTemp = NULL;
        mscp.SetEndString(">");

        if (!mscp.ReadRespCommandToWriteCommand("fli\n",strRead))
        {
            Log4AE(strRead.c_str());
            return false;
        }

        Log4AD(strRead.c_str());
        szTemp = strstr(strRead.c_str(),"manufacturer_id");
        if (NULL == szTemp) return false;
        sscanf_s(szTemp,"manufacturer_id:%X",&mnManufacturerID);
        Log4AI_F("manufacturer_id: %X",mnManufacturerID);

        szTemp = strstr(strRead.c_str(),"device_size");
        if (NULL == szTemp) return false;
        sscanf_s(szTemp,"device_size:%X",&muFlashTotalSize);
        Log4AI_F("device_size: %d",muFlashTotalSize);

        if (mnManufacturerID <= 0 || 255 == mnManufacturerID) return false;

        GetPDBAddrToFlash(muFlashTotalSize);
        //! disable auto calibration incase enabled - Calibration will affect timing
        if (!IsCliReady() || !mscp.ReadRespCommandToWriteCommand("mib s 11-42-1-9 00 00 00 00\n",strRead)) return false;

        Log4AD(strRead.c_str());

        return CheckComportResponce(strRead);
    }

    bool CCJGW_ExportPDBFile::GetOtpPDBImageSize()
    {
        std::string strDataSizePacketCmd = "",strRead;
        //! strCommand = "mr b F80E0008 4\n"
        JGW_FormatString(strDataSizePacketCmd,"mr b %08X 4\n",FALCON_FLASH_MEM_BASE + msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP_HEADER] + 8);
        if(!mscp.ReadRespCommandToWriteCommand(strDataSizePacketCmd.c_str(),strRead))
        {
            return false;
        }
        EraseCliString(strRead);
        std::vector<std::string> vData ;
        JGW_ParserStrA(strRead.c_str()," ",vData);
        if (vData.size() != 4)
        {
            return false;
        }

        std::string dataSizePacket = vData[3] + vData[2] + vData[1] + vData[0];
        size_t len = 0;
        //sscanf_s(dataSizePacket.c_str(),"%08X",&len);
        len = strtol(dataSizePacket.c_str(), NULL, 16);
        //msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG] = 
        msOTPConfigFiles.msDonglesPDBFileInfo[E_OTP].mnFileSize = len + 4;
        return true;
    }

    bool CCJGW_ExportPDBFile::GetConfigPDBImageSize()
    {
        std::string strDataSizePacketCmd = "",strRead;
        //! strCommand = "mr b F80E0008 4\n"
        JGW_FormatString(strDataSizePacketCmd,"mr b %08X 4\n",FALCON_FLASH_MEM_BASE + msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG_HEADER] + 8);
        if(!mscp.ReadRespCommandToWriteCommand(strDataSizePacketCmd.c_str(),strRead))
        {
            return false;
        }
        EraseCliString(strRead);
        std::vector<std::string> vData ;
        JGW_ParserStrA(strRead.c_str()," ",vData);
        if (vData.size() != 4)
        {
            return false;
        }

        std::string dataSizePacket = vData[3] + vData[2] + vData[1] + vData[0];
        size_t len = 0;
        //sscanf_s(dataSizePacket.c_str(),"%08X",&len);
        len = strtol(dataSizePacket.c_str(), NULL, 16);
        //msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG] = 
        msOTPConfigFiles.msDonglesPDBFileInfo[E_CONFIG].mnFileSize = len + 4;
        return true;
    }

    bool CCJGW_ExportPDBFile::IsCliReady()
    {
        std::string strRead;
        return (mscp.ReadRespCommandToWriteCommand(" \n",strRead) && strRead.compare(">"));
    }

    void CCJGW_ExportPDBFile::GetPDBAddrToFlash(UINT32 uFlashTotalSize)
    {
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP_HEADER] = uFlashTotalSize - SECTOR_SIZE;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_OTP] = uFlashTotalSize - SECTOR_SIZE + 0x20;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG_HEADER] = uFlashTotalSize - SECTOR_SIZE * 2;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_CONFIG] = uFlashTotalSize - SECTOR_SIZE * 2 + 0x20;
        msOTPConfigFiles.muDonglesFlashPDBAddr[E_FW_IMAGE] = 0x00;
    }

    bool CCJGW_ExportPDBFile::CheckComportResponce(std::string& strRead)
    {
        if (atoi(strRead.c_str()) > 0) return false;

        if (strRead.empty()){ return true;}

        if ('\n' == strRead.at(0))
        {
            return true;
        }    

        return false;
    }

    bool CCJGW_ExportPDBFile::FlashReadFormattedBufferBlock(std::string& strRead,size_t read_len,UINT32 offest)
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

    void CCJGW_ExportPDBFile::VerifyReadSizeDivisibleFour(size_t& write_size)
    {
        size_t write_size_temp = write_size;
        if (write_size < MAXCLIWRITE)
        {
            write_size = write_size - write_size % 4;
            if (0 == write_size)  write_size = write_size_temp;
        }
    }

    bool CCJGW_ExportPDBFile::ExportPDBAllFile()
    {
        for (int i = E_CONFIG;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            if (!ExportPDBFileToFileType((E_DONGLES_PDB_FILE_TYPE)i))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
                Log4AI_F("Export File(%s) Fail",gvszDonglesPDBFileNames[i]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                return false;
            }
        }
        return true;
    }

    bool CCJGW_ExportPDBFile::ExportPDBFileToFileType(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType)
    {
        if (!msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileType].mnFileSize)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            Log4AI_F("Warning Not Export File(%s)",gvszDonglesPDBFileNames[eDonglesPDBFileType]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            return true;
        }
        std::string strFileName = mstrExportFolder + gvszDonglesPDBFileNames[eDonglesPDBFileType];
        std::ofstream fileStream;
        {
            fileStream.imbue( std::locale("chs") );
            fileStream.open(strFileName.c_str(),std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
            if (!fileStream.is_open())
            {
                Log4WE_F(L"open %s file fail!!!",strFileName.c_str());
                return false;
            }
        }

        S_FILE_INFO* psFileInfo = &msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileType];
        size_t file_size = psFileInfo->mnFileSize,read_size = 0, file_pos = 0,current_offset = msOTPConfigFiles.muDonglesFlashPDBAddr[eDonglesPDBFileType];
        char read_buf[MAXCLIWRITE + 1] = {0};
        //! read file
        {
            VerifyReadSizeDivisibleFour(file_size);

            while (file_size > 0)
            {
                read_size = (file_size > MAXCLIWRITE)?MAXCLIWRITE:file_size;
                VerifyReadSizeDivisibleFour(read_size);
                if (!FlashReadBlock(read_buf,read_size,current_offset))
                {
                    Log4WE(L"Flash read Fail!!!");
                    return false;
                }
                fileStream.write(read_buf,read_size);
                file_pos += read_size;
                file_size -= read_size;
                current_offset += read_size;
            }
        }
        fileStream.flush();
        fileStream.close();

        Log4AI_F("Export File(%s) Ok !!!",strFileName.c_str());

        return true;
    }
    bool CCJGW_ExportPDBFile::FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf)
    {
        size_t a,b,c,d;
        sscanf_s(read_buf,"%02X%02X%02X%02X",&a,&b,&c,&d);
        file_buf[3] = a;
        file_buf[2] = b;
        file_buf[1] = c;
        file_buf[0] = d;
        return true;
    }

    void CCJGW_ExportPDBFile::FormattedReadLittleEndianBit(unsigned char* file_buf,const char* read_buf)
    {
        unsigned char a;
        sscanf_s(read_buf,"%02X",&a);
        file_buf[0] = a;
    }
    //! 56
    bool CCJGW_ExportPDBFile::FlashReadBlock(char* file_buf,size_t read_size,UINT32 offest)
    {
        std::string strRead;
        if (!FlashReadFormattedBufferBlock(strRead,read_size,offest)) return false;

        if (L'>' == strRead.at(strRead.size() - 1)) strRead.erase(strRead.size() - 2);


        if (read_size % 4) //! 按位读取
        {
            std::vector<std::string> vstrRead;
            JGW_ParserStrA(strRead.c_str()," ",vstrRead);
            if (read_size != vstrRead.size())
            {
                Log4WE(L"read len error !!!");
                return false;
            }

            for (size_t i = 0;i < vstrRead.size();i ++)
            {
                FormattedReadLittleEndianBit((unsigned char*)(file_buf + i * 4),vstrRead.at(i).c_str());
            }

        }
        else //! 按字节读取
        {
            std::vector<std::string> vstrRead;
            JGW_ParserStrA(strRead.c_str()," ",vstrRead);

            if (read_size / 4 != vstrRead.size())
            {
                Log4WE(L"read len error !!!");
                return false;
            }

            for (size_t i = 0;i < vstrRead.size();i ++)
            {
                FormattedReadLittleEndianBlock((unsigned char*)(file_buf + i * 4),vstrRead.at(i).c_str());
            }
        }
        return true;
    }

    bool CCJGW_ExportPDBFile::ComparePDBAllFile()
    {
        for (int i = E_CONFIG;i < E_MAX_DONGLES_PDB_FILE;i ++)
        {
            if (!ComparePDBFileToFileType((E_DONGLES_PDB_FILE_TYPE)i))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
                Log4AI_F("Compare File(%s) Fail",gvszDonglesPDBFileNames[i]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
        }
        return true;
    }

    bool CCJGW_ExportPDBFile::ComparePDBFileToFileType(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType)
    {
        if (0 == msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileType].mnFileSize)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            Log4AI_F("Warning %s Not Compare !!!",gvszDonglesPDBFileNames[eDonglesPDBFileType]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            return true;
        }
        std::string strFileName = mstrExportFolder + gvszDonglesPDBFileNames[eDonglesPDBFileType];
        S_FILE_INFO sFileInfo;
        sFileInfo.mszFileBuf = (unsigned char*)JGW_GetFileBufToPath_C(strFileName.c_str(),sFileInfo.mnFileSize);

        if (sFileInfo.mnFileSize != msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileType].mnFileSize)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
            Log4AI_F("src:dest(%s) file size not equal Fail !!!",gvszDonglesPDBFileNames[eDonglesPDBFileType]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            return false;
        }

        if (NULL != memcmp(sFileInfo.mszFileBuf,msOTPConfigFiles.msDonglesPDBFileInfo[eDonglesPDBFileType].mszFileBuf,sFileInfo.mnFileSize))
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
            Log4AI_F("src:dest(%s) file not same Fail !!!",gvszDonglesPDBFileNames[eDonglesPDBFileType]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            return false;
        }

        Log4AI_F("src:dest(%s) file is same Ok !!!",gvszDonglesPDBFileNames[eDonglesPDBFileType]);

        return true;
    }

    bool CCJGW_ExportPDBFile::PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount)
    {
        std::vector<std::string> vstrRead;
        JGW_ParserStrA(read_buf," ",vstrRead);
        if (vstrRead.size() != mnMaxBlockCount)
        {
            Log4AE("read buf error !!!");
            return false;
        }

        for (size_t i = 0;i < vstrRead.size();i ++)
        {
            FormattedReadLittleEndianBlock((unsigned char*)(output_buf + i * 4),vstrRead.at(i).c_str());
        }
        return true;
    }

    bool CCJGW_ExportPDBFile::PareseDonglesReadBitBuf(unsigned char* output_buf,const char* read_buf,size_t nMaxBitCount)
    {
        std::vector<std::string> vstrRead;
        JGW_ParserStrA(read_buf," ",vstrRead);
        if (vstrRead.size() != nMaxBitCount)
        {
            Log4AE("read buf error !!!");
            return false;
        }

        for (size_t i = 0;i < vstrRead.size();i ++)
        {
            FormattedReadLittleEndianBit((unsigned char*)(output_buf + i * 4),vstrRead.at(i).c_str());
        }
        return true;
    }

    void CCJGW_ExportPDBFile::EraseCliString(std::string& strRead)
    {
        while (!strRead.empty() && L'\n' == strRead[0]) strRead.erase(0,1);
        if (strRead.empty()) return ;
        char temp = strRead.at(strRead.size() - 1);

        while (!strRead.empty() && (temp == '\n' || '>' == temp || '\r' == temp))
        {
            strRead.erase(strRead.size() - 1,1);
            if (strRead.empty()) return ;
            temp = strRead.at(strRead.size() - 1);
        }
        // while (!strRead.empty() && L'>' == strRead[strRead.size() - 1]) strRead.erase(strRead.size() - 2);
    }

//     void CCJGW_ExportPDBFile::EraseCliString(std::string& strRead)
//     {
//         if (strRead.empty()) return ;
//         while (!strRead.empty() && L'\n' == strRead[0]) strRead.erase(0,1);
//         while (!strRead.empty() && L'>' == strRead[strRead.size() - 1]) strRead.erase(strRead.size() - 2);
//     }

    bool CCJGW_ExportPDBFile::ReadSerialNumber(std::wstring& strSerialNumber)
    {
        std::string strSerialNumberCommand = "mib q l 60-1-1-2\n",strRead;
        mscp.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        EraseCliString(strRead);

        unsigned char szBuf[MAX_SERIAL_NUMBER_FALSH_BIT + 1] = {0};
        if (!PareseDonglesReadBlockBuf(szBuf,strRead.c_str(),MAX_SERIAL_NUMBER_FALSH_BIT / 4)) return false;
        strSerialNumber = (wchar_t*)szBuf;

        return true;
    }


    bool CCJGW_ExportPDBFile::ReadMacAddress(std::wstring& strMacAddress)
    {
        std::string strSerialNumberCommand = "mib q b 60-1-1-1\n",strRead;
        mscp.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        EraseCliString(strRead);

        JGW_ReplaceStringA(strRead," ","");
        if (MAC_ADDRESS_MAX_LEN != strRead.length()) return false;
        strMacAddress = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_ExportPDBFile::ReadSerialNumberToBoardInfo(std::wstring& strSerialNumber)
    {
        std::string strSerialNumberCommand = "bdi\n",strRead;
        mscp.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        EraseCliString(strRead);

        const char* pRead = strRead.c_str(),*pTemp,*pEnd;
        pTemp = strstr(strRead.c_str(),"serial_number");
        if (!pTemp) return false;
        size_t start_pos = pTemp - pRead;
        pEnd = strstr(pTemp,"\n");
        if (!pEnd) pEnd = pTemp + strRead.length();
        std::string serial_number = strRead.substr(start_pos,pEnd - pTemp);

        JGW_ReplaceStringA(serial_number," ","");
        JGW_ReplaceStringA(serial_number,"=","");
        JGW_ReplaceStringA(serial_number,"serial_number","");
        strSerialNumber = JGW_A2W(serial_number);
        return true;
    }

    bool CCJGW_ExportPDBFile::ReadMacAddressToBoardInfo(std::wstring& strMacAddress)
    {
        std::string strSerialNumberCommand = "bdi\n",strRead;
        mscp.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        EraseCliString(strRead);

        const char* pRead = strRead.c_str(),*pTemp,*pEnd;
        pTemp = strstr(strRead.c_str(),"mac_addr");
        if (!pTemp) return false;
        size_t start_pos = pTemp - pRead;
        pEnd = strstr(pTemp,"\n");
        if (!pEnd) pEnd = pRead + strRead.length();
        std::string serial_number = strRead.substr(start_pos,pEnd - pTemp);

        JGW_ReplaceStringA(serial_number," ","");
        JGW_ReplaceStringA(serial_number,"=","");
        JGW_ReplaceStringA(serial_number,":","");
        JGW_ReplaceStringA(serial_number,"mac_addr","");
        strMacAddress = JGW_A2W(serial_number);
        return true;
    }
}