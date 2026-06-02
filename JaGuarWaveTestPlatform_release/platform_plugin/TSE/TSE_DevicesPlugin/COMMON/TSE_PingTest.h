#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
namespace JGW
{
    class CTSE_PingTest : public CTSE_TestBase
    {
    public:
        CTSE_PingTest(void);
        ~CTSE_PingTest(void);
    private:
        //!
        const wchar_t* TSE_GetParamDescription();
        //!
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //!
        bool TSE_Run();
    private:
        size_t mnPingTimeOutMs;
        //!
        size_t mnPingTestTimeSec;
        //! 测试超时时间
        size_t mnTimeOutSec;
        //! Socke 路由IP地址
        std::wstring mstrSourceAddrEnvironment;
        //! ping IP地址
        std::wstring mstrHostEnvironment;
        //! 
        std::wstring mstrEthernetGUIDEnvironment;
        //!
        CCJGW_ICMPSocket mIcmpSocket;
    };
}