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
        bool bResult = false;
        try
        {
            bResult = mpSession->isOpen() && mpSession->isLoggedIn();
        }
        catch (Poco::Exception& e2)
        {
        	LogE_F("IsOpenFTPClient ERROR:%s\n",e2.displayText().c_str());
        }
        return bResult;
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
            LogE_F("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            LogE_F("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            LogE_F("CloseFTPClient ERROR:%s\n",e2.displayText().c_str());
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
			mpSession->setPassive(true,false);
        }
        catch (Poco::Net::FTPException& e)
        {
            LogE_F("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            LogE_F("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            LogE_F("OpenFTPClient ERROR:%s\n",e2.displayText().c_str());
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
            LogE_F("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            LogE_F("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            LogE_F("DownloadFTPFile ERROR:%s\n",e2.displayText().c_str());
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
            LogE_F("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            LogE_F("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            LogE_F("UpdateFTPFile ERROR:%s\n",e2.displayText().c_str());
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
        //! Poco::Exception = {_msg="Cannot create directory 123\456: 550 Directory already exists." _pNested=0x00000000 _code=550 }
        catch (Poco::Net::FTPException& e)
        {
            LogE_F("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            LogE_F("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            LogE_F("CreateFTPDirectory ERROR:%s\n",e2.displayText().c_str());
        }
        return false;
    }
}