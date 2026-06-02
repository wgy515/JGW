// AdbDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_AdbPocoClient.h"
#include <iostream>
#include <memory>

#define DEFAULT_ADB_PORT 5037
static int __adb_server_port = DEFAULT_ADB_PORT;

class ShardDemo
{
public:
    ShardDemo()
    {
        std::cout << "Create ShardDemo" << std::endl;
    }

    ~ShardDemo()
    {
        std::cout << "Destory ShardDemo" << std::endl;
    }
};

void DeleteShardDemo(ShardDemo* pdemo)
{
    delete pdemo;
}

int _tmain(int argc, _TCHAR* argv[])
{

    {
        std::shared_ptr<ShardDemo> pShardDemo(new ShardDemo,DeleteShardDemo);
    }





    return 0;







    JGW::CCJGW_AdbPocoClient mAdbClient;

    int version  = 30;
    mAdbClient.GetAdbHostVersion(version);
    std::cout << version << std::endl;

    std::string recv;
    mAdbClient.GetAdbDevices(recv);
    std::cout << recv << std::endl;

    mAdbClient.AdbDevicesCommandRequest("root:",recv);
    //mAdbClient.SendAdbProtocolCommand("root:");
    //mAdbClient.AdbRead(recv);
    std::cout << recv << std::endl;

    bool result = mAdbClient.StartAdbShell();
    result = mAdbClient.RecvAdbShellCommand(recv);
    std::cout << recv << std::endl;
    result = mAdbClient.SendAdbShellCommand("ls\n");
    result = mAdbClient.RecvAdbShellCommand(recv);
    std::cout << recv << std::endl;
    result = mAdbClient.RecvAdbShellCommand(recv);
    std::cout << recv << std::endl;
    mAdbClient.StopAdbShell();
#if 0
    JGW::CCJGW_SyncSocket syncSocket;
    //htonl(INADDR_LOOPBACK);
    syncSocket.ConnectSocket("127.0.0.1","5037");

    std::string hostVersion = "000chost:version";
    std::string hostVersionLen;
    JGW::JGW_FormatString(hostVersionLen,"%04x",hostVersion.length());

    //int len = syncSocket.SendSocket(hostVersionLen.c_str(),hostVersionLen.length());
    int len = syncSocket.SendSocket(hostVersion.c_str(),hostVersion.length());

    char recvBuf[256] = {0};
    len = syncSocket.RecvSocket(recvBuf,4);
    //memset(recvBuf,0x00,256);
    //len = syncSocket.RecvSocket(recvBuf,4);
    //len = strtoul(recvBuf, 0, 16);

    memset(recvBuf,0x00,256);
    len = syncSocket.RecvSocket(recvBuf,4);
    int n = strtoul(recvBuf, 0, 16);
    memset(recvBuf,0x00,256);
    len = syncSocket.RecvSocket(recvBuf,n);
    int version = 30;
    if (sscanf(recvBuf, "%04x", &version) != 1) 
    {
        return 1;
    }
    syncSocket.CleanupSocket();
#endif
    //if(readx(fd, buf, 4)) goto error;

    //buf[4] = 0;
    //n = strtoul(buf, 0, 16);
    //if(n > (int)sizeof(buf)) goto error;
    //if(readx(fd, buf, n)) goto error;
    //adb_close(fd);


    return 0;
}

