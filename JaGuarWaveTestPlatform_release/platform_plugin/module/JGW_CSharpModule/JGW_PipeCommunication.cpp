#include "StdAfx.h"
#include "JGW_PipeCommunication.h"

namespace JGW
{
    CJGW_PipeCommunication::CJGW_PipeCommunication(void) : mhPipe(INVALID_HANDLE_VALUE),mbConnected(false)
    {
    }


    CJGW_PipeCommunication::~CJGW_PipeCommunication(void)
    {
        DisConnect();
    }

    bool CJGW_PipeCommunication::Create(const std::string& strHost,size_t port)
    {
        //mhPipe = CreateNamedPipeA(strHost.c_str(), PIPE_ACCESS_DUPLEX,PIPE_TYPE_BYTE|PIPE_READMODE_BYTE , 1, 0, 0, 1000, NULL);
        mhPipe = CreateNamedPipeA(strHost.c_str(), PIPE_ACCESS_DUPLEX,PIPE_TYPE_BYTE|PIPE_READMODE_BYTE , PIPE_UNLIMITED_INSTANCES, 0, 0, NMPWAIT_WAIT_FOREVER, NULL);
        return mhPipe != INVALID_HANDLE_VALUE;
    }

    bool CJGW_PipeCommunication::Connect()
    {
        if(TRUE == ConnectNamedPipe(mhPipe,NULL))
        {
            mbConnected = true;
        }
        return mbConnected;
    }

    bool CJGW_PipeCommunication::IsConnect()
    {
        return mbConnected;
    }

    int CJGW_PipeCommunication::Read(char* buf,size_t len)
    {
        DWORD bytesRead = 0;
        if (FALSE == PeekNamedPipe(mhPipe, NULL, 0, NULL, &bytesRead, NULL ))
        {
            return -1;
        }
        if (bytesRead > 0 && bytesRead <= (DWORD)len)
        {
            if ( ReadFile(mhPipe, buf, bytesRead, &bytesRead, NULL ) )//从命名管道中读取数据
            {
                return bytesRead;
            }
        }
        return bytesRead;
    }

    bool CJGW_PipeCommunication::Write(char* buf,size_t len)
    {
        DWORD dwNumberOfBytesWrite = 0;
        if ( !WriteFile(
            mhPipe,				 // handle of the write end of our pipe
            buf,               // address of buffer that send data
            len,                   // number of bytes to write
            &dwNumberOfBytesWrite,  // address of number of bytes read
            NULL                   // non-overlapped.
            ) && !dwNumberOfBytesWrite)
        {
            return false;
        }
        return true;
    }

    void CJGW_PipeCommunication::DisConnect()
    {
        mbConnected = false;
        if (mhPipe != INVALID_HANDLE_VALUE && NULL != mhPipe)
        {
            DisconnectNamedPipe(mhPipe);
            CloseHandle(mhPipe);//关闭管道
            mhPipe = INVALID_HANDLE_VALUE;
        }
    }
}

#if 0

// ClientPip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>
using namespace std;
#define BUFSIZE 5


int main()
{

    printf("命名管道:客户端上线\n");
    printf("按任意键以开始连接命名管道\n");
    _getch();
    printf("开始等待命名管道\n");

    if (WaitNamedPipe(L"\\\\.\\Pipe\\mypipe", NMPWAIT_WAIT_FOREVER) == FALSE)
        return 0;

    printf("打开命名管道\n");
    HANDLE hPipe = CreateFile(L"\\\\.\\Pipe\\mypipe", GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if ((long)hPipe == -1)
        return 0;


    //接收服务端发回的数据
    BOOL fSuccess = false;
    DWORD len = 0;
    char buffer[BUFSIZE];
    string recvData = "";
    do
    {
        fSuccess = ReadFile(hPipe, buffer, BUFSIZE * sizeof(char), &len, NULL);
        char buffer2[BUFSIZE + 1] = { 0 };
        memcpy(buffer2, buffer, len);
        recvData.append(buffer2);
        if (!fSuccess || len < BUFSIZE)
            break;
    } while (true);

    cout << "recv data:" << endl << recvData.c_str() << endl << endl;

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    system("pause");
    return 0;
}
#endif