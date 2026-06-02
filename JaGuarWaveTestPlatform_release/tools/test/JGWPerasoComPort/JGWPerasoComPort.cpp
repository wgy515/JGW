// JGWPerasoComPort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "CJGW_CSerialComPort.h"
#include "CJGW_DownloadPDBImage.h"

#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define PERASO_VID "VID_2932"
#define PERASO_SERVICE_NAME L"usbser"

std::wstring GetPerasoComPort()
{
    std::wstring strUsbPidVid;;
    std::wstring strComName;
    int nComPort = 0;
    size_t nCount = JGW::JGW_GetInsertUsbDeviceCountToServicesName(PERASO_SERVICE_NAME);
    if (nCount <= 0) return L"";

    for (size_t i = 0;i < nCount;i ++)
    {
        strUsbPidVid = JGW::JGW_GetUsbEnumPidVidToServiceName(PERASO_SERVICE_NAME,i);
        if (std::wstring::npos != strUsbPidVid.find(_T(PERASO_VID)) && JGW::JGW_GetComportToUsbPidVid(strUsbPidVid.c_str(),nComPort))
        {
            JGW::JGW_FormatWString(strComName,L"COM%d",nComPort);
            return strComName;
        }
    }

    return L"";
}

void help()
{
    std::cout << "-r            read sn and mac\n"
        << "-f              pdb image file path\n"
        << "-all            download config image and otp image\n"
        << "-config         download config image\n"
        <<"-otp             download opt image\n"
        <<"-sn              write sn\n"
        <<"-mac             write mac"
        << std::endl;
}


UINT32 checksum32 (UINT8 *data, unsigned int length) {
    UINT64 sum = 0;
    unsigned byte = 0;
    UINT32 word = 0;
    while (length > 0) {
        word |=*data++ << (byte * 8);
        if(++byte == 4) { // 4 bytes accumulated into a word
            sum += word;
            word = 0;
            byte = 0;
        }
        if(--length == 0) { // end of data, accumulate whatever we have left
            sum += word;
        }
    }
    // Now accumulate the carries
    while (sum > 0xffffffffULL) {
        sum = (sum & 0xffffffffULL) + (sum >> 32);
    }
    return UINT32(~sum);
}

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
    size_t file_size = 0;
    char* szFile = JGW::JGW_GetFileBufToPath_C("D:\\开发资料\\公司资料\\Release\\persistent_database_1_config.bin",file_size);


    UINT32 sum = checksum32((UINT8*)szFile,file_size - 4);

    JGW::CCJGW_DownloadPDBImage wctp;

    UINT32 sum1 = wctp.CalcChecksum32ToImageBuf((UINT8*)szFile,file_size - 4);

    return 0;
#endif
    std::string strRead,strOut,strDir = "D:\\开发资料\\公司资料\\Release\\",strSN,strMac;
    std::wstring strComPort = GetPerasoComPort();

    if (1 == argc)
    {
        JGW::CCJGW_CSerialComPort sp(">");

        if (strComPort.empty() || !sp.OpenSerialComPort(strComPort.c_str()))
        {
            std::cout << "not find comport.please check usb port" << std::endl;
            help();
            return -1;
        }

        while (1)
        {
            getline(std::cin,strOut);
            if (std::string::npos != strOut.find("QUIT") || std::string::npos != strOut.find("quit") || std::string::npos != strOut.find("exit") || std::string::npos != strOut.find("EXIT")) break;
            sp.ReadRespCommandToWriteCommand(strOut.c_str(),strRead);

            std::cout << strRead;
            std::cin.clear();
        }
        return -1;
    }
    else
    {
        JGW::CCJGW_DownloadPDBImage wctp;

        if (strComPort.empty() || !wctp.OpenDongleComPort(strComPort))
        {
            std::cout << "not find comport.please check usb port" << std::endl;
            help();
            return -1;
        }

        if (!wctp.InitPDBInfoToDongle())
        {
            std::cout << "Init PDB Info fail" << std::endl;
            help();
            return -1;
        }

        for (int i = 1;i < argc;i ++)
        {
            if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-r",argv[i]))
            {
                if (wctp.ReadSNMacToDongleFlash(strSN,strMac))
                {
                    std::cout << "Serial Number: " << strSN.c_str() << std::endl;
                    std::cout << "Mac Address: " << strMac.c_str() << std::endl;
                }
                else
                {
                    std::cout << "Read SerialNumber And Mac Address Fail" << std::endl;
                }
                return 0;
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-f",argv[i]))
            {
                if (i + 1 >= argc) return -1;
                i++;
                strDir = JGW::JGW_W2A_W(argv[i]);
                if (strDir.at(strDir.size() - 1) != '\\' && strDir.at(strDir.size() - 1) != '/')
                    strDir += "\\";
                wctp.LoadPDBImageFile(strDir);
            } 
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-all",argv[i]))
            {
                if (wctp.DonwloadAllPDBImageFileToDongleFlash())
                {
                    wctp.ResetDongleDevice();
                    std::cout << "Flash write complete\n";
                }
                else
                {
                    std::cout << "Flash write fail\n";
                }
                return 0;
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-fw",argv[i]))
            {
                if (wctp.DownloadPDBImageFileToDongleFlash(JGW::E_FW_IMAGE))
                {
                    wctp.ResetDongleDevice();
                    std::cout << "Flash write complete\n";
                }
                else
                {
                    std::cout << "Flash write fail\n";
                }
                return 0;
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-config",argv[i]))
            {
                if (wctp.DownloadPDBImageFileToDongleFlash(JGW::E_CONFIG))
                {
                    wctp.ResetDongleDevice();
                    std::cout << "Flash write complete\n";
                }
                else
                {
                    std::cout << "Flash write fail\n";
                }
                return 0;
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-otp",argv[i]))
            {
                if (wctp.DownloadPDBImageFileToDongleFlash(JGW::E_OTP))
                {
                    wctp.ResetDongleDevice();
                    std::cout << "Flash write complete\n";
                }
                else
                {
                    std::cout << "Flash write fail\n";
                }
                return 0;
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-sn",argv[i]))
            {
                if (i + 1 >= argc) return -1;
                i ++;
                strSN = JGW::JGW_W2A_W(argv[i]);
            }
            else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-mac",argv[i]))
            {
                if (i + 1 >= argc) return -1;
                i ++;
                strMac = JGW::JGW_W2A_W(argv[i]);
            }
            else
            {
                help();
                return -1;
            }
        }
        if (strMac.empty() && strSN.empty()) return -1;
        std::string strSrcSN,strSRCMac;

        if (strMac.empty() || strSN.empty())
        {
            if (wctp.ReadSNMacToDongleFlash(strSrcSN,strSRCMac))
            {
                std::cout << "src Serial Number: " << strSrcSN.c_str() << std::endl;
                std::cout << "src Mac Address: " << strSRCMac.c_str() << std::endl;
            }
            else
            {
                std::cout << "read sn and mac fail" << std::endl;
                return -1;
            }
        }
        if (strMac.empty()) strMac = strSRCMac;
        if (strSN.empty()) strSN = strSrcSN;

        if (wctp.DownloadSNMacToDongleFlash(strSN.c_str(),strMac.c_str()))
        {
            wctp.ResetDongleDevice();
            std::cout << "write Serial Number: " << strSN.c_str() << std::endl;
            std::cout << "write Mac Address: " << strMac.c_str() << std::endl;
            std::cout << "Flash write complete\n";
        }
        else
        {
            std::cout << "Flash write fail\n";
        }
    }

    return 0;
#if 0
//    unsigned char szBuf[5] = {0};
//       char szFormat[20] = "20CEC4001611";
//       unsigned char szMac[6] = {0};
// 
//       sscanf_s(szFormat,"%02X%02X%02X%02X%02X%02X",&szMac[0],&szMac[1],&szMac[2],&szMac[3],&szMac[4],&szMac[5]);
// 
//       return 0;
//    szBuf[0] = 0xDB;
//    szBuf[1] = 0x01;
//    szBuf[2] = 0x00;
//    szBuf[3] = 0x00;
//    UINT32 temp = FormattedLittleEndian(szBuf);
//
////     unsigned char szLittleEndian[5] = {0}/*{szBuf[3],szBuf[2],szBuf[1],szBuf[0]}*/;
////     szLittleEndian[0] = szBuf[0];
////     szLittleEndian[1] = szBuf[1];
////     szLittleEndian[2] = szBuf[2];
////     szLittleEndian[3] = szBuf[3];
//// 
////     sprintf_s(szFormat,20,"%02X%02X%02X%02X ",szBuf[3],szBuf[2],szBuf[1],szBuf[0]);
//// 
////     UINT32 len = 500;
////     unsigned char* szBufUINT32 = (unsigned char *)&len;
////     memcpy(szBufUINT32,szLittleEndian,4);
//    //memset(&len,)
//    //memcpy(szLittleEndian,&len,4);
//    //memcpy(&len,&szFormat[0],4);
//  //  sscanf_s(szFormat,"%X",&len);
//
//    return 0;
    
    JGW::CCJGW_DownloadPDBImage wctp;

    strComPort = GetPerasoComPort();

//     if (wctp.OpenDongleComPort(strComPort) && wctp.InitPDBInfoToDongle())
//         wctp.ReadSNMacToDongleFlash(strRead,strOut);
// 
//     return 0;
//     UINT32 crc = 0;
//     wctp.LoadOptConfigFile(strDir);
// 
//     crc = calcChecksum32(
//         wctp.msOTPConfigFiles.mspersistent_database_0_otp_header_file_info.mszFileBuf,
//         wctp.msOTPConfigFiles.mspersistent_database_0_otp_header_file_info.mnFileSize - 4);
// 
//     crc = calcChecksum32(
//         wctp.msOTPConfigFiles.mspersistent_database_0_otp_file_info.mszFileBuf,
//         wctp.msOTPConfigFiles.mspersistent_database_0_otp_file_info.mnFileSize - 4);
// 
//     crc = calcChecksum32(
//         wctp.msOTPConfigFiles.mspersistent_database_1_config_file_info.mszFileBuf,
//         wctp.msOTPConfigFiles.mspersistent_database_1_config_file_info.mnFileSize - 4);
// 
//     crc = calcChecksum32(
//         wctp.msOTPConfigFiles.mspersistent_database_1_config_header_file_info.mszFileBuf,
//         wctp.msOTPConfigFiles.mspersistent_database_1_config_header_file_info.mnFileSize - 4);
// 
    //     wctp.WriteSNAndMac("PER4161718010070","20CEC4001610");
    wctp.LoadPDBImageFile(strDir);
        if (
            wctp.OpenDongleComPort(strComPort)
            && wctp.InitPDBInfoToDongle()
            && wctp.DownloadSNMacToDongleFlash("PER4161718010070","20CEC4001610")
            )
        {
            wctp.ResetDongleDevice();
            wctp.CloseDongleComPort();
            return -1;
        }
        wctp.ResetDongleDevice();
        wctp.CloseDongleComPort();
        return 0;
    //     char szBuf[60] = {0},szBuf1[60] = {0};
    //     UINT32 mnFlashTotalSize = 10;
    //     UCHAR mnManufacturerID;
    //     const char* szTemp = NULL;
    //     //! read flash info
    //    strRead = "manufacturer_id: C2 device_id: 14 memory_code: 23 14 device_size: 00100000 block_size: 00010000 sector_size: 00001000 page_size: 00000100";
    // 
    //     szTemp = strstr(strRead.c_str(),"manufacturer_id");
    //     if (NULL == szTemp) return false;
    //     sscanf_s(szTemp,"manufacturer_id:%X",&mnManufacturerID);
    // 
    //     szTemp = strstr(strRead.c_str(),"device_size");
    //     if (NULL == szTemp) return false;
    //     sscanf_s(szTemp,"device_size:%X",&mnFlashTotalSize);
    // 
    // 
    //     return 0;
    return 0;
#endif
}

