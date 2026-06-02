#include "StdAfx.h"
#include "CJGW_LoadExcelDatabBase.h"
#include "CJGW_ExcelLibXL.h"

namespace JGW
{
    CCJGW_LoadExcelDatabBase::CCJGW_LoadExcelDatabBase(void)
    {
    }

    CCJGW_LoadExcelDatabBase::~CCJGW_LoadExcelDatabBase(void)
    {
    }

    bool CCJGW_LoadExcelDatabBase::LoadExcelDatabBase(const std::wstring& strExcelFilePath,S_EXCEL_CONTENT& sExcelContent)
    {
        mpsExcelContent = &sExcelContent;
        CCJGW_ExcelLibXL excelLibXL;

        if (!excelLibXL.OpenExcel(strExcelFilePath))
        {
            LogE_F(L"open excel fail(%s)(%s)",strExcelFilePath.c_str(),JGW_A2W_A(excelLibXL.GetErrorMsg()));
            return false;
        }

        std::vector<std::wstring> vstrTemp;
        for (size_t row = 0;row < excelLibXL.GetSheetMaxRow();row ++)
        {
            vstrTemp.clear();
            for (size_t col = 0;col < excelLibXL.GetSheetMaxCol();col ++)
            {
                vstrTemp.push_back(excelLibXL.ReadExcelContent(row,col));
            }
            mpsExcelContent->msvvExcelContent.push_back(vstrTemp);
        }
        
        return true;
    }
}