#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_Win32IocpFSWatcher.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <iostream>

namespace JGW
{

    Win32IocpFSWatcherInfo::Win32IocpFSWatcherInfo(size_t id,const std::wstring& strDirectory,HANDLE hFileHandle,DWORD dwNotifyFilter,bool bWatchSubtree,DWORD dwNotifBufferLength /* = NOTIFY_BUFFER_LENGTH */) : mnID(id),mstrDirectory(strDirectory),mhFileHandle(hFileHandle),mdwNotifBufferLength(dwNotifBufferLength),mdwNotifyFilter(dwNotifyFilter),mbWatchSubtree(bWatchSubtree),meState(Initialized)
    {
        mpNotifBuffer = new char[dwNotifBufferLength];
        hEvent = NULL;
        Internal = 0;
        InternalHigh = 0;
        this->Offset = 0;
        this->OffsetHigh = 0;
    }

    Win32IocpFSWatcherInfo::~Win32IocpFSWatcherInfo()
    {
        if (INVALID_HANDLE_VALUE != mhFileHandle && NULL != mhFileHandle)
        {
            CloseHandle(mhFileHandle);
            mhFileHandle = INVALID_HANDLE_VALUE;
        }

        if (NULL != mpNotifBuffer)
        {
            delete[] mpNotifBuffer;
            mpNotifBuffer = NULL;
        }
    }

    size_t Win32IocpFSWatcherInfo::GetID()
    {
        return mnID;
    }

    bool Win32IocpFSWatcherInfo::Listen()
    {
        if (this->meState != Initialized) 
        {
            mstrErrorMsg = L"Invalid state.";
            return false;
        }

        if (FALSE == ReadDirectoryChangesW(mhFileHandle, mpNotifBuffer,mdwNotifBufferLength , mbWatchSubtree ? true : false /* bWatchSubtree */, mdwNotifyFilter,nullptr /* lpBytesReturned */, this, nullptr /* lpCompletionRoutine */)) 
        {
            meState = Initialized;
            mstrErrorMsg = L"An error has occurred: ";
            mstrErrorMsg += JGW_GetSystemErrorString(GetLastError());
            return false;
        }
        meState = Listening;
        return true;
    }

    void Win32IocpFSWatcherInfo::Stop()
    {
        if (this->meState == Listening) 
        {
            this->meState = PendingClose;
        }

        if (INVALID_HANDLE_VALUE != mhFileHandle && NULL != mhFileHandle)
        {
            CloseHandle(mhFileHandle);
            mhFileHandle = INVALID_HANDLE_VALUE;
        }
    }

    void Win32IocpFSWatcherInfo::SetState(Win32IocpFSWatcherInfo::State state)
    {
        meState = state;
    }

    Win32IocpFSWatcherInfo::State Win32IocpFSWatcherInfo::GetState()
    {
        return meState;
    }

    const std::wstring& Win32IocpFSWatcherInfo::GetDirectory()
    {
        return mstrDirectory;
    }

    std::wstring Win32IocpFSWatcherInfo::tryToGetLongName(const std::wstring& pathName) const
    {
        const std::wstring fullPath = this->mstrDirectory + L"\\" + pathName;
        const DWORD longSize = GetLongPathNameW(fullPath.c_str(), NULL, 0);
        std::wstring longPathName;
        longPathName.resize(longSize);
        const DWORD retVal = GetLongPathNameW(
            fullPath.c_str(), const_cast<wchar_t*>(longPathName.data()), static_cast<DWORD>(longPathName.size()));
        if (retVal == 0) {
            return pathName;
        }
        while (!longPathName.empty() && longPathName.back() == L'\0') {
            longPathName.pop_back();
        }
        if (longPathName.find(this->mstrDirectory) == 0 && this->mstrDirectory.size() < longPathName.size()) {
            std::wstring longName = longPathName.substr(this->mstrDirectory.size() + 1);
            if (longName.empty()) {
                return pathName;
            }
            else {
                return longName;
            }
        }
        else {
            return pathName;
        }
    }


    void Win32IocpFSWatcherInfo::ProcessNotification(
        const FILE_NOTIFY_INFORMATION& notIf,
        std::set<std::pair<std::wstring, size_t>>& notifications) const
    {
        std::wstring wPathName(
            notIf.FileName, notIf.FileName + (notIf.FileNameLength / sizeof(notIf.FileName)));
        if (notIf.Action != FILE_ACTION_REMOVED && notIf.Action != FILE_ACTION_RENAMED_OLD_NAME) {
            std::wstring longName = this->tryToGetLongName(wPathName);
            if (longName != wPathName) {
                wPathName = longName;
            }
        }
        notifications.emplace(std::pair<std::wstring, size_t>(wPathName, notIf.Action));
    }

    void Win32IocpFSWatcherInfo::ProcessNotifications(std::set<std::pair<std::wstring, size_t>> & vsetNotifications)
    {
        vsetNotifications.clear();
        const FILE_NOTIFY_INFORMATION* notInf = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(mpNotifBuffer);
        for (bool moreNotif = true; moreNotif; moreNotif = notInf->NextEntryOffset > 0,
            notInf = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(
            reinterpret_cast<const char*>(notInf) + notInf->NextEntryOffset)) {
                this->ProcessNotification(*notInf, vsetNotifications);
        }
    }

#if 0
    void Win32IocpFSWatcherInfo::PrintWatcher()
    {
        std::set<std::pair<std::wstring, size_t>> notifications;
        const FILE_NOTIFY_INFORMATION* notInf = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(mpNotifBuffer);
        for (bool moreNotif = true; moreNotif; moreNotif = notInf->NextEntryOffset > 0,
            notInf = reinterpret_cast<const FILE_NOTIFY_INFORMATION*>(
            reinterpret_cast<const char*>(notInf) + notInf->NextEntryOffset)) {
                this->processNotification(*notInf, notifications);
        }
#if 0
        DWORD offset = 0;
        char fileName[MAX_PATH] = "";
        FILE_NOTIFY_INFORMATION *fni = NULL;

        do
        {
            fni = (FILE_NOTIFY_INFORMATION*)(&mpNotifBuffer[offset]);
            // since we do not use UNICODE, 
            // we must convert fni->FileName from UNICODE to multibyte
            int ret = ::WideCharToMultiByte(CP_ACP, 0, fni->FileName,
                fni->FileNameLength / sizeof(WCHAR),
                fileName, sizeof(fileName), NULL, NULL);

            switch (fni->Action)
            {
            case FILE_ACTION_ADDED:     
                {
                    std::cout << "FILE_ACTION_ADDED " << fileName << std::endl;
                }
                break;
            case FILE_ACTION_REMOVED:
                {
                    std::cout << "FILE_ACTION_REMOVED " << fileName << std::endl;
                }
                break;
            case FILE_ACTION_MODIFIED:
                {
                    std::cout << "FILE_ACTION_MODIFIED " << fileName << std::endl;
                }
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:
                {
                    std::cout << "FILE_ACTION_RENAMED_OLD_NAME " << fileName << std::endl;
                }
                break;
            case FILE_ACTION_RENAMED_NEW_NAME:
                {
                    std::cout << "FILE_ACTION_RENAMED_NEW_NAME " << fileName << std::endl;
                }
                break;
            default:
                break;
            }
            // clear string so we can reuse it
            ::memset(fileName, '\0', sizeof(fileName));
            // advance to next entry
            offset += fni->NextEntryOffset;

        } while (fni->NextEntryOffset != 0);
#endif
    }
#endif
    CCJGW_Win32IocpFSWatcher::CCJGW_Win32IocpFSWatcher(void) : mhIocpHandler(NULL),mbStartWatcher(false),mpmapDirectoryWatcherInfo(NULL)
    {
        mpmapDirectoryWatcherInfo = new std::map<std::wstring,Win32IocpFSWatcherInfo*>();
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_Win32IocpFSWatcher, CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    const std::wstring& CCJGW_Win32IocpFSWatcher::GetErrorMessage()
    {
        return mstrErrorMsg;
    }

    CCJGW_Win32IocpFSWatcher::~CCJGW_Win32IocpFSWatcher(void)
    {
        if (NULL != mpmapDirectoryWatcherInfo)
        {
            delete mpmapDirectoryWatcherInfo;
            mpmapDirectoryWatcherInfo = NULL;
        }
    }

    bool CCJGW_Win32IocpFSWatcher::StartWatcher()
    {
        if (NULL == mhIocpHandler)
        {
            mhIocpHandler = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
            if (NULL == mhIocpHandler)
            {
                JGW_FormatWString(mstrErrorMsg,L"Error when creating IOCP : %s",JGW_GetSystemErrorString(GetLastError()).c_str());
                return false;
            }
        }

        mbStartWatcher = true;
        if (!this->IsThreadRun())
        {
            this->CreateMessageThread(0,0,true);
        }

        return true;
    }

    bool CCJGW_Win32IocpFSWatcher::HasWatcherPending()
    {
        //! 全部关掉
        CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
        // 遍历监听目录集合
        for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
        {
            if (it->second->GetState() != Win32IocpFSWatcherInfo::PendingClose)
            {
                return true;
            }
        }
        return false;
    }

    void CCJGW_Win32IocpFSWatcher::RunThread()
    {
        DWORD numOfBytes = 0;
        OVERLAPPED* ov = nullptr;
        ULONG_PTR compKey = 0;
        BOOL res = FALSE;

        while (mbStartWatcher && TRUE == (res = GetQueuedCompletionStatus(mhIocpHandler, &numOfBytes, &compKey, &ov, INFINITE)))
        {
            if (compKey != 0 && compKey == reinterpret_cast<ULONG_PTR>(this)) 
            {
                // stop "magic packet" was sent, so we shut down:
                break;
            }
            else 
            {
                this->ProcessEvent(numOfBytes, ov);
            }
        }
        mbStartWatcher = false;
        if (res != FALSE)
        {
            while (HasWatcherPending()
                && (res = GetQueuedCompletionStatus(
                mhIocpHandler, &numOfBytes, &compKey, &ov, INFINITE))
                != FALSE) {
                    CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
                    // 遍历监听目录集合
                    for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
                    {
                        if (it->second->GetState() == Win32IocpFSWatcherInfo::PendingClose)
                        {
                            it = mpmapDirectoryWatcherInfo->erase(it);
                            continue;
                        }
                        if (it->second == (Win32IocpFSWatcherInfo*)ov)
                        {
                            it->second->SetState(Win32IocpFSWatcherInfo::PendingClose);
                            delete it->second;
                            it = mpmapDirectoryWatcherInfo->erase(it);
                        }
                    }
            }
        }
        else
        {
            mstrErrorMsg = L"There is something wrong with the IOCP: ";
            mstrErrorMsg += JGW_GetSystemErrorString(GetLastError()) ;
            CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
            // 遍历监听目录集合
            for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
            {
                // alert all subscribers that they will not receive events from now on:
                if (ErrorEvent) ErrorEvent(it->first);
            } 
        }
    }

    bool CCJGW_Win32IocpFSWatcher::AddWatcherDirectory(size_t id,const std::wstring& strDirectory,DWORD dwNotifyFilter /* = NOTIFY_LAST_WRITE */,bool bWatchSubtree /* = true */)
    {
        if (NULL == mhIocpHandler || !mbStartWatcher)
        {
            mstrErrorMsg = L"Call the function StartWatcher first";
            return false;
        }

        if (mpmapDirectoryWatcherInfo->end() != mpmapDirectoryWatcherInfo->find(strDirectory))
        {
            //mstrErrorMsg = L"The current directory has been listened to .";
            return true;
        }

        HANDLE hFile = CreateFileW(strDirectory.c_str(), FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            mstrErrorMsg = L"Cannot create directory handle: ";
            mstrErrorMsg += JGW_GetSystemErrorString(GetLastError());
            return false;
        }

        // check if it is even a directory:
        {
            BY_HANDLE_FILE_INFORMATION fileInfo = {0};
            BOOL res = GetFileInformationByHandle(hFile, &fileInfo);
            if (res == FALSE) 
            {
                mstrErrorMsg = L"Get File Information By Handle Error :  ";
                mstrErrorMsg += JGW_GetSystemErrorString(GetLastError());
                return false;
            }
            else if (!(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
            {
                mstrErrorMsg = strDirectory;
                mstrErrorMsg += L" Not a directory.";
                return false;
            }
        }

        // the "old" IOCP handle should not be freed, because:
        // https://devblogs.microsoft.com/oldnewthing/20130823-00/?p=3423
        if (!CreateIoCompletionPort(hFile, mhIocpHandler, NULL, 1)) {
            mstrErrorMsg += L"Cannot create IOCP: ";
            mstrErrorMsg += JGW_GetSystemErrorString(GetLastError());
            return false;
        }

        Win32IocpFSWatcherInfo* pFSWatcherInfoOverlapped = new Win32IocpFSWatcherInfo(id,strDirectory,hFile,dwNotifyFilter,bWatchSubtree,NOTIFY_BUFFER_LENGTH);
        (*mpmapDirectoryWatcherInfo)[strDirectory] = pFSWatcherInfoOverlapped;
        return pFSWatcherInfoOverlapped->Listen();
    }

    //void CCJGW_Win32IocpFSWatcher::RemoveWatcherDirectory(const std::wstring& strDirectory)
    //{
    //    std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mmapDirectoryWatcherInfo.find(strDirectory);
    //    if (mmapDirectoryWatcherInfo.end() != it)
    //    {
    //        Win32IocpFSWatcherInfo* pFSWatcherInfoOverlapped = it->second;
    //        pFSWatcherInfoOverlapped->StopFSWatcher();
    //        mmapDirectoryWatcherInfo.erase(it);
    //    }
    //}

    void CCJGW_Win32IocpFSWatcher::StopWatcherEventLoop()
    {
        {
            CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
            for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
            {
                it->second->Stop();
            }
        }
        if (mhIocpHandler != INVALID_HANDLE_VALUE && NULL != mhIocpHandler) {
            // send stop "magic packet"
            PostQueuedCompletionStatus(mhIocpHandler, 0, reinterpret_cast<ULONG_PTR>(this), nullptr);
        }
    }

    void CCJGW_Win32IocpFSWatcher::ProcessEvent(DWORD numberOfBytesTrs, OVERLAPPED* overlapped)
    {
        Win32IocpFSWatcherInfo* pWin32IocpFSWatcherInfo = NULL;
        {
            CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
            // 遍历监听目录集合
            for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
            {
                if (it->second == (Win32IocpFSWatcherInfo*)overlapped)
                {
                    // 检查当前监听目录是否为关闭状态
                    if (it->second->GetState() == Win32IocpFSWatcherInfo::PendingClose)
                    {
                        delete it->second;
                        it = mpmapDirectoryWatcherInfo->erase(it);
                        return ;
                    }
                    // actual logic: 当前读到0时候，出现异常比如句柄关闭等等
                    if (0 == numberOfBytesTrs)
                    {
                        if (NULL != ErrorEvent) this->ErrorEvent(it->first);
                        return;
                    }

                    pWin32IocpFSWatcherInfo = it->second;
                    break;
                }
            }
        }

        if (NULL == pWin32IocpFSWatcherInfo)
        {
            return ;
        }
        // 当前状态进行复位
        if (pWin32IocpFSWatcherInfo->GetState() == Win32IocpFSWatcherInfo::Listening) 
        {
            pWin32IocpFSWatcherInfo->SetState(Win32IocpFSWatcherInfo::Initialized);
        }
        // 
#if 0
        pWin32IocpFSWatcherInfo->PrintWatcher();
#endif
        std::set<std::pair<std::wstring, size_t>> vsetNotifications;
        pWin32IocpFSWatcherInfo->ProcessNotifications(vsetNotifications);
        if (!vsetNotifications.empty())
        {
            ChangeDirectoryEvent(pWin32IocpFSWatcherInfo->GetID(),pWin32IocpFSWatcherInfo->GetDirectory(),vsetNotifications);
        }

        if (false == pWin32IocpFSWatcherInfo->Listen())
        {
            CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
            // 遍历监听目录集合
            for (std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->begin();it != mpmapDirectoryWatcherInfo->end();++it)
            {
                if (it->second == (Win32IocpFSWatcherInfo*)overlapped)
                {
                    if (NULL != ErrorEvent) this->ErrorEvent(it->first);
                    delete it->second;
                    it = mpmapDirectoryWatcherInfo->erase(it);
                    return ;
                }
            }
        }
    }

    void CCJGW_Win32IocpFSWatcher::RemoveWatcherDirectory(const std::wstring& strDirectory)
    {
        CCJGW_CriticalSectionAutoLock autoLock(mcCriticalSectionLock);
        std::map<std::wstring,Win32IocpFSWatcherInfo*>::iterator it = mpmapDirectoryWatcherInfo->find(strDirectory);
        if (it != mpmapDirectoryWatcherInfo->end())
        {
            it->second->Stop();
        }
    }
}

