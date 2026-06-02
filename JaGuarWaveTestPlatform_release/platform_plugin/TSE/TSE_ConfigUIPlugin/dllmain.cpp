// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "Config/TSE_LANIPConfig.h"
#include "Throughput/TSE_ThroughputConfig.h"
#include "TSE/TSE_BaseConfig.h"
#include "TSE_CSVConfigUI.h"
#include "TSE_InputNumberWindow.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
	POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_ThroughputConfig)
    POCO_EXPORT_CLASS(CTSE_BaseConfig)
	POCO_EXPORT_CLASS(CTSE_LANIPConfig)
    POCO_EXPORT_CLASS(CTSE_CSVConfigUI)
	POCO_EXPORT_CLASS(CTSE_InputNumberWindow)
POCO_END_MANIFEST

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

