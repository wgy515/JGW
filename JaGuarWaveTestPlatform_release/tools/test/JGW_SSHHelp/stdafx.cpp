// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_SSHHelp.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "CJGW_SecureShellInit.h"
#include "JGW_SSHHelp.h"
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
JGW::CCJGW_SecureShellInit gSSHInit;


JGW::CJGW_SSHHelpBase* GetSSHHlepBase()
{
    return new JGW::CJGW_SSHHelp();
}

void ReleaseSSHHlepBase(JGW::CJGW_SSHHelpBase* pSSHHelpBase)
{
    if (NULL != pSSHHelpBase)
    {
        delete pSSHHelpBase;
    }
}