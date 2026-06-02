#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ConnectQcomDiag:public CTSE_TestBase
    {
    public:
        CTSE_ConnectQcomDiag(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        unsigned char   m_bUserQpst;
        int             mnPort;
        size_t          mnTimeOut;
        std::wstring mstrPortEnvironment;
        //!
        std::wstring mstrSaveResourceContextEnvironment;
    };
};


