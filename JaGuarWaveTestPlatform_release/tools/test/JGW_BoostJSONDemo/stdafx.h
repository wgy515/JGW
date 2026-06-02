// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

extern void LogF(const char* szBuf,...);
extern void Log(const char* szBuf);
// TODO: 在此处引用程序需要的其他头文件
#define LogE Log 
#define LogE_F LogF 