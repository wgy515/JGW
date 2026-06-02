#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include "CJGW_FtpClient.h"
#include "CJGW_SoftwareConfigFTPDownload.h"

namespace JGW
{
    class CCJGW_UpdateServiceThread;

    class CCJGW_SoftwareDownloadThread : public CCMessageThread
    {
    public:
        CCJGW_SoftwareDownloadThread(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_SoftwareDownloadThread(void);
    private:
        //! 软件版本更新，分为安装包和更新包更新
        void OnCheckSoftwareVersionUpdate(WPARAM wParam,LPARAM lParam);
    private: //! 更新包检测
        void OnStartCheckUpdateThread(CCJGW_FtpClient& ftpClient,LPARAM lParam);
        bool FTPDownloadUpdatePack(CCJGW_FtpClient& ftpClient,S_JGW_HELP_UPDATE& sUpdate);
        bool CheckSoftwareVersionIsUpdate(S_JGW_HELP_UPDATE& sUpdate);
    private: //! 安装包更新检测
        void OnStartCheckDownloadThread(CCJGW_FtpClient& ftpClient,LPARAM lParam);
        //! true 需要下载， false:不需要下载软件版本
        bool CheckSoftwareVersionIsIntall(S_JGW_HELP_INSTALL& sInstall);
        bool FTPDownloadInstallSoftware(CCJGW_FtpClient& ftpClient,S_JGW_HELP_INSTALL& sInstall);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbInstallUpdate; //! 是否更新安装包
        CCJGW_UpdateServiceThread* mpHelpServiceThread;
        S_JGW_HELP_CONFIG& msHelpConfig;
        CCJGW_SoftwareConfigFTPDownload mSoftwareConfigFTPDownload;  
    };
}


