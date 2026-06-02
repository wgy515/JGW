// stdafx.cpp : 只包括标准包含文件的源文件
// JaGuarWaveTestPlatformHelp.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include "CJGW_UpdateClientThread.h"
#include "../../include/JaGuarWaveTestPlatformHelp/CCJGW_HelpThreadImpl.h"

namespace JGW
{
    CCMessageThread* GetHelpModuleThread()
    {
        static CCJGW_UpdateClientThread gUpdateClientThread;
        return &gUpdateClientThread;
    }

    void SetMainWindowsHwnd(HWND hMainHwnd)
    {
        CCJGW_UpdateClientThread* pUpdateClientThread = (CCJGW_UpdateClientThread*)GetHelpModuleThread();
        pUpdateClientThread->SetMainWindowsHwnd(hMainHwnd);
    }

    void StopHelpModuleThread()
    {
        CCJGW_UpdateClientThread* pUpdateClientThread = (CCJGW_UpdateClientThread*)GetHelpModuleThread();
        pUpdateClientThread->StopClientThread();
/*        delete pUpdateClientThread;*/
    }
}