//
// xxx.h
//
// $Id: xxx.h#3 $
//
// Library: 
// Package: 
// Module:  
//
// TDO:
//
// Copyright (c) 2021-2021
//
// SPDX-License-Identifier:	
//
#ifndef TSE_MSFOUNDATIONFUNCLIB_DEFINE_H__
#define TSE_MSFOUNDATIONFUNCLIB_DEFINE_H__

#if (defined TSE_MSFOUNDATIONFUNCLIB_EXPORT) || (defined TSE_MSFOUNDATIONFUNCLIB_IMPORT) || (defined TSE_MSFOUNDATIONFUNCLIB_DLL_EXPORTS) || (defined  TSE_MSFOUNDATIONFUNCLIB_DLL_IMPORT)
#else
#define TSE_MSFOUNDATIONFUNCLIB_IMPORT
#endif

#if (defined TSE_MSFOUNDATIONFUNCLIB_EXPORT) || (defined TSE_MSFOUNDATIONFUNCLIB_IMPORT)
#	define TSE_MSFOUNDATIONFUNCLIB_API extern
#	define TSE_MSFOUNDATIONFUNCLIB_CLASS
#endif


#ifdef TSE_MSFOUNDATIONFUNCLIB_DLL_EXPORTS
#	define JGW_FOUNDATIONFUNC_DLL_EXTERN_
#	if _MSC_VER >= 1500
#		define TSE_MSFOUNDATIONFUNCLIB_API __declspec(dllexport)
#		define TSE_MSFOUNDATIONFUNCLIB_CLASS __declspec(dllexport)
#	else
#		define TSE_MSFOUNDATIONFUNCLIB_API 
#		define TSE_MSFOUNDATIONFUNCLIB_CLASS 
#	endif
#endif

// #ifdef TSE_MSFOUNDATIONFUNCLIB_IMPORT
// #		define TSE_MSFOUNDATIONFUNCLIB_API extern
// #		define TSE_MSFOUNDATIONFUNCLIB_CLASS 
// #endif

#ifdef TSE_MSFOUNDATIONFUNCLIB_DLL_IMPORT
#	define JGW_FOUNDATIONFUNC_DLL_EXTERN_
#	if _MSC_VER >= 1500
#		define TSE_MSFOUNDATIONFUNCLIB_API __declspec(dllimport)
#		define TSE_MSFOUNDATIONFUNCLIB_CLASS __declspec(dllimport)
#	else
#		define TSE_MSFOUNDATIONFUNCLIB_API 
#		define TSE_MSFOUNDATIONFUNCLIB_CLASS 
#	endif
// #	if _DEBUG
// #		pragma comment(lib,"JGW_FOUNDATIONFUNCFuncLib_d.lib")
// #	else
// #		pragma comment(lib,"JGW_FOUNDATIONFUNCFuncLib.lib")
// #	endif
#endif

#if (defined TSE_MSFOUNDATIONFUNCLIB_IMPORT) || (defined TSE_MSFOUNDATIONFUNCLIB_DLL_IMPORT)
#	if _DEBUG
#		pragma comment(lib,"TSE_MSFoundationLib_d.lib")
#	else
#		pragma comment(lib,"TSE_MSFoundationLib.lib")
#	endif
#endif

#include <string>
#define MAX_MSG_SIZE 5120
namespace JGW
{
    typedef struct 
    {
        //! encrypt : 
        bool mbEncrypt;
        //! msg :
        std::string mstrMessage;
    }S_TSE_LOG_MS,*LPS_TSE_LOG_MS;

    typedef struct 
    {
        //! encrypt : 
        bool mbEncrypt;
        //! msg :
        std::string mstrControl;
    }S_TSE_CONTROL_MS,*LPS_TSE_CONTROL_MS;

    /// <summary>
    /// MS(控制信息) -> MC(返回结果) -> MS(接收到结果,返回已接受信息到MC) -> MC
    /// </summary>
    enum TSEControlTypeEnum
    {
        /// <summary>
        /// 退出当前程序 => MC MS接收到此信息都进行退出操作
        /// </summary>
        TSEControlExitApplicationType = -999999,
        /// <summary>
        /// MC -> MS注册消息
        /// </summary>
        TSEControlUnknownType = -1,
        /// <summary>
        /// MC -> MS注册消息
        /// </summary>
        TSEControlRegType = 0,
        /// <summary>
        /// MS -> MC返回注册结果
        /// </summary>
        TSEControlRegResultType = 1,
        /// <summary>
        /// MS -> MC 初始化QSPR/XTT等信息
        /// </summary>
        TSEControlXttType = 2,
        /// <summary>
        /// MC -> MS 初始化QSPR/XTT结果 （MS -> MC告知已接收到结果）
        /// </summary>
        TSEControlXttResultType = 3,
        /// <summary>
        /// MS -> MC 初始化全局变量 
        /// </summary>
        TSEControlGlobalVariableType = 4,
        /// <summary>
        /// MC -> MS 初始化全局变量 （MS -> MC告知已接收到结果）
        /// </summary>
        TSEControlGlobalVariableResultType = 5,
        /// <summary>
        /// MS -> MC 进行QSPR测试操作相关
        /// </summary>
        TSEControlOperTestType = 6,
        /// <summary>
        /// MC -> MS 返回测试结果 （MS -> MC告知已接收到结果）
        /// 只是表示已接受到此信息
        /// </summary>
        TSEControlOperTestResultType = 7,
        /// <summary>
        /// 测试完后结果 MC -> MS
        /// </summary>
        TSEControlTestCompleteResultType = 8,
        /// <summary>
        /// MS -> MC告知已经收到此结果
        /// </summary>
        TSEControlTestCompleteResultResultType = 9,
    };

    typedef struct
    {
        TSEControlTypeEnum Type;
        /// Control Type
    }S_MCSBASEJSON;

    typedef struct
    {
        TSEControlTypeEnum Type;
        bool Result;
        std::string Error;
    }S_MC_RSP_JSON;



}

#endif