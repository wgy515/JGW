#include "StdAfx.h"
#include "CJGW_SharedMemory.h"
#include <Sddl.h>

#define STRING_SECURITY_DESCRIPTOR L"D:(A;OICI;GA;;;AU)(A;OICI;GA;;;BA)"

namespace JGW
{
    CCJGW_SharedMemory::CCJGW_SharedMemory(void) : mpMapViewOfFileBuffer(NULL),mhFileMapping(NULL)
    {
    }


    CCJGW_SharedMemory::~CCJGW_SharedMemory(void)
    {
        ReleaseSharedMemory();
    }

    bool CCJGW_SharedMemory::CreateSharedMemory(const std::wstring& strIPCName,__int64 dw64SharedMemorySize /* = 1024 * 10 */)
    {
        SECURITY_ATTRIBUTES secure= {sizeof(SECURITY_ATTRIBUTES),0,1};
        //! 安全描述符转换
        ConvertStringSecurityDescriptorToSecurityDescriptor(STRING_SECURITY_DESCRIPTOR,SDDL_REVISION_1,&(secure.lpSecurityDescriptor),NULL);
        //!
        ReleaseSharedMemory();

        //! 创建文件映射
        mhFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE,&secure,PAGE_READWRITE,(dw64SharedMemorySize >> 32),(dw64SharedMemorySize & 0xFFFFFFFF),strIPCName.c_str());
        if (NULL == mhFileMapping)
        {
            return false;
        }
        //! 创建视图，文件映射到内存
        mpMapViewOfFileBuffer = (void*)MapViewOfFile(mhFileMapping,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
        if (NULL == mpMapViewOfFileBuffer)
        {
            CloseHandle(mhFileMapping);
            mhFileMapping = NULL;
            return false;
        }

        return true;
    }

    void CCJGW_SharedMemory::ReleaseSharedMemory()
    {
        //! 取消文件映射
        if (NULL != mpMapViewOfFileBuffer)
        {
            UnmapViewOfFile(mpMapViewOfFileBuffer);
            mpMapViewOfFileBuffer = NULL;
        }
        //! 关闭文件映射句柄
        if (NULL != mhFileMapping)
        {
            CloseHandle(mhFileMapping);
            mhFileMapping = NULL;
        }
    }

    bool CCJGW_SharedMemory::OpenSharedMemory(const std::wstring& strIPCName)
    {
        //! 打开文件映射,检查映射是否存在
        HANDLE hFileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, strIPCName.c_str());
        if (NULL == hFileMapping)
        {
            return false;
        }
#if 0
        mhFileMapping = hFileMapping;
        mpMapViewOfFileBuffer = (void*)MapViewOfFile(mhFileMapping,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
        if (NULL == mpMapViewOfFileBuffer)
        {
            CloseHandle(mhFileMapping);
            mhFileMapping = NULL;
            return false;
        }

        return true;
#else
        //! 重复打开共享内存
        if (!CreateSharedMemory(strIPCName,1024*1024))
        {
            CloseHandle(hFileMapping);
            return false;
        }

        CloseHandle(hFileMapping);
        return true;
#endif
    }

    void* CCJGW_SharedMemory::GetSharedMemoryBuffer()
    {
        return mpMapViewOfFileBuffer;
    }

}
