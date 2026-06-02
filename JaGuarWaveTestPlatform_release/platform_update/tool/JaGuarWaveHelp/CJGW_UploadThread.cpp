#include "StdAfx.h"
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_UploadThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <poco/Path.h>
#include <poco/File.h>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_UploadThread::CCJGW_UploadThread(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig),mstrTestPlatformExecFolder(L"C:\\JW\\exec\\")
    {
        m_nThreadSleepTimeInterval = 2000;
    }


    CCJGW_UploadThread::~CCJGW_UploadThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UploadThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_UPLOAD_LOG_MSG,&CCJGW_UploadThread::OnStartUploadThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UploadThread::OnStartUploadThread(WPARAM wParam,LPARAM lParam)
    {
        CCJGW_UpdateServiceThread* pThread = (CCJGW_UpdateServiceThread*)msHelpConfig.msDownloadThreadParam.mpParam;
        CCJGW_FtpClient ftpClient;
        mstrTestPlatformExecFolder = msHelpConfig.msDownloadThreadParam.mstrExecFolder;
        if (mstrTestPlatformExecFolder.empty()) return ;
        //JGW_ReplaceSlash(const_cast<wchar_t*>(mstrTestPlatformExecFolder.data()));
        JGW_ReplaceStringW(mstrTestPlatformExecFolder,L"/",L"\\");
        if (L'\\' != mstrTestPlatformExecFolder[mstrTestPlatformExecFolder.length() - 1]) mstrTestPlatformExecFolder += L"\\";

        if (!ftpClient.OpenFTPClient(msHelpConfig.mstrFTPHost,msHelpConfig.mstrUserName,msHelpConfig.mstrUserPassword,msHelpConfig.mnFTPPort))
        {
            msHelpConfig.mnUploadIntervalTimeSec = 2 * 1000;
            goto UPLOAD_END;
        }

        if (!OnDownloadUploadConfigFTP(ftpClient))
        {
            goto UPLOAD_END;
        }
        //! 如果不上传log 则每隔一段时间查询下更新log时间
        if (!msUploadConfig.mbUploadLog && !msUploadConfig.mbUploadTestLog && msUploadConfig.mstrCustomDir.empty())
        {
            msHelpConfig.mnUploadIntervalTimeSec = 2 * 1000;
            goto UPLOAD_END;
        }

        if (msUploadConfig.mbUploadLog)
        {
            OnUploadLogFTP(ftpClient);
        }

        if (msUploadConfig.mbUploadTestLog)
        {
            OnUploadTestLogFTP(ftpClient);
        }

        if (!msUploadConfig.mstrCustomDir.empty())
        {
            OnUploadCustomLogFTP(ftpClient);
        }
UPLOAD_END:
        ftpClient.CloseFTPClient();
        PostMessage(gMainHwnd,WM_HWND_OPEN_UPLOADLOG_HELP_TIMER_MSG,NULL,NULL);
    }

    bool CCJGW_UploadThread::OnDownloadUploadConfigFTP(CCJGW_FtpClient& ftpClient)
    {
        std::wstring strFTPUploadConfigPath(msHelpConfig.mstrFTPLogRootPath);
        strFTPUploadConfigPath += L"/upload.ini";

        std::wstring strLocalUpdateConfigPath(JGW_GetApplicationFolder());
        strLocalUpdateConfigPath += L"upload.ini";
        //! 下载log 更新配置文件
        if (!ftpClient.DownloadFTPFile(strFTPUploadConfigPath,strLocalUpdateConfigPath))
        {
            PrintLogW(L"Download FTP Upload Config File Fail(%s)",strFTPUploadConfigPath.c_str());
            JGW_DeleteFile(strLocalUpdateConfigPath);
            msHelpConfig.mnUploadIntervalTimeSec = 2 * 1000;
            return false;
        }
        LoadFTPUploadConfig(strLocalUpdateConfigPath);
        return true;
    }

    void CCJGW_UploadThread::LoadFTPUploadConfig(const std::wstring& strUploadConfigFilePath)
    {
        CCJGW_ConfigIni uploadConfigIni;
        uploadConfigIni.InitIniFilePath(strUploadConfigFilePath.c_str());

        msHelpConfig.mnUploadIntervalTimeSec = uploadConfigIni.GetIniKeyIntValue(L"Time",L"UploadIntervalTimeSec",5 * 60);
        msUploadConfig.mbUploadLog = uploadConfigIni.GetIniKeyBoolValue(L"Log",L"upload");
        msUploadConfig.mbUploadTestLog = uploadConfigIni.GetIniKeyBoolValue(L"TestLog",L"upload");
        msUploadConfig.mstrCustomDir = uploadConfigIni.GetIniKeyValue(L"custom",L"dir");
    }

    bool CCJGW_UploadThread::OnUploadLogFTP(CCJGW_FtpClient& ftpClient)
    {
        UploadPackageLogFileFTP(ftpClient,L"log");
        return true;
    }

    bool CCJGW_UploadThread::OnUploadTestLogFTP(CCJGW_FtpClient& ftpClient)
    {
        UploadPackageLogFileFTP(ftpClient,L"TestLog");
        return true;
    }

    bool CCJGW_UploadThread::OnUploadCustomLogFTP(CCJGW_FtpClient& ftpClient)
    {
        UploadPackageLogFileFTP(ftpClient,msUploadConfig.mstrCustomDir);
        return true;
    }

    void CCJGW_UploadThread::UploadPackageLogFileFTP(CCJGW_FtpClient& ftpClient,const std::wstring& strFolder)
    {
        //! 防止打包失败后的文件失效
        std::wstring strPackageLogFilePath/*(msHelpConfig.msDownloadThreadParam.mstrExecFolder + "\\upload_log.zip")*/;
        std::wstring strUploadFilePathFTP,strUploadFolderFTP,strUploadFolder(strFolder);
        bool bPackageResult = true;
        //! 如果不是相对路径，则将此名称改为custom 表示是客户自定义上传的目录
        if (!JGW_PathIsRelativeW(strFolder.c_str())) strUploadFolder = L"custom";

        JGW_FormatWString(strPackageLogFilePath,L"%s\\%s_update.zip",msHelpConfig.msDownloadThreadParam.mstrExecFolder.c_str(),strUploadFolder.c_str());
        //! 检查打包上传的log文件是否存在，如果存在则直接上传，否则进行log打包
        //! 如果打包文件为空则说明打包失败，则直接返回
        if (!JGW_FileExistsToFilePath(strPackageLogFilePath.c_str()) && !PackageLogFile(strFolder,strPackageLogFilePath))
        {
            PrintLogW(L"Package Log File Fail(%s),Update File Is Empty",strFolder.c_str());
            JGW_DeleteFile(strPackageLogFilePath);
            return ;
        }

        //! 按照规则生成上传路径
        JGW_FormatWString(strUploadFilePathFTP,L"%s/%s/%s/%s_%s_%d.zip",msHelpConfig.mstrFTPLogRootPath.c_str(),JGW_GetTimeFolder().c_str(),JGW_A2W(GetHostNameIPAddr()).c_str(),strUploadFolder.c_str(),msHelpConfig.msDownloadThreadParam.mstrTaskNumber.c_str(),JGW_GetTimeOfDay());
        JGW_FormatWString(strUploadFolderFTP,L"%s/%s/%s/",msHelpConfig.mstrFTPLogRootPath.c_str(),JGW_GetTimeFolder().c_str(),JGW_A2W(GetHostNameIPAddr()).c_str());
        //! 直接上传FTP文件，如果上传失败，则检测目录是否未创建，或者创建失败
        if (!ftpClient.UpdateFTPFile(strUploadFilePathFTP,strPackageLogFilePath))
        {
            PrintLogW(L"Update FTP Package Log File Fail(%s => %s)",strPackageLogFilePath.c_str(),strUploadFilePathFTP.c_str());
            //! 创建FTP目录
            if (!ftpClient.CreateFTPDirectory(strUploadFolderFTP))
            {
                ftpClient.TraverseCreateFTPDirectory(strUploadFolderFTP);
            }
            //! 重新上传FTP文件 并且上传完成后删除FTP文件
            if (ftpClient.UpdateFTPFile(strUploadFilePathFTP,strPackageLogFilePath)) JGW_DeleteFile(strPackageLogFilePath);   
        }
        else
        {
            JGW_DeleteFile(strPackageLogFilePath);
        }
    }

    std::string CCJGW_UploadThread::GetHostNameIPAddr()
    {
        JGW::CCJGW_SyncSocket syncSocket;
        std::string host,strHostNameIPAddr;
        std::vector<std::string> vListAddr;
        syncSocket.GetHostIP(host,vListAddr);
        for (std::vector<std::string>::iterator it = vListAddr.begin();
            it != vListAddr.end();++ it) JGW_FormatString(strHostNameIPAddr,"%s_%s",host.c_str(),it->c_str());
            JGW_ReplaceStringA(strHostNameIPAddr,":","");
        return strHostNameIPAddr;
    }

    void ScanPuamThroughputTestResultFile(const char* filepath,const char* filename,void* lpData)
    {
        LPS_UPLOAD_FILEPATH_PARAM psUploadFilePathParam = (LPS_UPLOAD_FILEPATH_PARAM)lpData;
        std::string strFilePathTemp(filepath);
        strFilePathTemp += "\\";
        strFilePathTemp += filename;
        psUploadFilePathParam->mvUploadFilePath[JGW_A2W(strFilePathTemp)] = JGW_A2W(CCJGW_CryptFileMD5::GetFileMD5A(strFilePathTemp));
    }

    bool CCJGW_UploadThread::PackageLogFile(const std::wstring& strFolder,const std::wstring& strPackageLogFileNamePath)
    {
        //! 处理目录相关信息
        std::wstring strFolderPath = JGW_RealPathToAbsPathBasePath(strFolder.c_str(),msHelpConfig.msDownloadThreadParam.mstrExecFolder.c_str()),strRealativeFolder(strFolder);
        strFolderPath = JGW_FolderReplaceWindowsFolderAddBackslash(strFolderPath);
        std::wstring strExecFolderPath (JGW_FolderReplaceWindowsFolderAddBackslash(msHelpConfig.msDownloadThreadParam.mstrExecFolder));

        if (!JGW_PathIsRelativeW(strFolder.c_str())) strRealativeFolder = L"custom";

        std::wstring strUploadRecordFilePath,strTemp;
        int numFiles = 0;
        S_UPLOAD_FILEPATH_PARAM sUploadFilePathParam;
        //! 已上传规则文件路径
        JGW_FormatWString(strUploadRecordFilePath,L"%s%s.upload.ignore",strExecFolderPath.c_str(),strRealativeFolder.c_str());
        //! 加载需要忽略上传的文件信息  =》 防止重复上传相同的文件
        LoadUploadRecord(strUploadRecordFilePath,sUploadFilePathParam.mmapUploadIgnoreFilePath);
        //! 扫描遍历当前所有文件
        std::wstring strTraverseFolder = JGW_PathReplaceWindowsPathRemoveBackslash(strFolderPath);
        JGW_TraverseFolder_C(JGW_W2A(strTraverseFolder),numFiles,"\\*.*",ScanPuamThroughputTestResultFile,&sUploadFilePathParam);
        //! 如果未遍历到文件说明是空目录不需要上传
        if (sUploadFilePathParam.mvUploadFilePath.empty()) return false;

        std::ofstream out(strPackageLogFileNamePath, std::ios::binary);
        numFiles = 0;
        //! 构造忽略规则文件
        std::wofstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(strUploadRecordFilePath,std::ios::out | ios::trunc);
        if (!inputFile.is_open()) return false;

        Poco::Zip::Compress c(out, true);
        for (std::map<std::wstring,std::wstring>::iterator it = sUploadFilePathParam.mvUploadFilePath.begin();
            it != sUploadFilePathParam.mvUploadFilePath.end();
            ++ it)
        {
            std::map<std::wstring,std::wstring>::iterator itmap = sUploadFilePathParam.mmapUploadIgnoreFilePath.find(it->first);
            //! 如果找到规则文件，则判断MD5是否一致如果一致则不上传
            if (itmap != sUploadFilePathParam.mmapUploadIgnoreFilePath.end())
            {
                //! 如果MD5一致 则继续遍历
                if (NULL == itmap->second.compare(it->second))
                {
                    inputFile << it->second << L" " << it->first << std::endl;
                    continue;
                }
            }
            strTemp = it->first;
            Poco::Path theFile(JGW_W2A(strTemp,CP_UTF8));
            JGW_ReplaceStringW(strTemp,strFolderPath,L"");
            Poco::Path fileName(JGW_W2A(strTemp,CP_UTF8));
            try
            {
                c.addFile(theFile, fileName);
                inputFile << it->second << L" " << it->first << std::endl;
                numFiles ++;
            }
            catch (Poco::Exception& e)
            {
                PrintLogW(L"Compress File(%s) Fail(%s)",it->first.c_str(),JGW_A2W_A(e.what(),CP_UTF8));
            }
        }
        try
        {
            Poco::Zip::ZipArchive a(c.close());
            if (0 == numFiles) return false;
            return true;
        }
        catch (Poco::Exception& e)
        {
            PrintLog("Compress File Fail(%s)",e.what());
        }
        return false;
    }

    void CCJGW_UploadThread::LoadUploadRecord(const std::wstring& strUploadRecordFilePath,std::map<std::wstring, std::wstring>& mapUploadIgnoreFilePath)
    {
        std::wifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(strUploadRecordFilePath);
        if (!inputFile.is_open()) return;

        wchar_t str[512] = {0};
        size_t len = 0;
        wchar_t szFileMD5[33] = {0};

        while (inputFile.getline(str,512))
        {
            memcpy_s(szFileMD5,32,str,32);
            szFileMD5[32] = L'\0';
            mapUploadIgnoreFilePath[&str[33]] = szFileMD5;
        }
        inputFile.close();
    }
}