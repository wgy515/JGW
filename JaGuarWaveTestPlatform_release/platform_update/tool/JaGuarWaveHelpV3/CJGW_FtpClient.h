#pragma once
#include <Poco/Net/FTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>

namespace JGW
{
    class CCJGW_FtpClient
    {
    public:
        CCJGW_FtpClient(void);
        ~CCJGW_FtpClient(void);
    public:
        //! 打开FTP客户端
        bool OpenFTPClient(const std::string& host,
            const std::string& username = "",
            const std::string& password = "",
            Poco::UInt16 port = Poco::Net::FTPClientSession::FTP_PORT);
        //! 下载FTP文件
        bool DownloadFTPFile(const std::wstring& strFTPFilePath,const std::wstring& strDownloadFileSavePath);
        
        //!
        bool UpdateFTPFile(const std::wstring& strFTPFilePath,const std::wstring& strUpdateFilePath);
 
        //! 
        bool CreateFTPDirectory(const std::wstring& strFTPDir);
        //!
        bool TraverseCreateFTPDirectory(const std::wstring& strFTPDir);

        //! 关闭FTP客户端
        void CloseFTPClient();
    private:
        //!
        //! bool CurlDownloadFTPFile();
        //! 下载FTP文件
        bool DownloadFTPFile(const std::wstring& strFTPFilePath,std::ostream& downloadFileStream);
        //!
        bool UpdateFTPFile(const std::wstring& strFTPFilePath,std::istream& uploadFileStream);
        //! 
        bool IsOpenFTPClient();
    private:
        Poco::Net::FTPClientSession* mpSession;
        CCJGW_CurlFtp mcCurlFtpDownload;
    };
}