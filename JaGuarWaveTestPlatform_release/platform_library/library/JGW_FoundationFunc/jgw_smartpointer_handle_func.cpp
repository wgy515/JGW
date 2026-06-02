#include "stdafx.h"
#include <Windows.h>
#include <SetupAPI.h>
#pragma comment(lib,"setupapi.lib")
namespace JGW
{
    void CloseFileHandle(void* hFileHandel)
    {
        if (hFileHandel && INVALID_HANDLE_VALUE != hFileHandel)
        {
            CloseHandle(hFileHandel);
        }
    }

    void DestroyDeviceInfoList(void* hDevInfo)
    {
        if (hDevInfo && INVALID_HANDLE_VALUE != hDevInfo)
        {
            SetupDiDestroyDeviceInfoList(hDevInfo);
        }
    }

    void CloseRegKey(void* hKey)
    {
        HKEY* pkey = (HKEY*)hKey;
        if (pkey && *pkey )
        {
            RegCloseKey(*pkey);
            *pkey = NULL;
            delete pkey;
            pkey = NULL;
        }      
    }
    //! char *
//     void reg_close_key(HKEY* pkey)
//     {
//         if( pkey && *pkey )
//         {
//             RegCloseKey(*pkey);
//             *pkey = NULL;
//             delete pkey;
//             pkey = NULL;
//         }
//     }
}