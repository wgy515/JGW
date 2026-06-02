// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include "CRFAntTestView.h"
#include <poco/ClassLibrary.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

POCO_BEGIN_MANIFEST(CCJGWUiLibViewResolverImpl)
    POCO_EXPORT_CLASS(JGW::CCRFAntTestView)
POCO_END_MANIFEST


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

