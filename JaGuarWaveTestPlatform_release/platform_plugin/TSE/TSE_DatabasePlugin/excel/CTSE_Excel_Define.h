#pragma once
#include <string>
#include <vector>

namespace JGW
{
    typedef std::vector<std::vector<std::wstring>> _excel_content;

    typedef struct  
    {
        size_t mnPrimaryKeyPos;
        std::wstring mstrExcelFilePath;
        _excel_content msvvExcelContent;
    }S_EXCEL_CONTENT,*PS_EXCEL_CONTENT;
}