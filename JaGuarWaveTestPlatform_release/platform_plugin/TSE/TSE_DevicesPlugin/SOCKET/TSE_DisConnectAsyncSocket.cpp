#include "StdAfx.h"
#include "TSE_DisConnectAsyncSocket.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
namespace JGW
{
    CTSE_DisConnectAsyncSocket::CTSE_DisConnectAsyncSocket(void) : mstrConnectAsyncSocketInstanceEnvironment(L"TSE_ASYNC_SOCKET")
    {
    }


    CTSE_DisConnectAsyncSocket::~CTSE_DisConnectAsyncSocket(void)
    {
    }

    const wchar_t* CTSE_DisConnectAsyncSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisConnectAsyncSocket\",\
                \"TestName\": \"异步SOCKET断开\",\
                \"ConnectAsyncSocketInstanceEnvironment\": \"保存连接SOCKET实例变量名称\"\
                }";
    }

    bool CTSE_DisConnectAsyncSocket::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ConnectAsyncSocketInstanceEnvironment"))
        {
            mstrConnectAsyncSocketInstanceEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DisConnectAsyncSocket::TSE_Run()
    {
        if (mstrConnectAsyncSocketInstanceEnvironment.empty())
        {
            CCJGW_AsyncSocket* pAsyncSocket = (CCJGW_AsyncSocket*)(GetGlobalEnvironment()->GetInt(mstrConnectAsyncSocketInstanceEnvironment));
            if (NULL != pAsyncSocket) pAsyncSocket->CleanupSocket();
        }
        return true;
    }
}

