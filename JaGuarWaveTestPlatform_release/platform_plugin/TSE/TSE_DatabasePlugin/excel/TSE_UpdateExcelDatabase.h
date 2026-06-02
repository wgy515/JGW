#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CTSE_Excel_Define.h"
namespace JGW
{
    class CTSE_UpdateExcelDatabase : public CTSE_TestBase
    {
    public:
        CTSE_UpdateExcelDatabase(void);
        ~CTSE_UpdateExcelDatabase(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int GetHeaderPosToExcel();
    private:
        PS_EXCEL_CONTENT mpsExcelContent;
        int mnExcelHeaderPos;
        std::wstring mstrEnvironmentValue;
        std::wstring mstrExcelHeader;
        std::wstring mstrEnvironmentExcelSource;
    };
}