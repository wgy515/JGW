#include "stdafx.h"
#include <JGW_CSharpModule/JGW_CSharpModuleExport.h>
#include "JGW_CSharpModuleThread.h"


JGW::CJGW_CSharpModuleThread* gpCSharpModuleThread  = NULL;


bool StartCSharpModuleThread()
{
    if (NULL == gpCSharpModuleThread)
    {
         gpCSharpModuleThread = new JGW::CJGW_CSharpModuleThread;
         gpCSharpModuleThread->StartThread();
    }
    return true;
}

void StopCSharpModuleThread()
{
    if (NULL != gpCSharpModuleThread)
    {
        gpCSharpModuleThread->ExitThread(true);
        delete gpCSharpModuleThread;
        gpCSharpModuleThread = NULL;
    }
}