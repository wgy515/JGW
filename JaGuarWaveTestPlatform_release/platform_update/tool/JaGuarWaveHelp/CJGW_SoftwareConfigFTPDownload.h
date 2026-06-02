#pragma once
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include "CJGW_FtpClient.h"
#include <JaGuarWaveHelp/CJGW_InstallXmlConfig.h>
#include <JaGuarWaveHelp/CJGW_UpdateXmlConfig.h>
namespace JGW
{
    class CCJGW_SoftwareConfigFTPDownload
    {
    public:
        CCJGW_SoftwareConfigFTPDownload(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_SoftwareConfigFTPDownload(void);
    public:
        //! 配置文件下载或者更新失败
        bool DownloadSoftwareInstallConfigFTP(CCJGW_FtpClient& ftpClient);
        //! 
        bool DownloadSoftwareUpdateConfigFTP(CCJGW_FtpClient& ftpClient);
    private:
        S_JGW_HELP_CONFIG& msHelpConfig; 
        CCJGW_InstallXmlConfig mInstallXmlConfig;
        CCJGW_UpdateXmlConfig mUpdateXmlConfig;
    };
}


