#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include "CJGW_FtpClient.h"
namespace JGW
{
    class CCJGW_UpdateThread : public CCMessageThread
    {
    public:
        CCJGW_UpdateThread(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_UpdateThread(void);
    private:
        void OnStartUpdateThread(WPARAM wParam,LPARAM lParam);
    private:
        //! 首先下载FTP更新的配置文件
        bool DownloadUpdateConfig(CCJGW_FtpClient& ftpClient);
        //! 下载UPDATE PACK
        bool DownloadUpdatePackFile(CCJGW_FtpClient& ftpClient);
    private:
        //! 
        void LoadFTPUpdateConfig(const std::wstring& strLocalUpdateConfigPath);
    protected:   
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        S_JGW_HELP_CONFIG& msHelpConfig;
    };
}