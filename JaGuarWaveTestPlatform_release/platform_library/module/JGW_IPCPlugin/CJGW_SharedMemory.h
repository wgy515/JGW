#pragma once
#include <string>
#include <Windows.h>

namespace JGW
{
    class CCJGW_SharedMemory
    {
    public:
        CCJGW_SharedMemory(void);
        ~CCJGW_SharedMemory(void);
    public:
        /// @brief 创建共享内存
        /// @param strIPCName 共享内存名称
        /// @param dw64SharedMemorySize 共享内存缓冲区大小
        /// @return true : 成功 false : 失败
        bool CreateSharedMemory(const std::wstring& strIPCName,__int64 dw64SharedMemorySize = 1024 * 10);
        /// @brief 打开已经存在的共享内存
        /// @param strIPCName 共享内存名称
        /// @return true : 成功 false : 失败
        bool OpenSharedMemory(const std::wstring& strIPCName);
        /// @brief 释放共享内存
        void ReleaseSharedMemory();
        /// @brief 获取共享内存缓冲区
        /// @return 返回共享内存缓冲区
        void* GetSharedMemoryBuffer();
    private:
        //! 共享内存缓冲区
        void* mpMapViewOfFileBuffer;
        //! 文件映射句柄
        HANDLE mhFileMapping;
    };
}
