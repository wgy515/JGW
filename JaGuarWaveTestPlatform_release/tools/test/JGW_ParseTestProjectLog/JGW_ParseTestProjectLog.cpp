// JGW_ParseTestProjectLog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <fstream>
using namespace JGW;

std::vector<std::string> gvstrCSVHeader;
std::vector<std::string> gvstrCSVValue;
std::map<std::string,std::vector<std::string>> gMapSerailNumberCsvConfig;

void LoadConfig()
{
    CCJGW_ConfigIni configIni;
    std::wstring strConfigIniPath;

    JGW_FormatWString(strConfigIniPath,L"%s%s",JGW_GetApplicationFolder(),L"TestProjectInfo.ini");
    configIni.InitIniFilePath(strConfigIniPath.c_str());

    std::string strCSVHeader = configIni.GetIniKeyValueA(L"Config",L"CSV_HEADER");
    std::string strCSVValue = configIni.GetIniKeyValueA(L"Config",L"CSV_VALUE_FIND");

    gvstrCSVValue.clear();
    gvstrCSVHeader.clear();
    JGW_ParserStrA(strCSVHeader.c_str(),",",gvstrCSVHeader);
    JGW_ParserStrA(strCSVValue.c_str(),",",gvstrCSVValue);

    if (gvstrCSVValue.size() != gvstrCSVHeader.size() || 0 == gvstrCSVHeader.size())
    {
        std::cout << "Config Error" << std::endl;
        exit(-1);
        return;
    }
}

std::vector<std::string> gvstrTestProjectLogFiles; 
std::string strFilePathTemp;
void ScanPassTestProjectLogFile(const char* filepath,const char* filename,void* lpData)
{
    //! NULL != strstr(filename,"result") &&
    if (NULL != strstr(filename,"P_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvstrTestProjectLogFiles.push_back(strFilePathTemp);
    }
}

void ScanFailTestProjectLogFile(const char* filepath,const char* filename,void* lpData)
{
    //! NULL != strstr(filename,"result") &&
    if (NULL != strstr(filename,"F_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvstrTestProjectLogFiles.push_back(strFilePathTemp);
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

void ParseTestProjectLogFile(const std::string& strTestProjectLogFilePath,std::vector<std::string>& vCsvValue)
{
    std::string strTestProjectLogFileName = JGW_GetFileNameToFilePathA(strTestProjectLogFilePath.c_str());
    bool bTestResult = (NULL != strstr(strTestProjectLogFileName.c_str(),"P_"));

    std::ifstream inputFile;
    inputFile.imbue( std::locale("") );
    inputFile.open(strTestProjectLogFilePath);

    const int LINE_LENGTH = 1024 * 4; 
    char str[LINE_LENGTH] = {0}; 

    for (size_t index = 0;index  < gvstrCSVValue.size();index++)
    {
        if (0 == gvstrCSVValue[index].compare("$$_RESULT_$$"))
        {
            vCsvValue.push_back(bTestResult ? "PASS" : "FAIL" );
        }
        else
        {
            vCsvValue.push_back("");
        }
    }

    while (inputFile.getline(str,LINE_LENGTH,'\r'))
    {
        std::string strTemp(str);
        for (size_t index = 0;index  < gvstrCSVValue.size();index++)
        {
            if (std::string::npos != strTemp.find(gvstrCSVValue[index]))
            {
                vCsvValue[index] = JGW_GetSubStrToStartEndStr(str,gvstrCSVValue[index],"  ");
            }
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::string strTestLogFolder,strOutputCSVPath,strSerialNumber;
    if (argc >= 2)
    {
        strTestLogFolder = JGW_W2A(argv[1]);
    }
    if (argc >= 3)
    {
        strOutputCSVPath = JGW_W2A(argv[2]);
    }
    LoadConfig();

    int numFiles = 0,start = 0;
    std::cout << "Start Scan Test Project Log File ..." << std::endl;
    JGW_TraverseFolder_C(strTestLogFolder,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanFailTestProjectLogFile);
    JGW_TraverseFolder_C(strTestLogFolder,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanPassTestProjectLogFile);
    std::cout << "Scan Scan Wifi Test Result File Completed !!!" << std::endl;

    for (std::vector<std::string>::iterator it = gvstrTestProjectLogFiles.begin();
        it != gvstrTestProjectLogFiles.end();
        ++ it)
    {
        start = it->rfind('\\');
        strSerialNumber = PareseSerialNumber(it->substr(start + 1));
        std::cout << strSerialNumber << std::endl;

        std::vector<std::string> vCsvValue;
        ParseTestProjectLogFile(it[0],vCsvValue);
        gMapSerailNumberCsvConfig[strSerialNumber] = vCsvValue;
    }

    std::ofstream outputFile;
    outputFile.imbue( std::locale("") );
    outputFile.open(strOutputCSVPath,std::ios_base::out | std::ios_base::trunc );
    if (!outputFile.is_open() || 0 == gMapSerailNumberCsvConfig.size()) return -1;

    std::string strTableHeader("SerialNumber");
    for (std::vector<std::string>::iterator it = gvstrCSVHeader.begin();
        it != gvstrCSVHeader.end();
        ++ it)
    {
        strTableHeader += ",";
        strTableHeader += it[0];
    }
    outputFile << strTableHeader << std::endl;


    for (std::map<std::string,std::vector<std::string>>::iterator it = gMapSerailNumberCsvConfig.begin();
        it != gMapSerailNumberCsvConfig.end();
        ++ it)
    {
        std::string strTableValue (it->first);
        for (std::vector<std::string>::iterator valIterator = it->second.begin();
            valIterator != it->second.end();
            ++ valIterator)
        {
            strTableValue += ",";
            strTableValue += valIterator[0];
        }
        outputFile << strTableValue << std::endl;
    }
    std::cout << "save file path :" << strOutputCSVPath << std::endl;

    outputFile.flush();
    outputFile.close();
	return 0;
}

