#pragma once
#include <string>
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	extern "C" {
#endif
		JGW_FOUNDATIONFUNC_LIB_API bool jgw_single_application_instance_module( const wchar_t* szapplicationguid ,void* hModule = NULL);

		JGW_FOUNDATIONFUNC_LIB_API bool jgw_single_application_instance( const wchar_t* szapplicationguid ,const wchar_t* szapplicationname = NULL);

        JGW_FOUNDATIONFUNC_LIB_API bool jgw_check_single_application_instance( const wchar_t* szapplicationguid ,const wchar_t* szapplicationname = NULL);

		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetApplicationName2HModule( void* hModule );

#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	}
#endif
}