// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>
#include "CJGW_DevicesGlobalResource.h"
#include "RFCAL_QSEQ/CJGW_RFCalVFSSuite.h"
#include "QCOMM/CJGW_ConnectQcomDiag.h"
#include "QCOMM/CJGW_ReadQcomNV.h"
#include "QCOMM/CJGW_WriteQcomNV.h"
#include "QCOMM/CJGW_SyncQcomNV.h"
#include "QCOMM/CJGW_DisConnectQocmDiag.h"
#include <poco/ClassLibrary.h>



using namespace JGW;

POCO_BEGIN_MANIFEST(CCJGW_TestSuiteImpl)
    POCO_EXPORT_CLASS(CCJGW_RFCalVFSSuite)
    POCO_EXPORT_CLASS(CCJGW_ConnectQcomDiag)
    POCO_EXPORT_CLASS(CCJGW_WriteQcomNV)
    POCO_EXPORT_CLASS(CCJGW_ReadQcomNV)
    POCO_EXPORT_CLASS(CCJGW_SyncQcomNV)
    POCO_EXPORT_CLASS(CCJGW_DisConnectQocmDiag)
POCO_END_MANIFEST


    void pocoInitializeLibrary()
{
    PDLOG4WW(L"Initialize JGW_TestSuiteDevicesPlugin Plugin");
}

void pocoUninitializeLibrary()
{
    PDLOG4WW(L"Uninitialize JGW_TestSuiteDevicesPlugin Plugin");
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
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}