// JGW_BoxTestDataFile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <Shlwapi.h>
#include <poco/UnicodeConverter.h>
#include <poco/ASCIIEncoding.h>
#include <poco/UTF16Encoding.h>
#include <poco/TextConverter.h>
#include <poco/UTFString.h>
#include <poco/Latin1Encoding.h>

#pragma comment(lib,"Shlwapi.lib")
//JGW_BoxTestDataFile D:\Development\RFNET\TestLog\box D:\Development\RFNET\TestLog\box_wifi D:\Development\RFNET\TestLog\wifi_test
// 箱号SN信息路径
std::string gstrBoxSerialNumberInfoPath;
// 测试文件目录
std::string gstrBoxTestFileFolder;
// 保存箱号的测试文件目录
std::string gstrBoxTestFileSaveFolder;

std::string GetBoxNumberToFilePath(const std::string& strFilePath)
{
    std::string strFileName = JGW::JGW_GetFileNameToFilePathA(strFilePath.c_str());
    JGW::JGW_EraseAfterToRemoveChar(strFileName,'.');
    JGW::JGW_EraseLastAndFristTwoChars(strFileName,'.');
    return strFileName;
}

void GetBoxSerialNumbersToBoxFilePath(const std::string& strFilePath,std::vector<std::string>& vSerialNumbers)
{
    std::ifstream inputFile;
    inputFile.imbue( std::locale("") );
    inputFile.open(strFilePath);
    const int LINE_LENGTH = 1024 * 4; 
    char str[LINE_LENGTH] = {0}; 

    while (inputFile.getline(str,LINE_LENGTH))
    {
        if (NULL == JGW::JGW_StrCaseStr(str,"\t"))
        {
            std::string strTemp (str);
            JGW::JGW_EraseLastAndFristTwoChars(strTemp,' ');
            vSerialNumbers.push_back(strTemp);
        }
        else
        {
            std::vector<std::string> vTemp;
            JGW::JGW_ParserStrA(str,"\t",vTemp);
            for (size_t index = 0;index < vTemp.size();index++)
            {
                std::string strTemp (vTemp[index]);
                JGW::JGW_EraseLastAndFristTwoChars(strTemp,' ');
                vSerialNumbers.push_back(strTemp);
            }
        }
    }
}

std::vector<std::string> gvBoxDatas;
std::string strFilePathTemp;

void ScanBoxFile(const char* filepath,const char* filename,void* lpData)
{
    //! NULL != strstr(filename,"result") &&
    //if (NULL != strstr(filename,"P_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvBoxDatas.push_back(strFilePathTemp);
    }
}
std::vector<std::string> gvQSEQHtmlFile; 
void ScanWiFiTestResultFile(const char* filepath,const char* filename,void* lpData)
{
    //! NULL != strstr(filename,"result") &&
    if (NULL != strstr(filename,"P_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvQSEQHtmlFile.push_back(strFilePathTemp);
    }
}

std::string PareseSerialNumber(const std::string& fileName)
{
    int start = 0,end = 0;
    start = fileName.find('_');
    end = fileName.find('_',start + 1);
    if (start >= 0 && end > 0 && start != end)
    {
        return fileName.substr(start + 1,end - start - 1);
    }
    return "0000000000000";
}


void UTF16ToAssic(const std::wstring& strutf16String,std::string& strAssic)
{
    Poco::Latin1Encoding asciiEncoding;
    Poco::UTF16Encoding utf16Encoding;
    Poco::TextConverter converter(utf16Encoding,asciiEncoding);
    converter.convert(strutf16String.data(), (int) strutf16String.length() * sizeof(Poco::UTF16Char),strAssic);
}

std::wstring GetSingleToneTxCommandFormat(unsigned int channel,short txPower)
{
    unsigned char szLitteEndian[4] = {0};
    memcpy(szLitteEndian,&channel,4);

    short tx_power = txPower * 256;
    unsigned char szLitteEndianPower[2] = {0};
    memcpy(szLitteEndianPower,&tx_power,2);

    std::wstring strTemp;
    JGW::JGW_FormatWString(strTemp,L"L%02X%02X%02X%02X00D9%02X%02X",szLitteEndian[0],szLitteEndian[1],szLitteEndian[2],szLitteEndian[3],szLitteEndianPower[0],szLitteEndianPower[1]);

    return strTemp;
}

bool PareseSingleToneTxCommand(const std::wstring& strTxCommand,unsigned int& channel,short& txPower)
{
    if (17 != strTxCommand.size())
    {
        return false;
    }
    unsigned char szLitteEndian[5] = {0};
    unsigned char szLitteEndianPower[4] = {0};
#if 1
   
    for (int i = 0;i < 4;i ++)
    {
        szLitteEndian[i] = static_cast<unsigned char>(_tcstol(strTxCommand.substr(1 + i * 2,2).c_str(),NULL,16));
    }
    
    for (int i = 0;i < 2;i ++)
    {
        szLitteEndianPower[i] = static_cast<unsigned char>(_tcstol(strTxCommand.substr(13 + i * 2,2).c_str(),NULL,16));
    }
#else
    char prefix[4] = {0};
    int enffirx = 4;
    //! _stscanf(strTxCommand.c_str(),L"%c%2X%2X%2X%2X%4X%2X%2X",&prefix,&szLitteEndian[0],&szLitteEndian[1],&szLitteEndian[2],&szLitteEndian[3],&enffirx,&szLitteEndianPower[0],&szLitteEndianPower[1]);
    _stscanf(strTxCommand.c_str(),L"%c%08X%4X%04X",prefix,&channel,&enffirx,&txPower);
    szLitteEndian[3] = channel & 0xFF;
    szLitteEndian[2] = (channel & 0xFF00) >> 8;
    szLitteEndian[1] = (channel & 0xFF0000) >> 16;
    szLitteEndian[0] = (channel & 0xFF000000) >> 24;
    szLitteEndianPower[1] = (txPower & 0xFF);
    szLitteEndianPower[0] = (txPower & 0xFF00) >> 8;
#endif
    memcpy(&channel,szLitteEndian,4);
    memcpy(&txPower,szLitteEndianPower,2);
    txPower /= 256;

    return true;
}

std::wstring GetContinousRxCommandFormat(unsigned int rx_earfcn,short rx_power)
{
    unsigned char szLitteEndian[4] = {0};
    memcpy(szLitteEndian,&rx_earfcn,4);

    short rxPower = rx_power * 256;
    unsigned char szLitteEndianPower[2] = {0};
    memcpy(szLitteEndianPower,&rxPower,2);

    std::wstring strTemp;
    JGW::JGW_FormatWString(strTemp,L"L%02X%02X%02X%02XFF00%02X%02X",szLitteEndian[0],szLitteEndian[1],szLitteEndian[2],szLitteEndian[3],szLitteEndianPower[0],szLitteEndianPower[1]);

    return strTemp;
}

bool PareseATRxRssiResult(const std::wstring& strRxTestResult,short& rssi)
{
    unsigned char sz16littleEndian[2] = {0};
    if (9 != strRxTestResult.size())
    {
        return false;
    }
    for (int i = 0;i < 2;i ++)
    {
        sz16littleEndian[i] = static_cast<unsigned char>(_tcstol(strRxTestResult.substr(5 + i * 2,2).c_str(),NULL,16));
    }
    memcpy(&rssi,sz16littleEndian,2);
    rssi /= 256;
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::wstring strToneTxComman = GetSingleToneTxCommandFormat(25500,23);
    unsigned int channel;
    short txPower;
    PareseSingleToneTxCommand(strToneTxComman,channel,txPower);

    short rssi = -64;
    PareseATRxRssiResult(L"LB00000C4",rssi);

    strToneTxComman = GetContinousRxCommandFormat(2525,-60);


    std::wstring strUTF16Path = L"测试DEMO";
    std::string strUTF8Path =  "测试DEMO";
    UTF16ToAssic(strUTF16Path,strUTF8Path);

    Poco::UnicodeConverter::toUTF8(strUTF16Path, strUTF8Path);

    std::cout << strUTF8Path << std::endl;

    std::wstring strUTF16Path1;
    Poco::UnicodeConverter::toUTF16(strUTF8Path, strUTF16Path1);
    std::wcout << strUTF16Path1 << std::endl;
    return -1;












    if (argc < 4)
    {
        std::cout << "Invalid test parameters" << std::endl;
        return -1;
    }
    gstrBoxSerialNumberInfoPath = JGW::JGW_W2A(argv[1]);
    gstrBoxTestFileFolder = JGW::JGW_W2A(argv[2]);
    gstrBoxTestFileSaveFolder = JGW::JGW_W2A(argv[3]);
    int numFiles = 0;

    std::map<std::string,std::vector<std::string>> mapBoxNumberSerialNumberVector;
    if (PathIsDirectoryA(gstrBoxSerialNumberInfoPath.c_str()))
    {
        std::cout << "Start Scan Box Info  File ..." << std::endl;
        JGW::JGW_TraverseFolder_C(gstrBoxSerialNumberInfoPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanBoxFile);
        std::cout << "Scan Scan Box Info File Completed !!!" << std::endl;

        for (size_t index = 0;index < gvBoxDatas.size();index++)
        {
            std::string strBoxNumber = GetBoxNumberToFilePath(gvBoxDatas[index]);
            std::vector<std::string> vSerialNumber;
            GetBoxSerialNumbersToBoxFilePath(gvBoxDatas[index],vSerialNumber);
            mapBoxNumberSerialNumberVector[strBoxNumber] = vSerialNumber;
        }
    }
    else
    {
        std::string strBoxNumber = GetBoxNumberToFilePath(gstrBoxSerialNumberInfoPath);
        std::vector<std::string> vSerialNumber;
        GetBoxSerialNumbersToBoxFilePath(gstrBoxSerialNumberInfoPath,vSerialNumber);
        mapBoxNumberSerialNumberVector[strBoxNumber] = vSerialNumber;
    }


    std::cout << "Start Scan Wifi Test Result File ..." << std::endl;
    numFiles = 0;
    JGW::JGW_TraverseFolder_C(gstrBoxTestFileFolder,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanWiFiTestResultFile);
    std::cout << "Scan Scan Wifi Test Result File Completed !!!" << std::endl;
    std::map<std::string,std::string> mapSerialNumberFilePath;

    for (std::vector<std::string>::iterator it = gvQSEQHtmlFile.begin();
        it != gvQSEQHtmlFile.end();
        ++ it)
    {
        int start = it->rfind('\\');
        std::string strSerialNumber = PareseSerialNumber(it->substr(start + 1));
        std::cout << strSerialNumber << std::endl;
        mapSerialNumberFilePath[strSerialNumber] = it[0];
    }

    for (std::map<std::string,std::vector<std::string>>::iterator it = mapBoxNumberSerialNumberVector.begin();
        it != mapBoxNumberSerialNumberVector.end();
        ++ it)
    {
        std::string strBoxFolder = gstrBoxTestFileSaveFolder;
        strBoxFolder += "\\";
        strBoxFolder += it->first;
        strBoxFolder += "\\";
        JGW::JGW_CreateDirectoryA(strBoxFolder.c_str(),false);

        for (std::vector<std::string>::iterator itSN = it->second.begin();
            itSN != it->second.end();
            ++ itSN)
        {
            std::map<std::string,std::string>::iterator itSerialNumberFilePath = mapSerialNumberFilePath.find(itSN[0]);
            if (mapSerialNumberFilePath.end() == itSerialNumberFilePath)
            {
                std::cout << "Error Find SN Test File : " << itSN[0] << std::endl;
            }
            else
            {
                std::string strBoxTestFilePath (strBoxFolder);
                strBoxTestFilePath += JGW::JGW_GetFileNameToFilePathA(itSerialNumberFilePath->second.c_str());
                if (FALSE == CopyFileA(itSerialNumberFilePath->second.c_str(),strBoxTestFilePath.c_str(),true))
                {
                    std::cout << "Error Copy  SN Test File : " << itSN[0] << std::endl;
                }
            }
        }
    }

	return 0;
}

