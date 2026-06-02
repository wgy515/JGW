// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>


#define GREEN	RGB(0, 125, 125)
#define RED		RGB(250,0,0)
#define BULE	RGB(125,125,125)
#define BALCK	RGB(0,0,250)

// TODO: 在此处引用程序需要的其他头文件
#define SUB_PLUGIN_VERSION L"TSE Test Plugin V1R01"

#define JGW_TAG "main"
#include <JGW_Log4cplusImplPlugin\log4cplusimpl.h>