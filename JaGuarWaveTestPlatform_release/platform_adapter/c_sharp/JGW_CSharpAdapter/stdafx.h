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
#ifdef __cplusplus
extern "C" 
{
#endif
	/// <summary>
	/// 加载TSE模块
	/// </summary>
	/// <param name="moduleName">模块DLL名称</param>
	/// <returns></returns>
	__declspec(dllexport) void x32123458(const wchar_t* moduleName);
#ifdef __cplusplus
};
#endif