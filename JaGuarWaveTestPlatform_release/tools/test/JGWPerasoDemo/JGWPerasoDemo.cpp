// JGWPerasoDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CJGW_CSerialComPort.h"
#include "CJGW_PrsDownloadFlash.h"

#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_PrsDownloadProtocol.h>


#define PERASO_VID "VID_2932"
#define PERASO_SERVICE_NAME L"usbser"

using namespace JGW;

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
    std::cout << "-f              pdb image file path\n"
        <<"-sn              write sn\n"
        <<"-mac             write mac"
        << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    /*CCJGW_CSerialComPort serialComport;
    CCJGW_DonglesComPort donglesComPort(serialComport);
    std::wstring strComPort = GetPerasoComPort();
    std::wstring strSerialNumber,strMacAddress,strOtpVersion,strConfigVersion;

    if (!strComPort.empty() && serialComport.OpenSerialComPort(strComPort,CBR_115200))
    {
    donglesComPort.ReadSerialNumberAndMacAddress(strSerialNumber,strMacAddress);
    donglesComPort.ReadConfigPdbVersion(strConfigVersion);
    donglesComPort.ReadOtpPdbVersion(strOtpVersion);
    donglesComPort.CloseDonglesComPort();
    }*/

    CCJGW_CSerialComPort serialComPort;
    CCJGW_PrsDownloadFlash prsDownloadFlash(serialComPort);
    bool bDownLoadPDB = false;

    std::string strPDBFolder,strSerailNumber,strMac,strSavePDBFolder;
    JGW_GetModuleWorkPathA(strPDBFolder);

    for (int i = 1;i < argc;i ++)
    {
        if (NULL == JGW_WStrComparenoCaseWStr(L"-f",argv[i]))
        {
            if (i + 1 >= argc) return -1;
            i++;
            strPDBFolder = JGW_W2A(JGW_RealativePathToAbsPath(argv[i]));
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(L"-sn",argv[i]))
        {
            if (i + 1 >= argc) return -1;
            i ++;
            strSerailNumber = JGW_W2A_W(argv[i]);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(L"-mac",argv[i]))
        {
            if (i + 1 >= argc) return -1;
            i ++;
            strMac = JGW_W2A_W(argv[i]);
        }
        else if (NULL == JGW_WStrComparenoCaseWStr(L"-w",argv[i]))
        {
            ///if (i + 1 >= argc) return -1;
            bDownLoadPDB = true;
        }
        else
        {
            help();
            return -1;
        }
    }
    if (!bDownLoadPDB)
    {
        JGW_FormatString(strSavePDBFolder,"%s\\%s_%s",strPDBFolder.c_str(),strSerailNumber.c_str(),strMac.c_str());
        if (prsDownloadFlash.InitDownloadFlashImage(strPDBFolder,false))
        {
            prsDownloadFlash.SetSerailNumberMacAddressToOtpFile(strSerailNumber,strMac);
            prsDownloadFlash.BuildOTPCRC32();
            prsDownloadFlash.BuildSrcPDBFileToPDBBuf(strSavePDBFolder,false,true,true);
            std::cout << "Save Folder:" << strSavePDBFolder << std::endl;
        }
        else
        {
            std::cout << "Save PDB Fail !!!" << std::endl;
        }
    }
    else
    {
        std::wstring strComPort = GetPerasoComPort();
        if (!strComPort.empty() && serialComPort.OpenSerialComPort(strComPort,CBR_115200))
        {
            if (eDeviceBootMode_BootLoader == prsDownloadFlash.CheckDeviceIsBootloaderMode())
            {
                if (prsDownloadFlash.InitDownloadFlashImage(strPDBFolder,false))
                {
                    if (!strSerailNumber.empty() || !strMac.empty())
                    {
                        prsDownloadFlash.SetSerailNumberMacAddressToOtpFile(strSerailNumber,strMac);
                        prsDownloadFlash.BuildOTPCRC32();
                    }
                   bool result = prsDownloadFlash.DonwloadAllPDBImageFileToDongleFlash(false,true,true);
                   std::cout << "Donwload PDB Image File Complete." << std::endl;
                }
            }
            else
            {
                std::cout << "ERROR: device is not boot mode !!!" << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR: No suitable device found!!!" << std::endl;
        }
    }
    
    return 0;
}

