#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelpV2/CJaGuarWaveHelpV2Define.h>
#include "CJGW_FtpClient.h"
namespace JGW
{
    class CCJGW_UploadThread : public CCMessageThread
    {
    public:
        CCJGW_UploadThread(S_JGW_HELP_V2_CONFIG& sHelpV2Config);
        ~CCJGW_UploadThread(void);
    private:
        void OnStartUploadThread(WPARAM wParam,LPARAM lParam);
    private: 
        bool OnDownloadUploadConfigFTP(CCJGW_FtpClient& ftpClient);
        bool OnUploadLogFTP(CCJGW_FtpClient& ftpClient);
        bool OnUploadTestLogFTP(CCJGW_FtpClient& ftpClient);
        bool OnUploadCustomLogFTP(CCJGW_FtpClient& ftpClient);
    private:
        //! hostname_ip1_ip2
        std::string GetHostNameIPAddr();
   private:
        bool PackageLogFile(const std::wstring& strFolder,const std::wstring& strPackageLogFileNamePath);
        void UploadPackageLogFileFTP(CCJGW_FtpClient& ftpClient,const std::wstring& strFolder);
        void LoadUploadRecord(const std::wstring& strUploadRecordFilePath,std::map<std::wstring,std::wstring>& mapUploadIgnoreFilePath);
    private:
        void LoadFTPUploadConfig(const std::wstring& strUploadConfigFilePath);
    protected:   
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        S_JGW_HELP_V2_CONFIG& msHelpV2Config;
        std::wstring mstrTestPlatformExecFolder;
        S_JGW_HELP_UPLOAD_CONFIG msUploadConfig;
    };
}