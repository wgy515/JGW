// JGW_ComPDBFile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CJGW_ExportPDBFile.h"
#include "PerasoDefine.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#define PERASO_VID "VID_2932"
#define PERASO_SERVICE_NAME L"usbser"

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
    {
        std::wcout << "Please Invalid Param(Defalt Param :C\\PDB\\)" << std::endl;
        return -1;
    }
    
    std::string strFolder("D:\\开发资料\\公司资料\\Release\\");
    std::string strExportFolder("c:\\PDB\\");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    if (2 == argc)
    {
        strFolder = JGW::JGW_W2A_W(argv[1]);
        while ('"' == strFolder.at(strFolder.size() - 1) || '\'' == strFolder.at(strFolder.size() - 1))
            strFolder.erase(strFolder.size() - 2);
        if (!JGW::JGW_IsPathSlashA(strFolder.at(strFolder.size() - 1)))
            strFolder += "\\";


        strExportFolder = JGW::JGW_W2A_W(JGW::JGW_GetApplicationFolder());
        while ('"' == strExportFolder.at(strExportFolder.size() - 1) || '\'' == strExportFolder.at(strExportFolder.size() - 1))
            strExportFolder.erase(strExportFolder.size() - 2);
    }
    else
    {
        strFolder = JGW::JGW_W2A_W(argv[1]);
        while ('"' == strFolder.at(strFolder.size() - 1) || '\'' == strFolder.at(strFolder.size() - 1))
            strFolder.erase(strFolder.size() - 2);
        if (!JGW::JGW_IsPathSlashA(strFolder.at(strFolder.size() - 1)))
            strFolder += "\\";

        strExportFolder = JGW::JGW_W2A_W(argv[2]);
        while ('"' == strExportFolder.at(strExportFolder.size() - 1) || '\'' == strExportFolder.at(strExportFolder.size() - 1))
            strExportFolder.erase(strFolder.size() - 2);
    }

    std::cout << "Src PDB File Path: " << strFolder << std::endl;
    std::cout << "Dest Save PDB File Path: " << strExportFolder << std::endl;


    std::wstring strComport = JGW::JGW_GetComPortToUsbServiceName(PERASO_SERVICE_NAME,_T(PERASO_VID));

    if (strComport.empty())
    {
        std::wcout << L"not found dongles comport!!!" << std::endl;
        return 0;
    }

   

    JGW::CCJGW_ExportPDBFile exportPDBFile;

#if 1

    exportPDBFile.LoadPDBImageFile(strFolder,strExportFolder);
    if (!exportPDBFile.OpenDongleComPort(strComport))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::wcout << L"Open Dongle ComPort Fail!!!" << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return 0;
    }

    //! 获取CONFIG PDB SIZE


    if (exportPDBFile.InitPDBInfoToDongle()
        && exportPDBFile.GetOtpPDBImageSize()
        && exportPDBFile.GetConfigPDBImageSize()
        && exportPDBFile.ExportPDBAllFile()
        && exportPDBFile.ComparePDBAllFile())
    {
        std::wcout << L"Compare Complete !!!" << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return 0;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);

#else
    if (!exportPDBFile.OpenDongleComPort(strComport))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::wcout << L"Open Dongle ComPort Fail!!!" << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return 0;
    }

    std::wstring strSerialNumber,strMacAddress;
    exportPDBFile.ReadSerialNumberToBoardInfo(strSerialNumber);
    std::wcout << L"Serial Number:"<< strSerialNumber << std::endl;
    exportPDBFile.ReadMacAddressToBoardInfo(strMacAddress);
    std::wcout << L"Mac Address:"<< strMacAddress << std::endl;
#endif
	return 0;
}

