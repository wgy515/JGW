#include "StdAfx.h"
#include "CJGW_UpdateThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"

#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

#include "CJGW_TaskSQLite.h"
#include "CJGW_ProjectSQLite.h"
#include "CJGW_InstallVersionSQLite.h"

namespace JGW
{
    CCJGW_UpdateThread::CCJGW_UpdateThread(CCJaGuarWavePackageUpdateToolDlgV2 *pMainDlg) : mpAsyncWndMessage(NULL),mpMainDlg(pMainDlg)
    {
    }


    CCJGW_UpdateThread::~CCJGW_UpdateThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UpdateThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCJGW_UpdateThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_FTP_DOWNLOAD_MSG,&CCJGW_UpdateThread::OnHandleFtpDownloadMsg)
        ON_MYTHREAD_MESSGAE(WM_THREAD_FTP_UPLOAD_MSG,&CCJGW_UpdateThread::OnHandleFtpUploadMsg)
        ON_MYTHREAD_MESSGAE(WM_THREAD_ASYNC_FUNC_MSG,&CCJGW_UpdateThread::OnHandleAsyncFuncMsg)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UpdateThread::OnHandleAsyncFuncMsg(WPARAM wParam,LPARAM lParam)
    {
        bool result = mpMainDlg->OnHandleAsyncFuncMsg(wParam);
        mpAsyncWndMessage->PutAsyncMessage(WM_HWMD_OPERATION_COMPLETE_MSG,wParam,result ? 1 : 0);
    }

    void CCJGW_UpdateThread::PrintLogW(const wchar_t *format, ...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,format);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
    }

    void CCJGW_UpdateThread::OnInitThread( WPARAM wParam,LPARAM lParam )
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage *)wParam;
        mpsPackageUpdateConfig = (S_PACKAGE_UPDATE_CONFIG *)lParam;

        /// download help db
        CCJGW_CurlFtp curlFtpDownload(mpsPackageUpdateConfig->mstrFTPHost,mpsPackageUpdateConfig->mstrUserName,mpsPackageUpdateConfig->mstrUserPassword,mpsPackageUpdateConfig->mnFTPPort);
        std::wstring strHelpDBFTPPath,strDownloadDBFTPPath,strErrorMsg;

        JGW_FormatWString(strHelpDBFTPPath,L"%s/help.db",mpsPackageUpdateConfig->mstrFTPRootPath.c_str());
        JGW_FormatWString(strDownloadDBFTPPath,L"%s\\temp.db",JGW_GetApplicationFolder());
        PrintLogW(L"FTP HELP DB Path:%s",strHelpDBFTPPath.c_str());
        PrintLogW(L"Local HELP DB Path:%s",strDownloadDBFTPPath.c_str());
#ifndef DEBUG_HELP
        if (!curlFtpDownload.DownloadCurlFtpFile(strHelpDBFTPPath,strDownloadDBFTPPath,strErrorMsg))
        {
            PrintLogW(L"Curl FTP Download HELP.DB Fail : %s",strErrorMsg.c_str());;
            //! mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Curl FTP Download HELP.DB Fail");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
#endif
        //! open help db
        mpsPackageUpdateConfig->mstrHelpDBPath = strDownloadDBFTPPath;
        PrintLogW(L"Open Hellp DB Path:%s",mpsPackageUpdateConfig->mstrHelpDBPath.c_str());
        if (!mpsPackageUpdateConfig->mcSQLite.OpenSQLite3(JGW_W2A(mpsPackageUpdateConfig->mstrHelpDBPath.c_str(),CP_UTF8)))
        {
            PrintLogW(L"Open Hellp DB Fail");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        //! 安装版本
        CCJGW_InstallVersionSQLite installVersionSQLite;
        if (!installVersionSQLite.LoadInstallVersionSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsInstallVersion))
        {
            PrintLogW(L"Get Install Version Info Fail");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        //! 配置

        //! 获取订单信息
        CCJGW_TaskSQLite taskSQLite;
        if (!taskSQLite.LoadTaskSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsTask))
        {
            PrintLogW(L"Get Task Info Fail");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        //! 获取项目信息
        CCJGW_ProjectSQLite projectSQLite;
        if (!projectSQLite.LoadProjectSQLite(mpsPackageUpdateConfig->mcSQLite,mpsPackageUpdateConfig->mvsProject))
        {
            PrintLogW(L"Get Project Info Fail");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return;
        }

        PrintLogW(L"Initialization succeeded");
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CCJGW_UpdateThread::OnHandleFtpDownloadMsg(WPARAM wParam,LPARAM lParam)
    {
        std::wstring strFtpDownloadFilePath = (const wchar_t*)wParam;
        std::wstring strLocalSaveFilePath =  (const wchar_t*)lParam;
        std::wstring strErrorMsg;
        bool result = true;

        CCJGW_CurlFtp curlFtpDownload(mpsPackageUpdateConfig->mstrFTPHost,mpsPackageUpdateConfig->mstrUserName,mpsPackageUpdateConfig->mstrUserPassword,mpsPackageUpdateConfig->mnFTPPort);
        if (!curlFtpDownload.DownloadCurlFtpFile(strFtpDownloadFilePath,strLocalSaveFilePath,strErrorMsg))
        {
            result = false;
            MessageBox(mpAsyncWndMessage->GetMainHwnd(),L"Download Config update zip file Fail",L"Error",MB_ICONERROR);
        }
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWMD_OPERATION_COMPLETE_MSG,L"FTP_DOWNLOAD",result ? 1 : 0);
    }

    void CCJGW_UpdateThread::OnHandleFtpUploadMsg(WPARAM wParam,LPARAM lParam)
    {
        std::wstring strFtpUploadPath = (const wchar_t*)wParam;
        std::wstring strLocalFilePath = (const wchar_t*)lParam;
        std::wstring strErrorMsg;
        bool result = true;

        CCJGW_CurlFtp curlFtpDownload(mpsPackageUpdateConfig->mstrFTPHost,mpsPackageUpdateConfig->mstrUserName,mpsPackageUpdateConfig->mstrUserPassword,mpsPackageUpdateConfig->mnFTPPort);

        if (!curlFtpDownload.UploadCurlFtpFile(strFtpUploadPath,strLocalFilePath,strErrorMsg))
        {
            std::wstring strFtpUploadFolder = JGW_GetFileFolderToFilePath(strFtpUploadPath.c_str());
            if (!curlFtpDownload.CreateDirectoryCurlFtpFile(strFtpUploadFolder,strErrorMsg))
            {
                curlFtpDownload.TraverseCreateFTPDirectory(strFtpUploadFolder,strErrorMsg);
            }
            //! 重新上传FTP文件 并且上传完成后删除FTP文件
            if (curlFtpDownload.UploadCurlFtpFile(strFtpUploadPath,strLocalFilePath,strErrorMsg))
            {
                //JGW_DeleteFile(strLocalFilePath);   
            }
            else
            {
                MessageBox(mpAsyncWndMessage->GetMainHwnd(),strErrorMsg.c_str(),L"Error",MB_ICONERROR);
                result = false;
            }   
        }

        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWMD_OPERATION_COMPLETE_MSG,L"FTP_UPLOAD",result ? 1 : 0);
    }
}
