#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <Psapi.h>
namespace JGW
{
    HANDLE JGW_CreateProcess(const std::wstring& strProcessFilePath,const wchar_t* strCurrentProcessPath /*= NULL*/,DWORD dwFlags/* = STARTF_USESHOWWINDOW*/,WORD wShowWindow /*= SW_NORMAL*/)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi = {0};
        memset(&si,0,sizeof(STARTUPINFO));//初始化si在内存块中的值（详见memset函数）
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = dwFlags;
        si.wShowWindow = wShowWindow;

        if (!CreateProcess(strProcessFilePath.c_str(),NULL,
            NULL, NULL,
            TRUE, NULL,
            NULL,strCurrentProcessPath,&si,&pi))
        {
            return NULL;
        }
        return pi.hProcess;
    }

    bool JGW_CheckProcessIsRun(HANDLE hProcess)
    {
        DWORD dwExitCode = 0;
        GetExitCodeProcess(hProcess,&dwExitCode);
        return (STILL_ACTIVE == dwExitCode);
    }

    bool JGW_TerminateProcess(HANDLE hProcess)
    {
        if (NULL == hProcess) return true;
        DWORD dwExitCode = 0;
        GetExitCodeProcess(hProcess,&dwExitCode);
        //!if (STILL_ACTIVE != dwExitCode ) return true;
        if (TerminateProcess(hProcess,-1)) return true;
        JGW_GetPrivilege();
        return (TRUE == TerminateProcess(hProcess,-1));
    }

    //! 程序提权操作
    bool  JGW_SetPrivilege(const wchar_t *lpszPrivilege /*= SE_DEBUG_NAME*/, bool bEnablePrivilege/* = true*/)
    {
        TOKEN_PRIVILEGES tp = {0};  
        HANDLE hToken = NULL;  
        LUID luid = {0};

        if( !OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken) )  
        {
            //! throw JGW_exception(std::wstring(L"OpenProcessToken"),GetLastError(),_T(__FUNCTION__),__LINE__);
            g_strErrorMsg = L"OpenProcessToken error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
        }  
        if( !LookupPrivilegeValue(NULL, lpszPrivilege,&luid) )  
        {  
            //throw JGW_exception(std::wstring(L"LookupPrivilegeValue"),GetLastError(),_T(__FUNCTION__),__LINE__);
            g_strErrorMsg = L"LookupPrivilegeValue error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
        } 

        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = bEnablePrivilege?SE_PRIVILEGE_ENABLED:0;

        if( !AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,   (PDWORD) NULL) )  
        {   
            g_strErrorMsg = L"AdjustTokenPrivileges error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
            //throw JGW_exception(std::wstring(L"AdjustTokenPrivileges"),GetLastError(),_T(__FUNCTION__),__LINE__);
        }

        if( ERROR_NOT_ALL_ASSIGNED == GetLastError() )  
        {  
            g_strErrorMsg = L"The token does not have the specified privilege.error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
            //throw JGW_exception(std::wstring(L"The token does not have the specified privilege. "),GetLastError(),_T(__FUNCTION__),__LINE__);
        } 

        return true;  
    }
    //!
    bool JGW_KillProcessToName(const std::wstring& strName)
    {
        PROCESSENTRY32 ProcStruct = {sizeof(PROCESSENTRY32)};  
        BOOL		bResult;  
        HANDLE		ProcHandle = NULL;

        ProcHandle = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
        if( !ProcHandle ) 
        {
            g_strErrorMsg = L"CreateToolhelp32Snapshot error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
            //JGW_exception(std::wstring(L"CreateToolhelp32Snapshot"),GetLastError(),_T(__FUNCTION__),__LINE__);
        }
        //ProcStruct.dwSize=sizeof(ProcStruct);  

        bResult = Process32First(ProcHandle,&ProcStruct);  
        while(bResult)  
        {  
            if( NULL != JGW_WStrCaseWStr(ProcStruct.szExeFile,strName.c_str()) && GetCurrentProcessId() != ProcStruct.th32ProcessID)  
            {  
                JGW_KillProcessID(ProcStruct.th32ProcessID);
            }    
            bResult = Process32Next(ProcHandle,&ProcStruct);  
        }
        return true;
    }

    DWORD JGW_GetProcessModuleIDToProcessID(DWORD dwProcessID)
    {
        PROCESSENTRY32 ProcStruct = {sizeof(PROCESSENTRY32)};  
        BOOL		bResult;  
        HANDLE		ProcHandle = NULL;

        ProcHandle = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
        if( !ProcHandle ) 
        {
            g_strErrorMsg = L"CreateToolhelp32Snapshot error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return -1;
        }

        bResult = Process32First(ProcHandle,&ProcStruct);  
        while(bResult)  
        {  
            if( dwProcessID == ProcStruct.th32ProcessID)  
            {  
                return ProcStruct.th32ModuleID;
            }    
            bResult = Process32Next(ProcHandle,&ProcStruct);  
        }
        return -1;
    }


    std::wstring JGW_GetProcessNameToProcessID(DWORD dwProcessID)
    {
        PROCESSENTRY32 ProcStruct = {sizeof(PROCESSENTRY32)};  
        BOOL		bResult;  
        HANDLE		ProcHandle = NULL;

        ProcHandle = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
        if( !ProcHandle ) 
        {
            g_strErrorMsg = L"CreateToolhelp32Snapshot error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            return L"";
        }

        bResult = Process32First(ProcHandle,&ProcStruct);  
        while(bResult)  
        {  
            if( dwProcessID == ProcStruct.th32ProcessID)  
            {  
                return ProcStruct.szExeFile;
            }    
            bResult = Process32Next(ProcHandle,&ProcStruct);  
        }
        return L"";
    }

    //! 根据进程名称获取进程ID   不计算自身ID  未找到返回-1
    DWORD JGW_GetProcessID2Name(const std::wstring& strName)
    {
        PROCESSENTRY32 ProcStruct = {sizeof(PROCESSENTRY32)};  
        BOOL		bResult;  
        HANDLE		ProcHandle = NULL;

        ProcHandle = CreateToolhelp32Snapshot((DWORD)TH32CS_SNAPPROCESS,0);
        if( !ProcHandle ) 
        {
            g_strErrorMsg = L"CreateToolhelp32Snapshot error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return -1;
            //JGW_exception(std::wstring(L"CreateToolhelp32Snapshot"),GetLastError(),_T(__FUNCTION__),__LINE__);
        }
        //ProcStruct.dwSize=sizeof(ProcStruct);  

        bResult = Process32First(ProcHandle,&ProcStruct);  
        while(bResult)  
        {  
            if( NULL != JGW_WStrCaseWStr(ProcStruct.szExeFile,strName.c_str()) && GetCurrentProcessId() != ProcStruct.th32ProcessID)  
            {  
                return ProcStruct.th32ProcessID;
            }    
            bResult = Process32Next(ProcHandle,&ProcStruct);  
        }
        return -1;
    }
    //! Kill 进程
    void JGW_KillProcessID(DWORD dwProcessID)
    {
        if( dwProcessID <= 0)
        {
            return ;
        }

        HANDLE hp = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,dwProcessID);   
        if( !hp ) { JGW_GetPrivilege(); hp = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,dwProcessID);  }
        if( !hp ) 
        {
            g_strErrorMsg = L"OpenProcess error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return ;
            //! throw JGW_exception(std::wstring(L"OpenProcess"),GetLastError(),_T(__FUNCTION__),__LINE__);
        }
        JGW_TerminateProcess(hp);
    }
    //! windows 版本
    bool JGW_GetWindowsTypes(EWINDOWS_VERSION &eVersion)
    {
        OSVERSIONINFO OSInfo = {sizeof(OSVERSIONINFO)};

        if( !GetVersionEx(&OSInfo) )
        {
            g_strErrorMsg = L"GetVersionEx error:";
            g_strErrorMsg += JGW_GetSystemErrorString( GetLastError() );
            //CTFLogWE( g_strErrorMsg.c_str() );
            return false;
            //! throw JGW_exception(std::wstring(L"GetVersionEx"),GetLastError(),_T(__FUNCTION__),__LINE__);
        }

        switch( OSInfo.dwPlatformId )
        {
        case VER_PLATFORM_WIN32s:
            eVersion = Windows_3;
            break;
        case VER_PLATFORM_WIN32_WINDOWS:
            eVersion = Windows_95;
            break;
        case VER_PLATFORM_WIN32_NT:
            if( 3 == OSInfo.dwMajorVersion )
            {
                eVersion = Windows_NT_3;
            }
            else if ( 4 == OSInfo.dwMajorVersion )
            {
                eVersion = Windows_NT_4;
            }
            else if ( 5 == OSInfo.dwMajorVersion &&0 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_2000;
            }
            else if ( 5 == OSInfo.dwMajorVersion &&1 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_XP;
            }
            else if( 6 == OSInfo.dwMajorVersion &&0 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_VISTA;
            }
            else if ( 6 == OSInfo.dwMajorVersion && 1 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_WIN7;
            }
            else if ( 6 == OSInfo.dwMajorVersion && 2 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_WIN8;
            }
            else if ( 6 == OSInfo.dwMajorVersion && 3 == OSInfo.dwMinorVersion )
            {
                eVersion = WINDOWS_WIN8_1;
            }
            else if( OSInfo.dwMajorVersion >= 6 )
            {
                eVersion = WINDOWS_WIN10;
            }
            else
            {
                eVersion = UNKNOW_WINDOWS_VERSION;
            }
            break;
        default:
            eVersion = UNKNOW_WINDOWS_VERSION;
        }

        return true;
    }

    void JGW_GetPrivilege( const wchar_t *lpszPrivilege/* = SE_DEBUG_NAME*/, bool bEnablePrivilege/* = true*/ )
    {
        EWINDOWS_VERSION eVersion;
        JGW_GetWindowsTypes( eVersion );
        if ( eVersion >= WINDOWS_WIN7 )
        {
            JGW_SetPrivilege(lpszPrivilege,bEnablePrivilege);
        }
    }


    // 获取进程路径 参数1：进程ID 参数2：缓冲区指针，接收路径
    void JGW_GetProcessPath(DWORD dwProcessID, std::wstring& strProcessPath)
    {
        TCHAR Filename[MAX_PATH];
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
        if (hProcess == NULL)return;
        HMODULE hModule;
        DWORD cbNeeded;
        if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
        {
            if (GetModuleFileNameEx(hProcess, hModule, Filename, MAX_PATH)){
                strProcessPath = Filename;
                //RtlMoveMemory((void*)buffer, Filename, sizeof(TCHAR)*MAX_PATH);
            }
        }
        else{
            DWORD size = MAX_PATH;
            if (QueryFullProcessImageName(hProcess, 0, Filename, &size)){
                strProcessPath = Filename;
                //RtlMoveMemory((void*)buffer, Filename, sizeof(TCHAR)*MAX_PATH);
            }
        }
        CloseHandle(hProcess);
    }
}
