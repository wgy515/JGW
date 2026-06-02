// JGW_HtmlTestLogParse.cpp : 定义控制台应用程序的入口点。


#include "stdafx.h"
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

using namespace JGW;
std::string strFilePathTemp;
bool gbFristContent = true; //! 首行  不取数据
std::vector<std::string> gvQSEQHtmlFile; 
std::vector<std::string> gvLTERowName;
std::vector<std::string> gvWCDMARowName;
std::vector<std::string> gvTDSRowName;

enum E_SYS_
{
    E_LTE = 0,
    E_TDS,
    E_WCDMA
};

void ScanPuamThroughputTestResultFile(const char* filepath,const char* filename,void* lpData)
{
    if (NULL != strstr(filename,"result") && NULL != strstr(filename,"P_"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvQSEQHtmlFile.push_back(strFilePathTemp);
    }
}

void ScanPuamTestResultFile(const char* filepath,const char* filename,void* lpData)
{
    if (NULL != strstr(filename,"testresult"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvQSEQHtmlFile.push_back(strFilePathTemp);
    }
}

void ScanHtmlFile(const char* filepath,const char* filename,void* lpData)
{
    if (NULL != strstr(filename,"QSEQ"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvQSEQHtmlFile.push_back(strFilePathTemp);
        //! std::cout << strFilePathTemp << std::endl;
    }
    //! std::cout << filepath << "\\" << filename << std::endl;
}

//! SN LTE_36300	LTE_36275	TDS_10054	TDS_10088	WCDMA_9888
std::ofstream outputFile;

//! 过滤掉不需要保存的Rules
struct S_TEST_NAME_ROW_RULES 
{
    size_t mnPos; //! 0
    //! 50@0_22.0dBm_QPSK_PUSCH  当前列名称里面有类似字符串  模糊查找
    std::string mstrPosString; 
};

struct S_TEST_NAME_RULES
{
    E_SYS_ meSys;
    size_t mnTestDataTablePos;
    std::string mstrTestName;
    std::vector<S_TEST_NAME_ROW_RULES> mvRowRules;
};

struct S_SAVE_ROW_CONTENCT 
{
    std::string mstrSN;//! SN号段
    
    std::vector<std::string> mstrRowValue;  
};

struct S_TEST_NAME_TABLE_DATAS
{
    std::vector<std::vector<std::string>> mvRowData;
};

S_TEST_NAME_RULES* gpsTestNameRules;

std::string GetTDTextValue(const char* szTDText)
{
    int start,end;
    std::string strTemp = szTDText;

    start = strTemp.find('<');
    strTemp.erase(0,start);
    while (std::string::npos != start)
    {
        end = strTemp.find('>');
        strTemp.erase(start,end - start + 1);
        start = strTemp.find('<');
    }

    return strTemp;
}

void ParseSysTableNode(S_TEST_NAME_TABLE_DATAS& sTestNameTableDatas)
{
    if (E_LTE == gpsTestNameRules->meSys)
    {
        if (gbFristContent)
        {
            for (size_t i = 0;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                if (std::string::npos != sTestNameTableDatas.mvRowData[i][0].find("50@0_22.0dBm_QPSK_PUSCH"))
                {
                    std::string strTemp = sTestNameTableDatas.mvRowData[i][0];
                    gvLTERowName.push_back(sTestNameTableDatas.mvRowData[i][0]);
                    strTemp = sTestNameTableDatas.mvRowData[i][0].substr(0,sTestNameTableDatas.mvRowData[i][0].find("_"));
                    outputFile << "LTE_" << strTemp << ",";
                }
            }
        }
        else
        {
            std::vector<std::string> vValue;
            for (size_t i = 0;i < gvLTERowName.size();i ++) vValue.push_back("NA");
            for (size_t i = 1;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                for (size_t x = 0;x < gvLTERowName.size();x ++)
                {
                     if (NULL == gvLTERowName[x].compare(sTestNameTableDatas.mvRowData[i][0]))
                     {
                         vValue[x] = sTestNameTableDatas.mvRowData[i][4];
                     }
                }
            }

            for (size_t i = 0;i < gvLTERowName.size();i ++)
            {
                outputFile << vValue[i] << ",";
            }
        }
    }
    else if (E_WCDMA == gpsTestNameRules->meSys)
    {
        if (gbFristContent)
        {
            for (size_t i = 0;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                if (std::string::npos != sTestNameTableDatas.mvRowData[i][5].find("PRI"))
                {
                    gvWCDMARowName.push_back(sTestNameTableDatas.mvRowData[i][4]);
                    outputFile << "W_" << sTestNameTableDatas.mvRowData[i][4] << ",";
                }
            }
        }
        else
        {
            std::vector<std::string> vValue;
            for (size_t i = 0;i < gvWCDMARowName.size();i ++) vValue.push_back("NA");
            for (size_t i = 1;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                if (std::string::npos == sTestNameTableDatas.mvRowData[i][5].find("PRI")) continue;
                for (size_t x = 0;x < gvWCDMARowName.size();x ++)
                {
                    if (NULL == gvWCDMARowName[x].compare(sTestNameTableDatas.mvRowData[i][4]))
                    {
                        vValue[x] = sTestNameTableDatas.mvRowData[i][11];
                    }
                }
            }

            for (size_t i = 0;i < gvWCDMARowName.size();i ++)
            {
                outputFile << vValue[i] << ",";
            }
//             std::vector<std::string> vValue;
//             size_t x = 0;      
//             for (size_t i = 1;i < sTestNameTableDatas.mvRowData.size();i ++)
//             {
//                 while (x < gvWCDMARowName.size() && std::string::npos != sTestNameTableDatas.mvRowData[i][5].find("PRI"))
//                 {
//                     if (NULL == gvWCDMARowName[x].compare(sTestNameTableDatas.mvRowData[i][4]))
//                     {
//                         if (x == 0 || x == vValue.size() - 1) vValue.push_back(sTestNameTableDatas.mvRowData[i][11]);
//                         else
//                             vValue.at(x) = (sTestNameTableDatas.mvRowData[i][11]);
//                         break;
//                     }
//                     else
//                     {
//                         x ++;
//                         if (x > vValue.size()) vValue.push_back("NA");
//                     }              
//                 }
//             }
//             if (vValue.size() != gvWCDMARowName.size()) for (size_t i = 0;i < gvWCDMARowName.size();i ++) vValue.push_back("NA");
//             for (size_t i = 0;i < gvWCDMARowName.size();i ++)
//             {
//                 outputFile << vValue[i] << ",";
//             }
        }
    }
    else if (E_TDS == gpsTestNameRules->meSys)
    {
        if (gbFristContent)
        {
            for (size_t i = 0;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                if (std::string::npos != sTestNameTableDatas.mvRowData[i][10].find("PRIM"))
                {
                    gvTDSRowName.push_back(sTestNameTableDatas.mvRowData[i][6]);
                    outputFile << "TDS_" << sTestNameTableDatas.mvRowData[i][6] << ",";
                }
            }
        }
        else
        {
            std::vector<std::string> vValue;
            for (size_t i = 0;i < gvTDSRowName.size();i ++) vValue.push_back("NA");
            for (size_t i = 1;i < sTestNameTableDatas.mvRowData.size();i ++)
            {
                if (std::string::npos == sTestNameTableDatas.mvRowData[i][10].find("PRIM")) continue;
                for (size_t x = 0;x < gvTDSRowName.size();x ++)
                {
                    if (NULL == gvTDSRowName[x].compare(sTestNameTableDatas.mvRowData[i][6]))
                    {
                        vValue[x] = sTestNameTableDatas.mvRowData[i][21];
                    }
                }
            }

            for (size_t i = 0;i < gvTDSRowName.size();i ++)
            {
                outputFile << vValue[i] << ",";
            }
            //std::vector<std::string> vValue;
            //size_t x = 0;      
            //for (size_t i = 1;i < sTestNameTableDatas.mvRowData.size();i ++)
            //{
            //    while (x < gvTDSRowName.size() && std::string::npos != sTestNameTableDatas.mvRowData[i][10].find("PRIM"))
            //    {
            //        if (NULL == gvTDSRowName[x].compare(sTestNameTableDatas.mvRowData[i][6]))
            //        {
            //            if (0 == x  || x == vValue.size() - 1) vValue.push_back(sTestNameTableDatas.mvRowData[i][21]);
            //            else
            //                vValue.at(x) = (sTestNameTableDatas.mvRowData[i][21]);
            //            break;
            //        }
            //        else
            //        {
            //            x ++;
            //            if (x > vValue.size()) vValue.push_back("NA");
            //        }       
            //    }
            //}
            //if (vValue.size() != gvTDSRowName.size()) for (size_t i = 0;i < gvTDSRowName.size();i ++) vValue.push_back("NA");
            //for (size_t i = 0;i < gvTDSRowName.size();i ++)
            //{
            //    outputFile << vValue[i] << ",";
            //}
        }
    }
}

bool ParseTableNodeToHtmle(const std::string& szTableBuf)
{
    int nStartTR = szTableBuf.find("<tr");
    int nEndTR = 0,nStartTD = 0,nEndTD = 0;
    char szBuf[256] = {0};
    S_TEST_NAME_TABLE_DATAS sTestNameTableDatas;
    std::vector<std::string> vRowData;

    while (std::string::npos != nStartTR )
    {
        vRowData.clear();
        nEndTR = szTableBuf.find("/tr>",nStartTR);
        if (std::string::npos == nEndTR) return false;
        nEndTD = 0;
        nStartTD = szTableBuf.find("<td",nStartTR);
        while (std::string::npos != nStartTD && nEndTD < nEndTR && nStartTD < nEndTR)
        {
            nEndTD = szTableBuf.find("</td>",nStartTD);
            strncpy(szBuf,szTableBuf.c_str() + nStartTD,nEndTD - nStartTD + strlen("</td>"));
            szBuf[nEndTD - nStartTD + strlen("</td>")] = '\0';
            //! outputFile << GetTDTextValue(szBuf) << ",";
            vRowData.push_back(GetTDTextValue(szBuf));
            nStartTD = szTableBuf.find("<td",nEndTD);
        }
        nStartTR = szTableBuf.find("<tr",nEndTR);
        sTestNameTableDatas.mvRowData.push_back(vRowData);
        //! outputFile << std::endl;
    }
    ParseSysTableNode(sTestNameTableDatas);
    return true;
}

bool ParesTestNameRules(S_TEST_NAME_RULES& sTestNameRules,const std::string& strHtmlFileBuf)
{
    gpsTestNameRules = &sTestNameRules;
    int pos = strHtmlFileBuf.find(sTestNameRules.mstrTestName);
    if (std::string::npos == pos)
    {
        return false;
    }
    pos = strHtmlFileBuf.find(sTestNameRules.mstrTestName,pos + sTestNameRules.mstrTestName.length());
    if (std::string::npos == pos)
    {
        return false;
    }

    for (size_t i = 1;i <= sTestNameRules.mnTestDataTablePos;i ++)
    {
        pos = strHtmlFileBuf.find("<table",pos + (i - 1) * 5);
        if (std::string::npos == pos)
        {
            return false;
        }
    }
    int endPos = strHtmlFileBuf.find("/table>",pos);

    std::string strTemp = strHtmlFileBuf.substr(pos,endPos - pos + strlen("/table>"));

    return ParseTableNodeToHtmle(strTemp);
}

int _tmain(int argc, _TCHAR* argv[])
{
#if 1
    if (argc < 2) return -1;
    std::string strPath = JGW_W2A_W_C(argv[1]);
    std::string strTemp;
    int numFiles = 0;
    std::cout << "Start Scan Puma Test Result Html File ..." << std::endl;
    JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanPuamThroughputTestResultFile);
    std::cout << "Scan Scan Puma Test Result File Completed !!!" << std::endl;

    const int LINE_LENGTH = 1024; 
    char str[LINE_LENGTH] = {0}; 
    std::string strContent;

    outputFile.imbue( std::locale("chs") );
    outputFile.open("C://output.csv",std::ios_base::out | std::ios_base::trunc );
    if (!outputFile.is_open()) return -1;
    for (size_t i = 0;i < gvQSEQHtmlFile.size();i ++)
    {
        std::ifstream inputFile;
        inputFile.imbue( std::locale("chs") );
        inputFile.open(gvQSEQHtmlFile.at(i));
        std::string strTemp = gvQSEQHtmlFile.at(i);
        int start = strTemp.rfind("\\");
        int end = strTemp.rfind("\\",start - 1);
        std::string strSN = strTemp.substr(end + 1,start - end - 1);
        strTemp = "";
//         inputFile.seekg(0, std::ios::end);    // go to the end  
//         std::streamoff length = inputFile.tellg();           // report location (this is the length)  
//         inputFile.seekg(0, std::ios::beg);
//         inputFile.seekg((length - 256) > 0?(length - 256):0);
//         inputFile.read(str,LINE_LENGTH);
//         strTemp = str;
//         strContent = strTemp.substr(strTemp.rfind("[SUM]"),strTemp.size());
//         outputFile << strSN << "," << str << std::endl;
        while (inputFile.getline(str,LINE_LENGTH))
        {
            if (strlen(str) > 10)
            {
                strTemp = str;
            }
        }
        if (strTemp.empty()) continue;
        outputFile << strSN << "," << strTemp << std::endl;
        inputFile.close();
    }

    std::cout << "Parse Puma Test Result File Completed !!!" << std::endl;
    outputFile.flush();
    outputFile.close();
#else
    if (argc < 2) return -1;
    std::string strPath = JGW_W2A_W_C(argv[1]);
    int numFiles = 0;
    std::cout << "Start Scan QSEQ Test Html File ..." << std::endl;
    JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanHtmlFile);
    std::cout << "Scan QSEQ Test Html File Completed !!!" << std::endl;

    S_TEST_NAME_ROW_RULES sRowRules;
    S_TEST_NAME_RULES sLteNameRules;
    sLteNameRules.meSys = E_LTE;
    sLteNameRules.mnTestDataTablePos = 2;
    sLteNameRules.mstrTestName = "LTE_MaxPower";
    sRowRules.mnPos = 0;
    sRowRules.mstrPosString = "50@0_22.0dBm_QPSK_PUSCH";
    sLteNameRules.mvRowRules.push_back(sRowRules);
    sRowRules.mnPos = 4;
    sRowRules.mstrPosString = "";
    sLteNameRules.mvRowRules.push_back(sRowRules);

    S_TEST_NAME_RULES sTdsNameRules;
    sTdsNameRules.meSys = E_TDS;
    sTdsNameRules.mnTestDataTablePos = 1;
    sTdsNameRules.mstrTestName = "TDS MaxPwr";
    sLteNameRules.mvRowRules.push_back(sRowRules);

    S_TEST_NAME_RULES sWcdmaNameRules;
    sWcdmaNameRules.meSys = E_WCDMA;
    sWcdmaNameRules.mnTestDataTablePos = 1;
    sWcdmaNameRules.mstrTestName = "WCDMA_MaxOutputPower";

    outputFile.imbue( std::locale("chs") );
    outputFile.open("d://output.csv",std::ios_base::out | std::ios_base::trunc );
    //! 保存一个标准HTML模板
    int statrSN = 0,endSN = 0;
    {
        outputFile << "SN" << ",";
        std::ifstream htmlFile;
        std::string strFilePath = "E:\\测试LOG\\TestLog\\CAL_L2018-08-23\\HM011834000002\\_20180823_055049_QSEQ_Output_DeviceGroup_1.html";
        htmlFile.open(strFilePath);
        std::string fileBuf((std::istreambuf_iterator<char>(htmlFile)),  
           std::istreambuf_iterator<char>());

        statrSN = strFilePath.find_last_of('\\');
        endSN = strFilePath.find_last_of('\\',statrSN - 1);
        strPath = strFilePath.substr(endSN + 1,statrSN - endSN - 1);
        if (!gbFristContent) outputFile << strPath << ",";
        ParesTestNameRules(sLteNameRules,fileBuf);
        ParesTestNameRules(sTdsNameRules,fileBuf);
        ParesTestNameRules(sWcdmaNameRules,fileBuf);
        outputFile << std::endl;
        gbFristContent = false;
    }

    std::cout << "Start Parse QSEQ Test Html File ..." << std::endl;
    
    for (size_t i = 0;i < gvQSEQHtmlFile.size();i ++)
    {
        std::ifstream htmlFile;
        std::string strFilePath = gvQSEQHtmlFile[i];
        htmlFile.open(strFilePath);
        std::string fileBuf((std::istreambuf_iterator<char>(htmlFile)),  
            std::istreambuf_iterator<char>());

        statrSN = strFilePath.find_last_of('\\');
        endSN = strFilePath.find_last_of('\\',statrSN - 1);
        strPath = strFilePath.substr(endSN + 1,statrSN - endSN - 1);
        outputFile << strPath << ",";
        ParesTestNameRules(sLteNameRules,fileBuf);
        ParesTestNameRules(sTdsNameRules,fileBuf);
        ParesTestNameRules(sWcdmaNameRules,fileBuf);
        outputFile << std::endl;
    }
    std::cout << "Parse QSEQ Test Html File Completed !!!" << std::endl;
     outputFile.close();
#endif
    return 0;
}