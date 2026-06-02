// Module:  LOG4CPLUS
// File:    loggingserver.cxx
// Created: 5/2003
// Author:  Tad E. Smith
//
//
// Copyright 2003-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/socketappender.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/thread/threads.h>
#include <log4cplus/spi/loggingevent.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "RabbitMQCHelp.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace loggingserver
{

    class ClientThread : public log4cplus::thread::AbstractThread
    {
    public:
        ClientThread(log4cplus::helpers::Socket clientsock)
            : clientsock(clientsock) 
        {
            std::cout << "Received a client connection!!!!" << std::endl;
        }

        ~ClientThread()
        {
            std::cout << "Client connection closed." << std::endl;
        }

        virtual void run();

    private:
        log4cplus::helpers::Socket clientsock;
    };

}
std::string gstrHostName;
JGW::CRabbitMQCHelp gcRabbitMQCHelp;
#include <TlHelp32.h>

PROCESSENTRY32 peSnap; //全局变量
int getParentPid(DWORD pid, DWORD& ParentPid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bRet = TRUE;

	memset(&peSnap, 0, sizeof(PROCESSENTRY32));
	peSnap.dwSize = sizeof(PROCESSENTRY32);

	if (hSnap == INVALID_HANDLE_VALUE)
	{
		printf("Create snapshot error:%d\n", GetLastError());
		return 1;
	}

	bRet = Process32First(hSnap, &peSnap);
	while (bRet)
	{
		//printf("进程号：%6d-->进程名：%s\n", peSnap.th32ProcessID, peSnap.szExeFile);
		bRet = bRet && Process32Next(hSnap, &peSnap);
		if (int(peSnap.th32ProcessID) == pid)
		{
			ParentPid = peSnap.th32ParentProcessID;
			return int(peSnap.th32ParentProcessID);
		}
	}
	return -1;
}

HWND ghMainPlatformHwnd = NULL;

void deinit()
{
	if (NULL != ghMainPlatformHwnd && IsWindow(ghMainPlatformHwnd))
	{
		SendMessage(ghMainPlatformHwnd,WM_PLUGIN_APP_TO_MAIN_HWND_MSG_ID,WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM,0);
		ghMainPlatformHwnd = NULL;
	}
}

BOOL ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal. 当用户按下了CTRL+C,或者由GenerateConsoleCtrlEvent API发出. 
	case CTRL_C_EVENT:
		printf( "ctrl-c event\n\n" );
		deinit();
		return( false );
		// ctrl-close: confirm that the user wants to exit. - 当试图关闭控制台程序，系统发送关闭消息。
	case CTRL_CLOSE_EVENT:
		printf( "ctrl-close event\n\n" );
		deinit();
		return( false );

		// pass other signals to the next handler. 用户按下CTRL+BREAK, 或者由GenerateConsoleCtrlEvent API发出.
	case CTRL_BREAK_EVENT:
		printf( "ctrl-break event\n\n" );
		deinit();
		return false;
		//! 用户退出时，但是不能决定是哪个用户. 
	case CTRL_LOGOFF_EVENT:
		printf( "ctrl-logoff event\n\n" );
		deinit();
		return false;
		//! 当系统被关闭时.  
	case CTRL_SHUTDOWN_EVENT:
		printf( "ctrl-shutdown event\n\n" );
		deinit();
		return false;

	default:
		return false;
	}
}

int main(int argc, char** argv)
{
	std::string strHostName;
    std::locale::global(std::locale(""));
    //     if(argc < 3) {
    //         std::cout << "Usage: port config_file" << std::endl;
    //         return 1;
    //     }
    int port = /*std::atoi(argv[1])*/9998;
    if (argc > 1) port = std::atoi(argv[1]);
	if (argc > 2) strHostName = argv[2];
	gstrHostName = strHostName;

#if 0
	log4cplus::helpers::Time tm_ = log4cplus::helpers::Time::gettimeofday();
	//! 05-10-25 14:12:49,160
	//std::wstring strTemp = tm_.getFormattedTime(L"%m-%d-%y %H:%M:%S,%q");
	std::wstring strTemp = tm_.getFormattedTime(L"%Y-%m-%d %H:%M:%S");
	log4cplus::getLogLevelManager().toString(20000);
#else
#if 0
	DWORD dwProcessID = 0;
	getParentPid(GetCurrentProcessId(),dwProcessID);
	std::cout << "Process Id : " << dwProcessID  << std::endl;
	getParentPid(dwProcessID,dwProcessID);
	std::cout << "Process Id : " << dwProcessID  << std::endl;
#endif
#endif
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlhandler, true );

	ghMainPlatformHwnd = FindWindow(L"DUIL_B_CCJGWMainDlg",L"JaGuarWave Factory Test Application");
	if (NULL != ghMainPlatformHwnd && IsWindow(ghMainPlatformHwnd))
	{
		SendMessage(ghMainPlatformHwnd,WM_PLUGIN_APP_TO_MAIN_HWND_MSG_ID,WM_UPDATE_START_REMOTE_LOG_MSG_WPARAM,1);
	}
    //!const log4cplus::tstring configFile = LOG4CPLUS_C_STR_TO_TSTRING(argv[2]);

    log4cplus::PropertyConfigurator config(JGW::JGW_GetExecConfigFilePath(L"log\\log4cplus.remote.properties").c_str());
    config.configure();

    log4cplus::helpers::ServerSocket serverSocket(port);
    if (!serverSocket.isOpen()) {
        std::cout << "Could not open server socket, maybe port "
            << port << " is already in use." << std::endl;
        return 2;
    }

	if (!strHostName.empty())
	{
		if (!gcRabbitMQCHelp.Connect(strHostName,"admin","jgw_loc1"))
		{
			std::cout << gcRabbitMQCHelp.GetErrorMessage();
			return -1;
		}
	}


    while(1) {
        loggingserver::ClientThread *thr = 
            new loggingserver::ClientThread(serverSocket.accept());
        thr->start();
    }
	gcRabbitMQCHelp.Disconnect();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// loggingserver::ClientThread implementation
////////////////////////////////////////////////////////////////////////////////


void loggingserver::ClientThread::run()
{
    while(1) {
        if(!clientsock.isOpen()) {
            return;
        }
        log4cplus::helpers::SocketBuffer msgSizeBuffer(sizeof(unsigned int));
        if(!clientsock.read(msgSizeBuffer)) {
            return;
        }

        unsigned int msgSize = msgSizeBuffer.readInt();

        log4cplus::helpers::SocketBuffer buffer(msgSize);
        if(!clientsock.read(buffer)) {
            return;
        }

        log4cplus::spi::InternalLoggingEvent event
            = log4cplus::helpers::readFromBuffer(buffer);
		//event.getMessage();
        log4cplus::Logger logger
            = log4cplus::Logger::getInstance(event.getLoggerName());
		if (!gstrHostName.empty())
		{
			if (gcRabbitMQCHelp.IsConnected())
			{
				//! 2025-04-30 10:46:52 INFO[main]  - CCJGWMainDlg::CloseSubPluginView
				//std::wstring strTemp = tm_.getFormattedTime(L"%Y-%m-%d %H:%M:%S");
				//log4cplus::getLogLevelManager().toString(20000);
				gcRabbitMQCHelp.Publish(JGW::JGW_W2A(JGW::JGW_GetFormatWString(L"%s %s[%s] - %s",event.getTimestamp().getFormattedTime(L"%Y-%m-%d %H:%M:%S").c_str(),log4cplus::getLogLevelManager().toString(event.getLogLevel()).c_str(),event.getLoggerName().c_str(),event.getMessage().c_str())),"location-exchange","log4cplus.remote");
			}
		}
        logger.callAppenders(event);   
    }
}