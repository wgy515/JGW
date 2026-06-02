#include "StdAfx.h"
#include "CJGW_MultipleTestConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCJGW_MultipleTestConfig::CCJGW_MultipleTestConfig(void)
    {
    }


    CCJGW_MultipleTestConfig::~CCJGW_MultipleTestConfig(void)
    {
    }

    CCJGW_ConfigIni& CCJGW_MultipleTestConfig::GetConfigIniInstance()
    {
        return mConfigINI;
    }

    bool CCJGW_MultipleTestConfig::LoadMultipleTestConfig(const std::wstring& strModulePath,S_MULTIPLE_CONFIG& sMultipleConfig)
    {
        //! MultipleTest.ini
        mstrMultipleTestConfigPath = JGW_GetCacheConfigFilePath(strModulePath,MULTIPLE_TEST_CONFIG);
        if (!JGW_FileExistsToFilePath(mstrMultipleTestConfigPath.c_str())) return false;


        mConfigINI.InitIniFilePath(mstrMultipleTestConfigPath.c_str());
        sMultipleConfig.msDeviceProgramInfo.meDeviceQueueProgram = (E_DEVICE_QUEUE_PROGRAM)mConfigINI.GetIniKeyIntValue(L"MAIN",L"DeviceQueueProgram",0);
        sMultipleConfig.msDeviceProgramInfo.meFristDeviceMode = (E_DEVICE_MODE)mConfigINI.GetIniKeyIntValue(L"MAIN",L"FristDeviceMode",0);
        sMultipleConfig.msDeviceProgramInfo.meSecondDeviceMode = (E_DEVICE_MODE)mConfigINI.GetIniKeyIntValue(L"MAIN",L"SecondDeviceMode",0);
        sMultipleConfig.msDeviceProgramInfo.meThreeDeviceMode = (E_DEVICE_MODE)mConfigINI.GetIniKeyIntValue(L"MAIN",L"ThreeDeviceMode",0);
        sMultipleConfig.mbBinaryImageIsFolder = mConfigINI.GetIniKeyBoolValue(L"MAIN",L"BinaryImageIsFolder");
        //sMultipleConfig.mbBinaryImageIsFolder = JGW_RealativePathToAbsPath(sMultipleConfig.mbBinaryImageIsFolder.c_str());
        sMultipleConfig.mbSaveTestLog = mConfigINI.GetIniKeyBoolValue(L"MAIN",L"SaveTestLog");
        sMultipleConfig.mnMultipleTestRow = mConfigINI.GetIniKeyIntValue(L"MAIN",L"MultipleTestRow",2);
        sMultipleConfig.mnMultipleTestCol = mConfigINI.GetIniKeyIntValue(L"MAIN",L"MultipleTestCol",4);
        sMultipleConfig.mnTestTimeSec = mConfigINI.GetIniKeyIntValue(L"MAIN",L"TestTimeSec",200);
        sMultipleConfig.mnTestTimeOutSec = mConfigINI.GetIniKeyIntValue(L"MAIN",L"TestTimeOutSec",600);
        sMultipleConfig.mstrSaveTestLogFolder = mConfigINI.GetIniKeyValue(L"MAIN",L"SaveTestLogFolder");
        if (sMultipleConfig.mstrSaveTestLogFolder.empty())
        {
            sMultipleConfig.mstrSaveTestLogFolder = JGW_GetFormatWString(L"%sTestLog\\%s\\",JGW_GetApplicationFolder(),strModulePath.c_str());
        }
        sMultipleConfig.mstrBinaryImagePath = JGW_RealativePathToAbsPath(mConfigINI.GetIniKeyValue(L"MAIN",L"BinaryImagePath").c_str());
        sMultipleConfig.mstrUpgradeVersion = mConfigINI.GetIniKeyValue(L"MAIN",L"UpgradeVersion");

        sMultipleConfig.mstrTSEExecApplicationFilePath = JGW_RealativePathToAbsPath(mConfigINI.GetIniKeyValue(L"MAIN",L"TSEExecApplicationFilePath").c_str());
        sMultipleConfig.mstrTSEExecApplicationCurrentWorkDir = JGW_GetFileFolderToFilePath(sMultipleConfig.mstrTSEExecApplicationFilePath.c_str());
        std::wstring strBasePath = JGW_GetCacheConfigFolder(strModulePath);
        sMultipleConfig.mstrTSESuiteConfigFilePath  = JGW_RealPathToAbsPathBasePath(mConfigINI.GetIniKeyValue(L"MAIN",L"TSESuiteConfigFilePath").c_str(),strBasePath.c_str());
        /// netcard
        sMultipleConfig.msNetCardConfig.mstrHostIPAddr = mConfigINI.GetIniKeyValue(L"MAIN",L"HostIPAddr",L"192.168.1.1");
        sMultipleConfig.msNetCardConfig.mstrHostIPAddrAnsi = JGW_W2A(sMultipleConfig.msNetCardConfig.mstrHostIPAddr);
        sMultipleConfig.msDeviceProgramInfo.mstrNetCardHostIPSeg = sMultipleConfig.msNetCardConfig.mstrHostIPAddr.substr(0,sMultipleConfig.msNetCardConfig.mstrHostIPAddr.find_last_of('.') + 1);
        // 
        sMultipleConfig.mstrConfigTSEParam = mConfigINI.GetIniKeyValue(L"MAIN",L"TSE_Param",L"");
        sMultipleConfig.mstrDownloadFileConfig = L"";

        std::vector<std::wstring> vstrTSEParams;
        std::wstring strValue;
        JGW_ParserStrW(sMultipleConfig.mstrConfigTSEParam.c_str(),L",",vstrTSEParams);
        for (size_t index = 0;index < vstrTSEParams.size();index++)
        {
            strValue = mConfigINI.GetIniKeyValue(L"MAIN",vstrTSEParams[index].c_str());
            sMultipleConfig.mmapTSEParamValues.insert(std::make_pair<std::wstring,std::wstring>(vstrTSEParams[index],strValue));
            if (sMultipleConfig.mstrDownloadFileConfig.empty()) sMultipleConfig.mstrDownloadFileConfig += JGW_GetFormatWString(L"%s=%s",vstrTSEParams[index].c_str(),strValue.c_str());
            else sMultipleConfig.mstrDownloadFileConfig += JGW_GetFormatWString(L",%s=%s",vstrTSEParams[index].c_str(),strValue.c_str());
        }

        sMultipleConfig.mstrTFTPDIPAddr = mConfigINI.GetIniKeyValue(L"MAIN",L"TFTPDIPAddr",L"192.168.1.5");
        mstrTftpd32ConfigIni = JGW_RealativePathToAbsPath(mConfigINI.GetIniKeyValue(L"MAIN",L"Tftpd32ConfigIni",L"Tftpd32\\tftpd32.ini").c_str());

        return true;
    }

    void CCJGW_MultipleTestConfig::SettingTftp32BaseDirectory(const std::wstring& strBaseDirectory)
    {
        if (JGW_PathFileExists(mstrTftpd32ConfigIni.c_str()))
        {
            CCJGW_ConfigIni configIni;
            configIni.InitIniFilePath(mstrTftpd32ConfigIni.c_str());
            configIni.SetIniKeyValue(L"TFTPD32",L"BaseDirectory",strBaseDirectory.c_str());
        }
    }

    void CCJGW_MultipleTestConfig::SaveMultipleTestConfig(S_MULTIPLE_CONFIG& sMultipleConfig)
    {
        //CCJGW_mConfigINI mConfigINI;
        mConfigINI.InitIniFilePath(mstrMultipleTestConfigPath.c_str());

        mConfigINI.SetIniKeyValue(L"MAIN",L"DeviceQueueProgram",(int)sMultipleConfig.msDeviceProgramInfo.meDeviceQueueProgram);
        mConfigINI.SetIniKeyValue(L"MAIN",L"FristDeviceMode",(int)sMultipleConfig.msDeviceProgramInfo.meFristDeviceMode);
        mConfigINI.SetIniKeyValue(L"MAIN",L"SecondDeviceMode",(int)sMultipleConfig.msDeviceProgramInfo.meSecondDeviceMode);
        mConfigINI.SetIniKeyValue(L"MAIN",L"ThreeDeviceMode",(int)sMultipleConfig.msDeviceProgramInfo.meThreeDeviceMode);
        mConfigINI.SetIniKeyValue(L"MAIN",L"BinaryImageIsFolder",sMultipleConfig.mbBinaryImageIsFolder?1:0);
        mConfigINI.SetIniKeyValue(L"MAIN",L"SaveTestLog",sMultipleConfig.mbSaveTestLog?1:0);
        mConfigINI.SetIniKeyValue(L"MAIN",L"MultipleTestRow",sMultipleConfig.mnMultipleTestRow);
        mConfigINI.SetIniKeyValue(L"MAIN",L"MultipleTestCol",sMultipleConfig.mnMultipleTestCol);
        mConfigINI.SetIniKeyValue(L"MAIN",L"TestTimeSec",sMultipleConfig.mnTestTimeSec);
        mConfigINI.SetIniKeyValue(L"MAIN",L"TestTimeOutSec",sMultipleConfig.mnTestTimeOutSec);

        mConfigINI.SetIniKeyValue(L"MAIN",L"TFTPDIPAddr",sMultipleConfig.mstrTFTPDIPAddr.c_str());
        //mConfigINI.SetIniKeyValue(L"MAIN",L"Tftpd32ConfigIni",sMultipleConfig.mstrTftpd32ConfigIni.c_str());
        //mConfigINI.SetIniKeyValue(L"MAIN",L"SaveTestLogFolder", sMultipleConfig.mstrSaveTestLogFolder);

        std::wstring strProgEmmcFirehoseLitembnFilePath(sMultipleConfig.mstrBinaryImagePath);
        JGW_ReplaceStringW(strProgEmmcFirehoseLitembnFilePath,JGW_GetApplicationFolder(),L"");
        mConfigINI.SetIniKeyValue(L"MAIN",L"BinaryImagePath",strProgEmmcFirehoseLitembnFilePath.c_str());
        mConfigINI.SetIniKeyValue(L"MAIN",L"UpgradeVersion",sMultipleConfig.mstrUpgradeVersion.c_str());
        mConfigINI.SetIniKeyValue(L"MAIN",L"HostIPAddr",sMultipleConfig.msNetCardConfig.mstrHostIPAddr.c_str());

        mConfigINI.SetIniKeyValue(L"MAIN",L"TSE_Param",sMultipleConfig.mstrConfigTSEParam.c_str());
        for (std::map<std::wstring,std::wstring>::iterator it = sMultipleConfig.mmapTSEParamValues.begin();
            it != sMultipleConfig.mmapTSEParamValues.end();++it)
        {
            mConfigINI.SetIniKeyValue(L"MAIN",it->first.c_str(),it->second.c_str());
        }
    }
}
