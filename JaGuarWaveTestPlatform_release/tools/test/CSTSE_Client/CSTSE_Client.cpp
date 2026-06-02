// CSTSE_Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
using namespace JGW;
int _tmain(int argc, _TCHAR* argv[])
{
    CCJGW_AsyncSocket asyncSocket;
    char buf[1024] = {0};

    if (asyncSocket.ConnectSocketTimeOut("127.0.0.1",3008))
    {
        asyncSocket.AsyncWriteSocket("111111111111");
        asyncSocket.AsyncReadSocket(buf,1024);
        asyncSocket.CleanupSocket();
    }

    return 0;

}

