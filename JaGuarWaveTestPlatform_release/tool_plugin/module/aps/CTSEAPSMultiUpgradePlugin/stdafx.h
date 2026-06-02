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
#define SUB_PLUGIN_VERSION L"APS Multiunit Upgrade Firmware Plugin V1R01"

#define JGW_TAG "main"
#include <JGW_Log4cplusImplPlugin\log4cplusimpl.h>

/*
1、检测版本是否和配置的版本匹配，如果匹配则直接返回PASS
2、不匹配，开启升级模式->直到升级完成
3、再回复到一状态，不过判断下一状态当前流程是否完成，如果完成则将界面的BUSY显示改为PASS显示
*/