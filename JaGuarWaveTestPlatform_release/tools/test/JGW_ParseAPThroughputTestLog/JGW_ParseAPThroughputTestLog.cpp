// JGW_ParseAPThroughputTestLog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <iomanip>
using namespace JGW;

typedef struct
{
    size_t mnTestTime;
    float mfTestSpeed;
}S_TEST_SPEED_INFO;

typedef struct
{
    S_TEST_SPEED_INFO msFristTestSpeedInfo; //! 首次速率测试
    S_TEST_SPEED_INFO msSecTestSpeedInfo; //! 第二次速率测试
    std::string mstrRSSI; //! rssi
}S_SERIAL_NUMBER_TEST_INFO;

std::string strFilePathTemp;
bool mbMultThread = true;
std::vector<std::string> gvQSEQHtmlFile; 
std::map<std::string,S_SERIAL_NUMBER_TEST_INFO> gmapThroughputTestResults;

void ScanPuamThroughputTestResultFile(const char* filepath,const char* filename,void* lpData)
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

//! 解析RSSI文件
void ParseRssiTestLog(S_SERIAL_NUMBER_TEST_INFO& sSerialNumberTestInfo,const std::string& strTestLogFilePath)
{
    std::ifstream inputFile;
    inputFile.imbue( std::locale("chs") );
    inputFile.open(strTestLogFilePath);
    const int LINE_LENGTH = 1024; 
    char str[LINE_LENGTH] = {0}; 
    while (inputFile.getline(str,LINE_LENGTH))
    {
        if (strlen(str) > 10)
        {
            sSerialNumberTestInfo.mstrRSSI = str;
        }
    }
}

bool HandIperfTestLog(const std::string& strIperfLog,std::string& strID,size_t& start,size_t& end,double& speed)
{
    if (std::string::npos != strIperfLog.find("sec") && std::string::npos != strIperfLog.find("s/sec") && strIperfLog[0] == '[')
    {
        int id = 0;
        id = strIperfLog.find(']',2) - 1;
        strID = strIperfLog.substr(1,id);
        start = atoi(strIperfLog.substr(id + 2).c_str());
        id = strIperfLog.find('-',id);
        end = atoi(strIperfLog.substr(id + 1).c_str());
        id = strIperfLog.find("s/sec",id + 1);
        id = strIperfLog.rfind('s',id - 1);
        speed = atof (strIperfLog.substr(id + 1).c_str());
        return true;
    }
    return false;
}

void ParseThroughputTestLog(S_SERIAL_NUMBER_TEST_INFO& sSerialNumberTestInfo,const std::string& strTestLogFilePath)
{
    std::ifstream inputFile;
    inputFile.imbue( std::locale("chs") );
    inputFile.open(strTestLogFilePath);
    const int LINE_LENGTH = 1024; 
    char str[LINE_LENGTH] = {0}; 

    std::string strID;
    size_t start = 0,end = 0;
    float fSpeed = 0.00;
    double speed = 0.00;

    while (inputFile.getline(str,LINE_LENGTH))
    {
        if (!HandIperfTestLog(str,strID,start,end,speed)) continue ;
        if (mbMultThread && NULL == strstr(str,"[SUM]")) continue ;

        if (0 == start && end - start >= 10)
        {
            fSpeed = static_cast<float>(speed);
            break;
        }
    }

    start = strTestLogFilePath.rfind('_');
    end = strTestLogFilePath.rfind('_',start - 1);
    size_t dwTime = atoi(strTestLogFilePath.substr(end + 1,start - end - 1).c_str());

    if (0 == sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime && 0 == sSerialNumberTestInfo.msSecTestSpeedInfo.mnTestTime)
    {
        sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime = dwTime;
        sSerialNumberTestInfo.msFristTestSpeedInfo.mfTestSpeed = fSpeed;
        return ;
    }
    //! 如果小于
    if (dwTime < sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime)
    {
        sSerialNumberTestInfo.msSecTestSpeedInfo.mnTestTime = sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime;
        sSerialNumberTestInfo.msSecTestSpeedInfo.mfTestSpeed = sSerialNumberTestInfo.msFristTestSpeedInfo.mfTestSpeed;
        sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime = dwTime;
        sSerialNumberTestInfo.msFristTestSpeedInfo.mfTestSpeed = fSpeed;
        return ;
    }

    //! 则将第二次的赋予给第一次
    if (dwTime > sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime)
    {
        if (sSerialNumberTestInfo.msSecTestSpeedInfo.mnTestTime > 0)
        {
            sSerialNumberTestInfo.msFristTestSpeedInfo.mnTestTime = sSerialNumberTestInfo.msSecTestSpeedInfo.mnTestTime;
            sSerialNumberTestInfo.msFristTestSpeedInfo.mfTestSpeed = sSerialNumberTestInfo.msSecTestSpeedInfo.mfTestSpeed;
        }
        sSerialNumberTestInfo.msSecTestSpeedInfo.mnTestTime = dwTime;
        sSerialNumberTestInfo.msSecTestSpeedInfo.mfTestSpeed = fSpeed;
        return;
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    std::string strPath = "D:\\ftp\\DHW247105_log\\FGSJ\\";
    std::string strTemp ("D:\\ftp\\log\\APS_THROUGHPUT\\2019-04-03\\LADK1912000018\\P_62232140_iperf.txt");
    int numFiles = 0,start,end;
    std::string strSerialNumber ;
    S_SERIAL_NUMBER_TEST_INFO* psSerialNumberTestInfo = NULL;

    std::cout << "Start Scan AP Throughput Test Result File ..." << std::endl;
    JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanPuamThroughputTestResultFile);
    std::cout << "Scan Scan AP Throughput Test Result File Completed !!!" << std::endl;
    //! 遍历文件
    for (std::vector<std::string>::iterator it = gvQSEQHtmlFile.begin();
        it != gvQSEQHtmlFile.end();
        ++ it)
    {
        start = it->rfind('\\');
        end = it->rfind('\\',start - 1);
        strSerialNumber = it->substr(end + 1,start - end - 1);
        if (gmapThroughputTestResults.end() == gmapThroughputTestResults.find(strSerialNumber))
        {
            psSerialNumberTestInfo = &gmapThroughputTestResults[strSerialNumber];
            psSerialNumberTestInfo->msFristTestSpeedInfo.mfTestSpeed = 0.00f;
            psSerialNumberTestInfo->msFristTestSpeedInfo.mnTestTime = 0;
            psSerialNumberTestInfo->msSecTestSpeedInfo.mfTestSpeed = 0.00f;
            psSerialNumberTestInfo->msSecTestSpeedInfo.mnTestTime = 0;
        }
        //! iperf 测试文件
        if (std::string::npos == it->find("_result"))
        {
            ParseThroughputTestLog(gmapThroughputTestResults[strSerialNumber],it[0]);
        }
        else //! rssi测试文件
        {
            ParseRssiTestLog(gmapThroughputTestResults[strSerialNumber],it[0]);
        }
    }

    std::ofstream outputFile;
    outputFile.imbue( std::locale("chs") );
    outputFile.open("D://output.csv",std::ios_base::out | std::ios_base::trunc );
    if (!outputFile.is_open()) return -1;

    for (std::map<std::string,S_SERIAL_NUMBER_TEST_INFO>::iterator it = gmapThroughputTestResults.begin();
        it != gmapThroughputTestResults.end();
        ++ it)
    {
        outputFile << it->first << ",";
        JGW_FormatString(strTemp,"%.2f,%.2f",it->second.msFristTestSpeedInfo.mfTestSpeed,it->second.msSecTestSpeedInfo.mfTestSpeed);
        outputFile << strTemp << "," << it->second.mstrRSSI << std::endl;
    }

    outputFile.flush();
    outputFile.close();

	return 0;
}

