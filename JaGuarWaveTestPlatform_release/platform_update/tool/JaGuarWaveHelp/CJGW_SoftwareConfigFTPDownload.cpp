#include "StdAfx.h"
#include "CJGW_SoftwareConfigFTPDownload.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_SoftwareConfigFTPDownload::CCJGW_SoftwareConfigFTPDownload(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig)
    {
    }


    CCJGW_SoftwareConfigFTPDownload::~CCJGW_SoftwareConfigFTPDownload(void)
    {
    }

    bool CCJGW_SoftwareConfigFTPDownload::DownloadSoftwareInstallConfigFTP(CCJGW_FtpClient& ftpClient)
    {
        std::wstring strInstallConfigFTPPath,strInstallConfigLocalPath;
        JGW_GetModuleWorkPath(strInstallConfigLocalPath);
        strInstallConfigLocalPath += L"install.xml";
        JGW_FormatWString(strInstallConfigFTPPath,L"%s/Install/install.xml",msHelpConfig.mstrFTPRootPath.c_str());
        msHelpConfig.msInstallXMLConfig.mvstrInstallInfo.clear();
        msHelpConfig.msInstallXMLConfig.msNewInstallConfig.mstrVersion = L"";

        if (!ftpClient.DownloadFTPFile(strInstallConfigFTPPath,strInstallConfigLocalPath))
        {
            PrintLogW(L"Download FTP Install Config File Fail(%s)",strInstallConfigFTPPath.c_str());
            JGW_DeleteFile(strInstallConfigLocalPath);
            return false;
        }

        if (!mInstallXmlConfig.LoadInstallXmlConfig(strInstallConfigLocalPath,msHelpConfig.msInstallXMLConfig))
        {
            PrintLogW(L"Load Install Config File Fail(%s)",strInstallConfigLocalPath.c_str());
            JGW_DeleteFile(strInstallConfigLocalPath);
            return false;
        }
        return true;
    }

    bool CCJGW_SoftwareConfigFTPDownload::DownloadSoftwareUpdateConfigFTP(CCJGW_FtpClient& ftpClient)
    {
        std::wstring strUpdateConfigFTPPath,strUpdateConfigLocalPath;
        JGW_FormatWString(strUpdateConfigFTPPath,L"%s/Update/%s/update.xml",msHelpConfig.mstrFTPRootPath.c_str(),msHelpConfig.msDownloadThreadParam.mstrTaskNumber.c_str());
        JGW_GetModuleWorkPath(strUpdateConfigLocalPath);
        strUpdateConfigLocalPath += L"update.xml";
        msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.clear();
        msHelpConfig.msUpdateXMLConfig.mbVersieLimiet = false;

        if (!ftpClient.DownloadFTPFile(strUpdateConfigFTPPath,strUpdateConfigLocalPath))
        {
            PrintLogW(L"Download FTP Update Config File Fail(%s)",strUpdateConfigFTPPath.c_str());
            JGW_DeleteFile(strUpdateConfigLocalPath);
            return false;
        }

        if (!mUpdateXmlConfig.LoadUpdateXmlConfig(strUpdateConfigLocalPath,msHelpConfig.msUpdateXMLConfig))
        {
            PrintLogW(L"Load Update Config File Fail(%s)",strUpdateConfigLocalPath.c_str());
            JGW_DeleteFile(strUpdateConfigLocalPath);
            return false;
        }
        return true;
    }
}