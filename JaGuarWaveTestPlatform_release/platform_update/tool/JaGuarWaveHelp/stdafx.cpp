// stdafx.cpp : 只包括标准包含文件的源文件
// JaGuarWaveHelp.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
HWND gMainHwnd = NULL;
HINSTANCE ghInstance = NULL;

void PrintLogW(const wchar_t *format, ...)
{
    std::wstring strLog;
    FORMAT_WSTRING(strLog,format);
    Log4WI(strLog.c_str());
    SendMessage(gMainHwnd,WM_HWND_RICHEDIT_APPEND_TEST_MSG,WPARAM(strLog.c_str()),NULL);
}

void PrintLog(const char *format, ...)
{
    std::string strLog("");
    FORMAT_STRING(strLog,format);
    std::wstring wstrLog = JGW::JGW_A2W(strLog);
    Log4WI(wstrLog.c_str());
    SendMessage(gMainHwnd,WM_HWND_RICHEDIT_APPEND_TEST_MSG,WPARAM(wstrLog.c_str()),NULL);
}