// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "TSE_RKDInit.h"
#include "TSE_RKInitialize.h"
#include "TSE_RKReadOtpData.h"
#include "TSE_RKSwitchLoader.h"
#include "TSE_ScanRKDevice.h"
#include "TSE_RKResetRockusb.h"
#include "TSE_RKReadVendorRpmbData.h"
#include "TSE_RKWriteVendorRpmbData.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_RKDInit)
    POCO_EXPORT_CLASS(CTSE_RKInitialize)
    POCO_EXPORT_CLASS(CTSE_RKReadOtpData)
    POCO_EXPORT_CLASS(CTSE_RKSwitchLoader)
    POCO_EXPORT_CLASS(CTSE_RKResetRockusb)
    POCO_EXPORT_CLASS(CTSE_ScanRKDevice)
    POCO_EXPORT_CLASS(CTSE_RKWriteVendorRpmbData)
    POCO_EXPORT_CLASS(CTSE_RKReadVendorRpmbData)
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

