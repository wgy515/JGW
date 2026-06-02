// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <poco/ClassLibrary.h>
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include <TSE_MessageServer/TSE_MessageServerDllExport.h>
#include "TSE_QSPRAdaptTest.h"
#include "TSE_CopyDataQiaTest.h"
using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    /*POCO_EXPORT_CLASS(CTSE_QSPRAdaptTest)*/
    POCO_EXPORT_CLASS(CTSE_CopyDataQiaTest)
POCO_END_MANIFEST

void POCO_LIBRARY_API pocoInitializeLibrary()
{
    //StartTSEMessageServer();
}

void POCO_LIBRARY_API pocoUninitializeLibrary()
{
    //StopTSEMessageServer();
}

#ifdef _DEBUG
#pragma comment(lib,"TSE_FoundationLib_d.lib")
#else
#pragma comment(lib,"TSE_FoundationLib.lib")
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
#ifdef IS_ENCRYPT_MODULE
        return JGW::EncryptModuleIsLoaded(hModule) ? TRUE : FALSE;
#endif  
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

