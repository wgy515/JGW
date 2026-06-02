// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "TSE_OpenInstrumentGPIB.h"
#include "TSE_OpenInstrumentVISA.h"
#include "TSE_WriteInstrumentCommand.h"
#include "TSE_WriteInstrumentRawCommand.h"
#include "TSE_CloseInstrument.h"
#include "TSE_ReadInstrumentVoltageCurrent.h"
#include "TSE_MultiWriteInstrumentCommand.h"
#include "TSE_FetchResultInstrumentCommand.h"
#include "TSE_ParseP760RssiTestResult.h"
#include "truntable/TSE_OpenTruntable.h"
#include "truntable/TSE_CloseTruntable.h"
#include "truntable/TSE_SettingTruntable.h"
#include "TSE_LoadWlanLossConfig.h"
#include "TSE_GetRFConfigLoss.h"
#include "TSE_CheckQueryInstrumentCommand.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_OpenInstrumentGPIB)
    POCO_EXPORT_CLASS(CTSE_OpenInstrumentVISA)
    POCO_EXPORT_CLASS(CTSE_WriteInstrumentCommand)
    POCO_EXPORT_CLASS(CTSE_ReadInstrumentVoltageCurrent)
    POCO_EXPORT_CLASS(CTSE_WriteInstrumentRawCommand)
    POCO_EXPORT_CLASS(CTSE_MultiWriteInstrumentCommand)
    POCO_EXPORT_CLASS(CTSE_OpenTruntable)
    POCO_EXPORT_CLASS(CTSE_CloseTruntable)
    POCO_EXPORT_CLASS(CTSE_SettingTruntable)
    POCO_EXPORT_CLASS(CTSE_CloseInstrument)
    POCO_EXPORT_CLASS(CTSE_FetchResultInstrumentCommand)
    POCO_EXPORT_CLASS(CTSE_ParseP760RssiTestResult)
    POCO_EXPORT_CLASS(CTSE_LoadWlanLossConfig)
	POCO_EXPORT_CLASS(CTSE_CheckQueryInstrumentCommand)
    POCO_EXPORT_CLASS(CTSE_GetRFConfigLoss)
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

