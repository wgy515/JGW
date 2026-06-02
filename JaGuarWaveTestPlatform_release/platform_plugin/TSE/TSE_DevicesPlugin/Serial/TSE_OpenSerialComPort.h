#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_OpenSerialComPort : public CTSE_TestBase
    {
    public:
        CTSE_OpenSerialComPort(void);
        ~CTSE_OpenSerialComPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool SetControlCommState();
    private:
        bool mbIsSetControlCommState;
        size_t mnTimeOut; //! 连接端口超时
        size_t mnBaudRate; //! 波特率
        size_t mnDatabits; //！ 数据位默认8
        size_t mnStopbits; //！ 停止位
        size_t mnParity;//! 校验位
        size_t mnFlowControl;//! 流控制
        std::string mstrEndString;
        std::wstring mstrUsbServiceName;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrPortEnvironment; //! 端口名称环境变量
    };
}

