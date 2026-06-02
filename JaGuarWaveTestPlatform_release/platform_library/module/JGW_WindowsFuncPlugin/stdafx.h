// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
// TODO: 在此处引用程序需要的其他头文件
 #define JGW_TAG "main"
 #include <JGW_Log4cplusImplPlugin/log4cplusimpl.h>
#if 0
#define Log4AE //!
#define Log4AD //!
#define Log4AD_F //!
#define Log4WE_F //!
#define Log4WD_F //! 
#define Log4WE //! 
#define Log4WD //!
#define Log4WI_F //!
#define Log4WI //!
#define Log4AI_F //! 
#define Log4AI //! 
#define Log4AE_F //!
#endif
// #define TESTFAIL(x) \
//     if( !(x) ) return false;

#define TESTFAILHADNLE(x) \
    if( !(x) || INVALID_HANDLE_VALUE == (x) ) return false;

#define CTF_CLOSEHANDLE(hHandle)\
    if( hHandle && hHandle != INVALID_HANDLE_VALUE ){\
    CloseHandle(hHandle);\
    hHandle = NULL;\
    }