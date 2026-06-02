#pragma once
#include <string>
namespace JGW
{
    class CCJGW_CurlFtp
    {
    public:
        CCJGW_CurlFtp(const std::string& host,const std::string& username = "",const std::string& password = "",size_t nFtpPort = 22);
        ~CCJGW_CurlFtp(void);
    public:
        //! 
        bool UploadCurlFtpFile(const std::string& strFTPFilePath,const std::string& strUploadFilePath,std::string& strErrorMsg,size_t mnTimeOut = 10 * 60);
        //!
        bool DownloadCurlFtpFile(const std::string& strFTPFilePath,const std::string& strDownloadFilePath,std::string& strErrorMsg,size_t mnTimeOut = 10 * 60);
        //!
        bool CreateDirectoryCurlFtpFile(const std::string& strFTPDirectory,std::string& strErrorMsg,size_t mnTimeOut = 60);
        //!
        bool CheckPathCurlFtpExist(const std::string& strFTPPath,std::string& strErrorMsg,size_t mnTimeOut = 60);
    private:
        size_t mnFtpPort;
        char mstrHost[128];
        char mstrUserName[128];
        char mstrPassword[128];
    };
}