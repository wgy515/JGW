#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <string>
#include <vector>
namespace JGW
{
    class CCJGW_SortRecordConfig
    {
    public:
        CCJGW_SortRecordConfig(void);
        ~CCJGW_SortRecordConfig(void);
    public:
        void LoadSortRecordConfig(const std::wstring& strSortRecordPath);
        int GetSortRecordIndexToSortIndex(int nSortIndex);
        void ClearSortRecordConfig();
    private:
        //! nSortRecordIndex 插件排序索引文件
        void SaveSortRecordIndex(int nSortRecordIndex,int nSortIndex);
    private:
        CCJGW_ConfigIni mConfigIni;
        std::vector<int> mvSortRecordIndex;
    };
}
