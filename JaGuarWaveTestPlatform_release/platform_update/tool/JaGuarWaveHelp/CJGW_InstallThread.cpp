#include "StdAfx.h"
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_InstallThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_InstallThread::CCJGW_InstallThread(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig),mpsDownloadThreadParam(NULL)
    {
        m_nThreadSleepTimeInterval = (2000);
    }


    CCJGW_InstallThread::~CCJGW_InstallThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_InstallThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_REQ_DOWNLOAD_MSG,&CCJGW_InstallThread::OnStartDownloadThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_InstallThread::OnStartDownloadThread(WPARAM wParam,LPARAM lParam)
    {
        mpsDownloadThreadParam = (LPS_JGW_HELP_DOWNLOAD_THREAD_PARAM)wParam;
        CCJGW_UpdateServiceThread* pThread = (CCJGW_UpdateServiceThread*)mpsDownloadThreadParam->mpParam;

        CCJGW_FtpClient ftpClient;
        if (!ftpClient.OpenFTPClient(msHelpConfig.mstrFTPHost,msHelpConfig.mstrUserName,msHelpConfig.mstrUserPassword,msHelpConfig.mnFTPPort))
        {
            mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":-2,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"FTP无法连接\"}";
            goto END;
        }

        if (!DownloadFTPInstallUpdateConfig(ftpClient))
        {
            mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新配置文件下载失败\"}";
            goto END;
        }

        if (!DownloadFTPVersionFile(ftpClient))
        {
            mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新版本文件下载失败\"}";
            goto END;
        }
#if 0
        mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"false\",\"bUpdate\":\"true\",\"error_msg\":\"请求线程下载检测命令失败\"}";
        Sleep(5 * 1000);
        pThread->OnRspDownloadThreadResultMsg(WPARAM(mpsDownloadThreadParam),LPARAM(1));
#endif
END:
        ftpClient.CloseFTPClient();
        pThread->OnRspDownloadThreadResultMsg(WPARAM(mpsDownloadThreadParam),LPARAM(1));
        //! delete mpsDownloadThreadParam;
        /*if (!msHelpConfig.mbOpenDownLoadIntervalTimer)*/ PostMessage(gMainHwnd,WM_HWND_OPEN_DOWNLOAD_HELP_TIMER_MSG,NULL,NULL);
    }

    bool CCJGW_InstallThread::DownloadFTPInstallUpdateConfig(CCJGW_FtpClient& ftpClient)
    {
        std::string strFTPUpdateConfigPath(msHelpConfig.mstrFTPRootPath),strFTPInstallConfigPath(msHelpConfig.mstrFTPRootPath);
        strFTPUpdateConfigPath += "/Update/";
        strFTPUpdateConfigPath += mpsDownloadThreadParam->mstrTaskNumber;
        strFTPUpdateConfigPath += "/update.ini";
        strFTPInstallConfigPath += "/Install/install.ini";

        std::string strLocalUpdateConfigPath,strLocalInstallConfigPath;
        JGW_GetModuleWorkPathA(strLocalUpdateConfigPath);
        strLocalInstallConfigPath = strLocalUpdateConfigPath;
        strLocalUpdateConfigPath += "update.ini";
        strLocalInstallConfigPath += "install.ini";

        if (!ftpClient.DownloadFTPFile(strFTPUpdateConfigPath,strLocalUpdateConfigPath))
        {
            PrintLog("Download FTP Update Config File Fail(%s)",strFTPUpdateConfigPath.c_str());
            JGW_DeleteFile(JGW_A2W(strLocalUpdateConfigPath));
        }
        LoadFTPUpdateConfig(JGW_A2W(strLocalUpdateConfigPath));
        //! 如果为空，则直接下载安装文件
        if (msHelpConfig.msUpdateConfig.mstrInstallVersion.empty())
        {
            if (!ftpClient.DownloadFTPFile(strFTPInstallConfigPath,strLocalInstallConfigPath))
            {
                PrintLog("Download FTP Install Config File Fail(%s)",strFTPInstallConfigPath.c_str());
                JGW_DeleteFile(JGW_A2W(strLocalInstallConfigPath));
                return false;
            }
            LoadFTPInstallConfig(JGW_A2W(strLocalInstallConfigPath));
            msHelpConfig.msUpdateConfig.mstrInstallVersion = msHelpConfig.msInstallConfig.mstrNewsInstallVersion;
            msHelpConfig.msUpdateConfig.mstrInstallFileName = msHelpConfig.msInstallConfig.mstrNewsInstallFTPFileName;
            msHelpConfig.msUpdateConfig.mstrInstallFileMD5 = msHelpConfig.msInstallConfig.mstrNewsInstallMD5;
        } //! 否则则匹配版本是否一致不一致，则下载当前指定版本
        else
        {

        }
        return true;
    }

    void CCJGW_InstallThread::LoadFTPUpdateConfig(const std::wstring& strLocalUpdateConfigPath)
    {
        CCJGW_ConfigIni updateConfigIni;
        updateConfigIni.InitIniFilePath(strLocalUpdateConfigPath.c_str());
        msHelpConfig.msUpdateConfig.mstrInstallVersion = updateConfigIni.GetIniKeyValue(L"install",L"version");
        msHelpConfig.msUpdateConfig.mstrInstallFileName = updateConfigIni.GetIniKeyValueA(L"install",L"filename");
        msHelpConfig.msUpdateConfig.mstrInstallFileMD5 = updateConfigIni.GetIniKeyValue(L"install",L"md5");

        msHelpConfig.msUpdateConfig.mstrUpdateVersion = updateConfigIni.GetIniKeyValue(L"update",L"version");
        msHelpConfig.msUpdateConfig.mstrUpdatePack = updateConfigIni.GetIniKeyValueA(L"update",L"updatepack");
        msHelpConfig.msUpdateConfig.mstrUpdatePackMD5 = updateConfigIni.GetIniKeyValue(L"update",L"updatepackmd5");
        //JGW_MakeWstringToUpper(msHelpConfig.msUpdateConfig.mstrUpdatePackMD5);
    }

    void CCJGW_InstallThread::LoadFTPInstallConfig(const std::wstring& strLocalInstallConfigPath)
    {
        CCJGW_ConfigIni installConfigIni;
        installConfigIni.InitIniFilePath(strLocalInstallConfigPath.c_str());

        msHelpConfig.msInstallConfig.mstrNewsInstallVersion = installConfigIni.GetIniKeyValue(L"news_install",L"version");
        msHelpConfig.msInstallConfig.mstrNewsInstallFTPFileName = installConfigIni.GetIniKeyValueA(L"news_install",L"filename");
        msHelpConfig.msInstallConfig.mstrNewsInstallMD5 = installConfigIni.GetIniKeyValue(L"news_install",L"md5");
        //JGW_MakeWstringToUpper(msHelpConfig.msInstallConfig.mstrNewsInstallMD5);
    }

    bool CCJGW_InstallThread::DownloadFTPVersionFile(CCJGW_FtpClient& ftpClient)
    {
        //! 如果版本一致，则直接返回true表示不需要更新
        if (NULL == msHelpConfig.msUpdateConfig.mstrInstallVersion.compare(msHelpConfig.msVersionConfig.mstrInstallVersion))
        {
            mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}";
            return true;
        }

        //! 如果不一致则首先检查更新下载文件是否已经下载
        if (JGW_FileExistsToFilePath(_T(DEFAULT_INSTALL_FILE_PATH)))
        {
            std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(_T(DEFAULT_INSTALL_FILE_PATH));
            //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
            if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),msHelpConfig.msUpdateConfig.mstrInstallFileMD5.c_str()))
            {
                mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\"}";
                return true;
            }
            JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
        }

        std::string strFTPInstallFilePath(msHelpConfig.mstrFTPRootPath);
        strFTPInstallFilePath += "/Install/";
        strFTPInstallFilePath += msHelpConfig.msUpdateConfig.mstrInstallFileName;

        if (!ftpClient.DownloadFTPFile(strFTPInstallFilePath,DEFAULT_INSTALL_FILE_PATH))
        {
            PrintLog("Download FTP Install File Fail(%s)",strFTPInstallFilePath.c_str());
            JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
            return false;
        }

        std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(_T(DEFAULT_INSTALL_FILE_PATH));
        //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
        if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),msHelpConfig.msUpdateConfig.mstrInstallFileMD5.c_str()))
        {
            mpsDownloadThreadParam->mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\"}";
            return true;
        }

        return false;
    }
}