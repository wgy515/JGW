// JGW_Quts.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#define JGW_Application_Instance_GUID "{62FF3450-6297-4448-A4AB-F384407E01ED}"
static unsigned int port = 2500;

BOOL ctrlhandler(DWORD fdwctrltype)
{
    switch (fdwctrltype)
    {
        // handle the ctrl-c signal.
    case CTRL_C_EVENT:
        printf( "ctrl-c event\n\n" );
        QLIB_QUTS_Stop_TCP_Server(port);
        return( false );
        // ctrl-close: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        printf( "ctrl-close event\n\n" );
        QLIB_QUTS_Stop_TCP_Server(port);
        return( false );

        // pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        printf( "ctrl-break event\n\n" );
        return false;

    case CTRL_LOGOFF_EVENT:
        printf( "ctrl-logoff event\n\n" );
        return false;

    case CTRL_SHUTDOWN_EVENT:
        QLIB_QUTS_Stop_TCP_Server(port);
        printf( "ctrl-shutdown event\n\n" );
        return false;

    default:
        return false;
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    std::wstring strTemp;
    strTemp = JGW::JGW_GetApplicationName2HModule(NULL);
    if (!JGW::jgw_check_single_application_instance(_T(JGW_Application_Instance_GUID),strTemp.c_str()))
    {
        return -1;
    }

    printf("start QUTS Service....\r\n");
    if (argc >= 2)
    {
        port = _ttoi(argv[1]);
    }

    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlhandler, true );
    unsigned char status = QLIB_QUTS_Start_TCP_Server(0,2500);
    while (status != 1)
    {
        Sleep(5000);
        status = QLIB_QUTS_Start_TCP_Server(0,2500);
        printf("start QUTS Service....\rn");
    }

    printf("start QUTS successful....\r\n");
    while (1)
    {
        Sleep(5000);
    }
    return 0;
}

