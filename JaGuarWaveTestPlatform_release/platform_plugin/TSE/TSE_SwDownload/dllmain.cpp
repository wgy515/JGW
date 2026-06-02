// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <poco/ClassLibrary.h>

#include "TSE_SaharaOpenPort.h"
#include "TSE_SaharaArmPrgDownload.h"
#include "TSE_DisconnectSaharaPort.h"
#include "TSE_ConnectDiagPhone.h"
#include "TSE_StreamingDownloadUserPartitions.h"
#include "TSE_DisconnectDiagPhone.h"
#include "TSE_QMSLLog.h"
#include "sb3\TSE_OpenSaharaDownloadPort.h"
#include "sb3\TSE_SaharaFileDownload.h"
#include "sb3\TSE_SaharaSwitchMode.h"
#include "sb3\TSE_CloseDownloadPort.h"
#include "sb3\TSE_StreamingFileDownload.h"
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_SaharaOpenPort)
    POCO_EXPORT_CLASS(CTSE_SaharaArmPrgDownload)
    POCO_EXPORT_CLASS(CTSE_DisconnectSaharaPort)
    POCO_EXPORT_CLASS(CTSE_QMSLLog)
    POCO_EXPORT_CLASS(CTSE_ConnectDiagPhone)
    POCO_EXPORT_CLASS(CTSE_StreamingDownloadUserPartitions)
    POCO_EXPORT_CLASS(CTSE_DisconnectDiagPhone)
    POCO_EXPORT_CLASS(CTSE_OpenSaharaDownloadPort)
    POCO_EXPORT_CLASS(CTSE_SaharaFileDownload)
    POCO_EXPORT_CLASS(CTSE_SaharaSwitchMode)
    POCO_EXPORT_CLASS(CTSE_CloseDownloadPort)
    POCO_EXPORT_CLASS(CTSE_StreamingFileDownload)
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

