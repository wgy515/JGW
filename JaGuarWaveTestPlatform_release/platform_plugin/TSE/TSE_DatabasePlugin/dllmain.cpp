// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "excel/TSE_GetExcelDataToPrimaryKey.h"
#include "excel/TSE_LoadExcelDatabase.h"
#include "excel/TSE_UpdateExcelDatabase.h"
#include "sqlite3/TSE_GetSQLite3DBToPrimaryKey.h"
#include "sqlite3/TSE_UpdateSQLite3DB.h"
#include "mes/TSE_MESCheckSNStatus.h"
#include "mes/TSE_MESNextSNStation.h"
#include "mes/TSE_MESGetIMEIInfo.h"
#include "mes/TSE_MESNextIMEIStation.h"
#include "mes/TSE_MESSaveTestRecords.h"
#include "mes/TSE_MESGetSNInfo.h"
#include "bin/TSE_BuildDeviceInfoBin.h"
#include "bin/TSE_ReadDeviceInfoBin.h"
#include "bin/TSE_SaveDeviceInfoBin.h"
#include "bin/TSE_UpdateDeviceInfoBin.h"
#include "TSE_CloseCSVDatabase.h"
#include "TSE_GetCSVDataToPrimaryKey.h"
#include "TSE_LoadCSVDatabase.h"
#include "TSE_SetExcelDataToPrimaryKey.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_LoadExcelDatabase)
    POCO_EXPORT_CLASS(CTSE_GetExcelDataToPrimaryKey)
    POCO_EXPORT_CLASS(CTSE_UpdateExcelDatabase)
    POCO_EXPORT_CLASS(CTSE_GetSQLite3DBToPrimaryKey)
    POCO_EXPORT_CLASS(CTSE_UpdateSQLite3DB)
    POCO_EXPORT_CLASS(CTSE_MESCheckSNStatus)
    POCO_EXPORT_CLASS(CTSE_MESNextSNStation)
    POCO_EXPORT_CLASS(CTSE_MESGetIMEIInfo)
    POCO_EXPORT_CLASS(CTSE_MESNextIMEIStation)
    POCO_EXPORT_CLASS(CTSE_MESSaveTestRecords)
    POCO_EXPORT_CLASS(CTSE_MESGetSNInfo)
    POCO_EXPORT_CLASS(CTSE_BuildDeviceInfoBin)
    POCO_EXPORT_CLASS(CTSE_ReadDeviceInfoBin)
    POCO_EXPORT_CLASS(CTSE_SaveDeviceInfoBin)
    POCO_EXPORT_CLASS(CTSE_UpdateDeviceInfoBin)
    POCO_EXPORT_CLASS(CTSE_CloseCSVDatabase)
    POCO_EXPORT_CLASS(CTSE_GetCSVDataToPrimaryKey)
    POCO_EXPORT_CLASS(CTSE_LoadCSVDatabase)
    POCO_EXPORT_CLASS(CTSE_SetExcelDataToPrimaryKey)
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

