#pragma once
#include <JaGuarWaveHelpV3/CJaGuarWaveHelpV3Define.h>
#include <vector>
#include <map>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_Win32IocpFSWatcher.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include <JGW_CurlPlugin/CJGW_CurlFtp.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>

#define OLD_FILE_TIME_SEC (3 * 24 * 60 * 60)
namespace JGW
{
    typedef struct  
    {
        std::wstring mstrRealativePath;
    } S_Notify_File_Info;

    typedef struct  
    {
        std::wstring mstrRealativeFolderName;
        std::wstring mstrAbsFolder;
    } S_NotifyFolderInfo;

    typedef struct  
    {
        std::wstring mstrFileMd5;
        std::wstring mstrRealativeFilePath;
        std::wstring mstrAbsFilePath;
    }S_RecordFileInfo;

    class CCJGW_FTPUploadingTestLogsThread : public CCMessageThread
    {
    public:
        CCJGW_FTPUploadingTestLogsThread(S_HELP_V3_CONFIG& sHelpV3Config);
        ~CCJGW_FTPUploadingTestLogsThread(void);
    public:
        bool IsUploadingTestLogsThread();
    public:
        enum E_LOG_FOLODER_TYPE
        {
            E_TEST_LOG_FOLODER_TYPE = 0,
            E_APPLICATION_LOG_FOLODER_TYPE,
            E_CUSTOM_LOG_FOLODER_TYPE
        };
    private:
        friend void ChangeEventFunc(size_t id,const std::wstring& strPath,const std::set<std::pair<std::wstring, size_t>>& vsetNotify);
        friend void ErrorEventPrint(const std::wstring& strPath);
        void OnChangeWatcherEvent(size_t id,const std::wstring& strPath,const std::set<std::pair<std::wstring, size_t>>& vsetNotify);
    private:
        void StartUploadingTestLogsThread(WPARAM wParam,LPARAM lParam);
        void UploadingTestLogsThreadLoop(WPARAM wParam,LPARAM lParam);
        void UploadingTestLogsThreadNow(WPARAM wParam,LPARAM lParam);
    private:
        //! 开始监听目录
        bool StartNotifyLogFolder();
        /****************************** LOOP *************************************/
        //! 检查当HELP.DB文件关于TASKCONFIG数据库表对应订单有更新时候
        bool CheckDBTaskConfigTableIsUpdate();
        //! 更新目录监听动作
        bool ChangeNotifyLogFolder();
        //! 上传测试日志文件到FTP
        bool UploadingTestLogsToFTP(E_LOG_FOLODER_TYPE eLogFolderType);
        //! 每隔一小时将这个小时内上传的FTP文件，重新进行打包上传，然后将这一小时内的FTP文件进行文件删除动作
        bool CollectThisHourUploadedZipFilePackageUploading();
        //! 4、监听目录文件更新消息超过三十分钟未监听到则进行全LOG目录遍历扫描，并且JaGuarWaveTestPlatform.exe、PTPV2.exe以及JaGuarWaveStart.exe都未运行则退出JaGuarWaveHelpV3程序,未上传的文件进行打包上传
        void TraverseAllListeningDirectorieseUploading();
        //! JaGuarWaveHelpV3工具每次重新打开后，遍历目录类型(TestLog/Log/Custom三种文件类型).upload.ignore，检查此文件中记录已上传文件是否与当前时间间隔指定时间（可配置按小时计算）以上，如果间隔指定时间以上则删除此文件，然后重新整理输出 目录类型(TestLog/Log/Custom三种文件类型).upload.ignore 文件，然后遍历目录类型(TestLog/Log/Custom三种文件类型)下的目录文件，将空目录执行删除动作
        void DeleteOldTestLogAndFolder();
    private:
        const std::wstring& GetNotifyLogFolder(E_LOG_FOLODER_TYPE eLogFolderType);
        ////! 获取log的绝对路径
        //std::wstring GetLocalLogFileAbsPath(E_LOG_FOLODER_TYPE eLogFolderType,const std::wstring& strRealativePath);
        //! 检查当前文件是否已记录,并获取当前文件MD5
        bool CheckCurrentFileIsReordedAndGetFileMd5(S_RecordFileInfo& sRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType);
        //! 当前本地测试日志为压缩文件(压缩失败的文件继续回退到监听目录中????????)
        bool PackageLocalLogFileToZip(std::vector<S_RecordFileInfo>& vsRecordFileInfo,std::wstring& strPackageLogFileZipFilePath,E_LOG_FOLODER_TYPE eLogFolderType);
        //! 将压缩文件上传到FTP上
        bool ZipLogFileUploadToFTP(const std::wstring& strPackageLogFileZipFilePath,E_LOG_FOLODER_TYPE eLogFolderType);
        //! 将压缩文件中包含的日志文件记录到中
        bool RecordTestLogsToRecordFile(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType);
        //! 加载所有xx.upload.ignore记录文件
        void LoadAllUploadIgnoreFile();
    private:
        //! 添加目录监听
        bool AddLogWatcherDirectory(E_LOG_FOLODER_TYPE eLogFolderType,const std::wstring& strFolderName);
        //! 将记录的文件重新添加到通知ID以及相对文件路径中
        void AddRecordFileInfoToNotifyIdRelFilePath(const S_RecordFileInfo& sRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType);
        void AddRecordFileInfosToNotifyIdRelFilePath(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType);
        //! 写记录文件到
        bool WriteRecordFileInfosToRecordFile(const std::vector<S_RecordFileInfo>& vsRecordFileInfo,E_LOG_FOLODER_TYPE eLogFolderType,std::ios::openmode eOpenMode = std::ios::out | std::ios::app | std::ios::ate);
    private:
        //! $$_EXEC_$$\\TestLog_upload.zip  获取本地FTP压缩LOG文件路径
        std::wstring GetLocalFTPLogZipFilePath(E_LOG_FOLODER_TYPE eLogFolderType);
        //! /JaGuarWaveTestPlatform_Log/DHW218211/2021-10-28/TestLog_TEST-MMI-06_192.168.99.176_1635385966.zip 
        std::wstring GetRemoteFTPLogZipFilePath(E_LOG_FOLODER_TYPE eLogFolderType);
        //! 记录已上传文件信息绝对路径
        std::wstring GetReordsFTPUploadedFilesInfoFilePath(E_LOG_FOLODER_TYPE eLogFolderType);
    private:
        //! Records information about uploaded files 记录已上传文件信息：目录类型(TestLog/Log/Custom三种文件类型).upload.ignore
        const std::wstring GetReordsFTPUploadedFilesInfoFileName(E_LOG_FOLODER_TYPE eLogFolderType);
        //! TestLog_upload.zip
        const std::wstring GetLocalFTPLogZipFileName(E_LOG_FOLODER_TYPE eLogFolderType);
        // /JaGuarWaveTestPlatform_Log/DHW218211/2021-10-28/
        std::wstring GetFTPLogsFolder();
        //! TestLog_TEST-MMI-06_192.168.99.176_1635385966.zip
        std::wstring GetFTPLogsZipFileName(E_LOG_FOLODER_TYPE eLogFolderType);
        //! 电脑名称_电脑IP地址
        const std::wstring& GetHostNameIPAddr();
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbIsUploadingTestLogsThread;
        S_HELP_V3_CONFIG& msHelpV3Config;
        __int64 mdw64CurrentTimeStamp;
        jgw_timer mcTestLogTimer;
        jgw_timer mcAppCustomTimer;
        jgw_timer mcTraverseAllLogFolderTimer;
        //! 监听LOG目录，从E_TEST_LOG_FOLODER_TYPE、E_APPLICATION_LOG_FOLODER_TYPE、E_CUSTOM_LOG_FOLODER_TYPE
        std::vector<S_NotifyFolderInfo> mvsNotifyLogFolders;
        //! 记录监听目录ID，监听到的文件修改信息
        std::map<size_t,std::vector<std::wstring>> mmapNotifyIdRelFilePath;
        //! 已上传的日志文件信息
        std::map<size_t,std::vector<S_RecordFileInfo>> mmapIdRecordedFileInfo;
        CCJGW_CriticalSectionLock mModifiedMapLock;
        CCJGW_CurlFtp mcCurlFtp;
        CCJGW_Win32IocpFSWatcher mcWin32IocpFSWatcher;
    };
    
}

