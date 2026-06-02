#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialModemPort.h>
namespace JGW
{
    CCJGW_CSerialModemPort* GetSerialModemPort();
    class CTSE_OpenModemPort : public CTSE_TestBase
    {
    public:
        CTSE_OpenModemPort(void);
        ~CTSE_OpenModemPort(void);
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
        CCJGW_CSerialModemPort mcSerialModemPort;
    };
}
