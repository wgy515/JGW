#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_QIARFTestHelp.h"
namespace JGW
{
    class CTSE_QIATest : public CTSE_TestBase
    {
    public:
        CTSE_QIATest(void);
        ~CTSE_QIATest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsCalTest;
        //! 0 : 不显示PASS和FAIL LOG
        //! 1 : TSET FAIL 显示FAIL LOG 但不显示PASS LOG
        //! 2 : TSET FAIL 显示所有LOG
        //! 3 : TSET PASS 显示所有LOG
        int mnShowTestLogType;
        std::wstring mstrSerialNumberEnvironment;
        std::wstring mstrQCServerDllPath;
        std::wstring mstrXttFilePathCommand;
        CCJGW_QIARFTestHelp mcQIARFTestHelp;
    };
}
