// JGW_SSHDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
using namespace JGW;

std::string destIPAddr = "192.168.1.1";
std::string srcIPAddr = "";
std::string user = "root";
std::string password = "20171015";

void help()
{
    std::cout << "-d            dest ip Addr\n"
        << "-u              ssh user\n"
        << "-p            ssh password\n"
        << "-S         src ip Addr \n"
        << std::endl;
    //system("pause");
}
CCJGW_SSHShell* gpShell = NULL;



BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
    char szBuf[3] = {0x03,'\n','\0'};
    std::string strRead;
    switch( fdwCtrlType ) 
    { 
        // Handle the CTRL-C signal. 
    case CTRL_C_EVENT: 
        printf( "Ctrl-C event\n\n" );
        //         Beep( 750, 300 ); 

        if (gpShell->IsConnected()) gpShell->SendSSHShellCharacter(szBuf,2);
        //shell.RecvSSHShell(strRead);
        return( TRUE );

        //         // CTRL-CLOSE: confirm that the user wants to exit. 
        //     case CTRL_CLOSE_EVENT: 
        //         Beep( 600, 200 ); 
        //         printf( "Ctrl-Close event\n\n" );
        //         return( TRUE ); 
        // 
        //         // Pass other signals to the next handler. 
        //     case CTRL_BREAK_EVENT: 
        //         Beep( 900, 200 ); 
        //         printf( "Ctrl-Break event\n\n" );
        //         return FALSE; 
        // 
        //     case CTRL_LOGOFF_EVENT: 
        //         Beep( 1000, 200 ); 
        //         printf( "Ctrl-Logoff event\n\n" );
        //         return FALSE; 
        // 
        //     case CTRL_SHUTDOWN_EVENT: 
        //         Beep( 750, 500 ); 
        //         printf( "Ctrl-Shutdown event\n\n" );
        //         return FALSE; 

    default: 
        return FALSE; 
    } 
} 

// void deinit()
// {
//     shell.DisConnectSSHShell();
// }

int _tmain(int argc, _TCHAR* argv[])
{
/*    atexit(deinit);*/

    //     if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, true))
    //     {
    //         while (1) { Sleep(1000); }
    //     }
    //     else
    //     {
    //         printf("exit \r\n");
    //     }

    CCJGW_SSHShell shell;
    gpShell = &shell;
    std::string strRead,strWrite;

    for (int i = 1;i < argc;i ++)
    {
        if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-d",argv[i]))
        {
            if (i + 1 >= argc) {help(); return -1;}
            i ++;
            destIPAddr = JGW_W2A_W(argv[i]);
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-u",argv[i]))
        {
            if (i + 1 >= argc) {help(); return -1;}
            i ++;
            user = JGW_W2A_W(argv[i]);
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-p",argv[i]))
        {
            if (i + 1 >= argc) {help(); return -1;}
            i ++;
            password = JGW_W2A_W(argv[i]);
        }
        else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-S",argv[i]))
        {
            if (i + 1 >= argc) {help(); return -1;}
            i ++;
            srcIPAddr = JGW_W2A_W(argv[i]);
        }
    }

    if (!shell.ConnectSSHShell(destIPAddr.c_str(),user.c_str(),password.c_str(),srcIPAddr.empty()?NULL:srcIPAddr.c_str()))
    {
        std::cout << "Connect SSH Shell Fail" << std::endl;
        help();
        return -1;
    }

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, true);

    if (!shell.RecvSSHShell(strRead))
    {
        std::cout << "Recv SSH Shell Fail" << std::endl;
        //system("pause");
        return -1;
    }
    std::cout << strRead;
    while (1)
    {
        strRead = "";
        getline(std::cin,strWrite);
        if (strWrite.empty()) {strWrite = "\n";}
        if (std::string::npos != strWrite.find("QUIT") || std::string::npos != strWrite.find("quit") || std::string::npos != strWrite.find("exit") || std::string::npos != strWrite.find("EXIT")) break;

        if (!shell.SendSSHShell(strWrite,120))
        {
            std::cout << "write timeout" << std::endl;
            continue;
        }

        if (!shell.RecvSSHShellCallBack(strRead,[&](const std::string& strRead,int len)-> bool{
            std::cout << strRead;
            return true;
        }))
        {
            std::cout << "read write timeout" << std::endl;
            continue;
        }

        //         if (!shell.RecvSSHShell(strRead,60 * 60 * 24 *24))
        //         {
        //             std::cout << "read write timeout" << std::endl;
        //             continue;
        //         }
        JGW_EraseFristToRemoveChar(strRead,L'\n');
        //JGW_ReplaceStringA(strRead,strWrite + "\n","");
        //std::cout << strRead;

        std::cin.clear();
    }
    shell.DisConnectSSHShell();

    return 0;
}

