#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_RemoteConnectPeraso : public CTSE_TestBase
    {
    public:
        CTSE_RemoteConnectPeraso(void);
        ~CTSE_RemoteConnectPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 
        int mnTimeOut;
        //! 主机IP地址 默认127.0.0.1
        std::wstring mstrHostIPEnvironment;
        //! 
        std::wstring mstrSocketPortEnvironment;
    };
}