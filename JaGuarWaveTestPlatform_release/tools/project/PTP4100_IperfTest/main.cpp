// PTP4100_IperfTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PTP4100_IperfTest.h"
#include "SendPipeCommand.h"
#include "JGW_FoundationFunc/JGW_StringFunc.h"
#include "JGW_FoundationFunc/JGW_FilePath.h"
#include <iostream>
#include "CJGW_Iperf3Command.h"
using namespace JGW;

void Iperf3TestInfo(float fSpeedMbitssec)
{
    std::cout << "Mbits/sec " << fSpeedMbitssec << std::endl;
}


#if 0
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
#else
int _tmain(int argc, _TCHAR* argv[])
#endif
{
#if 0
    SendPipeCommand sendPipeCmd;
    std::string cmd;
    std::vector<std::string> args;
    //args.push_back("tool\\iperf3.exe");
    args.push_back("-s");
    args.push_back("-J");
    args.push_back("-i");
    args.push_back("-1");
    std::string initialDirectory(JGW_W2A_W(JGW_GetApplicationFolder()));
    cmd += initialDirectory;
    cmd += "tool\\iperf3.exe";
    sendPipeCmd.LaunchRedirectConsole(cmd,args,initialDirectory);
    
    std::wstring strTemp;
    while (sendPipeCmd.ConsoleisRunning())
    {
        sendPipeCmd.ReadConsoleProcessPipeEOF(strTemp);
        std::wcout << strTemp;
    }
#else
    CCJGW_Iperf3Command iperf3Command;
    iperf3Command.RegisterCallbackIperf3TestInfo(Iperf3TestInfo);
#if 0
    if (iperf3Command.StartIperf3Client(L"127.0.0.1",10,4))
#else
    if (iperf3Command.StartIper3Server())
#endif
    {
        
    }
#endif

    while (iperf3Command.CheckIper3IsRunning()) Sleep(200);
    Sleep(5 * 1000);
	return 0;
}
