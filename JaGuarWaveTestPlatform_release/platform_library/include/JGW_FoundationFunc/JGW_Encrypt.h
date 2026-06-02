#pragma once
#ifndef JGW_FOUNDATIONFUNC_FUNCLIB_ENCRYPT_H__
#define JGW_FOUNDATIONFUNC_FUNCLIB_ENCRYPT_H__
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        //! ¼ÓÃÜ×Ö·û´®
        JGW_FOUNDATIONFUNC_LIB_API void JGW_Encrypt(char* buf,size_t buf_len);
        /// ½âÃÜ×Ö·û´®
        JGW_FOUNDATIONFUNC_LIB_API void JGW_Decrypt(char* buf,size_t buf_len);
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}

#endif