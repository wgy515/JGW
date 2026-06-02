#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WriteInstrumentCommand : public CTSE_TestBase
    {
    public:
        CTSE_WriteInstrumentCommand(void);
        ~CTSE_WriteInstrumentCommand(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        std::string mstrFindOkString;
        std::wstring mstrInstrumentCommand;
        std::wstring mstrInstrumentEnvironment;
    };
}