#pragma once
#include <string>

#include <Sddl.h>
//! StringSecurityDescriptor
#define STRING_SECURITY_DESCRIPTOR L"D:(A;OICI;GA;;;AU)(A;OICI;GA;;;BA)"

namespace JGW
{
    template <class StructType>
    class CCJGW_IPCSharedStruct
    {
    public:
        CCJGW_IPCSharedStruct(void);
        CCJGW_IPCSharedStruct(const std::wstring& strSharedName);
        ~CCJGW_IPCSharedStruct(void);
    public:
        StructType* mpDataItem;
    private:
        /*!
        *  public Release
        *  Unmaps the mapped file represented by "this".  Closes any open file handles
        *
        *  @return none
        */
        void Release();
        /*!
        *  public Acquire
        *  Acquires a file mapping uniquely identified by the name provided.
        *
        *  @param [in]       name const string &    unique id of the mapping
        *
        *  @return bool indicator of acquisition success/failure.
        */
        bool Acquire( const std::wstring& strSharedName );
    private:
        bool mbCreated;
        HANDLE mhFileMapping;
        std::wstring mstrSharedName;
    };

    template <class StructType>
    CCJGW_IPCSharedStruct<StructType>::CCJGW_IPCSharedStruct() : mhFileMapping(NULL),mpDataItem(NULL),mbCreated(false)
    {

    }
    template <class StructType>
    CCJGW_IPCSharedStruct<StructType>::CCJGW_IPCSharedStruct(const std::wstring& strSharedName) : mhFileMapping(NULL),mpDataItem(NULL),mbCreated(false),mstrSharedName(strSharedName)
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
