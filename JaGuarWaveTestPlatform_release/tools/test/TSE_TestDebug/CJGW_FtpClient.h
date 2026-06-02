#pragma once
#include <Poco/Net/FTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>

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
        bool DownloadFTPFile(const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath);
        //! 下载FTP文件
        bool DownloadFTPFile(const std::string& strFTPFilePath,std::ostream& downloadFileStream);
        //!
        bool UpdateFTPFile(const std::string& strFTPFilePath,const std::string& strUpdateFilePath);
        //!
        bool UpdateFTPFile(const std::string& strFTPFilePath,std::istream& uploadFileStream);
        //! 
        bool CreateFTPDirectory(const std::string& strFTPDir);
        //!
        Poco::Net::FTPClientSession* GetSession() { return mpSession; }

        //! 关闭FTP客户端
        void CloseFTPClient();
    private:
        bool IsOpenFTPClient();
    private:
        Poco::Net::FTPClientSession* mpSession;
    };
}