#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ReadSerialComPort : public CTSE_TestBase
    {
    public:
        CTSE_ReadSerialComPort(void);
        ~CTSE_ReadSerialComPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnTimeOut; //! 读取超时时间 默认50S
    };
}


