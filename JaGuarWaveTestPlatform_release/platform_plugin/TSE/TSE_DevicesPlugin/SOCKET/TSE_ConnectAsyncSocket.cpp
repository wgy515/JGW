#include "StdAfx.h"
#include "TSE_ConnectAsyncSocket.h"

namespace JGW
{
    CTSE_ConnectAsyncSocket::CTSE_ConnectAsyncSocket(void) : mnSocketPort(80),mnConnectTimeOutSec(10),mstrConnectAsyncSocketInstanceEnvironment(L"TSE_ASYNC_SOCKET")
    {
    }


    CTSE_ConnectAsyncSocket::~CTSE_ConnectAsyncSocket(void)
    {
    }

    const wchar_t* CTSE_ConnectAsyncSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectAsyncSocket\",\
                \"TestName\": \"异步SOCKET 连接\",\
                \"SourceAddrEnvironment\": \"src IP地址\",\
                \"HostAddrEnvironment\": \"dest IP地址\",\
                \"ConnectAsyncSocketInstanceEnvironment\": \"保存连接SOCKET实例变量名称\",\
                \"SocketPort\": \"socket port\",\
                \"TimeOutSec\": \"连接超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_ConnectAsyncSocket::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SourceAddrEnvironment"))
        {
            mstrSourceAddrEnvironment = strParamValue;
        } 
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostAddrEnvironment"))
        {
            mstrHostAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ConnectAsyncSocketInstanceEnvironment"))
        {
            mstrConnectAsyncSocketInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnConnectTimeOutSec = _wtoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SocketPort"))
        {
            mnSocketPort = _wtoi(strParamValue);    
        }
        else
        {
            return false;
        }
        return true;
    }
    
    bool CTSE_ConnectAsyncSocket::TSE_Run()
    {
        std::string strSocketIP,strSourceSocketIP;

        strSocketIP = GetGlobalEnvironment()->GetAnsiString(mstrHostAddrEnvironment);
        if (!mstrSourceAddrEnvironment.empty()) strSourceSocketIP = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);

        LogI_F("Async Socket IP : %s,Source IP : %s\n",strSocketIP.c_str(),strSourceSocketIP.c_str());
        if (!mcAsyncSocket.ConnectSocketTimeOut(
            strSocketIP,
            mnSocketPort,
            mnConnectTimeOutSec * 1000,mnConnectTimeOutSec * 1000,strSourceSocketIP.empty() ? NULL : strSourceSocketIP.c_str()))
        {
            LogE_F("Connect Socket(%s-%s) Fail\n",strSocketIP.c_str(),strSourceSocketIP.c_str());
            return false;
        }

        if (!mstrConnectAsyncSocketInstanceEnvironment.empty())
        {
            GetGlobalEnvironment()->PutInt(mstrConnectAsyncSocketInstanceEnvironment,int(&mcAsyncSocket));
        }

        return true;
    }
}

