#include "StdAfx.h"
#include "CJGW_FTPUploadingTestLogsThread.h"

#include <vector>
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <poco/Path.h>
#include <poco/File.h>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>

#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include <algorithm>
#include <JGW_FoundationFunc/vecfunc.h>

#pragma comment(lib,"Shlwapi.lib")

namespace JGW
{
    CCJGW_FTPUploadingTestLogsThread *gpFTPUploadingTestLogsThread = NULL;

    __int64 GetCurrentTimestampSec()
    {
        __int64 timeStamp = 0;
        (void)::GetSystemTimeAsFileTime((FILETIME*)&timeStamp);
        return (timeStamp - 116444736000000000) / 10000000;
    }

    CCJGW_FTPUploadingTestLogsThread::CCJGW_FTPUploadingTestLogsThread(S_HELP_V3_CONFIG& sHelpV3Config) : msHelpV3Config(sHelpV3Config),mcCurlFtp(msHelpV3Config.msFTPlogInfo.mstrHost,msHelpV3Config.msFTPlogInfo.mstrUserName,msHelpV3Config.msFTPlogInfo.mstrUserPassword,msHelpV3Config.msFTPlogInfo.mnPort),mbIsUploadingTestLogsThread(false)
    {
        S_NotifyFolderInfo sNotifyFolderInfo;
        mvsNotifyLogFolders.push_back(sNotifyFolderInfo);
        mvsNotifyLogFolders.push_back(sNotifyFolderInfo);
        mvsNotifyLogFolders.push_back(sNotifyFolderInfo);
        gpFTPUploadingTestLogsThread = this;
    }


    CCJGW_FTPUploadingTestLogsThread::~CCJGW_FTPUploadingTestLogsThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_FTPUploadingTestLogsThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_START_UPDALODING_TEST_THREAD,&CCJGW_FTPUploadingTestLogsThread::StartUploadingTestLogsThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_HELP_UPLOAD_LOG_MSG,&CCJGW_FTPUploadingTestLogsThread::UploadingTestLogsThreadLoop)
        ON_MYTHREAD_MESSGAE(WM_THREAD_UPLODING_TEST_LOG_LOOP,&CCJGW_FTPUploadingTestLogsThread::UploadingTestLogsThreadNow)
    END_MYTHREAD_MESSAGE_MAP()

    void ErrorEventPrint(const std::wstring& strPath)
    {
        PrintLogW(L"An error has occurred, no further events will be sent for Path= %s",strPath);
    }
    // /JaGuarWaveTestPlatform_Log/DHW225817/2022-08-28
    void ChangeEventFunc(size_t id,const std::wstring& strPath,const std::set<std::pair<std::wstring, size_t>>& vsetNotify)
    {
        gpFTPUploadingTestLogsThread->OnChangeWatcherEvent(id,strPath,vsetNotify);
    }

    bool CCJGW_FTPUploadingTestLogsThread::IsUploadingTestLogsThread()
    {
        return mbIsUploadingTestLogsThread;
    }
    /// strPath 格式范例C:\JGWStart\JaGuarWaveTestPlatform_V20210506\exec\TestLog 为空则表示是程序添加，否则是文件系统变动导致
    void CCJGW_FTPUploadingTestLogsThread::OnChangeWatcherEvent(size_t id,const std::wstring& strPath,const std::set<std::pair<std::wstring, size_t>>& vsetNotify)
    {
        //! mmapNotifyIdRelFilePath
        CCJGW_CriticalSectionAutoLock autoLock(mModifiedMapLock);
        std::wstring strFilePath;
        //! 首先遍历文件变更信息集合
        for (std::set<std::pair<std::wstring, size_t>>::iterator it = vsetNotify.begin();it != vsetNotify.end();++it)
        {
            //! 过滤掉非文件更改的信息通知
            if (it->second  != FILE_ACTION_MODIFIED) continue;
            //! 过滤掉目录通知信息
            if (!strPath.empty() && JGW_IsFolderExist(strPath + L"\\" + it->first)) continue;
            //! 检查当前ID是否存在监听信息集合中,如果不存在此ID，则进行添加
            std::map<size_t,std::vector<std::wstring>>::iterator itNotifyIdRelFilePath = mmapNotifyIdRelFilePath.find(id);
            if (itNotifyIdRelFilePath == mmapNotifyIdRelFilePath.end())
            {
#if 0
                std::vector<std::wstring> vstrTemp;
                vstrTemp.push_back(it->first);
                mmapNotifyIdRelFilePath.insert(std::make_pair<size_t,std::vector<std::wstring>>(id,vstrTemp));
#else
                mmapNotifyIdRelFilePath[id].push_back(it->first);
                continue;
#endif
            }
#if 1
            //! 当前ID代表目录类型为Custom时候以及strPath不为空，使用绝对路径
            //! 当前ID代表目录类型以及strPath为空时候，vsetNotify集合使用的绝对路径，否则为相对路径
            if (id == E_CUSTOM_LOG_FOLODER_TYPE && !strPath.empty())
            {
                strFilePath = strPath + L"\\" + it->first;
            }
            else
            {
                strFilePath = it->first;
            }
#else
            //! 如果路径是相对路径，那么直接添加路径，否则使用绝对路径
            if (PathIsRelative(it->first.c_str()))
            {
                strFilePath = strPath + L"\\" + it->first;
            }
            else
            {
                strFilePath = it->first;
            }
#endif
            //! 当前ID存在监听信息集合，则判断此文件是否已经在监听信息集合中 == TDO : 可能会存在重复比较，但是因为文件不多，没必要去做去重比较处理
            //! 如果需要做去重处理，首先处理vsetNotify的去重，然后和mmapNotifyIdRelFilePath现有的做比较即可
            if (itNotifyIdRelFilePath->second.end() == std::find_if(itNotifyIdRelFilePath->second.begin(),itNotifyIdRelFilePath->second.end(),[&strFilePath](std::wstring strRelFilePath){ return 0 == JGW_WStrComparenoCaseWStr(strRelFilePath.c_str(),strFilePath.c_str());}))
            {
                itNotifyIdRelFilePath->second.push_back(strFilePath);
            }
        }


        //std::find_if
#if 0
        //! 遍历文件监听事件信息
        for (std::set<std::pair<std::wstring, size_t>>::iterator it = vsetNotify.begin();it != vsetNotify.end();++it)
        {
            //! 过滤目录
            if (JGW_IsFolderExist(strPath + L"\\" + it->first))
            {
                continue;
            }
            //! 只处理文件更改信息
            if (it->second == FILE_ACTION_MODIFIED)
            {
                std::map<size_t,std::vector<std::wstring>>::iterator itNotifyIdRelFilePath = mmapNotifyIdRelFilePath.find(id);
                if (itNotifyIdRelFilePath == mmapNotifyIdRelFilePath.end())
                {
                    std::vector<std::wstring> vstrTemp;
                    vstrTemp.push_back(it->first);
                    mmapNotifyIdRelFilePath.insert(std::make_pair<size_t,std::vector<std::wstring>>(id,vstrTemp));
                }
                else
                {
                    //! 检查当前文件是否已经记录在未处理文件信息中
                    bool bModifiedFilePath = false;
                    for (std::vector<std::wstring>::iterator itModifiedFilePath = itNotifyIdRelFilePath->second.begin();itModifiedFilePath != itNotifyIdRelFilePath->second.end();++itModifiedFilePath)
                    {
                        //! 如果已经在未处理文件信息中则不予继续新增到未处理文件信息中
                        if (0 == JGW_WStrComparenoCaseWStr(itModifiedFilePath->c_str(),it->first.c_str()))
                        {
                            bModifiedFilePath = true;
                            break;
                        }
                    }

                    if (!bModifiedFilePath)
                    {
                        if (id == E_CUSTOM_LOG_FOLODER_TYPE)
                        {
                            if (strPath.empty())
                            {
                                itNotifyIdRelFilePath->second.push_back(it->first);
                            }
                            else
                            {
                                itNotifyIdRelFilePath->second.push_back(strPath + L"\\" + it->first);
                            }     
                        }
                        else
                        {
                            itNotifyIdRelFilePath->second.push_back(it->first);
                        }
                    }
                }
            }
        }
#endif
    }

    void CCJGW_FTPUploadingTestLogsThread::AddRecordFileInfosToNotifyIdRelFilePath(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        std::set<std::pair<std::wstring, size_t>> vsetNotify;
        for (std::vector<S_RecordFileInfo>::const_iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();++it)
        {
            vsetNotify.insert(std::pair<std::wstring, size_t>(it->mstrRealativeFilePath,FILE_ACTION_MODIFIED));
        }
        if (!vsetNotify.empty()) OnChangeWatcherEvent(eLogFolderType,L"",vsetNotify);
    }

    void CCJGW_FTPUploadingTestLogsThread::AddRecordFileInfoToNotifyIdRelFilePath(const S_RecordFileInfo& sRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        std::set<std::pair<std::wstring, size_t>> vsetNotify;
        vsetNotify.insert(std::pair<std::wstring, size_t>(sRecordFileInfo.mstrRealativeFilePath,FILE_ACTION_MODIFIED));
        OnChangeWatcherEvent(eLogFolderType,L"",vsetNotify);
    }

    void CCJGW_FTPUploadingTestLogsThread::StartUploadingTestLogsThread(WPARAM wParam,LPARAM lParam)
    {
        //! 获取当前时间戳
        mdw64CurrentTimeStamp = GetCurrentTimestampSec();
        //! 加载所有的xx.upload.ignore记录上传文件信息
        LoadAllUploadIgnoreFile();
        //！删除以前的测试日志和目录
        DeleteOldTestLogAndFolder();
        //! 事件监听
        mcWin32IocpFSWatcher.ErrorEvent = ErrorEventPrint;
        mcWin32IocpFSWatcher.ChangeDirectoryEvent = ChangeEventFunc;
        //! 启动监听
        mcWin32IocpFSWatcher.StartWatcher();
        //！开始监听LOG目录
        StartNotifyLogFolder();
    }

    void CCJGW_FTPUploadingTestLogsThread::LoadAllUploadIgnoreFile()
    {
        wchar_t str[512] = {0};
        size_t len = 0;
        wchar_t szFileMD5[33] = {0};
        std::vector<S_RecordFileInfo> vsRecordFileInfo;
        //! 不同ID对应不同目录
        std::wstring strFolder[3];
        JGW_FormatWString(strFolder[0],L"%sTestLog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
        JGW_FormatWString(strFolder[1],L"%slog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
        //! 遍历目录日志类型
        for (size_t id = E_TEST_LOG_FOLODER_TYPE;id <= E_CUSTOM_LOG_FOLODER_TYPE;id++)
        {
            //! 获取xx.upload.ignore记录文件路径
            std::wstring mstrReordsFilePath = GetReordsFTPUploadedFilesInfoFilePath((E_LOG_FOLODER_TYPE)id);
            //! 检查记录文件是否存在
            if (!JGW_PathFileExists(mstrReordsFilePath.c_str())) continue;

            //! 打开记录文件
            S_RecordFileInfo sRecordFileInfo;
            std::wifstream inputFile;
            inputFile.imbue( std::locale("") );
            inputFile.open(mstrReordsFilePath);
            if (!inputFile.is_open()) continue;
            vsRecordFileInfo.clear();

            while (inputFile.getline(str,512))
            {
                wcsncpy_s(szFileMD5,33,str,32);
                szFileMD5[32] = L'\0';
                sRecordFileInfo.mstrFileMd5 = szFileMD5;
                sRecordFileInfo.mstrRealativeFilePath = &str[33];
                //! 检查当前目录是否是相对目录，以前V2的版本使用的绝对路径
                if (TRUE == PathIsRelative(sRecordFileInfo.mstrRealativeFilePath.c_str()))
                {
                    sRecordFileInfo.mstrAbsFilePath = strFolder[id] + sRecordFileInfo.mstrRealativeFilePath;
                }
                else
                {
                    sRecordFileInfo.mstrAbsFilePath = sRecordFileInfo.mstrRealativeFilePath;
                }
                vsRecordFileInfo.push_back(sRecordFileInfo);
                memset(str,0x00,512 * sizeof(wchar_t));
            }
            inputFile.close();
            mmapIdRecordedFileInfo.insert(std::make_pair<size_t,std::vector<S_RecordFileInfo>>(id,vsRecordFileInfo));
        }
    }

    //! 删除监听目录中，老旧的测试LOG文件以及空目录
    void CCJGW_FTPUploadingTestLogsThread::DeleteOldTestLogAndFolder()
    {
        //! 遍历目录ID和目录ID对应文件信息
        for (std::map<size_t,std::vector<S_RecordFileInfo>>::iterator it = mmapIdRecordedFileInfo.begin();it != mmapIdRecordedFileInfo.end();++it)
        {
            //! 遍历目录ID对应文件信息
            for (std::vector<S_RecordFileInfo>::iterator itRecordFileInfo = it->second.begin();itRecordFileInfo != it->second.end();)
            {
                //! 检查当前记录文件是否存在,如果文件不存在，则删除当前文件
                if (!JGW_PathFileExists(itRecordFileInfo->mstrAbsFilePath.c_str()))
                {
                    itRecordFileInfo = it->second.erase(itRecordFileInfo);
                    continue;
                }
                //! 检查最后访问时间是否超过定义的时间，如果超过则删除此文件
#ifdef _DEBUG
                __int64 dw64AccessTimeStamp = JGW_GetFileLastAccessTimeTimeStampSec(itRecordFileInfo->mstrAbsFilePath);
#endif
                if ((mdw64CurrentTimeStamp - JGW_GetFileLastAccessTimeTimeStampSec(itRecordFileInfo->mstrAbsFilePath)) >= OLD_FILE_TIME_SEC)
                {
                    if (true == JGW_DeleteFile(itRecordFileInfo->mstrAbsFilePath))
                    {
                        itRecordFileInfo = it->second.erase(itRecordFileInfo);
                        continue;
                    }
                }
                ++itRecordFileInfo;
            }
            //! 重新构造记录文件
            WriteRecordFileInfosToRecordFile(it->second,(E_LOG_FOLODER_TYPE)it->first,std::ios::out | std::ios::trunc);
        }
        //! 删除空目录,TDO : 没啥意义，不实线
#if 0
        std::wstring strAbsFolder,strAbsLogFilePath;
        //! 遍历目录日志类型
        for (std::map<size_t,std::vector<S_RecordFileInfo>>::iterator it = mmapIdRecordedFileInfo.begin();it != mmapIdRecordedFileInfo.end();++it)
        {
            // 不同目录类型对应不同路径
            if (it->first == E_CUSTOM_LOG_FOLODER_TYPE)
            {
                strAbsFolder = L"";
            }
            else if (it->first == E_APPLICATION_LOG_FOLODER_TYPE)
            {
                JGW_FormatWString(strAbsFolder,L"%sTestLog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
            }
            else
            {
                JGW_FormatWString(strAbsFolder,L"%slog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
            }
            //! 遍历已记录日志文件
            for (std::vector<S_RecordFileInfo>::iterator itRecordFileInfo = it->second.begin();itRecordFileInfo != it->second.end();)
            {
                //! 判断当前目录是否是相对目录，相对目录则加上绝对目录
                if (TRUE == PathIsRelative(itRecordFileInfo->mstrRealativeFilePath.c_str()))
                {
                    strAbsLogFilePath = strAbsFolder + itRecordFileInfo->mstrRealativeFilePath;
                }
                else
                {
                    strAbsLogFilePath = itRecordFileInfo->mstrRealativeFilePath;
                }
                //! 检查当前文件是否存在,如果文件不存在，则删除当前文件
                if (!JGW_PathFileExists(strAbsLogFilePath.c_str()))
                {
                    itRecordFileInfo = it->second.erase(itRecordFileInfo);
                    continue;
                }
#ifdef _DEBUG
                __int64 dw64AccessTimeStamp = JGW_GetFileLastAccessTimeTimeStampSec(strAbsLogFilePath);
#endif
                if ((mdw64CurrentTimeStamp - JGW_GetFileLastAccessTimeTimeStampSec(strAbsLogFilePath)) >= OLD_FILE_TIME_SEC)
                {
                    if (true == JGW_DeleteFile(strAbsLogFilePath))
                    {
                        itRecordFileInfo = it->second.erase(itRecordFileInfo);
                        continue;
                    }
                }
                ++itRecordFileInfo;
            }
            WriteRecordFileInfosToRecordFile(it->second,(E_LOG_FOLODER_TYPE)it->first,std::ios::out | std::ios::trunc);
        }
#endif
    }

    void CCJGW_FTPUploadingTestLogsThread::UploadingTestLogsThreadNow(WPARAM wParam,LPARAM lParam)
    {
        mbIsUploadingTestLogsThread = true;
        if (msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadTestLog)
        {
            UploadingTestLogsToFTP(E_TEST_LOG_FOLODER_TYPE);
        }
        //! 每隔一段时间上传日志
        if (msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadLog)
        {
            UploadingTestLogsToFTP(E_CUSTOM_LOG_FOLODER_TYPE);
            UploadingTestLogsToFTP(E_APPLICATION_LOG_FOLODER_TYPE);
        }
        PostMessage(gMainHwnd,WM_HWND_OPEN_UPLOADLOG_HELP_TIMER_MSG,NULL,NULL);
        mbIsUploadingTestLogsThread = false;
    }
    
    //! 
    void CCJGW_FTPUploadingTestLogsThread::UploadingTestLogsThreadLoop(WPARAM wParam,LPARAM lParam)
    {
        //! 当HELP.DB文件关于TASKCONFIG数据库表对应订单有更新时候，关闭目录监听或者新增目录监听动作
        //TraverseAllListeningDirectorieseUploading();
        mbIsUploadingTestLogsThread = true;

        if (CheckDBTaskConfigTableIsUpdate())
        {
            TraverseAllListeningDirectorieseUploading();
            //! 修改目录监听动作
            ChangeNotifyLogFolder();
            TraverseAllListeningDirectorieseUploading();
        }
        //! 每隔一段时间上传日志
        if ((msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadTestLog) && mcTestLogTimer.elapsed() >= msHelpV3Config.msTaskSQL.msTaskConfig.mnUploadTestLogSec)
        {
            UploadingTestLogsToFTP(E_TEST_LOG_FOLODER_TYPE);
            mcTestLogTimer.restart();
        }
        //! 每隔一段时间上传日志
        if ((msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadLog) && mcAppCustomTimer.elapsed() >= msHelpV3Config.msTaskSQL.msTaskConfig.mnUploadLogSec)
        {
            UploadingTestLogsToFTP(E_CUSTOM_LOG_FOLODER_TYPE);
            UploadingTestLogsToFTP(E_APPLICATION_LOG_FOLODER_TYPE);
            mcAppCustomTimer.restart();
        }
        //! 每隔一小时将这个小时内上传的FTP文件，重新进行打包上传，然后将这一小时内的FTP文件进行文件删除动作
        CollectThisHourUploadedZipFilePackageUploading();
        //! 监听目录文件更新消息超过三十分钟未监听到则进行全LOG目录遍历扫描，并且JaGuarWaveTestPlatform.exe、PTPV2.exe以及JaGuarWaveStart.exe都未运行则退出JaGuarWaveHelpV3程序
        if (mcTraverseAllLogFolderTimer.elapsed() >= (30 * 60))
        {
            TraverseAllListeningDirectorieseUploading();
        }

        PostMessage(gMainHwnd,WM_HWND_OPEN_UPLOADLOG_HELP_TIMER_MSG,NULL,NULL);   
        mbIsUploadingTestLogsThread = false;
    }

    bool CCJGW_FTPUploadingTestLogsThread::StartNotifyLogFolder()
    {
        bool result = true;
        // 是否上传TestLog目录的日志文件
        if (msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadTestLog)
        { 
            if (!AddLogWatcherDirectory(E_TEST_LOG_FOLODER_TYPE,L"TestLog"))
            {
                result = false;
            }
        }
        // 是否上传log目录的日志文件
        if (msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadLog)
        {
            if (!AddLogWatcherDirectory(E_APPLICATION_LOG_FOLODER_TYPE,L"log"))
            {
                result = false;
            }
        }
        // 是否上传$$_Custom_$$目录的日志文件
        if (!msHelpV3Config.msTaskSQL.msTaskConfig.mstrCustomLogDir.empty())
        {
            if (!AddLogWatcherDirectory(E_CUSTOM_LOG_FOLODER_TYPE,msHelpV3Config.msTaskSQL.msTaskConfig.mstrCustomLogDir))
            {
                result = false;
            }
        }
        return result;
    }

    bool CCJGW_FTPUploadingTestLogsThread::CheckDBTaskConfigTableIsUpdate()
    {
#if 0
        //! 当监听TestLog目录为空,需要上传TestLog 或者 当监听TestLog目录不为空,不需要上传TestLog 
        if ((mvstrNotifyLogFolders[E_TEST_LOG_FOLODER_TYPE].empty() && msHelpV3Config.msFTPlogInfo.mbIsUploadTestLog) || (!mvstrNotifyLogFolders[E_TEST_LOG_FOLODER_TYPE].empty() && !msHelpV3Config.msFTPlogInfo.mbIsUploadTestLog))
        {
            return true;
        }
        //! 当监听TestLog目录为空,需要上传Log 或者 当监听TestLog目录不为空,不需要上传Log 
        if ((mvstrNotifyLogFolders[E_APPLICATION_LOG_FOLODER_TYPE].empty() && msHelpV3Config.msFTPlogInfo.mbIsUploadLog) || (!mvstrNotifyLogFolders[E_APPLICATION_LOG_FOLODER_TYPE].empty() && !msHelpV3Config.msFTPlogInfo.mbIsUploadLog))
        {
            return true;
        }
        //! 当监听CustomLogDir目录为空,配置CustomLogDir不为空 或者 当监听CustomLogDir目录不为空,配置CustomLogDir为空
        if ((mvstrNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].empty() && !msHelpV3Config.msFTPlogInfo.mstrCustomLogDir.empty()) || (!mvstrNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].empty() && msHelpV3Config.msFTPlogInfo.mstrCustomLogDir.empty()))
        {
            return true;
        }
        //! CustomLogDir监听以及配置都不为空，
        if (!mvstrNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].empty() && !msHelpV3Config.msFTPlogInfo.mstrCustomLogDir.empty())
        {
            //! CustomLogDir配置的目录不一致
            std::wstring strCustomFloder = JGW_GetFileNameToFilePath(mvstrNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE]);
            if (0 != JGW_WStrComparenoCaseWStr(strCustomFloder.c_str(),msHelpV3Config.msFTPlogInfo.mstrCustomLogDir.c_str()))
            {
                return true;
            }
        }
        return false;
#else
        return msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUpdateDBLogConfig;
#endif
    }

    bool CCJGW_FTPUploadingTestLogsThread::AddLogWatcherDirectory(E_LOG_FOLODER_TYPE eLogFolderType,const std::wstring& strFolderName)
    {
        //! 检查当前目录是否已经监听
        if (mvsNotifyLogFolders[eLogFolderType].mstrRealativeFolderName.empty())
        {
            JGW_FormatWString(mvsNotifyLogFolders[eLogFolderType].mstrAbsFolder,L"%s%s",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str(),strFolderName.c_str());
            if (!mcWin32IocpFSWatcher.AddWatcherDirectory(eLogFolderType,mvsNotifyLogFolders[eLogFolderType].mstrAbsFolder,CCJGW_Win32IocpFSWatcher::NOTIFY_LAST_WRITE))
            {
                PrintLogW(mcWin32IocpFSWatcher.GetErrorMessage().c_str());
                return false;
            }
            mvsNotifyLogFolders[eLogFolderType].mstrRealativeFolderName = strFolderName;
        }
        else
        {
            //! 检查当前监听目录和现在目录是否一致,如果不一致则移除，然后在添加
            if (0 != JGW_WStrComparenoCaseWStr(mvsNotifyLogFolders[eLogFolderType].mstrRealativeFolderName.c_str(),strFolderName.c_str()))
            {
                mcWin32IocpFSWatcher.RemoveWatcherDirectory(mvsNotifyLogFolders[eLogFolderType].mstrAbsFolder);
                mvsNotifyLogFolders[eLogFolderType].mstrRealativeFolderName = L"";
                mvsNotifyLogFolders[eLogFolderType].mstrAbsFolder = L"";
                return AddLogWatcherDirectory(eLogFolderType,strFolderName);
            }
        }

        return true;
    }

    bool CCJGW_FTPUploadingTestLogsThread::ChangeNotifyLogFolder()
    {
        //! 取消TESTLOG目录监听
        if (!msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadTestLog)
        {
            mcWin32IocpFSWatcher.RemoveWatcherDirectory(mvsNotifyLogFolders[E_TEST_LOG_FOLODER_TYPE].mstrAbsFolder);
            mvsNotifyLogFolders[E_TEST_LOG_FOLODER_TYPE].mstrRealativeFolderName = L"";
            mvsNotifyLogFolders[E_TEST_LOG_FOLODER_TYPE].mstrAbsFolder = L"";
        }
        else
        {
            //! 判断TESTLOG目录监听
            AddLogWatcherDirectory(E_TEST_LOG_FOLODER_TYPE,L"TestLog");
        }
        //! 不监听Log目录
        if (!msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUploadLog)
        {
            mcWin32IocpFSWatcher.RemoveWatcherDirectory(mvsNotifyLogFolders[E_APPLICATION_LOG_FOLODER_TYPE].mstrAbsFolder);
            mvsNotifyLogFolders[E_APPLICATION_LOG_FOLODER_TYPE].mstrRealativeFolderName = L"";
            mvsNotifyLogFolders[E_APPLICATION_LOG_FOLODER_TYPE].mstrAbsFolder = L"";
        }
        else
        {
            AddLogWatcherDirectory(E_APPLICATION_LOG_FOLODER_TYPE,L"log");
        }
        //! 不监听Custom类型目录
        if (msHelpV3Config.msTaskSQL.msTaskConfig.mstrCustomLogDir.empty())
        {
            mcWin32IocpFSWatcher.RemoveWatcherDirectory(mvsNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].mstrAbsFolder);
            mvsNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].mstrRealativeFolderName = L"";
            mvsNotifyLogFolders[E_CUSTOM_LOG_FOLODER_TYPE].mstrAbsFolder = L"";
        }
        else
        {
            AddLogWatcherDirectory(E_CUSTOM_LOG_FOLODER_TYPE,msHelpV3Config.msTaskSQL.msTaskConfig.mstrCustomLogDir);
        }

        msHelpV3Config.msTaskSQL.msTaskConfig.mbIsUpdateDBLogConfig = false;
        return true;
    }
    // 上传测试日志到FTP
    bool CCJGW_FTPUploadingTestLogsThread::UploadingTestLogsToFTP(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        //! 绝对目录,相对目录集合
        std::vector<S_RecordFileInfo> vsRecordFileInfo;
        //! 检查监听目录ID是否存在修改的文件信息
        {
            CCJGW_CriticalSectionAutoLock autoLock(mModifiedMapLock);
            //! 检查监听目录ID是否存在ID对应的监听文件信息,或者文件信息为空
            std::map<size_t,std::vector<std::wstring>>::iterator it = mmapNotifyIdRelFilePath.find((size_t)eLogFolderType);
            if (it == mmapNotifyIdRelFilePath.end() || it->second.empty())
            {
                return true;
            }
            //! 将监听文件信息转移到记录文件集合中 E_CUSTOM_LOG_FOLODER_TYPE ID中监听文件路径为绝对路径
            S_RecordFileInfo sRecordFileInfo;
            //! 不同ID对应不同目录
            std::wstring strFolder[3];
            JGW_FormatWString(strFolder[0],L"%sTestLog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
            JGW_FormatWString(strFolder[1],L"%slog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
            //! 遍历监听目录文件信息
            for (std::vector<std::wstring>::iterator itModifiedFile = it->second.begin();itModifiedFile != it->second.end();++itModifiedFile)
            {
                sRecordFileInfo.mstrRealativeFilePath = itModifiedFile[0];
                sRecordFileInfo.mstrAbsFilePath = strFolder[eLogFolderType] + sRecordFileInfo.mstrRealativeFilePath;
                vsRecordFileInfo.push_back(sRecordFileInfo);
            }
#if 0
            else
            {
                //! TestLog/Log 相对目录QIA\WIFI_QIA_DEMO\wifi_test.xml\2022-02-19\F__347922437.log
                //! Custom 相对目录 $$_Custom_$$\QIA\WIFI_QIA_DEMO\wifi_test.xml\2022-02-19\F__347922437.log
                //std::wstring strAbsPath,strRelPath;
                S_RecordFileInfo sRecordFileInfo;
                for (std::vector<std::wstring>::iterator itModifiedFile = it->second.begin();itModifiedFile != it->second.end();++itModifiedFile)
                {  
                    if (eLogFolderType != E_CUSTOM_LOG_FOLODER_TYPE)
                    {
                        sRecordFileInfo.mstrAbsFilePath = mvsNotifyLogFolders[eLogFolderType].mstrAbsFolder + L"\\" + itModifiedFile[0];
                    }
                    else
                    {
                        sRecordFileInfo.mstrAbsFilePath = itModifiedFile[0];
                    }
                    sRecordFileInfo.mstrRealativeFilePath = itModifiedFile[0];
                    vsRecordFileInfo.push_back(sRecordFileInfo);
                }
                it->second.clear();
            }

            if (vsRecordFileInfo.empty())
            {
                return true;
            }
#endif
        }

        //! 检查当前文件是否已记录在(TestLog/Log/Custom三种文件类型).upload.ignore中
#if 0
        for (std::vector<S_RecordFileInfo>::iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();)
        {
            //! 检查当前文件是否已经记录，并获取MD5值
            if (CheckCurrentFileIsReordedAndGetFileMd5(it[0],eLogFolderType))
            {
                it = vsRecordFileInfo.erase(it);
            }
            else
            {
                ++it;
            }
        }
#else
        while (JGW::erase_if(vsRecordFileInfo,[&eLogFolderType,this](S_RecordFileInfo& sTempRecordFileInfo){ return this->CheckCurrentFileIsReordedAndGetFileMd5(sTempRecordFileInfo,eLogFolderType); })) ;
#endif
        //! 打包当前监听目录的测试为压缩文件,当压缩失败的文件继续回退到监听目录中,并返回已添加如ZIP压缩文件的LOG文件信息
        std::wstring strPackageLogFileZipFilePath;
        if (!PackageLocalLogFileToZip(vsRecordFileInfo,strPackageLogFileZipFilePath,eLogFolderType))
        {
            return false;
        }
        //! 将压缩文件进行上传FTP上
        if (!ZipLogFileUploadToFTP(strPackageLogFileZipFilePath,eLogFolderType))
        {
#if 0
            //! 当出现异常则全部添加进去
            for (std::vector<S_RecordFileInfo>::const_iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();++it)
            {
                AddRecordFileInfoToNotifyIdRelFilePath(it[0],eLogFolderType);
            }
#else
            AddRecordFileInfosToNotifyIdRelFilePath(vsRecordFileInfo,eLogFolderType);
#endif
            return false;
        }
        //! 将当前日志文件记录到文件中
        if (!RecordTestLogsToRecordFile(vsRecordFileInfo,eLogFolderType))
        {
            return false;
        }

        return true;
    }

    bool CCJGW_FTPUploadingTestLogsThread::CheckCurrentFileIsReordedAndGetFileMd5(S_RecordFileInfo& sRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        //! 获取日志文件MD5值
        sRecordFileInfo.mstrFileMd5 = CCJGW_CryptFileMD5::GetFileMD5(sRecordFileInfo.mstrAbsFilePath);
        //! 检查当前目录ID对应的文件记录信息是否存在
        std::map<size_t,std::vector<S_RecordFileInfo>>::iterator it = mmapIdRecordedFileInfo.find((size_t)eLogFolderType);
        if (it == mmapIdRecordedFileInfo.end())
        {
            return false;
        }  
#if 0
        //! 检查当前文件目录是否已经记录,文件名以及文件MD5值都相同
        for (std::vector<S_RecordFileInfo>::iterator itRecordFileInfo = it->second.begin();itRecordFileInfo != it->second.end();++itRecordFileInfo)
        {
            if (0 == JGW_WStrComparenoCaseWStr(sRecordFileInfo.mstrAbsFilePath.c_str(),itRecordFileInfo->mstrAbsFilePath.c_str()) && 0 == JGW_WStrComparenoCaseWStr(sRecordFileInfo.mstrFileMd5.c_str(),itRecordFileInfo->mstrFileMd5.c_str()))
            {
                return true;
            }
        }
        return false;
#else
        return std::count_if(it->second.begin(),it->second.end(),[&sRecordFileInfo](S_RecordFileInfo& sTempRecordFileInfo){ return 0 == JGW_WStrComparenoCaseWStr(sRecordFileInfo.mstrAbsFilePath.c_str(),sTempRecordFileInfo.mstrAbsFilePath.c_str()) && 0 == JGW_WStrComparenoCaseWStr(sRecordFileInfo.mstrFileMd5.c_str(),sTempRecordFileInfo.mstrFileMd5.c_str());}) >= 1;
#endif
    }
    //! 当前本地测试日志为压缩文件(压缩失败的文件继续回退到监听目录中????????)
    bool CCJGW_FTPUploadingTestLogsThread::PackageLocalLogFileToZip(std::vector<S_RecordFileInfo>& vsRecordFileInfo,std::wstring& strPackageLogFileZipFilePath,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        // 获取打包LOG zip文件路径
        strPackageLogFileZipFilePath = GetLocalFTPLogZipFilePath(eLogFolderType);
        std::ofstream out(strPackageLogFileZipFilePath, std::ios::binary);
        if (!out.is_open() || vsRecordFileInfo.empty()) return false;

        //! 开始进行打包
        int numFiles = 0;
        Poco::Zip::Compress c(out, true);
        //! 添加进入压缩包
        for (std::vector<S_RecordFileInfo>::iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();)
        {
            Poco::Path theFile(JGW_W2A(it->mstrAbsFilePath,CP_UTF8));
            //! JGW_ReplaceStringW(strTemp,strFolderPath,L"");
            Poco::Path fileName(JGW_W2A(it->mstrRealativeFilePath,CP_UTF8));
            try
            {
                c.addFile(theFile, fileName);
                numFiles ++;
                ++it;
            }
            catch (Poco::Exception& e)
            {
                AddRecordFileInfoToNotifyIdRelFilePath(it[0],eLogFolderType);
                PrintLogW(L"Compress File(%s) Fail(%s)",it->mstrAbsFilePath.c_str(),JGW_A2W_A(e.what(),CP_UTF8));
                it = vsRecordFileInfo.erase(it);
            }   
        }
        //! 开始打包压缩包
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
        //! 当出现异常则全部添加进去
        AddRecordFileInfosToNotifyIdRelFilePath(vsRecordFileInfo,eLogFolderType);
        //for (std::vector<S_RecordFileInfo>::const_iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();++it)
        //{
        //    AddRecordFileInfoToNotifyIdRelFilePath(it[0],eLogFolderType);
        //}
        return false;
    }

    bool CCJGW_FTPUploadingTestLogsThread::ZipLogFileUploadToFTP(const std::wstring& strPackageLogFileZipFilePath,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        std::wstring strErrorMessage;
        std::wstring strRemoteFTPLogZipFilePath = GetRemoteFTPLogZipFilePath(eLogFolderType);
        //! FTP开始上传
        if (mcCurlFtp.UploadCurlFtpFile(strRemoteFTPLogZipFilePath,strPackageLogFileZipFilePath,strErrorMessage))
        {
            JGW_DeleteFile(strPackageLogFileZipFilePath);
            return true;
        }

        //! 失败原因可能是目录未创建
        if (!mcCurlFtp.CreateDirectoryCurlFtpFile(GetFTPLogsFolder(),strErrorMessage))
        {
            mcCurlFtp.TraverseCreateFTPDirectory(GetFTPLogsFolder(),strErrorMessage);
        }

        //! 开始重新进行上传
        if (mcCurlFtp.UploadCurlFtpFile(strRemoteFTPLogZipFilePath,strPackageLogFileZipFilePath,strErrorMessage))
        {
            JGW_DeleteFile(strPackageLogFileZipFilePath);
            return true;
        }
        PrintLogW(strErrorMessage.c_str());
        return false;
    }

    bool CCJGW_FTPUploadingTestLogsThread::RecordTestLogsToRecordFile(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType)
    {
        //! 检查当前目录ID对应的文件记录信息是否存在
        std::map<size_t,std::vector<S_RecordFileInfo>>::iterator it = mmapIdRecordedFileInfo.find((size_t)eLogFolderType);
        if (it == mmapIdRecordedFileInfo.end())
        {
            std::vector<S_RecordFileInfo> vsTempRecordFileInfo(vsRecordFileInfo.begin(),vsRecordFileInfo.end());
            mmapIdRecordedFileInfo.insert(std::make_pair<size_t,std::vector<S_RecordFileInfo>>((size_t)eLogFolderType,vsTempRecordFileInfo));
        }
        else
        {
            for (std::vector<S_RecordFileInfo>::const_iterator itRecordFileInfo = vsRecordFileInfo.begin();itRecordFileInfo != vsRecordFileInfo.end();++itRecordFileInfo)
            {
                it->second.push_back(itRecordFileInfo[0]);
            }
        }

        //! 将当前记录文件信息，写入到文件中
        return WriteRecordFileInfosToRecordFile(vsRecordFileInfo,eLogFolderType);
    }

    bool CCJGW_FTPUploadingTestLogsThread::WriteRecordFileInfosToRecordFile(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType,std::ios::openmode eOpenMode /* = std::ios::out | std::ios::app | std::ios::ate */)
    {
        std::wstring strUploadRecordFilePath = GetReordsFTPUploadedFilesInfoFilePath(eLogFolderType);
        //! 构造忽略规则文件
        std::wofstream outputFile;
        outputFile.imbue( std::locale("") );
        outputFile.open(strUploadRecordFilePath,eOpenMode);
        if (!outputFile.is_open()) return false;
        outputFile.setf(std::ios_base::end);

        for (std::vector<S_RecordFileInfo>::const_iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();++it)
        {
            outputFile << it->mstrFileMd5 << L" " << it->mstrRealativeFilePath << std::endl;
        }
        outputFile.flush();
        outputFile.close();

        return true;
    }

    //! 收集最近一小时已上传的FTP日志文件 TDO : 暂时搁置意义不大
    bool CCJGW_FTPUploadingTestLogsThread::CollectThisHourUploadedZipFilePackageUploading()
    {
        return true;
    }

    void ScanLogFileFunc(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring> *pvstrFilePaths = (std::vector<std::wstring> *)lpData;
        if (NULL != pvstrFilePaths)
        {
            std::wstring strFilePathTemp = filepath;
            strFilePathTemp += L"\\";
            strFilePathTemp += filename;
            pvstrFilePaths->push_back(strFilePathTemp);
        }
    }


    //! 遍历所有正在监听的目录，检查是否有未上传的文件。然后进行统一上传
    void CCJGW_FTPUploadingTestLogsThread::TraverseAllListeningDirectorieseUploading()
    {
        int numFiles = 0;
        std::vector<std::wstring> vstrFilePaths;
        std::vector<S_RecordFileInfo> vsRecordFileInfo;
        S_RecordFileInfo sRecordFileInfo;

        std::wstring strFolder[3];
        JGW_FormatWString(strFolder[0],L"%sTestLog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
        JGW_FormatWString(strFolder[1],L"%slog\\",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str());
        // 遍历当前目录LOG ID
        for (size_t id = E_TEST_LOG_FOLODER_TYPE;id <= E_CUSTOM_LOG_FOLODER_TYPE;id++)
        {
            //! 检查当前目录ID对应的目录是否监听，为空则未监听直接返回
            if (mvsNotifyLogFolders[id].mstrAbsFolder.empty()) continue;

            //！开始遍历当前目录
            JGW_TraverseFolder_VC(mvsNotifyLogFolders[id].mstrAbsFolder,numFiles,L"\\*.*",ScanLogFileFunc,&vstrFilePaths);
            //! 遍历当前目录下所有文件集合
            for (std::vector<std::wstring>::iterator it = vstrFilePaths.begin();it != vstrFilePaths.end();++it)
            {
                sRecordFileInfo.mstrAbsFilePath = it[0];
                //! 如果已经记录过则删除当前文件
                if (!CheckCurrentFileIsReordedAndGetFileMd5(sRecordFileInfo,(E_LOG_FOLODER_TYPE)id))
                {
                    sRecordFileInfo.mstrRealativeFilePath = sRecordFileInfo.mstrAbsFilePath;
                    JGW_ReplaceStringW(sRecordFileInfo.mstrRealativeFilePath,strFolder[id],L"");
                    vsRecordFileInfo.push_back(sRecordFileInfo);
                }
            }
            //! 添加到通知中
            AddRecordFileInfosToNotifyIdRelFilePath(vsRecordFileInfo,(E_LOG_FOLODER_TYPE)id);
#if 0
            //! 打包当前监听目录的测试为压缩文件,当压缩失败的文件继续回退到监听目录中,并返回已添加如ZIP压缩文件的LOG文件信息
            std::wstring strPackageLogFileZipFilePath;
            if (!PackageLocalLogFileToZip(vsRecordFileInfo,strPackageLogFileZipFilePath,eLogFolderType))
            {
                continue;
            }
            //! 将压缩文件进行上传FTP上
            if (!ZipLogFileUploadToFTP(strPackageLogFileZipFilePath,eLogFolderType))
            {
                //! 当出现异常则全部添加进去
                for (std::vector<S_RecordFileInfo>::const_iterator it = vsRecordFileInfo.begin();it != vsRecordFileInfo.end();++it)
                {
                    AddRecordFileInfoToNotifyIdRelFilePath(it[0],eLogFolderType);
                }
                continue;
            }
            //! 将当前日志文件记录到文件中
            if (!RecordTestLogsToRecordFile(vsRecordFileInfo,eLogFolderType))
            {
                return false;
            }
#endif
        }
    }

    const std::wstring& CCJGW_FTPUploadingTestLogsThread::GetNotifyLogFolder(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        return mvsNotifyLogFolders[eLogFolderType].mstrRealativeFolderName;
    }

    std::wstring CCJGW_FTPUploadingTestLogsThread::GetLocalFTPLogZipFilePath(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        return JGW_GetFormatWString(L"%s%s",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str(),GetLocalFTPLogZipFileName(eLogFolderType).c_str());
    }

    std::wstring CCJGW_FTPUploadingTestLogsThread::GetRemoteFTPLogZipFilePath(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        return GetFTPLogsFolder() + GetFTPLogsZipFileName(eLogFolderType);
    }

    std::wstring CCJGW_FTPUploadingTestLogsThread::GetReordsFTPUploadedFilesInfoFilePath(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        return JGW_GetFormatWString(L"%s%s",msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str(),GetReordsFTPUploadedFilesInfoFileName(eLogFolderType).c_str());
    }

    std::wstring CCJGW_FTPUploadingTestLogsThread::GetFTPLogsFolder()
    {
        //! /JaGuarWaveTestPlatform_Log/DHW218211/2021-10-28/
        return JGW_GetFormatWString(L"%s%s/%s/",msHelpV3Config.msFTPlogInfo.mstrLogRootPath.c_str(),msHelpV3Config.msDownloadThreadParam.mstrTaskNumber.c_str(),JGW_GetTimeFolder().c_str());
    }

    const std::wstring& CCJGW_FTPUploadingTestLogsThread::GetHostNameIPAddr()
    {
        static std::wstring wstrHostNameIpAddr;
        if (wstrHostNameIpAddr.empty())
        {
            CCJGW_SyncSocket syncSocket;
            std::string host,strHostNameIPAddr;
            std::vector<std::string> vListAddr;

            syncSocket.GetHostIP(host,vListAddr);
            strHostNameIPAddr = host;
            for (std::vector<std::string>::iterator it = vListAddr.begin();
                it != vListAddr.end();++ it) 
                JGW_FormatString(strHostNameIPAddr,"%s_%s",host.c_str(),it->c_str());
            JGW_ReplaceStringA(strHostNameIPAddr,":","");

            wstrHostNameIpAddr = JGW_A2W(strHostNameIPAddr);
        }
        return wstrHostNameIpAddr;
    }

    std::wstring CCJGW_FTPUploadingTestLogsThread::GetFTPLogsZipFileName(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        switch (eLogFolderType)
        {
        case E_APPLICATION_LOG_FOLODER_TYPE:
            //! AppLog_TEST-MMI-06_192.168.99.176_1635385966.zip 
            return JGW_GetFormatWString(L"AppLog_%s_%d.zip",GetHostNameIPAddr().c_str(),JGW_GetTimeOfDay());
        case E_CUSTOM_LOG_FOLODER_TYPE:
            //! CustomLog_TEST-MMI-06_192.168.99.176_1635385966.zip 
            return JGW_GetFormatWString(L"CustomLog_%s_%d.zip",GetHostNameIPAddr().c_str(),JGW_GetTimeOfDay());
        default:
        case E_TEST_LOG_FOLODER_TYPE:
            //! TestLog_TEST-MMI-06_192.168.99.176_1635385966.zip 
            return JGW_GetFormatWString(L"TestLog_%s_%d.zip",GetHostNameIPAddr().c_str(),JGW_GetTimeOfDay());
        }
    }

    const std::wstring CCJGW_FTPUploadingTestLogsThread::GetLocalFTPLogZipFileName(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        switch (eLogFolderType)
        {
        case E_TEST_LOG_FOLODER_TYPE:
            return L"TestLog_upload.zip";
        case E_APPLICATION_LOG_FOLODER_TYPE:
            return L"AppLog_upload.zip";
        case E_CUSTOM_LOG_FOLODER_TYPE:
            return L"CustomLog_upload.zip";
        }
        return L"TestLog_upload.zip";
    }

    const std::wstring CCJGW_FTPUploadingTestLogsThread::GetReordsFTPUploadedFilesInfoFileName(E_LOG_FOLODER_TYPE eLogFolderType)
    {
        switch (eLogFolderType)
        {
        case E_TEST_LOG_FOLODER_TYPE:
            return L"TestLog.upload.ignore";
        case E_APPLICATION_LOG_FOLODER_TYPE:
            return L"AppLog.upload.ignore";
        case E_CUSTOM_LOG_FOLODER_TYPE:
            return L"CustomLog.upload.ignore";
        }
        return L"TestLog.upload.ignore";
    }
}

