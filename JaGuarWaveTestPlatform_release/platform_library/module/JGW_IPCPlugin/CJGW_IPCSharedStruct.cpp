#include "StdAfx.h"
#include "CJGW_IPCSharedStruct.h"

//! StringSecurityDescriptor
#define STRING_SECURITY_DESCRIPTOR L"D:(A;OICI;GA;;;AU)(A;OICI;GA;;;BA)"

namespace JGW
{
    template <class StructType>
    CCJGW_IPCSharedStruct<StructType>::CCJGW_IPCSharedStruct() : mhFileMapping(NULL),mpDataItem(NULL),mbCreated(NULL)
    {

    }
    template <class StructType>
    CCJGW_IPCSharedStruct<StructType>::CCJGW_IPCSharedStruct(const std::wstring& strSharedName) : mhFileMapping(NULL),mpDataItem(NULL),mbCreated(NULL),mstrSharedName(strSharedName)
    {
        Acquire(strSharedName);
    }

    template <class StructType>
    CCJGW_IPCSharedStruct<StructType>::~CCJGW_IPCSharedStruct()
    {
        Release();
    }

    template <class StructType>
    bool CCJGW_IPCSharedStruct<StructType>::Acquire( const std::wstring& strSharedName )
    {
#if 1
        SECURITY_ATTRIBUTES secure= {sizeof(SECURITY_ATTRIBUTES),0,1};
        ConvertStringSecurityDescriptorToSecurityDescriptor(STRING_SECURITY_DESCRIPTOR,SDDL_REVISION_1,&(secure.lpSecurityDescriptor),NULL);

        mstrSharedName = strSharedName;
        mhFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE,&secure,PAGE_READWRITE,0,100000,strSharedName.c_str());
        if (NULL == mhFileMapping)
        {
            //OutputDebugString(GetLastError());
            return false;
        }

        mpDataItem = (StructType*)MapViewOfFile(mhFileMapping,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
        if (NULL == mpDataItem)
        {
            CloseHandle(mhFileMapping);
            return false;
        }

        return true;
#else
        ostringstream ostr;
        SECURITY_ATTRIBUTES secure= {sizeof(SECURITY_ATTRIBUTES),0,1};
        TCHAR* szSD=TEXT("D:")               // Discretionary ACL
            TEXT("(A;OICI;GA;;;AU)")   // Allow full control to authenticated users
            TEXT("(A;OICI;GA;;;BA)");   // Allow full control to administrators

        ConvertStringSecurityDescriptorToSecurityDescriptor(szSD,SDDL_REVISION_1,&(secure.lpSecurityDescriptor),NULL);
        m_hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, &secure, PAGE_READWRITE, 0, 100000/*sizeof(StructType)*/, name.c_str() );

        if (m_hFileMapping == NULL)
        {
            int err = GetLastError();
            return FALSE;
        }

        strncpy_s(m_hSharedName, MAX_PATH, name.c_str(), MAX_PATH - 1);

        m_pDataItem = (StructType *) MapViewOfFile( m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

        if (m_pDataItem == NULL)
        {
            CloseHandle(m_hFileMapping);
            return FALSE;
        }

        return TRUE;
#endif
    }

    template <class StructType>
    void CCJGW_IPCSharedStruct<StructType>::Release()
    {
        if (NULL != mpDataItem)
        {
            UnmapViewOfFile(mpDataItem);
            mpDataItem = NULL;
        }

        if (NULL != mhFileMapping)
        {
            CloseHandle(mhFileMapping);
            mhFileMapping = NULL;
        }
    }
}