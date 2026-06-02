#include "StdAfx.h"
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_SoftwareDownloadThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_SoftwareDownloadThread::CCJGW_SoftwareDownloadThread(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig),mSoftwareConfigFTPDownload(sHelpConfig),mpHelpServiceThread(NULL),mbInstallUpdate(false)
    {
    }


    CCJGW_SoftwareDownloadThread::~CCJGW_SoftwareDownloadThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_SoftwareDownloadThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_REQ_DOWNLOAD_MSG,&CCJGW_SoftwareDownloadThread::OnCheckSoftwareVersionUpdate)
    END_MYTHREAD_MESSAGE_MAP()


    void CCJGW_SoftwareDownloadThread::OnCheckSoftwareVersionUpdate(WPARAM wParam,LPARAM lParam)
    {
        CCJGW_FtpClient ftpClient;
        mbInstallUpdate = false;
        mpHelpServiceThread = (CCJGW_UpdateServiceThread*)msHelpConfig.msDownloadThreadParam.mpParam;

        if (!ftpClient.OpenFTPClient(msHelpConfig.mstrFTPHost,msHelpConfig.mstrUserName,msHelpConfig.mstrUserPassword,msHelpConfig.mnFTPPort))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-2,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"FTP无法连接\"}";
            goto CHECK_END;
        }

        //! 下载软件安装配置FTP文件
        if (!mSoftwareConfigFTPDownload.DownloadSoftwareInstallConfigFTP(ftpClient))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"安装配置文件下载失败\"}";
            goto CHECK_END;
        }

        //! 下载软件更新配置FTP文件  下载失败，则说明不需要更新
        if (!mSoftwareConfigFTPDownload.DownloadSoftwareUpdateConfigFTP(ftpClient)) PrintLog("更新配置文件下载失败");
        //! 请求检查安装包信息
        OnStartCheckDownloadThread(ftpClient,NULL);
        //! 处理线程更新的消息
        if (!mbInstallUpdate) OnStartCheckUpdateThread(ftpClient,NULL);
CHECK_END:
        ftpClient.CloseFTPClient();
        PostMessage(gMainHwnd,WM_HWND_OPEN_DOWNLOAD_HELP_TIMER_MSG,NULL,NULL);
    }

    void CCJGW_SoftwareDownloadThread::OnStartCheckDownloadThread(CCJGW_FtpClient& ftpClient,LPARAM lParam)
    {
        S_JGW_HELP_INSTALL sInstall;
        //! 检查软件版本更新机制
        if (!CheckSoftwareVersionIsIntall(sInstall))
        {
            goto DOWNLOAD_END;
        }
        //! 下载软件安装包
        if (!FTPDownloadInstallSoftware(ftpClient,sInstall))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"安装文件下载失败,请联系FAE\"}";
            mbInstallUpdate = true;
            goto DOWNLOAD_END;
        }
DOWNLOAD_END:
        mpHelpServiceThread->OnRspDownloadThreadResultMsg(WPARAM(&msHelpConfig.msDownloadThreadParam),NULL);
    }

    bool CCJGW_SoftwareDownloadThread::CheckSoftwareVersionIsIntall(S_JGW_HELP_INSTALL& sInstall)
    {
        //! 首先检查当前安装版本是否在更新版本包里面，如果在则表示不需要重新安装
        for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.begin();
            it != msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.end();
            ++ it)
        {
            //! 如果一致 则直接返回表示不需要更新
            if (NULL == it->mstrInstallVersion.compare(msHelpConfig.msVersionConfig.mstrInstallVersion))
            {
                msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}";
                return false;
            }
            //! 当前版本是最新版本，而且配置*，表示只需要更新配置即可
            if (NULL == it->mstrInstallVersion.compare(L"*") && NULL == msHelpConfig.msVersionConfig.mstrInstallVersion.compare(msHelpConfig.msInstallXMLConfig.msNewInstallConfig.mstrVersion))
            {
                msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}";
                return false;
            }
        }
        //! 如果不在最新的更新版本里面
        //! 不限制版本 1、检查当前的版本是否是最新版本，如果是最新版本，则返回
        if (!msHelpConfig.msUpdateXMLConfig.mbVersieLimiet)
        {
            if (NULL == msHelpConfig.msVersionConfig.mstrInstallVersion.compare(msHelpConfig.msInstallXMLConfig.msNewInstallConfig.mstrVersion))
            {
                msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}";
                return false;
            }
            sInstall = msHelpConfig.msInstallXMLConfig.msNewInstallConfig;
            return true;
        }

#if 1
        //! 3、如果限制版本，则下载package 里面默认的第一个版本,如果第一个版本未找到，则找第二个版本，
        //! 如果都没找到，则表明package中的版本已经被删除，程序直接报错
        for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.begin();
            it != msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.end();
            ++ it)
        {
            for (std::vector<S_JGW_HELP_INSTALL>::iterator itinstall = msHelpConfig.msInstallXMLConfig.mvstrInstallInfo.begin();
                itinstall != msHelpConfig.msInstallXMLConfig.mvstrInstallInfo.end();
                ++ itinstall)
            {
                if (NULL == it->mstrInstallVersion.compare(itinstall->mstrVersion))
                {
                    sInstall = itinstall[0];
                    return true;
                }
            }
        } 
#endif
        //! 4、否则报错，告知
        msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"未找到符合当前订单生产的版本软件，请与FAE协助\"}";
        mbInstallUpdate = true;
        return false;
    }

    bool CCJGW_SoftwareDownloadThread::FTPDownloadInstallSoftware(CCJGW_FtpClient& ftpClient,S_JGW_HELP_INSTALL& sInstall)
    {
        int nInstallDownload = 1;
        do 
        {
            //! 如果不一致则首先检查更新下载文件是否已经下载
            if (JGW_FileExistsToFilePath(_T(DEFAULT_INSTALL_FILE_PATH)))
            {
                std::wstring strMD5 = JGW_A2W(CCJGW_CryptFileMD5::GetFileMD5A(DEFAULT_INSTALL_FILE_PATH),CP_UTF8);
                //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
                if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),sInstall.mstrMD5.c_str()))
                {
                    msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":1,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\"}";
                    mbInstallUpdate = true;
                    return true;
                }
                JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
            }

            if (nInstallDownload != 1) return false;

            std::wstring strInstallFTPPath;
            JGW_FormatWString(strInstallFTPPath,L"%s/Install/%s",msHelpConfig.mstrFTPRootPath.c_str(),sInstall.mstrFileName.c_str());
            if (!ftpClient.DownloadFTPFile(strInstallFTPPath,_T(DEFAULT_INSTALL_FILE_PATH)))
            {
                PrintLogW(L"FTP Download Install File Fail(%s)",strInstallFTPPath.c_str());
                JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
                return false;
            }
        } while (nInstallDownload-- > 0);
        
        return false;
    }


    void CCJGW_SoftwareDownloadThread::OnStartCheckUpdateThread(CCJGW_FtpClient& ftpClient,LPARAM lParam)
    {
        S_JGW_HELP_UPDATE sUpdateInfo;

        //! false 表示不需要更新
        if (!CheckSoftwareVersionIsUpdate(sUpdateInfo))
        {
            goto UPDATE_END;
        }
        //! 下载更新包文件
        if (!FTPDownloadUpdatePack(ftpClient,sUpdateInfo))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新包文件下载失败,请检查网络连接情况\"}";
            goto UPDATE_END;
        }
UPDATE_END:
        mpHelpServiceThread->OnRspDownloadThreadResultMsg(WPARAM(&msHelpConfig.msDownloadThreadParam),NULL);
    }

    bool CCJGW_SoftwareDownloadThread::CheckSoftwareVersionIsUpdate(S_JGW_HELP_UPDATE& sUpdate)
    {
        for (std::vector<S_JGW_HELP_UPDATE_PACKAGE>::iterator it = msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.begin();
            it != msHelpConfig.msUpdateXMLConfig.mvsHelpUpdatePackage.end();
            ++ it)
        {
            //! 如果安装版本一致，则检测更新版本
            if (NULL == msHelpConfig.msVersionConfig.mstrInstallVersion.compare(it->mstrInstallVersion)
                || NULL == it->mstrInstallVersion.compare(L"*"))
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

    bool CCJGW_SoftwareDownloadThread::FTPDownloadUpdatePack(CCJGW_FtpClient& ftpClient,S_JGW_HELP_UPDATE& sUpdate)
    {
        int nDownloadUpdate = 1;

        do 
        {
            //! 检查更新文件是否存在，
            if (JGW_FileExistsToFilePath(_T(DEFAULT_UPDATE_FILE_PATH)))
            {
                std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(_T(DEFAULT_UPDATE_FILE_PATH));
                //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
                if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),sUpdate.mstrUpdatePackMD5.c_str()))
                {
                    JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\",\"version\":\"%s\"}",sUpdate.mstrVersion.c_str());
                    return true;
                }
                JGW_DeleteFile(_T(DEFAULT_UPDATE_FILE_PATH));
            }

            if (nDownloadUpdate != 1) return false;

            std::wstring strUpdatePackFTPPath;
            JGW_FormatWString(strUpdatePackFTPPath,L"%s/Update/%s/%s",msHelpConfig.mstrFTPRootPath.c_str(),msHelpConfig.msDownloadThreadParam.mstrTaskNumber.c_str(),sUpdate.mstrUpdatePack.c_str());
            if (!ftpClient.DownloadFTPFile(strUpdatePackFTPPath,_T(DEFAULT_UPDATE_FILE_PATH)))
            {
                PrintLogW(L"FTP Download UpdatePack File Fail(%s)",strUpdatePackFTPPath.c_str());
                JGW_DeleteFile(_T(DEFAULT_INSTALL_FILE_PATH));
                return false;
            }

        } while (nDownloadUpdate-- > 0);

        return false;
    }
}