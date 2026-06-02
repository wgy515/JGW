#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelpV2/JaGuarWaveHelpDefineV2.h>
#include "CJGW_FtpClient.h"

namespace JGW
{
    class CCJGW_InstallThread : public CCMessageThread
    {
    public:
        CCJGW_InstallThread(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_InstallThread(void);
      
    private: //! thread msg
        void OnStartDownloadThread(WPARAM wParam,LPARAM lParam);
    private: //!
        //! 首先下载FTP更新的配置文件
        bool DownloadFTPInstallUpdateConfig(CCJGW_FtpClient& ftpClient);
        //! 下载FTP版本文件
        bool DownloadFTPVersionFile(CCJGW_FtpClient& ftpClient);
    private:
        //! 
        void LoadFTPUpdateConfig(const std::wstring& strLocalUpdateConfigPath);
        //! 
        void LoadFTPInstallConfig(const std::wstring& strLocalInstallConfigPath);
    protected:   
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        S_JGW_HELP_CONFIG& msHelpConfig;
        LPS_JGW_HELP_DOWNLOAD_THREAD_PARAM mpsDownloadThreadParam;
    };
}