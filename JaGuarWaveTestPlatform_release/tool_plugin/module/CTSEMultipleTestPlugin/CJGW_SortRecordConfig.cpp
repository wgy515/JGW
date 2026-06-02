#include "StdAfx.h"
#include "CJGW_SortRecordConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_SortRecordConfig::CCJGW_SortRecordConfig(void)
    {
    }


    CCJGW_SortRecordConfig::~CCJGW_SortRecordConfig(void)
    {
    }

    void CCJGW_SortRecordConfig::LoadSortRecordConfig(const std::wstring& strSortRecordPath)
    {
        std::wstring strTemp;
        mvSortRecordIndex.clear();
        mConfigIni.InitIniFilePath(strSortRecordPath.c_str());
        int count = mConfigIni.GetIniKeyIntValue(L"MAIN",L"DevCounts");

        for (int i = 0;i < count;i ++)
        {
            JGW_FormatWString(strTemp,L"DEV_%d",i + 1);
            mvSortRecordIndex.push_back(mConfigIni.GetIniKeyIntValue(L"MAIN",strTemp.c_str()));
        }
    }

    void CCJGW_SortRecordConfig::SaveSortRecordIndex(int nSortRecordIndex,int nSortIndex)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"DEV_%d",nSortRecordIndex);
        mConfigIni.SetIniKeyValue(L"MAIN",strTemp.c_str(),nSortIndex);
    }

    int CCJGW_SortRecordConfig::GetSortRecordIndexToSortIndex(int nSortIndex)
    {
        //! int nEmptyIndex = -1;
        for (size_t i = 0;i < mvSortRecordIndex.size();i ++)
        {
            if (nSortIndex == mvSortRecordIndex[i])
            {
                return i + 1;
            }
        }
        mvSortRecordIndex.push_back(nSortIndex);
        mConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",mvSortRecordIndex.size());
        SaveSortRecordIndex(mvSortRecordIndex.size(),nSortIndex);
        return mvSortRecordIndex.size();
    }

    void CCJGW_SortRecordConfig::ClearSortRecordConfig()
    {
        mConfigIni.SetIniKeyValue(L"MAIN",L"DevCounts",0);
        mvSortRecordIndex.clear();
    }
}

