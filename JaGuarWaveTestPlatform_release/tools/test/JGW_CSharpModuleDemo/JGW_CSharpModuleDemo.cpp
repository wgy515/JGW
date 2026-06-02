// JGW_CSharpModuleDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <JGW_CSharpModule/JGW_CSharpModuleExport.h>

int _tmain(int argc, _TCHAR* argv[])
{
    StartCSharpModuleThread();
    while (1)
    {
        Sleep(1000);
    }
    StopCSharpModuleThread();
	return 0;
}

