// CSTSEClientTool.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <CSTSE_FoundationLib/CSTSE_ClientBase.h>
#include <CSTSE_FoundationLib/CSTSE_ActionDefine.h>

using namespace JGW;
int _tmain(int argc, _TCHAR* argv[])
{
    CCSTSE_ClientBase clientBase;
    std::string strRead;
    std::wcout.imbue(std::locale("chs"));
    std::cout.imbue(std::locale("chs"));
    std::wcin.imbue(std::locale("chs"));
    std::cin.imbue(std::locale("chs"));

    while (true)
    {
        ECSTSE_ACTION action = clientBase.RecvCSTSEMsg(strRead);
        switch (action)
        {
        case ECSTSE_ACTION_Unknown:
            break;
        case ECSTSE_ACTION_COMMAND_PARAM_INIT_TSE:
            clientBase.InitCSTSEClient(strRead);
            break;
        case ECSTSE_ACTION_Enverionment:
            clientBase.UpdateGetEnverionment(strRead);
            break;
        case ECSTSE_ACTION_GET_TSE_INFO:
            //! TBD:
            break;
        case ECSTSE_ACTION_TSE_TEST:
            clientBase.TSETest(strRead);
            break;
        default:
            break;
        }
        Sleep(100);
    }
    return 0;
}

