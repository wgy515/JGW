#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_QutsStartTcpServer : public CTSE_TestBase
    {
    public:
        CTSE_QutsStartTcpServer(void);
        ~CTSE_QutsStartTcpServer(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! Set 0 to indicate diag type
        int mnQutsType;
        //! 
        int mnQutsTcpPort;
    };
}
