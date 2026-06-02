#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include "CJGW_FtpClient.h"
#include "CJGW_SoftwareConfigFTPDownload.h"

namespace JGW
{
    class CCJGW_SoftwareUpdateThread : public CCMessageThread
    {
    public:
        CCJGW_SoftwareUpdateThread(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_SoftwareUpdateThread(void);
    private:
        void OnStartCheckUpdateThread(WPARAM wParam,LPARAM lParam);
        
        bool FTPDownloadUpdatePack(CCJGW_FtpClient& ftpClient,S_JGW_HELP_UPDATE& sUpdate);

        bool CheckSoftwareVersionIsUpdate(S_JGW_HELP_UPDATE& sUpdate);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        S_JGW_HELP_CONFIG& msHelpConfig;
        CCJGW_SoftwareConfigFTPDownload mSoftwareConfigFTPDownload;
    };

}