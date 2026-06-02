#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include "CMessageThread.h"
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include "JGW_WindowsFuncPlugin_Define.h"

#define NOTIFY_BUFFER_LENGTH (10 * 1024)



namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS Win32IocpFSWatcherInfo : public OVERLAPPED
    {
    public:
        Win32IocpFSWatcherInfo(size_t id,const std::wstring& strDirectory,HANDLE hFileHandle,DWORD dwNotifyFilter,bool bWatchSubtree,DWORD dwNotifBufferLength = NOTIFY_BUFFER_LENGTH);
        ~Win32IocpFSWatcherInfo();

        // 监听状态
        enum State
        {
            Initialized, // No outstanding RDC() call.
            Listening, // RDC() call was made, and we're waiting for changes.
            PendingClose, // Directory handle was closed, and we're waiting for the "closing"
            // notification on IOCP.
            // Most of the time this is an "empty" notification, but sometimes it is a
            // legitimate notification about a change. This is behavior is not documented
            // explicitly.
        };
    public:
        // 开启监听此目录
        bool Listen();
        // 停止监听
        void Stop();
        // 将指定目录中更改的信息整理集合上传
        void ProcessNotifications(std::set<std::pair<std::wstring, size_t>>& vsetNotifications);
        // 获取当前监听状态
        State GetState();
        // 设置监听状态
        void SetState(State state);
        ///
        const std::wstring& GetDirectory();
        //
        size_t GetID();
#if 0
        void PrintWatcher();
#endif
    private:
        void ProcessNotification(
            const FILE_NOTIFY_INFORMATION& notIf,
            std::set<std::pair<std::wstring, size_t>>& notifications) const;
        std::wstring tryToGetLongName(const std::wstring& pathName) const;
    private:
        //! 是否监听子目录
        bool mbWatchSubtree;
        //! 监听缓存区
        char* mpNotifBuffer;
        //! 监听缓存区大小
        DWORD mdwNotifBufferLength;
        //! 监听目录句柄
        HANDLE mhFileHandle;
        //! 监听筛选
        DWORD mdwNotifyFilter;
        size_t mnID;
        volatile State meState;
        std::wstring mstrErrorMsg;
        std::wstring mstrDirectory;
    };

    //typedef void (*ErrorEventFunc)(const std::wstring& strPath);

    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_Win32IocpFSWatcher : public CCMessageThread
    {
    public:
        CCJGW_Win32IocpFSWatcher(void);
        ~CCJGW_Win32IocpFSWatcher(void);
    public:
        // 可监视的属性
        enum {
            NOTIFY_FILE_NAME = FILE_NOTIFY_CHANGE_FILE_NAME, //监视文件名更改
            NOTIFY_DIR_NAME = FILE_NOTIFY_CHANGE_DIR_NAME, //监视目录名更改
            NOTIFY_ATTRIBUTES = FILE_NOTIFY_CHANGE_ATTRIBUTES, //监视文件属性更改
            NOTIFY_SIZE = FILE_NOTIFY_CHANGE_SIZE, //监视文件大小更改
            NOTIFY_LAST_WRITE = FILE_NOTIFY_CHANGE_LAST_WRITE, //监视文件最后写入时间更改
            NOTIFY_LAST_ACCESS = FILE_NOTIFY_CHANGE_LAST_ACCESS, //监视文件最后访问时间更改
            NOTIFY_CREATION = FILE_NOTIFY_CHANGE_CREATION, //监视文件创建
            NOTIFY_SECURITY = FILE_NOTIFY_CHANGE_SECURITY, //监视文件安全描述符更改
            NOTIFY_ALL = NOTIFY_DIR_NAME|NOTIFY_FILE_NAME|NOTIFY_SIZE| NOTIFY_LAST_WRITE| NOTIFY_LAST_ACCESS| NOTIFY_CREATION| NOTIFY_ATTRIBUTES| NOTIFY_SECURITY //监视所有情况
        };
        // 发生的行为
        enum {
            ACTION_ADD = FILE_ACTION_ADDED, //文件添加
            ACTION_REMOVE = FILE_ACTION_REMOVED, //文件删除
            ACTION_MODIFIED = FILE_ACTION_MODIFIED, //文件更改
            ACTION_RENAME_OLD=FILE_ACTION_RENAMED_OLD_NAME, //文件重命名（旧名字）
            ACTION_RENAME_NEW=FILE_ACTION_RENAMED_NEW_NAME //文件重命名（新名字）
        };
        //! 开启监听
        bool StartWatcher();
        //! 添加监听目录
        bool AddWatcherDirectory(size_t id,const std::wstring& strDirectory,DWORD dwNotifyFilter = NOTIFY_ALL,bool bWatchSubtree = true);
        //! 移除监听目录
        void RemoveWatcherDirectory(const std::wstring& strDirectory);
        //! 停止目录监听循环
        void StopWatcherEventLoop();
    public:
        const std::wstring& GetErrorMessage();
        //! 事件回调
        //std::function<void(const std::wstring&)> ErrorEvent;
         void (*ErrorEvent)(const std::wstring& strPath);
        //! 事件回调,目录中更改的信息
        //std::function<void(const std::wstring&,const std::set<std::pair<std::wstring /* path */, size_t /* action */>>&)> ChangeDirectoryEvent;
         void (*ChangeDirectoryEvent)(size_t,const std::wstring&,const std::set<std::pair<std::wstring /* path */, size_t /* action */>>&);
    private:
        //! 传递事件
        void ProcessEvent(DWORD numberOfBytesTrs, OVERLAPPED* overlapped);
        //! 是否还有监听在等待循环中
        bool HasWatcherPending();
        //! TSE执行测试序列消息响应
        virtual void RunThread();
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbStartWatcher;
        HANDLE mhIocpHandler;
        std::wstring mstrErrorMsg;
        std::map<std::wstring,Win32IocpFSWatcherInfo*>* mpmapDirectoryWatcherInfo;
        CCJGW_CriticalSectionLock mcCriticalSectionLock;
    };
}


