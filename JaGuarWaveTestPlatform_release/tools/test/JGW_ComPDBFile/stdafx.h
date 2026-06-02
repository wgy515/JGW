// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>


// TODO: 在此处引用程序需要的其他头文件
#define Log4WE(log) std::wcout << log << std::endl

#define Log4WI(log) std::wcout << log << std::endl

#define Log4AE(log) std::cout << log << std::endl
#define Log4AD(log) //! std::cout << log << std::endl
#define Log4AD_F(log_buf,...) //! JGW_Write_LogA_Format(0,"",log_buf,__VA_ARGS__)
#define JGW_GETLASTERRORINFO L"" 
#define Log4WI_F(log_buf,...)  JGW_Write_LogW_Format(0,"",log_buf,__VA_ARGS__)
#define Log4AI_F(log_buf,...)  JGW_Write_LogA_Format(0,"",log_buf,__VA_ARGS__)
#define Log4WE_F(log_buf,...)  JGW_Write_LogW_Format(0,"",log_buf,__VA_ARGS__)

extern void JGW_Write_LogW_Format(int log_type,const char* tag,const wchar_t* log_buf,...);

extern void JGW_Write_LogA_Format(int log_type,const char* tag,const char* log_buf,...);
