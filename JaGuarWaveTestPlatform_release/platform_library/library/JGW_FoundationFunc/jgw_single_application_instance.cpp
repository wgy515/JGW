#include "stdafx.h"
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
namespace JGW
{
    bool jgw_check_single_application_instance( const wchar_t* szapplicationguid ,const wchar_t* szapplicationname/* = NULL*/)
    {
        HANDLE hMutex = CreateMutex( NULL, false,szapplicationguid );
        if ( GetLastError() == ERROR_ALREADY_EXISTS )
        {
            CloseHandle(hMutex);
            return false; 
            /*if( IDOK == MessageBox(GetFocus(),_T("程序正在在运行中,是否等待程序正常退出!\n1、请等10S左右在进行打开程序\n2、在任务管理起中关闭此进程\n3、点击取消按钮，程序自动关闭前面实例"), _T("提示"),MB_OKCANCEL) )
            {
                HANDLE   hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId());   
                TerminateProcess(hp,0);
                return false;
            }
            else
            {
                std::wstring strTemp;
                if( NULL == szapplicationname )
                {
                    strTemp = JGW_GetApplicationName2HModule(NULL);
                    szapplicationname = strTemp.c_str();
                }
#ifdef THROW_CTF
                try
                {
                    JGW_GetPrivilege();
                    JGW_KillProcessID(JGW_GetProcessID2Name(std::wstring(szapplicationname)));
                    Sleep(100);
                    hMutex = CreateMutex( NULL, false,szapplicationguid );
                }
                catch (JGW_exception& e)
                {
                    e.GetErrorDescription();
                    return false;
                }
#else
                JGW_GetPrivilege();
                JGW_KillProcessID(JGW_GetProcessID2Name(std::wstring(szapplicationname)));
                Sleep(100);
                hMutex = CreateMutex( NULL, false,szapplicationguid );
#endif
            }*/
        }
        return true;
    }

	bool jgw_single_application_instance( const wchar_t* szapplicationguid ,const wchar_t* szapplicationname/* = NULL*/)
	{
		HANDLE hMutex = CreateMutex( NULL, false,szapplicationguid );
		if ( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			CloseHandle(hMutex);
			if( IDOK == MessageBox(GetFocus(),GetString(E_APPLICATON_REPEAT_RUN_TYPE), GetString(E_PROMPT_TYPE), /*MB_OK +*/MB_OKCANCEL) )
			{
				HANDLE   hp   =   OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId());   
				TerminateProcess(hp,0);
				return false;
			}
			else
			{
				std::wstring strTemp;
				if( NULL == szapplicationname )
				{
					strTemp = JGW_GetApplicationName2HModule(NULL);
					szapplicationname = strTemp.c_str();
				}
#ifdef THROW_CTF
				try
				{
					JGW_GetPrivilege();
					JGW_KillProcessID(JGW_GetProcessID2Name(std::wstring(szapplicationname)));
					Sleep(100);
					hMutex = CreateMutex( NULL, false,szapplicationguid );
				}
				catch (JGW_exception& e)
				{
					e.GetErrorDescription();
					return false;
				}
#else
				JGW_GetPrivilege();
				JGW_KillProcessID(JGW_GetProcessID2Name(std::wstring(szapplicationname)));
				Sleep(100);
				hMutex = CreateMutex( NULL, false,szapplicationguid );
#endif
			}
		}
		return true;
	}

	bool jgw_single_application_instance_module(const wchar_t* szapplicationguid ,void* hModule /* = NULL */)
	{
		std::wstring strTemp;
		strTemp = JGW_GetApplicationName2HModule(hModule);
		return jgw_single_application_instance(szapplicationguid,strTemp.c_str());
	}

	std::wstring JGW_GetApplicationName2HModule( void* hModule )
	{
		wchar_t tszModule[MAX_PATH + 1] = { 0 };
		::GetModuleFileName((HMODULE)hModule, tszModule, MAX_PATH);
		/*std::wstring strtemp = tszModule;
		size_t nIndex = strtemp.find_last_of(_T('\\'));
		if( std::wstring::npos == nIndex )
		{
			return strtemp;
		}
		strtemp.erase(0,nIndex+1);*/
		return JGW_GetFileNameToFilePath(tszModule);
	}
}