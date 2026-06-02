// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
/*#include "QSEQ_Debug.h"*/
#include <poco/ClassLibrary.h>
#include "TSE_SequenceManageLuaExport.h"
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;

POCO_BEGIN_MANIFEST(CTSE_SequenceManageInterface)
	POCO_EXPORT_CLASS(CTSE_SequenceManageLuaExport)
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
	default:
		break;
	} 
#ifdef QSEQ_MEMORY_LEAK_CHECK
	_CrtSetReportMode(_CRT_WARN,  _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_DETACH:
		{
			_RPT0 ( _CRT_ERROR, "***** Memory Leak Check - QSEQ_Core.dll *****\n" );
			_CrtDumpMemoryLeaks();
		}
		break;

	default:
		break;
	}
#endif
	return TRUE;
}

