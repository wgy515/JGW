// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_GlobalResourcesPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "CJGW_GlobalResources.h"
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
namespace JGW
{
    CCJGW_GlobalResourcesImpl* GetGlobalResourcesImpl()
    {
        static CCJGW_GlobalResources gr;
        return &gr;
    }
}

