// TG28ConfigBin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>

#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

std::wstring gstrConfigTxtFilePath;
std::wstring gstrConfigBinSavePath;
size_t gnBinBufferLength = 2048;

typedef unsigned int   uint32_t;
typedef unsigned long long uint64_t;

uint32_t CheckSum32 (unsigned char *data, int len) { 
    uint64_t sum = 0; 
    len = len / sizeof(uint32_t); 
    while (len > 0) { 
        sum += *((uint32_t *)data); 
        data += sizeof(uint32_t); 
        --len; 
    } 
    while (sum > 0xffffffffULL) { 
        sum = (sum & 0xffffffffULL) + (sum >> 32); 
    } 
    return (uint32_t)(~sum); 
} 

int _tmain(int argc, _TCHAR* argv[])
{
    unsigned char *pBinBuf = NULL;
    if (argc > 1)
    {
        gstrConfigTxtFilePath = JGW::JGW_RealativePathToAbsPath(argv[1]);
        std::wcout << L"Config Text Path:" << gstrConfigTxtFilePath << std::endl;
    }

    if (argc > 2)
    {
        gstrConfigBinSavePath = JGW::JGW_RealativePathToAbsPath(argv[2]);
        std::wcout << L"Save Config Bin Path:" << gstrConfigBinSavePath << std::endl;
    }
    else
    {
        gstrConfigBinSavePath = JGW::JGW_GetFileNameToCurrentTimeStamp();
        gstrConfigBinSavePath += L"_config.bin";
        gstrConfigBinSavePath = JGW::JGW_RealativePathToAbsPath(gstrConfigBinSavePath.c_str());
    }
    if (argc > 3)
    {
        gnBinBufferLength = _ttoi(argv[2]);
        gnBinBufferLength = gnBinBufferLength < 1024 ? 2048 : gnBinBufferLength;
    }
    pBinBuf = new unsigned char[gnBinBufferLength];
    memset(pBinBuf,0x00,gnBinBufferLength);

    if (!JGW::JGW_FileExistsToFilePath(gstrConfigTxtFilePath.c_str()))
    {
        std::wcout << L"File does not exist : " << gstrConfigTxtFilePath << std::endl;
        return -1;
    }

    {
        std::ifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(JGW::JGW_W2A(gstrConfigTxtFilePath));
        const int LINE_LENGTH = 2048; 
        char str[LINE_LENGTH] = {0};
        if (!inputFile.is_open()) return -1;

        std::ofstream outputFile;
        outputFile.imbue( std::locale(""));
        outputFile.open(JGW::JGW_W2A(gstrConfigBinSavePath),std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
        if (!outputFile.is_open()) return -1;

        unsigned char buf[1024] = {0};
        size_t bufPos = 0;
#if 0
        while (inputFile.getline(str,LINE_LENGTH))
        {
            std::vector<std::string> vstrTem;
            JGW::JGW_ParserStrA(str," ",vstrTem);
            if (vstrTem.size() >= 6)
            {
                size_t index = 0;
                memset(buf,0x00,1024);
                for (;index < vstrTem.size();index++)
                {
                    buf[index] = (unsigned char)strtol(vstrTem[index].c_str(),NULL,16);
                }

                if (buf[0] == 0xFF && (0xFE == buf[1] || 0xFD == buf[1])
                    && 0x0A == buf[index - 1] && 0xFF == buf[index - 2] && 0xFE ==buf[index - 3])
                {
                    if (bufPos + index > gnBinBufferLength - 4)
                    {
                        outputFile.flush();
                        outputFile.close();
                        JGW::JGW_DeleteFile(gstrConfigBinSavePath);
                        std::wcout << L"The file length exceeds the range..." << std::endl;
                        return -1;
                    }

                    for (size_t i = 0;i < index;i ++)
                    {
                        pBinBuf[bufPos + i] = buf[i];
                    }
                    bufPos += index;
                }
            }
        }
#else
        /*
        typedef struct SParameterConfigurationCommandInfo
        {
            // 起始帧 0xFF 0xFF
            uint16_t start_frame;
            // 命令ID
            uint8_t command_id;
            // 命令长度
            uint16_t command_length;
            // 命令缓冲区内容
            uint8_t *command_buffer;
            // 末尾帧 0xFF 0xFF
            uint16_t end_frame;
            //
            SParameterConfigurationCommandInfo *pNextCommandInfo;
        }SParameterConfigurationCommandInfo;
        */
        const char* pszCommandID = NULL;
        unsigned char command_id = 0;
        uint32_t command_pos = 0;
        uint32_t command_length = 0;
        pBinBuf[bufPos++] = 0xFF;
        pBinBuf[bufPos++] = 0xFF;
        pBinBuf[bufPos++] = command_id;
        // 命令数据量大小
        command_pos = bufPos;
        pBinBuf[bufPos++] = 00;
        pBinBuf[bufPos++] = 00;
        
#define COMMAND_ID_FLAG  "COMMAND_ID : "

        while (inputFile.getline(str,LINE_LENGTH))
        {
            pszCommandID = JGW::JGW_StrCaseStr(str,COMMAND_ID_FLAG);
            if (pszCommandID != NULL)
            {
                command_id = atoi(pszCommandID + strlen(COMMAND_ID_FLAG)); 
                // 命令数据量长度
                pBinBuf[command_pos] = command_length & 0xFF;
                pBinBuf[command_pos + 1] = (command_length >> 8) & 0xFF;
                // end frame
                pBinBuf[bufPos++] = 0xFF;
                pBinBuf[bufPos++] = 0xFF;
                // strat frame
                pBinBuf[bufPos++] = 0xFF;
                pBinBuf[bufPos++] = 0xFF;
                pBinBuf[bufPos++] = command_id;
                command_length = 0;
                command_pos = bufPos;
                pBinBuf[bufPos++] = 00;
                pBinBuf[bufPos++] = 00;
            }

            std::vector<std::string> vstrTem;
            JGW::JGW_ParserStrA(str," ",vstrTem);
            if (vstrTem.size() >= 6)
            {
                size_t index = 0;
                memset(buf,0x00,1024);
                for (;index < vstrTem.size();index++)
                {
                    buf[index] = (unsigned char)strtol(vstrTem[index].c_str(),NULL,16);
                }

                if (buf[0] == 0xFF && (0xFE == buf[1] || 0xFD == buf[1])
                    && 0x0A == buf[index - 1] && 0xFF == buf[index - 2] && 0xFE ==buf[index - 3])
                {
                    if (bufPos + index > gnBinBufferLength - 4)
                    {
                        outputFile.flush();
                        outputFile.close();
                        JGW::JGW_DeleteFile(gstrConfigBinSavePath);
                        std::wcout << L"The file length exceeds the range..." << std::endl;
                        return -1;
                    }
                    
                    //pBinBuf[bufPos++] = index & 0xFF;
                    //pBinBuf[bufPos++] = (index >> 8) & 0xFF;
                    command_length += index;
                    for (size_t i = 0;i < index;i ++)
                    {
                        pBinBuf[bufPos++] = buf[i];
                    }
                }
            }
        }
#endif
        // 命令数据量长度
        pBinBuf[command_pos] = command_length & 0xFF;
        pBinBuf[command_pos + 1] = (command_length >> 8) & 0xFF;
        // end frame
        pBinBuf[bufPos++] = 0xFF;
        pBinBuf[bufPos++] = 0xFF;
        // crc
        uint32_t crc = CheckSum32(pBinBuf,bufPos);
        pBinBuf[bufPos++] = crc & 0xFF;
        pBinBuf[bufPos++] = (crc >> 8) & 0xFF;
        pBinBuf[bufPos++] = (crc >> 16) & 0xFF;
        pBinBuf[bufPos++] = (crc  >> 24) & 0xFF;

        outputFile.write((char*)&bufPos,sizeof(size_t));
        outputFile.write((char*)pBinBuf,gnBinBufferLength - 4);
        for (size_t index = 0;index < bufPos;index++)
        {
            printf("0x%02X,",pBinBuf[index]);
        }
        std::wcout << std::endl;
        //uint32_t crc = CheckSum32(pBinBuf,bufPos);
        //outputFile.write((char*)&crc,sizeof(uint32_t));

        inputFile.close();
        outputFile.flush();
        outputFile.close();
        std::wcout << L"File generated successfully : " << gstrConfigBinSavePath << std::endl;
    }
    return 0;
}

