#pragma once
#include "CTSE_Excel_Define.h"


namespace JGW
{
    class CCJGW_LoadExcelDatabBase
    {
    public:
        CCJGW_LoadExcelDatabBase(void);
        ~CCJGW_LoadExcelDatabBase(void);

        bool LoadExcelDatabBase(const std::wstring& strExcelFilePath,S_EXCEL_CONTENT& sExcelContent);
    private:
        S_EXCEL_CONTENT* mpsExcelContent;
    };
}