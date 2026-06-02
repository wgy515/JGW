// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <JGW_TestRecord/JGW_TestRecord_Exports.h>
#include "CJGW_TestRecordConfig.h"
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>

JGW::CCJGW_CriticalSectionLock gcCriticalSectionLock;
std::vector<std::shared_ptr<JGW::CCJGW_TestRecordConfig>> gvPtrTestRecordConfig;

JGW::CCJGW_TestRecordImpl* GetTestRecordInstance()
{
    static JGW::CCJGW_TestRecordConfig gTestRecordConfig;
    return &gTestRecordConfig;
}



JGW::CCJGW_TestRecordImpl* GetTestRecordInstanceToIndex()
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gcCriticalSectionLock);
    gvPtrTestRecordConfig.push_back(std::make_shared<JGW::CCJGW_TestRecordConfig>());
    return gvPtrTestRecordConfig[gvPtrTestRecordConfig.size() - 1].get();
}


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

