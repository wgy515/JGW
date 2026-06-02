#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
namespace JGW
{
    class CTSE_ConnectDMGPeraso : public CTSE_TestBase
    {
    public:
        CTSE_ConnectDMGPeraso(void);
        ~CTSE_ConnectDMGPeraso(void);
    private:
        //! 
        const wchar_t* TSE_GetParamDescription();
        //! 
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        bool TSE_Run();
    private:    
        size_t mnTimeOut; //! 连接端口超时
        std::wstring mstrUsbServiceName;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrGoldenSNEnv;
        std::wstring mstrGuSerialComPortEnv;
        std::wstring mstrDutSerialComPortEnv;
        std::wstring mstrDutSerialNumberEnv;
        CCJGW_CSerialComPort mGuSerialComPort;
        CCJGW_CSerialComPort mDutSerialComPort;
    };
}


