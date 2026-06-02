#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WriteGuData : public CTSE_TestBase
    {
    public:
        CTSE_WriteGuData(void);
        ~CTSE_WriteGuData(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring FormatWriteDataCommand(const std::wstring& strWriteDataCommand);
    private:
        std::wstring mstrWriteDataCommand;
        std::wstring mstrGuDataFilePathEnvionment;
        std::map<std::wstring,std::wstring> mmapParamNames;
    };
}


