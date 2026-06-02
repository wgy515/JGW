#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_CheckUsbInterfaceStatus : public CTSE_TestBase
    {
    public:
        CTSE_CheckUsbInterfaceStatus(void);
        ~CTSE_CheckUsbInterfaceStatus(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnTestTimeOut;
        unsigned short mBcdUsb;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrUsbServiceName;
    };
}