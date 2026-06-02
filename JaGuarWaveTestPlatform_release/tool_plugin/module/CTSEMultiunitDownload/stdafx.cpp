// stdafx.cpp : 只包括标准包含文件的源文件
// CTSEMultiunitDownload.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <string>
#include "CTSEMultiunitDonwloadDefine.h"
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
namespace JGW
{
    std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];
}
