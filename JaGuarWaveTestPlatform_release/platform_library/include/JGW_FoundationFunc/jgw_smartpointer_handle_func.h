/*!
 *********************************************************************
 *  $Id: jgw_smartpointer_handle_func.h
 *
 *  Project : JaGuarWaveTestPlatform
 *
 *  Package : JGW_FoundationFunc
 *
 *  Company :Jaguar Wave Technology
 *
 *  Purpose : Declaration of JGW_FoundationFunc
 *
 * Des: 智能智能句柄释放函数
 *********************************************************************
<b>Version History:</b>
\verbatim
V0.1	06/xx/2018		First Revision
\endverbatim
 *********************************************************************
 */
#pragma once
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" {
#endif
        //! 
        JGW_FOUNDATIONFUNC_LIB_API void CloseFileHandle(void* hFileHandel);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API void DestroyDeviceInfoList(void* hDevInfo);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API void CloseRegKey(void* hKey);
            
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}