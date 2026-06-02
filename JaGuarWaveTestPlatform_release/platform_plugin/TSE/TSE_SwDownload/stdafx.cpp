// stdafx.cpp : 只包括标准包含文件的源文件
// TSE_SwDownload.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
using namespace JGW;
void swdlQPHONEMSCB_Func(HANDLE hQMSLContext,char   *sMsg,unsigned short iMsgSize)
{
    if (iMsgSize >= 0 && NULL != sMsg)
    {
        LogI_F(L"QPHONEMSCB:%s",JGW_A2W(sMsg).c_str());
    }
}