#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CTSE_Excel_Define.h"
#include "CJGW_LoadExcelDatabBase.h"

namespace JGW
{
    class CTSE_LoadExcelDatabase : public CTSE_TestBase
    {
    public:
        CTSE_LoadExcelDatabase(void);
        ~CTSE_LoadExcelDatabase(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrEnvironmentExcelFilePath;
        std::wstring mstrEnvironmentExcelContent;
        S_EXCEL_CONTENT msExcelContent;
        CCJGW_LoadExcelDatabBase mcLoadExcelDataBase;
    };
}