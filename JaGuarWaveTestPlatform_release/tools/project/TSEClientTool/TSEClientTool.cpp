// TSEClientTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "json/CJGW_FTM_Json.h"

#include "TSETcpClientThread.h"
#include "TSETestInfoForward.h"
#include "TSETestThread.h"

#include <Windows.h>
using namespace JGW;
/// flags => server socket ip => server socket port 

int _tmain(int argc, _TCHAR* argv[])
{
    size_t socketPort = 5766;
    std::string strServerSocketIP = "127.0.0.1";
    std::string flags = "DebugTestMode中";


    CCJGW_FTM_Json ftmJson;
    if (!ftmJson.FromJsonToString("{\"TestGuid\":\"{0AC2981A-4474-45FE-AAF7-694A9EB95D73}\",\"Params\":{},\"ActionEnumID\":5}"))
    {
        return 0;
    }
    /*for (int i = 1;i < argc;i ++)
    {
    if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-f",argv[i]))
    {
    if (i + 1 >= argc) return -1;
    i++;
    strDir = JGW::JGW_W2A_W(argv[i]);
    if (strDir.at(strDir.size() - 1) != '\\' && strDir.at(strDir.size() - 1) != '/')
    strDir += "\\";
    wctp.LoadPDBImageFile(strDir);
    }
    else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-sn",argv[i]))
    {
    if (i + 1 >= argc) return -1;
    i ++;
    strSN = JGW::JGW_W2A_W(argv[i]);
    }
    else if (NULL == JGW::JGW_WStrComparenoCaseWStr(L"-mac",argv[i]))
    {
    if (i + 1 >= argc) return -1;
    i ++;
    strMac = JGW::JGW_W2A_W(argv[i]);
    }
    else
    {
    help();
    return -1;
    }
    }*/
    bool stopTest = false;
    CTSETestInfoForward tseTestInfoForward(stopTest);
    boost::thread_group threadGroup;
    //! 创建socket线程 => 发送控制命令
    CTSETcpClientThread tseTcpClientThread(tseTestInfoForward,strServerSocketIP,socketPort,flags);
    threadGroup.create_thread(boost::bind(&CTSETcpClientThread::AsyncConnnectSocket,&tseTcpClientThread,10));
    //! 创建收发socket 线程 => 处理控制命令
    tseTestInfoForward.SetWriteInfoServerInterfaceInstance(&tseTcpClientThread);
    threadGroup.create_thread(boost::bind(&CTSETestInfoForward::OnForWardTSETestInfo,&tseTestInfoForward));
    //! 主要处理测试线程 => 主线程用于测试相关
    CTSETestThread tseTestThread(tseTestInfoForward,stopTest);
    std::vector<std::string> vstrControlCmds;

    while (true)
    {
        //! 如果接收到开始测试控制命令信息
        if (tseTestInfoForward.GetRecvControlCmdInfos(vstrControlCmds))
        {
            tseTestThread.HandleTestControlCmds(vstrControlCmds);
        }
        Sleep(500);
    }

    return 0;
}

