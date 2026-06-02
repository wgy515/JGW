#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CTSE_Excel_Define.h"
namespace JGW
{
    class CTSE_GetExcelDataToPrimaryKey : public CTSE_TestBase
    {
    public:
        CTSE_GetExcelDataToPrimaryKey(void);
        ~CTSE_GetExcelDataToPrimaryKey(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int GetPrimaryKeyToExcel();
        bool InitExcelHeaderToExcel();
        int GetPrimaryValuePosToExcel(const std::wstring& strPrimartValue);
    private:
        int mnPrimaryKey;
        std::wstring mstrPrimaryKey;
        std::wstring mstrPrimaryValueEnvironment;
        std::wstring mstrEnvironmentExcelSource;
        std::vector<std::wstring> mvstrExcelHeader;
        std::vector<std::wstring> mvstrEnvironmentValues;
        //! 表头字符串对应的列位置
        std::vector<int> mvExcelHeaderPos;
        PS_EXCEL_CONTENT mpsExcelContent;
    };
}