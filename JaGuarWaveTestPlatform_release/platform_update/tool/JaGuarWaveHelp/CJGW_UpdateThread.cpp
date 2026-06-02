#include "StdAfx.h"
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_UpdateThread.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_UpdateThread::CCJGW_UpdateThread(S_JGW_HELP_CONFIG& sHelpConfig) : msHelpConfig(sHelpConfig)
    {
        m_nThreadSleepTimeInterval = (1000);
    }


    CCJGW_UpdateThread::~CCJGW_UpdateThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UpdateThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_REQ_UPDATE_MSG,&CCJGW_UpdateThread::OnStartUpdateThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UpdateThread::OnStartUpdateThread(WPARAM wParam,LPARAM lParam)
    {
        CCJGW_UpdateServiceThread* pThread = (CCJGW_UpdateServiceThread*)msHelpConfig.msDownloadThreadParam.mpParam;
        CCJGW_FtpClient ftpClient;

        if (!ftpClient.OpenFTPClient(msHelpConfig.mstrFTPHost,msHelpConfig.mstrUserName,msHelpConfig.mstrUserPassword,msHelpConfig.mnFTPPort))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":-2,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"FTP无法连接\"}";
            goto UPDATE_END;
        }

        if (!DownloadUpdateConfig(ftpClient))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新配置文件下载失败\"}";
            goto UPDATE_END;
        }

        if (!DownloadUpdatePackFile(ftpClient))
        {
            msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"false\",\"bUpdate\":\"false\",\"error_msg\":\"更新包下载失败\"}";
            goto UPDATE_END;
        }

UPDATE_END:
        ftpClient.CloseFTPClient();
        pThread->OnRspDownloadThreadResultMsg(WPARAM(&msHelpConfig.msDownloadThreadParam),LPARAM(1));
        PostMessage(gMainHwnd,WM_HWND_OPEN_UPDATE_HELP_TIMER_MSG,NULL,NULL);
    }

    bool CCJGW_UpdateThread::DownloadUpdateConfig(CCJGW_FtpClient& ftpClient)
    {
        std::string strFTPUpdateConfigPath(msHelpConfig.mstrFTPRootPath);
        strFTPUpdateConfigPath += "/Update/";
        strFTPUpdateConfigPath +=  msHelpConfig.msDownloadThreadParam.mstrTaskNumber;
        strFTPUpdateConfigPath += "/update.ini";

        std::string strLocalUpdateConfigPath;
        JGW_GetModuleWorkPathA(strLocalUpdateConfigPath);
        strLocalUpdateConfigPath += "update.ini";

        if (!ftpClient.DownloadFTPFile(strFTPUpdateConfigPath,strLocalUpdateConfigPath))
        {
            PrintLog("Download FTP Update Config File Fail(%s)",strFTPUpdateConfigPath.c_str());
            JGW_DeleteFile(JGW_A2W(strLocalUpdateConfigPath));
        }
        LoadFTPUpdateConfig(JGW_A2W(strLocalUpdateConfigPath));
        return true;
    }

    bool CCJGW_UpdateThread::DownloadUpdatePackFile(CCJGW_FtpClient& ftpClient)
    {
        if (msHelpConfig.msUpdateConfig.mstrUpdateVersion.empty() || NULL == msHelpConfig.msUpdateConfig.mstrUpdateVersion.compare(msHelpConfig.msVersionConfig.mstrUpdateVersion))
        {
            //! msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}";
            JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\",\"version\":\"%s\"}",JGW_W2A(msHelpConfig.msUpdateConfig.mstrUpdateVersion).c_str());
            return true;
        }

        //! 如果不一致则首先检查更新下载文件是否已经下载
        if (JGW_FileExistsToFilePath(_T(DEFAULT_UPDATE_FILE_PATH)))
        {
            std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(_T(DEFAULT_UPDATE_FILE_PATH));
            //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
            if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),msHelpConfig.msUpdateConfig.mstrUpdatePackMD5.c_str()))
            {
                //msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\"}";
                JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\",\"version\":\"%s\"}",JGW_W2A(msHelpConfig.msUpdateConfig.mstrUpdateVersion).c_str());
                return true;
            }
            JGW_DeleteFile(_T(DEFAULT_UPDATE_FILE_PATH));
        }

        std::string strFTPUpdatePackPath(msHelpConfig.mstrFTPRootPath);
        strFTPUpdatePackPath += "/Update/";
        strFTPUpdatePackPath += msHelpConfig.msDownloadThreadParam.mstrTaskNumber;
        strFTPUpdatePackPath += "/";
        strFTPUpdatePackPath += msHelpConfig.msUpdateConfig.mstrUpdatePack;

        if (!ftpClient.DownloadFTPFile(strFTPUpdatePackPath,DEFAULT_UPDATE_FILE_PATH))
        {
            PrintLog("Download FTP Update Config File Fail(%s)",strFTPUpdatePackPath.c_str());
            JGW_DeleteFile(_T(DEFAULT_UPDATE_FILE_PATH));
        }

        std::wstring strMD5 = CCJGW_CryptFileMD5::GetFileMD5(_T(DEFAULT_UPDATE_FILE_PATH));
        //! 如果已经下载则检查MD5是否一致如果一致直接发送更新命令即可
        if (NULL == JGW_WStrComparenoCaseWStr(strMD5.c_str(),msHelpConfig.msUpdateConfig.mstrUpdatePackMD5.c_str()))
        {
            //! msHelpConfig.msDownloadThreadParam.mstrJsonCommand = "{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\"}";
            JGW_FormatString(msHelpConfig.msDownloadThreadParam.mstrJsonCommand,"{\"helpRspType\":3,\"status\":\"true\",\"bUpdate\":\"true\",\"error_msg\":\"\",\"version\":\"%s\"}",JGW_W2A(msHelpConfig.msUpdateConfig.mstrUpdateVersion).c_str());
            return true;
        }

        return false;
    }

    void CCJGW_UpdateThread::LoadFTPUpdateConfig(const std::wstring& strLocalUpdateConfigPath)
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
}