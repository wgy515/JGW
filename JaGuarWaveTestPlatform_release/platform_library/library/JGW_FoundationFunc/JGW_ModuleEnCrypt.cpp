#include "StdAfx.h"
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#define MAX_ENCRYPT_DATA_LEN 32

typedef const unsigned int* (_cdecl *GetDecryptDataFunc)(const char* strManufacturer,int len);
namespace JGW
{
    static const unsigned int gEncryptData[MAX_ENCRYPT_DATA_LEN] = {
        0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
        0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
        0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
        0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
        0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
        0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
        0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
        0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    };

    DWORD MyGetProcAddress(
        HMODULE hModule,    // handle to DLL module
        LPCSTR lpProcName   // function name
        )
    {
        int i=0;
        PIMAGE_DOS_HEADER pImageDosHeader = NULL;
        PIMAGE_NT_HEADERS pImageNtHeader = NULL;
        PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = NULL;

        pImageDosHeader=(PIMAGE_DOS_HEADER)hModule;
        pImageNtHeader=(PIMAGE_NT_HEADERS)((DWORD)hModule+pImageDosHeader->e_lfanew);
        pImageExportDirectory=(PIMAGE_EXPORT_DIRECTORY)((DWORD)hModule+pImageNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

        DWORD *pAddressOfFunction = (DWORD*)(pImageExportDirectory->AddressOfFunctions + (DWORD)hModule);
        DWORD *pAddressOfNames = (DWORD*)(pImageExportDirectory->AddressOfNames + (DWORD)hModule);
        DWORD dwNumberOfNames = (DWORD)(pImageExportDirectory->NumberOfNames);
        DWORD dwBase = (DWORD)(pImageExportDirectory->Base);

        WORD *pAddressOfNameOrdinals = (WORD*)(pImageExportDirectory->AddressOfNameOrdinals + (DWORD)hModule);

        // 这个是查一下是按照什么方式（函数名称or函数序号）来查函数地址的
        DWORD dwName = (DWORD)lpProcName;
        if ((dwName & 0xFFFF0000) == 0)
        {
            goto xuhao;
        }
        for (i=0; i<(int)dwNumberOfNames; i++)
        {
            char *strFunction = (char *)(pAddressOfNames[i] + (DWORD)hModule);
            if (lstrcmpA(lpProcName, strFunction) == 0)
            {
                return (pAddressOfFunction[pAddressOfNameOrdinals[i]] + (DWORD)hModule);
            }
        }
        return 0;
        // 这个是通过以序号的方式来查函数地址的
xuhao:
        if (dwName < dwBase || dwName > dwBase + pImageExportDirectory->NumberOfFunctions - 1)
        {
            return 0;
        }
        return (pAddressOfFunction[dwName - dwBase] + (DWORD)hModule);
    }

    bool EncryptModuleIsLoaded(HMODULE hModule)
    {
#ifdef IS_ENCRYPT_MODULE
        //! 处理加密数据
        unsigned int encryptData[MAX_ENCRYPT_DATA_LEN] = {0};
        for (int i = 0;i < MAX_ENCRYPT_DATA_LEN;i ++)
        {
            encryptData[i] ^= gEncryptData[i];
            encryptData[i] += i;
        }
        wchar_t buf[512] = {0};
        GetModuleFileName(NULL,buf,256);
        std::wstring strFileName = JGW_GetFileNameToFilePath(buf);
		if (0 == _wcsnicmp(strFileName.c_str(), L"JGW.TSE.", wcslen(L"JGW.TSE.")))
		{
			strFileName = L"JGW_CSharpAdapter.dll";
		}

        HMODULE hModuleInstance = GetModuleHandle(strFileName.c_str());
        //! 获取解密数据
        GetDecryptDataFunc GetDecryptData = (GetDecryptDataFunc)MyGetProcAddress(hModuleInstance,"x2x31313axaxazzadasdazz");
        if (NULL == GetDecryptData)
        {
#ifdef _DEBUG
			exit(-1);
#else
			exit(-1);
#endif
#if 0
            hModuleInstance = GetModuleHandle(L"JGW_CSharpAdapter.dll");
            if (NULL == hModuleInstance)
            {
#ifdef _DEBUG
                exit(-1);
#else
                exit(-1);
#endif
            }
            GetDecryptData = (GetDecryptDataFunc)MyGetProcAddress(hModuleInstance,"x2x31313axaxazzadasdazz");
            if (NULL == GetDecryptData)
            {
#ifdef _DEBUG
                exit(-1);
#else
                exit(-1);
#endif
            }
#endif
            //! return false;
        }
        //! JaGuarWave202009041703
        //! 加密厂商字符串
        std::string strJaGuarWave = "pgeTP4/X757K45bcMscg8LviTNtTJkeygjXRRXcEQzY=";
        char szBuf[128] = {0};
        for (size_t i = 0;i < strJaGuarWave.length();i ++)
        {
            szBuf[i] ^= strJaGuarWave[i];
            szBuf[i] += i * 2;
        }
        //! 根据加密厂商字符串解密
        const unsigned int* decryptData = GetDecryptData(szBuf,strJaGuarWave.length());
        //! 判断加密解密数据是否一致
        for (int i = 0;i < MAX_ENCRYPT_DATA_LEN;i ++)
        {
            if (encryptData[i] != decryptData[i])
            {
#ifdef _DEBUG
                exit(-1);
#else
                exit(-1);
#endif
                return false;
            }
        }
#endif
        return true;
    }
}