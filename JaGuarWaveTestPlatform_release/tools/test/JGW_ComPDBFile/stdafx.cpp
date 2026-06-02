// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_ComPDBFile.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

void JGW_Write_LogW_Format(int log_type,const char* tag,const wchar_t* log_buf,...)
{
    std::wstring strLog(L"");
    FORMAT_WSTRING(strLog,log_buf);
    std::wcout << strLog << std::endl;
}

void JGW_Write_LogA_Format(int log_type,const char* tag,const char* log_buf,...)
{
    std::string strLog("");
    FORMAT_STRING(strLog,log_buf);
    std::cout << strLog << std::endl;
}