#if 0
#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"
#include <string>
namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_CurlFtpDownload
    {
    public:
        CCJGW_CurlFtpDownload(const std::string& host,const std::string& username = "",const std::string& password = "",size_t nFtpPort = 22);
        ~CCJGW_CurlFtpDownload(void);
    public:
        void UpdateHostUserNamePassword(const std::string& host,const std::string& username = "",const std::string& password = "",size_t nFtpPort = 22);
        //!
        bool DownloadCurlFTPFile(const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath,size_t mnTimeOut = 10 * 60);
        //!
        bool UpdateCurlFTPFile(const std::string& strFTPFilePath,const std::string& strUpdateFilePath,size_t mnTimeOut = 10 * 60);
        //! 
        bool CheckCurlFTPFileExist(bool& bFileExist,const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath,size_t mnTimeOut = 10 * 60);
        //!
        bool TraverseCreateFTPDirectory(const std::string& strFTPDir);
        //! 
        bool CreateCurlFTPDirectory(const std::string& strFTPDir);
    private:
        bool ExecuteCurlCommand(const std::string& strArg,size_t mnTimeOut = 3 * 60);

        bool ExecuteCurlCommand(const std::string& strArg,std::wstring& strRead,size_t mnTimeOut = 3 * 60);
    private:
        size_t mnFtpPort;
        char mstrHost[128];
        char mstrUserName[128];
        char mstrPassword[128];
    };
}
#endif