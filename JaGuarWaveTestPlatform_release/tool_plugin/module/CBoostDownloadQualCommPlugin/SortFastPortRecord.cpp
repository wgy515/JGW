#include "StdAfx.h"
#include "SortFastPortRecord.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

std::wstring CSortFastPortRecord::m_strSortRecordFile = _T("");

CSortFastPortRecord::CSortFastPortRecord(void)
{

}


CSortFastPortRecord::~CSortFastPortRecord(void)
{
}

void CSortFastPortRecord::WriteFastPortRecordFile(int nTestThread,int nBindPortIndex/*,LPCTSTR strSerialNumber*/)
{
	AddPortRecordVector(nBindPortIndex);
	TCHAR szKeyName[100] = {0};
	TCHAR szKeyPortIndex[30] = {0};

	_stprintf_s(szKeyName,100,_T("DEV_%d"),nTestThread);
	_stprintf_s(szKeyPortIndex,30,_T("%d"),nBindPortIndex);

	m_cInifConfig.SetIniKeyValue(szKeyName,_T("FastDBName"),szKeyPortIndex);
}

void CSortFastPortRecord::ReadFastPortRecordFile(int nTestThread,int &bBindPortIndex/*,CString &strSerialNumber*/)
{
	TCHAR szKeyName[100] = {0};
	_stprintf_s(szKeyName,100,_T("DEV_%d"),nTestThread);

	bBindPortIndex = m_cInifConfig.GetIniKeyIntValue(szKeyName,_T("FastDBName"));
}

int CSortFastPortRecord::GetFastPortShowIndex(int nBindPortIndex)
{
	int nSortIndex = 0;
	int nDevCounts = GetDevCounts();

	for (int i = 1;i <= nDevCounts;i++)
	{
		ReadFastPortRecordFile(i,nSortIndex);
		if( nBindPortIndex == nSortIndex )
		{
			return i;
		}
	}

	return nDevCounts+1;
}

int CSortFastPortRecord::GetDevCounts()
{
	std::wstring strTemp = m_cInifConfig.GetIniKeyValue( _T("MAIN"),_T("DevCounts") );

	return _ttoi(strTemp.c_str());
}

// void CSortFastPortRecord::GetPortRecordVector(std::vector<int> &vThread)
// {
// 	if( !m_pInifConfig ) return;
// 	std::wstring strTemp = m_pInifConfig->GetIniKeyValue( _T("MAIN"),_T("DevVector") );
// 
// 	if( !strTemp.empty() )
// 	{
// 		ParserIntW(strTemp.c_str(),_T(","),vThread);
// 	}
// }

void CSortFastPortRecord::ClearSortRecorder()
{
		m_cInifConfig.SetIniKeyValue(_T("MAIN"),_T("DevCounts"),_T("0"));
}

void CSortFastPortRecord::AddPortRecordVector(int nBindPortIndex)
{
	int nSortIndex = 0;
	std::wstring strTemp = m_cInifConfig.GetIniKeyValue( _T("MAIN"),_T("DevCounts") );
	int nDevCounts = _ttoi(strTemp.c_str());

	for (int i = 1;i <= nDevCounts;i++)
	{
		ReadFastPortRecordFile(i,nSortIndex);
		if( nBindPortIndex == nSortIndex )
		{
			return;
		}
	}

	JGW::FormatWString(strTemp,L"%d",nDevCounts+1);
	m_cInifConfig.SetIniKeyValue(_T("MAIN"),_T("DevCounts"),strTemp.c_str());
// 	std::vector<int> vThreadIndex;
// 
// 	GetPortRecordVector(vThreadIndex);
// 	for (size_t i = 0;i < vThreadIndex.size();i ++)
// 	{
// 		if (nThreadIndex == vThreadIndex.at(i))
// 		{
// 			return ;
// 		}
// 	}
// 	std::wstring strVector(_T(""));
// 	std::wstring strTemp(_T(""));
// 
// 	strVector = m_pInifConfig->GetIniKeyValue(_T("MAIN"),_T("DevVector"));
// 	
// 	if (0 == vThreadIndex.size())
// 	{
// 		FormatWString(strTemp,_T("%d"),nThreadIndex);
// 	}
// 	else
// 	{
// 		FormatWString(strTemp,_T(",%d"),nThreadIndex);
// 	}
// 	
// 	strVector += strTemp;
// 
// 	m_pInifConfig->SetIniKeyValue(_T("MAIN"),_T("DevVector"),strVector.c_str());
}

