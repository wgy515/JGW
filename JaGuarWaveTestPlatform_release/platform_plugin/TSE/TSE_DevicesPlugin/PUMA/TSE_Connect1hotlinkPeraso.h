#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_Connect1hotlinkPeraso : public CTSE_TestBase
    {
    public:
        CTSE_Connect1hotlinkPeraso(void);
        ~CTSE_Connect1hotlinkPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 查找金机端口号
        std::wstring FindGoldenDongles(std::vector<std::wstring>& vstrComPort);
    private:
        size_t mnTimeOut; //! 连接端口超时
        std::wstring mstrUsbServiceName;
        std::wstring mstrUsbVid;
        std::wstring mstrUsbPid;
        std::wstring mstrPortEnvironment;
        std::wstring mstrGoldenSerialNumberEnvironment;
    };
}

