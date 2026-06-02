#include "StdAfx.h"
#include "CPEMultiDownloadConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCPEMultiDownloadConfig::CCPEMultiDownloadConfig(void)
    {
    }


    CCPEMultiDownloadConfig::~CCPEMultiDownloadConfig(void)
    {
    }

    void CCPEMultiDownloadConfig::LoadMultiDownloadConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,const std::wstring& strDownloadConfigFilePath)
    {
        mConfigIni.InitIniFilePath(strDownloadConfigFilePath.c_str());

        psMultiDownloadConfig->mstrDownloadPath = mConfigIni.GetIniKeyValue(L"MAIN",L"DownLoadPath",L"C://");
        psMultiDownloadConfig->mnDownloadTime = mConfigIni.GetIniKeyIntValue(L"MAIN",L"DownLoadTime",100);
        psMultiDownloadConfig->mnDownloadTimeOut = mConfigIni.GetIniKeyIntValue(L"MAIN",L"DownloadTimeOut",200);
        mstrTftpd32ConfigFilePath = mConfigIni.GetIniKeyValue(L"MAIN",L"Tftpd32ConfigIni",L"Tftpd32\tftpd32.ini");
        mstrTftpd32ConfigFilePath = JGW_RealativePathToAbsPath(mstrTftpd32ConfigFilePath.c_str());
        std::wstring strIpAddr = mConfigIni.GetIniKeyValue(L"MAIN",L"TFTPDIPAddr",L"127.0.0.1");
        if (!strIpAddr.empty())
        {
            psMultiDownloadConfig->mstrTFtpdIPAddr = strIpAddr;
        }
        psMultiDownloadConfig->mstrDownloadParam = mConfigIni.GetIniKeyValue(L"MAIN",L"DownloadFileParam",L"TSE_SAHARA_FIREHOSE_IMAGE_PARAM=sbl1_flashless.mbn,TZ_MBN=tz.mbn,DEVCFG_MBN=devcfg_noac.mbn,RPM_MBN=rpm.mbn,BOOT_MBN=openwrt-ipq807x-u-boot.mbn,CDT_DDR4=cdt_DDR4.bin");
        /*psMultiDownloadConfig->mstrSBL = mConfigIni.GetIniKeyValue(L"MAIN",L"SBL",L"sbl1_flashless.mbn");
        psMultiDownloadConfig->mstrCDT = mConfigIni.GetIniKeyValue(L"MAIN",L"CDT",L"cdt-AP-HK09_512M16_DDR4.bin");
        psMultiDownloadConfig->mstrTZ = mConfigIni.GetIniKeyValue(L"MAIN",L"TZ",L"tz.mbn");
        psMultiDownloadConfig->mstrDEV = mConfigIni.GetIniKeyValue(L"MAIN",L"DEV",L"devcfg_noac.mbn");
        psMultiDownloadConfig->mstrRPM = mConfigIni.GetIniKeyValue(L"MAIN",L"RPM",L"rpm.mbn");
        psMultiDownloadConfig->mstrBOOT = mConfigIni.GetIniKeyValue(L"MAIN",L"BOOT",L"openwrt-ipq807x-u-boot.mbn");*/
    }

    void CCPEMultiDownloadConfig::SettingTftp32BaseDirectory(const std::wstring& strBaseDirectory)
    {
        if (JGW_PathFileExists(mstrTftpd32ConfigFilePath.c_str()))
        {
            CCJGW_ConfigIni configIni;
            configIni.InitIniFilePath(mstrTftpd32ConfigFilePath.c_str());
            configIni.SetIniKeyValue(L"TFTPD32",L"BaseDirectory",strBaseDirectory.c_str());
        }
    }

    void CCPEMultiDownloadConfig::SaveMultiDownloadConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig)
    {
        mConfigIni.SetIniKeyValue(L"MAIN",L"DownLoadPath",psMultiDownloadConfig->mstrDownloadPath.c_str());
        mConfigIni.SetIniKeyValue(L"MAIN",L"DownLoadTime",psMultiDownloadConfig->mnDownloadTime);
        mConfigIni.SetIniKeyValue(L"MAIN",L"DownloadTimeOut",psMultiDownloadConfig->mnDownloadTimeOut);
        mConfigIni.SetIniKeyValue(L"MAIN",L"TFTPDIPAddr",psMultiDownloadConfig->mstrTFtpdIPAddr.c_str());
        mConfigIni.SetIniKeyValue(L"MAIN",L"DownloadFileParam",psMultiDownloadConfig->mstrDownloadParam.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"SBL",psMultiDownloadConfig->mstrSBL.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"CDT",psMultiDownloadConfig->mstrCDT.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"TZ",psMultiDownloadConfig->mstrTZ.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"DEV",psMultiDownloadConfig->mstrDEV.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"RPM",psMultiDownloadConfig->mstrRPM.c_str());
        //mConfigIni.SetIniKeyValue(L"MAIN",L"BOOT",psMultiDownloadConfig->mstrBOOT.c_str());
    }

}
