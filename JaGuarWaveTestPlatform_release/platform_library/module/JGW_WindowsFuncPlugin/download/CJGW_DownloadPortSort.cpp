#include "StdAfx.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPortSort.h>

namespace JGW
{
    CCJGW_DownloadPortSort::CCJGW_DownloadPortSort(void)
    {
    }


    CCJGW_DownloadPortSort::~CCJGW_DownloadPortSort(void)
    {
    }

    void CCJGW_DownloadPortSort::InitDownloadPortSortConfig(const std::wstring& strDownloadPortSortConfigFilePath)
    {
        mstrDownloadPortSortConfigFilePath = strDownloadPortSortConfigFilePath;
        m_cInifConfig.InitIniFilePath(mstrDownloadPortSortConfigFilePath.c_str());
    }

    void CCJGW_DownloadPortSort::WriteFastPortRecordFile(int nTestThread,int nBindPortIndex/*,LPCTSTR strSerialNumber*/)
    {
        AddPortRecordVector(nBindPortIndex);
        std::wstring strKeyName,strKeyPortIndex;

        JGW_FormatWString(strKeyName,_T("DEV_%d"),nTestThread);
        JGW_FormatWString(strKeyPortIndex,_T("%d"),nBindPortIndex);
        m_cInifConfig.SetIniKeyValue(strKeyName.c_str(),_T("FastDBName"),strKeyPortIndex.c_str());
    }

    void CCJGW_DownloadPortSort::ReadFastPortRecordFile(int nTestThread,int &bBindPortIndex/*,CString &strSerialNumber*/)
    {
        std::wstring strKeyName;
        JGW_FormatWString(strKeyName,_T("DEV_%d"),nTestThread);
        bBindPortIndex = m_cInifConfig.GetIniKeyIntValue(strKeyName.c_str(),_T("FastDBName"));
    }

    int CCJGW_DownloadPortSort::GetFastPortShowIndex(int nBindPortIndex)
    {
        int nSortIndex = 0;
        int nDevCounts = GetDevCounts();

        for (int i = 1;i <= nDevCounts;i++)
        {
            ReadFastPortRecordFile(i,nSortIndex);
            if(nBindPortIndex == nSortIndex) return i;
        }

        return nDevCounts+1;
    }

    int CCJGW_DownloadPortSort::GetDevCounts()
    {
        return m_cInifConfig.GetIniKeyIntValue(_T("MAIN"),_T("DevCounts"));
    }

    void CCJGW_DownloadPortSort::ClearSortRecorder()
    {
        m_cInifConfig.SetIniKeyValue(_T("MAIN"),_T("DevCounts"),_T("0"));
    }

    void CCJGW_DownloadPortSort::AddPortRecordVector(int nBindPortIndex)
    {
        int nSortIndex = 0;
        int nDevCounts = m_cInifConfig.GetIniKeyIntValue(_T("MAIN"),_T("DevCounts"));
        for (int i = 1;i <= nDevCounts;i++)
        {
            ReadFastPortRecordFile(i,nSortIndex);
            if(nBindPortIndex == nSortIndex) return;
        }
        m_cInifConfig.SetIniKeyValue(_T("MAIN"),_T("DevCounts"),nDevCounts + 1);
    }
}