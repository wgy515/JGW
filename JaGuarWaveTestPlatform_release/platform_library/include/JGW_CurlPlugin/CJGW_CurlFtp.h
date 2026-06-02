#pragma once
#include <string>
#include "JGW_CurlPluginDefine.h"
namespace JGW
{
    class JGW_CURLPLUGIN_CLASS CCJGW_CurlFtp
    {
    public:
        CCJGW_CurlFtp(const std::string& host,const std::string& username = "",const std::string& password = "",size_t nFtpPort = 22);
        ~CCJGW_CurlFtp(void);
    public:
        void UpdateHostUserNamePassword(const std::string& host,const std::string& username = "",const std::string& password = "",size_t nFtpPort = 22);
        //! 
        bool UploadCurlFtpFile(const std::wstring& strFTPFilePath,const std::wstring& strUploadFilePath,std::wstring& strErrorMsg,size_t mnTimeOut = 10 * 60);
        //!
        bool DownloadCurlFtpFile(const std::wstring& strFTPFilePath,const std::wstring& strDownloadFilePath,std::wstring& strErrorMsg,size_t mnTimeOut = 10 * 60);

        bool TraverseCreateFTPDirectory(const std::wstring& strFTPDirectory,std::wstring& strErrorMsg,size_t mnTimeOut = 60);

        //bool CreateDirectoryCurlFtpFile(const std::string& strFTPDirectory,std::string& strErrorMsg,size_t mnTimeOut = 60);
        //!
        bool CreateDirectoryCurlFtpFile(const std::wstring& strFTPDirectory,std::wstring& strErrorMsg,size_t mnTimeOut = 60);
        //!
        bool CheckPathCurlFtpExist(const std::wstring& strFTPPath,std::wstring& strErrorMsg,size_t mnTimeOut = 60);
        //!
        bool GetCurlFtpFileTimeAndFileSize(const std::wstring& strFTPPath,std::wstring& strErrorMsg,long& lFileTime,double& dFileSize,size_t mnTimeOut = 60);
    private:
        size_t mnFtpPort;
        char mstrHost[128];
        char mstrUserName[128];
        char mstrPassword[128];
    };
}