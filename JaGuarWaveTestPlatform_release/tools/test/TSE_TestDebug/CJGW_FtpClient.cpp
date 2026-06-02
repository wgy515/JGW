#include "StdAfx.h"
#include "CJGW_FtpClient.h"
#include <fstream>
namespace JGW
{
    CCJGW_FtpClient::CCJGW_FtpClient(void) : mpSession(NULL)
    {
    }


    CCJGW_FtpClient::~CCJGW_FtpClient(void)
    {
        CloseFTPClient();
    }

    bool CCJGW_FtpClient::IsOpenFTPClient()
    {
        if (NULL == mpSession) return false;
        return mpSession->isOpen() && mpSession->isLoggedIn();
    }

    void CCJGW_FtpClient::CloseFTPClient()
    {
        if (NULL == mpSession) return;

        try
        {
            mpSession->close();
        }
        catch (Poco::Net::FTPException& e)
        {
            //! std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
        }
        catch (Poco::Net::NetException& e1)
        {
            //! std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
        }

        delete mpSession;
        mpSession = NULL;
    }

    bool CCJGW_FtpClient::OpenFTPClient(const std::string& host, const std::string& username /* = "" */, const std::string& password /* = "" */, Poco::UInt16 port /* = Poco::Net::FTPClientSession::FTP_PORT */)
    {
        CloseFTPClient();
        try
        {
            mpSession = new Poco::Net::FTPClientSession(host,port,username,password);
        }
        catch (Poco::Net::FTPException& e)
        {
            //! std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
        }
        catch (Poco::Net::NetException& e1)
        {
            //! std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
        }
        catch (Poco::Exception& e2)
        {

        }
        return (NULL != mpSession);
    }

    bool CCJGW_FtpClient::DownloadFTPFile(const std::string& strFTPFilePath,std::ostream& downloadFileStream)
    {
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->setFileType(Poco::Net::FTPClientSession::TYPE_BINARY);
            std::istream& istr = mpSession->beginDownload(strFTPFilePath);
            Poco::StreamCopier::copyStream(istr, downloadFileStream);
            mpSession->endDownload();
            return true;
        }
        catch (Poco::Net::FTPException& e)
        {
            //! std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
        }
        catch (Poco::Net::NetException& e1)
        {
            //! std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
        }
        catch (Poco::Exception& e2)
        {

        }
        return false;
    }

    bool CCJGW_FtpClient::DownloadFTPFile(const std::string& strFTPFilePath,const std::string& strDownloadFileSavePath)
    {
        std::ofstream downloadFileStream(strDownloadFileSavePath,std::ios::binary|std::ios::trunc);
        if(!downloadFileStream.is_open()) return false;

        return DownloadFTPFile(strFTPFilePath,downloadFileStream);
    }

    bool CCJGW_FtpClient::UpdateFTPFile(const std::string& strFTPFilePath,const std::string& strUpdateFilePath)
    {
        std::ifstream uploadFileStream(strUpdateFilePath,std::ios::in | std::ios::binary);
        if(!uploadFileStream.is_open()) return false;
        return UpdateFTPFile(strFTPFilePath,uploadFileStream);
    }

    bool CCJGW_FtpClient::UpdateFTPFile(const std::string& strFTPFilePath,std::istream& uploadFileStream)
    {
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->setFileType(Poco::Net::FTPClientSession::TYPE_BINARY);
            std::ostream& ostr = mpSession->beginUpload(strFTPFilePath);
            ostr << uploadFileStream.rdbuf();
            mpSession->endUpload();
            return true;
        }
        catch (Poco::Net::FTPException& e)
        {
            std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
        }
        catch (Poco::Net::NetException& e1)
        {
            //! std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
        }
        catch (Poco::Exception& e2)
        {

        }
        return false;
    }

    bool CCJGW_FtpClient::CreateFTPDirectory(const std::string& strFTPDir)
    {
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->createDirectory(strFTPDir);
            return true;
        }
        catch (Poco::Net::FTPException& e)
        {
            //! Poco::Exception = {_msg="Cannot create directory 123\456: 550 Directory already exists." _pNested=0x00000000 _code=550 }
            std::cout<<"FTP ERROR:"<<e.displayText()<< std::endl;
        }
        catch (Poco::Net::NetException& e1)
        {
            //! std::cout<<"NET ERROR:"<<e1.displayText()<<std::endl;
        }
        catch (Poco::Exception& e2)
        {

        }
        return false;
    }
}