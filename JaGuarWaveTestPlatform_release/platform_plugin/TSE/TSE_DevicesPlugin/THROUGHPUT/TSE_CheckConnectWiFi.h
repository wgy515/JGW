#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_CheckConnectWiFi : public CTSE_TestBase
    {
    public:
        CTSE_CheckConnectWiFi(void);
        ~CTSE_CheckConnectWiFi(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        bool mbConnectFailResetPeraso; //! 连接失败后重启peraso设备
        std::wstring mstrInterfaceDescription;
        std::wstring mstrSSIDEnvironment;
        std::wstring mstrPASSWORDEnvironment;
        std::wstring mstrHostIPEnvironment; //! 连接WLAN的局域网IP地址
        std::wstring mstrSocketPortEnvironment; //! 
        std::wstring mstrWlanGatewayEnvironment;
        std::wstring mstrWlanIPAddressEnvironment;
    };
}