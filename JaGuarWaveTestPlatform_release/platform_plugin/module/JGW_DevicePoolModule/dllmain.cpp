// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include "CJGW_DevicePoolThread.h"
#include <poco/ClassLibrary.h>

POCO_BEGIN_MANIFEST(JGW::CCJGW_DevicePoolImpl)
    POCO_EXPORT_SINGLETON(JGW::CCJGW_DevicePoolThread)
POCO_END_MANIFEST

void pocoInitializeLibrary()
{

}

void pocoUninitializeLibrary()
{

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

