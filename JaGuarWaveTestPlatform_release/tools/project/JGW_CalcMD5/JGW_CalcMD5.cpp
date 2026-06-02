// JGW_CalcMD5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
std::wstring gstrFolder = L"C:\\JW";
std::wstring gstrExec,gstrHelp,gstrDatabases,gstrJaGuarWaveTestPlatformInstall,gstrNopad = L"notepad++";
using namespace JGW;
//! 遍历目录下的MD5  JGW_CalcMD5 -l c://JGW
//! exec help JaGuarWaveTestPlatformInstall Databases
void ScanPuamThroughputTestResultFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
{
    std::map<std::wstring,std::wstring>* mpMapFileMD5 = (std::map<std::wstring, std::wstring>*)lpData;

    //if (NULL != JGW_WStrCaseWStr(gstrExec.c_str(),filepath) 
    //    || NULL != JGW_WStrCaseWStr(gstrHelp.c_str(),filepath) 
    //    || NULL != JGW_WStrCaseWStr(gstrDatabases.c_str(),filepath) 
    //    || NULL != JGW_WStrCaseWStr(gstrJaGuarWaveTestPlatformInstall.c_str(),filepath))
    //{
    std::wstring strTmep (filepath);
    strTmep += L"/";
    strTmep += filename;
    std::wstring strMD5 = JGW::CCJGW_CryptFileMD5::GetFileMD5(strTmep);
    JGW_ReplaceStringW(strTmep,gstrFolder + L"/",L"");

    mpMapFileMD5->insert(std::make_pair<std::wstring,std::wstring>(strTmep,strMD5));
    //}
    //! NULL != strstr(filename,"result") &&
    /*if (NULL != strstr(filename,"P_"))
    {
    strFilePathTemp = filepath;
    strFilePathTemp += "\\";
    strFilePathTemp += filename;
    gvQSEQHtmlFile.push_back(strFilePathTemp);
    }*/
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2) return -1;
    if (2 == argc)
    {
        std::wstring strFileName = argv[1];
        std::wcout << JGW::CCJGW_CryptFileMD5::GetFileMD5(strFileName) << std::endl;
    }
    else if (argc > 2)
    {
        std::wcout.imbue(std::locale(""));
        std::cout.imbue(std::locale(""));
        gstrFolder = argv[2];
        std::wstring strSaveIniFile = JGW::JGW_GetExecConfigFilePath(L"md5.list");
        std::map<std::wstring,std::wstring> mmapFileMD5;
        /// std::wstring gstrExec,gstrHelp,gstrDatabases;
        /// using namespace JGW;
        //! 遍历目录下的MD5  JGW_CalcMD5 -l c://JGW
        //! exec help JaGuarWaveTestPlatformInstall Databases
        JGW_FormatWString(gstrExec,L"%s/%s",gstrFolder.c_str(),L"exec");
        JGW_FormatWString(gstrHelp,L"%s/%s",gstrFolder.c_str(),L"help");
        JGW_FormatWString(gstrDatabases,L"%s/%s",gstrFolder.c_str(),L"Databases");
        JGW_FormatWString(gstrJaGuarWaveTestPlatformInstall,L"%s/%s",gstrFolder.c_str(),L"JaGuarWaveTestPlatformInstall");
        JGW_DeleteFile(strSaveIniFile.c_str());

        int numFiles = 0;
        JGW_EraseLastAndFristTwoCharsW(gstrFolder,L'\\',L'/');
        std::wstring strFolder(gstrFolder);
        std::cout << "Start Scan File ..." << std::endl;
        JGW_TraverseFolder_VC(strFolder,numFiles,L"\\*.*"/*"/ *QSEQ*.html"*/,ScanPuamThroughputTestResultFile,&mmapFileMD5);
        std::cout << "Scan File Completed !!!" << std::endl;


        CCJGW_ConfigIni ini;
        ini.InitIniFilePath(strSaveIniFile.c_str());
        for (std::map<std::wstring,std::wstring>::iterator it = mmapFileMD5.begin();it != mmapFileMD5.end();++it)
        {
            ini.SetIniKeyValue(L"LIST",it->first.c_str(),it->second.c_str());
        }
        std::wcout << L"Save MD5 List File: " << strSaveIniFile << std::endl;
    } 
    return 0;
}

