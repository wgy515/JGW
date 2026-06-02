// stdafx.cpp : 只包括标准包含文件的源文件
// LanguageTranslationPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


typedef const wchar_t* (*PGetStringFunc)(E_STRING_TYPE eStringType);
PGetStringFunc gPGetStringFunc = GetStringEn;

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
void InitLanguage(E_LOCAL_INDEX local)
{
    if (E_CHS_LOCAL == local)
    {
        gPGetStringFunc = GetStringChs; 
    }
    else
    {
        gPGetStringFunc = GetStringEn; 
    }
}

const wchar_t* GetString(E_STRING_TYPE eStringType)
{
    return gPGetStringFunc(eStringType);
}