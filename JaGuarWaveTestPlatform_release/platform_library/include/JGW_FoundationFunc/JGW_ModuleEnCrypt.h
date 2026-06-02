#pragma once
#ifndef JGW_FOUNDATIONFUNC_FUNCLIB_MODULEENCRYPT_H__
#define JGW_FOUNDATIONFUNC_FUNCLIB_MODULEENCRYPT_H__
#include "JGW_FoundationFuncLib_Define.h"

#define IS_ENCRYPT_MODULE

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        //! 加密DLL是否能被加载
        JGW_FOUNDATIONFUNC_LIB_API bool EncryptModuleIsLoaded(HMODULE hModule);
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}
#endif