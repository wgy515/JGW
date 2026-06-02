#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_TSEIntrument.h"
namespace JGW
{
    class CTSE_MultiWriteInstrumentCommand : public CTSE_TestBase
    {
    public:
        CTSE_MultiWriteInstrumentCommand(void);
        ~CTSE_MultiWriteInstrumentCommand(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        void GetInstrumentEnvironmentCommandString(std::wstring& strCommand);
        bool WriteReadCommand(IEquipCommTL* pEquipInter,const std::string& strCommand,const std::string& strFindOutput,int timeOutSec);
    private:
        std::wstring mstrInstrumentConfigFilePathEnvironment;
        std::wstring mstrInstrumentConfigFilePath;
        std::wstring mstrCommandGroupName;
        std::wstring mstrInstrumentEnvironment;
        std::map<std::wstring,std::wstring> mmapParam;
    };
}
