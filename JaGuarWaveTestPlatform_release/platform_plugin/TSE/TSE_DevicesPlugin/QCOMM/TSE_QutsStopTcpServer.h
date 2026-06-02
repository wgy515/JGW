#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_QutsStopTcpServer : public CTSE_TestBase
    {
    public:
        CTSE_QutsStopTcpServer(void);
        ~CTSE_QutsStopTcpServer(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int mnQutsTcpPort;
    };
}
