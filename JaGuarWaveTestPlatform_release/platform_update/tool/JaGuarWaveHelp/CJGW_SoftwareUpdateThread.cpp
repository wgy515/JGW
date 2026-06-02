#include "StdAfx.h"
#include "CJGW_SoftwareUpdateThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGW_SoftwareUpdateThread::CCJGW_SoftwareUpdateThread(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig),mSoftwareConfigFTPDownload(sHelpConfig)
    {
    }


    CCJGW_SoftwareUpdateThread::~CCJGW_SoftwareUpdateThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_SoftwareUpdateThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_REQ_UPDATE_MSG,&CCJGW_SoftwareUpdateThread::OnStartCheckUpdateThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_SoftwareUpdateThread::OnStartCheckUpdateThread(WPARAM wParam,LPARAM lParam)
    {
        CCJGW_FtpClient ftpClient;
        S_JGW_HELP_UPDATE sUpdateInfo;
        if (!ftpClient.OpenFTPClient(msHelpConfig.mstrFTPHost,msHelpConfig.mstrUserName,msHelpConfig.mstrUserPassword,msHelpConfig.mnFTPPort))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-2,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"FTP无法连接\"}";
            goto END;
        }

        //! 下载软件安装配置FTP文件
        if (!mSoftwareConfigFTPDownload.DownloadSoftwareInstallConfigFTP(ftpClient))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"安装配置文件下载失败\"}";
            goto END;
        }
        //! 下载软件更新配置FTP文件  下载失败，则说明不需要更新
        if (!mSoftwareConfigFTPDownload.DownloadSoftwareUpdateConfigFTP(ftpClient))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新配置文件下载失败\"}";
            goto END;
        }
        //! false 表示不需要更新
        if (!CheckSoftwareVersionIsUpdate(sUpdateInfo))
        {
            goto END;
        }
        //! 下载更新包文件
        if (!FTPDownloadUpdatePack(ftpClient,sUpdateInfo))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新包文件下载失败,请检查网络连接情况\"}";
            goto END;
        }
END:
        ftpClient.CloseFTPClient();
        PostMessage(gMainHwnd,WM_HWND_OPEN_DOWNLOAD_HELP_TIMER_MSG,NULL,NULL);
    }

    bool CCJGW_SoftwareUpdateThread::CheckSoftwareVersionIsUpdate(S_JGW_HELP_UPDATE& sUpdate)
    {
        for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.begin();
            it != msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.end();
            ++ it)
        {
            //! 如果安装版本一致，则检测更新版本
            if (NULL == msHelpConfig.msVersionConfig.mstrInstallVersion.compare(it->mstrInstallVersion))
            {
                //! 如果更新版本不一致，则提示需要更新
                if (NULL != msHelpConfig.msVersionConfig.mstrUpdateVersion.compare(it->msUpdateInfo.mstrVersion))
                {
                    sUpdate = it->msUpdateInfo;
                    return true;
                }
            }
        }

        //! 默认不需要更新
        JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\",\"version\":\"%s\"}",msHelpConfig.msVersionConfig.mstrUpdateVersion.c_str());
        return false;
    }

    bool CCJGW_SoftwareUpdateThread::FTPDownloadUpdatePack(CCJGW_FtpClient& ftpClient,S_JGW_HELP_UPDATE& sUpdate)
    {
        //! 检查更新文件是否存在，
        if (JGW_FileExistsToFilePath(_T(DEFAULT_UPDATE_FILE_PATH)))
        {
            std::string strMD5 = CCJGW_CryptFileMD5::GetFileMD5A(DEFAULT_UPDATE_FILE_PATH);
            //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
            if (NULL == JGW_StrComparenoCaseStr(strMD5.c_str(),sUpdate.mstrUpdatePackMD5.c_str()))
            {
                JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\",\"version\":\"%s\"}",sUpdate.mstrVersion.c_str());
                return true;
            }
            JGW_DeleteFile(_T(DEFAULT_UPDATE_FILE_PATH));
        }

        std::string strUpdatePackFTPPath;
        JGW_FormatString(strUpdatePackFTPPath,"%s/Update/%s/%s",msHelpConfig.mstrFTPRootPath.c_str(),msHelpConfig.msDownloadThreadParam.mstrTaskNumber.c_str(),sUpdate.mstrUpdatePack.c_str());
        if (!ftpClient.DownloadFTPFile(strUpdatePackFTPPath,DEFAULT_UPDATE_FILE_PATH))
        {
            PrintLog("FTP Download UpdatePack File Fail(%s)",strUpdatePackFTPPath.c_str());
            JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
            return false;
        }

        std::string strMD5 = CCJGW_CryptFileMD5::GetFileMD5A(DEFAULT_UPDATE_FILE_PATH);
        //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
        if (NULL == JGW_StrComparenoCaseStr(strMD5.c_str(),sUpdate.mstrUpdatePackMD5.c_str()))
        {
            JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\",\"version\":\"%s\"}",sUpdate.mstrVersion.c_str());
            return true;
        }
        JGW_DeleteFile(_T(DEFAULT_UPDATE_FILE_PATH));
        return false;
    }
}