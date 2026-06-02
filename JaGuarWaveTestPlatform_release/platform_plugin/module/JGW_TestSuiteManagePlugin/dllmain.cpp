// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteManageImpl.h>
#include "TSE_SequenceManager.h"
#include <poco/ClassLibrary.h>
using namespace JGW;

POCO_BEGIN_MANIFEST(CTSE_SequenceManageImpl)
    POCO_EXPORT_CLASS(TSE_SequenceManager)
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
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}