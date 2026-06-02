#include "StdAfx.h"
#include "CJGW_FtpClient.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#define TEST_CURL_FTP
namespace JGW
{
    CCJGW_FtpClient::CCJGW_FtpClient(void) : mpSession(NULL),mcCurlFtpDownload("")
    {
    }


    CCJGW_FtpClient::~CCJGW_FtpClient(void)
    {
        CloseFTPClient();
    }

    bool CCJGW_FtpClient::IsOpenFTPClient()
    {
#ifdef TEST_CURL_FTP
        return true;
#else
        if (NULL == mpSession) return false;
        bool bResult = false;
        try
        {
            bResult = mpSession->isOpen() && mpSession->isLoggedIn();
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("IsOpenFTPClient ERROR:%s\n",e2.displayText().c_str());
        }
        return bResult;
#endif
    }

    void CCJGW_FtpClient::CloseFTPClient()
    {
#ifdef TEST_CURL_FTP
        return ;
#else
        if (NULL == mpSession) return;

        try
        {
            mpSession->close();
        }
        catch (Poco::Net::FTPException& e)
        {
            PrintLog("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            PrintLog("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("CloseFTPClient ERROR:%s\n",e2.displayText().c_str());
        }

        delete mpSession;
        mpSession = NULL;
#endif
    }

    bool CCJGW_FtpClient::OpenFTPClient(const std::string& host, const std::string& username /* = "" */, const std::string& password /* = "" */, Poco::UInt16 port /* = Poco::Net::FTPClientSession::FTP_PORT */)
    {
        CloseFTPClient();
        try
        {
            //mstrHost = host;
            //mstrPassword = password;
            //mstrUserName = username;
            //mnFtpPort = port;
            mcCurlFtpDownload.UpdateHostUserNamePassword(host,username,password,port);
            Log4WI_F(L"FTP Host:%s,UserName:%s,Port:%d",JGW_A2W(host).c_str(),JGW_A2W(username).c_str(),port);
#ifdef TEST_CURL_FTP
            return true;
#endif
            mpSession = new Poco::Net::FTPClientSession(host,port,username,password);
            mpSession->setPassive(true,false);
        }
        catch (Poco::Net::FTPException& e)
        {
            PrintLog("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            PrintLog("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("OpenFTPClient ERROR:%s\n",e2.displayText().c_str());
        }
        return (NULL != mpSession);
    }

    bool CCJGW_FtpClient::DownloadFTPFile(const std::wstring& strFTPFilePath,std::ostream& downloadFileStream)
    {
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->setFileType(Poco::Net::FTPClientSession::TYPE_BINARY);
            std::istream& istr = mpSession->beginDownload(JGW_W2A(strFTPFilePath,CP_UTF8));
            Poco::StreamCopier::copyStream(istr, downloadFileStream);
            mpSession->endDownload();
            return true;
        }
        catch (Poco::Net::FTPException& e)
        {
            PrintLog("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            PrintLog("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("DownloadFTPFile ERROR:%s\n",e2.displayText().c_str());
        }

        return false;
    }

    bool CCJGW_FtpClient::DownloadFTPFile(const std::wstring& strFTPFilePath,const std::wstring& strDownloadFileSavePath)
    {
        std::wstring strErrorMsg;
#ifdef TEST_CURL_FTP
        Log4WI_F(L"FTP File Path:%s,Save File Path:%s",strFTPFilePath.c_str(),strDownloadFileSavePath.c_str());
        bool result = mcCurlFtpDownload.DownloadCurlFtpFile(strFTPFilePath,strDownloadFileSavePath,strErrorMsg);
        Log4WI_F(L"Curl FTP Download Msg:%s",strErrorMsg.c_str());
        return result;
#endif
        std::ofstream downloadFileStream(strDownloadFileSavePath,std::ios::binary|std::ios::trunc);
        if(!downloadFileStream.is_open()) return false;
        bool bResult = DownloadFTPFile(strFTPFilePath,downloadFileStream);
        downloadFileStream.flush();
        downloadFileStream.close();
        if (!bResult)
        {
            return mcCurlFtpDownload.DownloadCurlFtpFile(strFTPFilePath,strDownloadFileSavePath,strErrorMsg);
        }

        return bResult;
    }

    bool CCJGW_FtpClient::UpdateFTPFile(const std::wstring& strFTPFilePath,const std::wstring& strUpdateFilePath)
    {
        std::wstring strErrorMsg;
#ifdef TEST_CURL_FTP
        Log4WI_F(L"Update FTP File Path:%s,Local File Path:%s",strFTPFilePath.c_str(),strUpdateFilePath.c_str());
        bool result =  mcCurlFtpDownload.UploadCurlFtpFile(strFTPFilePath,strUpdateFilePath,strErrorMsg);
        Log4WI_F(L"Curl FTP Update Msg:%s",strErrorMsg.c_str());
        return result;
#endif
        std::ifstream uploadFileStream(strUpdateFilePath,std::ios::in | std::ios::binary);
        if(!uploadFileStream.is_open()) return false;
        if (!UpdateFTPFile(strFTPFilePath,uploadFileStream))
        {
            return mcCurlFtpDownload.UploadCurlFtpFile(strFTPFilePath,strUpdateFilePath,strErrorMsg);
        }
        return true;
    }

    bool CCJGW_FtpClient::UpdateFTPFile(const std::wstring& strFTPFilePath,std::istream& uploadFileStream)
    {
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->setFileType(Poco::Net::FTPClientSession::TYPE_BINARY);
            std::ostream& ostr = mpSession->beginUpload(JGW_W2A(strFTPFilePath,CP_UTF8));
            ostr << uploadFileStream.rdbuf();
            mpSession->endUpload();
            return true;
        }
        catch (Poco::Net::FTPException& e)
        {
            PrintLog("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            PrintLog("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("UpdateFTPFile ERROR:%s\n",e2.displayText().c_str());
        }
        return false;
    }

    bool CCJGW_FtpClient::CreateFTPDirectory(const std::wstring& strFTPDir)
    {
        std::wstring strErrorMsg;
#ifdef TEST_CURL_FTP
        Log4WI_F(L"Create Directory:%s",strFTPDir.c_str());
        bool result = mcCurlFtpDownload.CreateDirectoryCurlFtpFile(strFTPDir,strErrorMsg);
        Log4WI_F(L"Curl Create Directory Msg:%s",strErrorMsg.c_str());
        return result;
#endif
        if (!IsOpenFTPClient()) return false;
        try
        {
            mpSession->createDirectory(JGW_W2A(strFTPDir));
            return true;
        }
        //! Poco::Exception = {_msg="Cannot create directory 123\456: 550 Directory already exists." _pNested=0x00000000 _code=550 }
        catch (Poco::Net::FTPException& e)
        {
            PrintLog("FTP ERROR:%s\n",e.displayText().c_str());
        }
        catch (Poco::Net::NetException& e1)
        {
            PrintLog("NET ERROR:%s\n",e1.displayText().c_str());
        }
        catch (Poco::Exception& e2)
        {
            PrintLog("CreateFTPDirectory ERROR:%s\n",e2.displayText().c_str());
        }
        return mcCurlFtpDownload.CreateDirectoryCurlFtpFile(strFTPDir,strErrorMsg);
    }

    bool CCJGW_FtpClient::TraverseCreateFTPDirectory(const std::wstring& strFTPDir)
    {
        std::wstring strErrorMsg;
#ifdef TEST_CURL_FTP
        return mcCurlFtpDownload.TraverseCreateFTPDirectory(strFTPDir,strErrorMsg);
#endif
        wchar_t path[MAX_PATH] = {0};

        wchar_t cSaveChar;
        wcscpy_s(path, MAX_PATH, strFTPDir.c_str());
        JGW_PathAddBackslash(path);
        bool bResult = true;
        size_t i, nLen = wcslen(path);

        for (i = 2; i < nLen; i++)
        {
            if (JGW_IsPathSlash(path[i]))
            {
                cSaveChar = path[i];
                path[i] = 0;
                bResult = CreateFTPDirectory(path);
                path[i] = cSaveChar;
            }
        }
        return bResult;
    }
}