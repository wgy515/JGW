#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>

namespace JGW
{
    class CTSE_ConnectAsyncSocket : public CTSE_TestBase
    {
    public:
        CTSE_ConnectAsyncSocket(void);
        ~CTSE_ConnectAsyncSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run(void);
    private:
        size_t mnSocketPort; 
        int mnConnectTimeOutSec;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrHostAddrEnvironment;
        std::wstring mstrConnectAsyncSocketInstanceEnvironment;
        CCJGW_AsyncSocket mcAsyncSocket;
    };
}


