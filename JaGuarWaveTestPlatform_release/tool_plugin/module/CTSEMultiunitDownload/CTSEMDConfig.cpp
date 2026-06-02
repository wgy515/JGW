#include "StdAfx.h"
#include "CTSEMDConfig.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCTSEMDConfig::CCTSEMDConfig(void)
    {
        msTseMultiunitDownloadConfig.mbResetAfterDownload = false;
        msTseMultiunitDownloadConfig.mbAutoSwitchDownloadPort = false;
        msTseMultiunitDownloadConfig.mbHaveProvisionXml = true;
        msTseMultiunitDownloadConfig.mstrProgFirehoseLitembnFileName = L"prog_ufs_firehose_8996_ddr.elf";
        msTseMultiunitDownloadConfig.mstrRawprograms = L"rawprogram_unsparse0.xml,rawprogram_unsparse4.xml,rawprogram1.xml,rawprogram2.xml,rawprogram3.xml,rawprogram5.xml";
        msTseMultiunitDownloadConfig.mstrPacths = L"patch0.xml,patch1.xml,patch2.xml,patch3.xml,patch4.xml,patch5.xml";
    }


    CCTSEMDConfig::~CCTSEMDConfig(void)
    {
    }

    bool CCTSEMDConfig::LoadMultiunitDownloadConfig(const std::wstring& strConfigPath)
    {
        if (strConfigPath.empty()) return false;
        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(strConfigPath.c_str())) return false; 
        mstrConfigPath = strConfigPath;
		std::wstring strTemp = JGW_GetPathOfFile(strConfigPath.c_str());
        msTseMultiunitDownloadConfig.mbResetAfterDownload = configIni.GetIniKeyBoolValue(L"MAIN",L"ResetAfterDownload",0);
        msTseMultiunitDownloadConfig.mbAutoSwitchDownloadPort = configIni.GetIniKeyBoolValue(L"MAIN",L"AutoSwitchComport",0);
        msTseMultiunitDownloadConfig.mnDownloadTimeSec = configIni.GetIniKeyIntValue(L"MAIN",L"DownloadTimeSec",60 * 2);
        msTseMultiunitDownloadConfig.mnDownloadTimeOutSec = configIni.GetIniKeyIntValue(L"MAIN",L"DownloadTimeOutSec",60 * 10);
        msTseMultiunitDownloadConfig.mstrDownloadPath = configIni.GetIniKeyValue(L"MAIN",L"DownLoadPath");
		if (JGW_PathIsRelativeW(msTseMultiunitDownloadConfig.mstrDownloadPath.c_str())) msTseMultiunitDownloadConfig.mstrDownloadPath = strTemp + msTseMultiunitDownloadConfig.mstrDownloadPath;
        msTseMultiunitDownloadConfig.mstrModeName = configIni.GetIniKeyValue(L"MAIN",L"ModeName");
        msTseMultiunitDownloadConfig.mstrProvisionXml = configIni.GetIniKeyValue(L"MAIN",L"ProvisionXml",L"provision_samsung.xml");
        msTseMultiunitDownloadConfig.mbHaveProvisionXml = configIni.GetIniKeyBoolValue(L"MAIN",L"HaveProvisionXml",true);
        msTseMultiunitDownloadConfig.mstrProgFirehoseLitembnFileName = configIni.GetIniKeyValue(L"MAIN",L"ProgEmmcFirehoseLitembnFileName");
        msTseMultiunitDownloadConfig.mstrRawprograms = configIni.GetIniKeyValue(L"MAIN",L"Rawprograms");
        msTseMultiunitDownloadConfig.mstrPacths = configIni.GetIniKeyValue(L"MAIN",L"Patchs");
        
        return true;
    }

    void CCTSEMDConfig::SaveMultiunitDownloadConfig()
    {
        if (mstrConfigPath.empty()) return ;
        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(mstrConfigPath.c_str())) return ;

		std::wstring strTemp = JGW_GetPathOfFile(mstrConfigPath.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"ResetAfterDownload",msTseMultiunitDownloadConfig.mbResetAfterDownload?1:0);
        configIni.SetIniKeyValue(L"MAIN",L"AutoSwitchComport",msTseMultiunitDownloadConfig.mbAutoSwitchDownloadPort?1:0);
		JGW_ReplaceStringW(msTseMultiunitDownloadConfig.mstrDownloadPath,strTemp.c_str(),L"");
        configIni.SetIniKeyValue(L"MAIN",L"DownLoadPath",msTseMultiunitDownloadConfig.mstrDownloadPath.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"ModeName",msTseMultiunitDownloadConfig.mstrModeName.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"ProvisionXml",msTseMultiunitDownloadConfig.mstrProvisionXml.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"ProgEmmcFirehoseLitembnFileName",msTseMultiunitDownloadConfig.mstrProgFirehoseLitembnFileName.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"Rawprograms",msTseMultiunitDownloadConfig.mstrRawprograms.c_str());
        configIni.SetIniKeyValue(L"MAIN",L"Patchs",msTseMultiunitDownloadConfig.mstrPacths.c_str());
    }
}