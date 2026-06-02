#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
namespace JGW
{
    class CTSE_CheckSocketPortOpen : public CTSE_TestBase
    {
    public:
        CTSE_CheckSocketPortOpen(void);
        ~CTSE_CheckSocketPortOpen(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run(void);
    private:
        int mnTimeOutSec;
        size_t mnSocketPort;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrHostAddrEnvironment;
        CCJGW_SyncSocket mcSyncSocket;
    };

}

