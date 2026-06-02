#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ConnectWiFi : public CTSE_TestBase
    {
    public:
        CTSE_ConnectWiFi(void);
        ~CTSE_ConnectWiFi(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        bool CheckScanSSIDExists(const std::string& strSSID);
    private:
        //!
        int mnTimeOut;
        //! 
        std::wstring mstrSSIDEnvironment;
        //! 
        std::wstring mstrPasswordEnvironment;
        //! 主机IP地址 默认127.0.0.1
        std::wstring mstrHostIPEnvironment;
        //! 
        std::wstring mstrSocketPortEnvironment;
    };
}