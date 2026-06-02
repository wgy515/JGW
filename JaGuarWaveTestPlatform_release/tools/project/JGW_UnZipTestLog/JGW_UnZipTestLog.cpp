// JGW_UnZipTestLog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <poco/Zip/Decompress.h>
#include <poco/Zip/ZipLocalFileHeader.h>
#include <poco/Delegate.h>

using namespace JGW;
// 测试LOG目录
std::wstring gstrTestLogFolder;
// 测试LOG解压路径
std::wstring gstrTestLogUnZipFolder;
//
DWORD _errCnt = 0;

class DecompressCallBack
{
public:
    DecompressCallBack()
    {
    }

    ~DecompressCallBack()
    {
    }


    void OnDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
    {
        ++_errCnt;
        std::cout << "Decompression failure : " << info.second  << std::endl;
        //OnAppendRichEditTest(WPARAM(JGW_A2W(info.second).c_str()),NULL);
        //MessageBox(m_hWnd,JGW_A2W(info.second).c_str(),L"当前文件升级失败",MB_ICONERROR);
    }

    void OnDecompressOkCallback(const void* pSender,std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
    {
        std::string path = info.second.toString();
        //std::cout << path << std::endl;
        //OnAppendRichEditTest(WPARAM(JGW_A2W(msUpdateHelpConfig.mstrUpdateExecFolder + path).c_str()),NULL);
    }
};



std::vector<std::string> gvTestLogZipFile;
std::string strFilePathTemp;

bool has_suffix(const std::string& filename, const std::string& suffix) {
    std::string strFileName (filename);
    JGW_MakeStringToLower(strFileName);
    return strFileName.size() >= suffix.size() && 
        strFileName.compare(strFileName.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void ScanTestLogZipFile(const char* filepath,const char* filename,void* lpData)
{
    
    //! NULL != strstr(filename,"result") &&
    if (has_suffix(filename,".zip"))
    //if (NULL != strstr(filename,".zip"))
    {
        strFilePathTemp = filepath;
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        gvTestLogZipFile.push_back(JGW_W2A(JGW_A2W(strFilePathTemp),CP_UTF8));
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    if (argc > 1)
    {
        gstrTestLogFolder = argv[1];
    }
    if (!JGW_IsFolderExist(gstrTestLogFolder.c_str()))
    {
        std::wcout << L"Directory does not exist :" << gstrTestLogFolder << std::endl;
        return -1;
    }

    if (argc > 2)
    {
        gstrTestLogUnZipFolder = argv[2];
    }
    else
    {
        gstrTestLogUnZipFolder = JGW_GetApplicationFolder() + JGW_GetCurrentTimeStampYMDHMS();
    }
    JGW_CreateDirectory(gstrTestLogUnZipFolder.c_str(),true);

    int numFiles = 0,start,end;
    std::cout << "Start Scan Test Log Zip File ..." << std::endl;
    JGW_TraverseFolder_C(JGW_W2A(gstrTestLogFolder),numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanTestLogZipFile);
    std::cout << "Scan Scan Test Log Zip File Completed !!!" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    DecompressCallBack cDecompressCallBack;
    for (std::vector<std::string>::iterator it = gvTestLogZipFile.begin();it != gvTestLogZipFile.end();++it)
    {
        std::ifstream inp(it[0], std::ios::binary);
        _errCnt = 0;
        Poco::Zip::Decompress dec(inp,Poco::Path(JGW_W2A(gstrTestLogUnZipFolder,CP_UTF8)));
        dec.EError += Poco::Delegate<DecompressCallBack, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(&cDecompressCallBack, &DecompressCallBack::OnDecompressError);
        dec.EOk += Poco::Delegate<DecompressCallBack, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(&cDecompressCallBack, &DecompressCallBack::OnDecompressOkCallback);

        dec.decompressAllFiles();
        dec.EError -= Poco::Delegate<DecompressCallBack, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(&cDecompressCallBack, &DecompressCallBack::OnDecompressError);
        dec.EOk -= Poco::Delegate<DecompressCallBack, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(&cDecompressCallBack, &DecompressCallBack::OnDecompressOkCallback);
    }
    //dec.EOk += Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
    //dec.decompressAllFiles();
    //dec.EError -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressError);
    //dec.EOk -= Poco::Delegate<CCJaGuarwaveUpdateHelpDlg, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &CCJaGuarwaveUpdateHelpDlg::OnDecompressOkCallback);
    


	return 0;
}

