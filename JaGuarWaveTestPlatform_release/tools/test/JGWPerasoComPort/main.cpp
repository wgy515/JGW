#include "stdafx.h"
#include "CJGW_CSerialComPort.h"
#include "CJGW_DownloadPDBImage.h"

#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

void help()
{
    std::cout << "-f              pdb image file path\n"
        <<"-sn              write sn\n"
        <<"-mac             write mac"
        << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    char buf[260] = {0};
    sprintf(buf,"%2.02X",-16 & 0xFF);

    JGW::CCJGW_DownloadPDBImage wctp;
    std::string strRead,strOut,strDir = "D:\\开发资料\\公司资料\\Release\\",strSN,strMac;
    int mask = 0;
    int dutPortMask = 1 << (int)mask;

    for (int i = 1;i < argc;i ++)
    {
        if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-f",argv[i]))
        {
            if (i + 1 >= argc) return -1;
            i++;
            strDir = JGW::JGW_W2A_W(argv[i]);
            if (strDir.at(strDir.size() - 1) != '\\' && strDir.at(strDir.size() - 1) != '/')
                strDir += "\\";
            wctp.LoadPDBImageFile(strDir);
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

    wctp.BuildConfigCRC32();
//     if (wctp.BuildSNMacToFile(strSN.empty()?NULL:strSN.c_str(),strMac.empty()?NULL:strMac.c_str()))
//     {
//         std::cout << "write Serial Number: " << strSN.c_str() << std::endl;
//         std::cout << "write Mac Address: " << strMac.c_str() << std::endl;
//         std::cout << "Flash write complete\n";
//     }
//     else
//     {
//         std::cout << "Flash write fail\n";
//     }
    return -1;
}