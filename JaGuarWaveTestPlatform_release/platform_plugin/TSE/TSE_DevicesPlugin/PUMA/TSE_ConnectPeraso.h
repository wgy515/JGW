#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ConnectPeraso : public CTSE_TestBase
    {
    public:
        CTSE_ConnectPeraso(void);
        ~CTSE_ConnectPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnTimeOut; //! 连接端口超时
        std::wstring mstrUsbServiceName;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrPortEnvironment;
    };
}