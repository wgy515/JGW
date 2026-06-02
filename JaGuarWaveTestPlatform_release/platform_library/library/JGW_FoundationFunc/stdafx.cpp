// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_FoundationFunc.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
 std::wstring g_strErrorMsg;

 const wchar_t* JGW_GetCommonFuncErrorMsg()
 {
     return g_strErrorMsg.c_str();
}