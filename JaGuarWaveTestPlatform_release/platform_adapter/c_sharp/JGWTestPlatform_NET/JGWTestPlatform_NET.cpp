// JGWTestPlatform_NET.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "JGWTestPlatform_NET.h"
#include "JGWMainViewNet.h"

static JGW::JGWMainViewNet* gpMainView;

HWND LoadWin32Window(HWND hMainWnd)
{
    CPaintManagerUI::SetInstance(ghInstance);
    Log4WE_F(L"LoadSubWindow HWND: %08X",hMainWnd);
    //return 0;
    gpMainView = new JGW::JGWMainViewNet();
    return gpMainView->DoModal(hMainWnd);
}

void UnLoadWin32Window()
{
    if (NULL == gpMainView)
    {
        delete gpMainView;
        gpMainView = NULL;
    }    
}