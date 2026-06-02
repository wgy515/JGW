// stdafx.cpp : 只包括标准包含文件的源文件
// StreamingDownload.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "9x25_download_define.h"
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
void MyLog(int log_level ,const char *func_name, int line_number, const char *format, ...)
{
    if (log_level > LOG_INFO) return;
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
    printf("\r\n");
}