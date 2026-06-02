#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_ExcelLibXL.h"
#include <comdef.h>
void dellibxlBook(libxl::Book* book)
{
	if( NULL != book )
	{
		book->release();
		book = NULL;
	}
}

namespace JGW
{
	CCJGW_ExcelLibXL::CCJGW_ExcelLibXL(void):mpSheet(NULL),mbOpenExcel(false)
	{
	}


	CCJGW_ExcelLibXL::~CCJGW_ExcelLibXL(void)
	{
		CloseExcel();
	}

	bool CCJGW_ExcelLibXL::IsOpenExcel( int nsheet /* = 0 */ )
	{
		if( !mbOpenExcel ) return false;
		return mpSheet == m_ptrBook->getSheet(nsheet);
	}

	bool CCJGW_ExcelLibXL::IsOpenExcel( const std::wstring& sheetname )
	{
		if( !mbOpenExcel ) return false;
		std::wstring strTemp;

		for ( int i = 0; i < m_ptrBook->sheetCount() ; i ++)
		{
			strTemp = m_ptrBook->getSheet(i)->name();
			if ( NULL == wstrcomparenocasewstr(sheetname.c_str(),strTemp.c_str()) && mpSheet == m_ptrBook->getSheet(i) ) return true;
		}

		return false;
	}

	bool CCJGW_ExcelLibXL::OpenExcel(const std::wstring& strExcelPath,const std::wstring& sheetname)
	{
		if( IsOpenExcel(sheetname) ) return true;

		if ( !OpenExcel(strExcelPath) ) return false;

		std::wstring strTemp;
		for ( int i = 0; i < m_ptrBook->sheetCount() ; i ++)
		{
			mpSheet = m_ptrBook->getSheet(i);
			if ( !mpSheet || !mpSheet->name() ) continue;
			strTemp = mpSheet->name();
			if ( NULL == wstrcomparenocasewstr(sheetname.c_str(),strTemp.c_str()) ) return true;
		}

		return false;
	}

	bool CCJGW_ExcelLibXL::OpenExcel(const std::wstring& strExcelPath,int nsheet /* = 0 */)
	{
		if( IsOpenExcel(nsheet) ) return true;

		wchar_t ext[20] = { 0 };
		mpSheet = NULL;
		mstrExcelPath = strExcelPath;
		JGW_GetPathExtensionNameW(strExcelPath.c_str(), ext, 20);

		if ( NULL == wstrcomparenocasewstr(ext, L".xls") )
		{
			m_ptrBook.reset(xlCreateBook(),dellibxlBook); //= std::make_shared<Book>(xlCreateBook(),dellibxlBook);
		}
		else if( NULL == wstrcomparenocasewstr(ext, L".xlsx") )
		{
			m_ptrBook.reset(xlCreateXMLBook(),dellibxlBook);
		}

		if ( !m_ptrBook ) return false;
		m_ptrBook->setKey(L"GCCG", L"windows-282123090cc0e6036db16b60a1t3u0h9");
		if( !m_ptrBook->load(strExcelPath.c_str()) ) return false;
		mpSheet = m_ptrBook->getSheet(nsheet);
		mbOpenExcel = mpSheet?true:false;

		return mbOpenExcel;
	}

	bool CCJGW_ExcelLibXL::CheckExcelIsOpen()
	{
		return ( m_ptrBook && mpSheet );
	}

	bool CCJGW_ExcelLibXL::CreateExcelSheet(const std::wstring& strExcelPath,const wchar_t* strSheetName )
	{
		wchar_t ext[20] = { 0 };
		mpSheet = NULL;
		mstrExcelPath = strExcelPath;
		JGW_GetPathExtensionNameW(strExcelPath.c_str(), ext, 20);

		if ( NULL == wstrcomparenocasewstr(ext, L".xls") )
		{
			m_ptrBook.reset(xlCreateBook(),dellibxlBook); //= std::make_shared<Book>(xlCreateBook(),dellibxlBook);
		}
		else if( NULL == wstrcomparenocasewstr(ext, L".xlsx") )
		{
			m_ptrBook.reset(xlCreateXMLBook(),dellibxlBook);
		}
		if( m_ptrBook )
		{
			m_ptrBook->setKey(L"GCCG", L"windows-282123090cc0e6036db16b60a1t3u0h9");
			mpSheet = m_ptrBook->addSheet(strSheetName);
		}
		return mpSheet?true:false;
	}

	size_t CCJGW_ExcelLibXL::GetSheetMaxCol()
	{
		if( !m_ptrBook || !mpSheet ) return 0;
		return mpSheet->lastCol();
	}

	size_t CCJGW_ExcelLibXL::GetSheetMaxRow()
	{
		if( !m_ptrBook || !mpSheet ) return 0;
		return mpSheet->lastRow();
	}

	bool CCJGW_ExcelLibXL::SetExcelSheet(int nsheet)
	{
		if( !m_ptrBook ) return false;
		mpSheet = m_ptrBook->getSheet(nsheet);
		return mpSheet?true:false;
	}

    bool CCJGW_ExcelLibXL::SetExcelSheet(const std::wstring& sheetname)
    {
        if(!m_ptrBook) return false;
        std::wstring strTemp;
        for ( int i = 0; i < m_ptrBook->sheetCount() ; i ++)
        {
            mpSheet = m_ptrBook->getSheet(i);
            if ( !mpSheet || !mpSheet->name() ) continue;
            strTemp = mpSheet->name();
            if ( NULL == wstrcomparenocasewstr(sheetname.c_str(),strTemp.c_str()) ) return true;
        }
        return false;
    }

	bool CCJGW_ExcelLibXL::WriteExcelColNumFormat(int colfrist,int collast,int width,libxl::NumFormat numformat)
	{
		if( !m_ptrBook || !mpSheet ) return false;
		libxl::IFormatT<TCHAR>* pformat = mpSheet->cellFormat(0,0);
		pformat->setNumFormat(numformat);
		return mpSheet->setCol(colfrist,collast,width,pformat);
	}


	bool CCJGW_ExcelLibXL::WriteExcelHeader(int col,libxl::CellType ecelltype,void* cellvalue)
	{
		if( !m_ptrBook || !mpSheet || !cellvalue ) return false;
		return WriteExcelContent(0,col,ecelltype,cellvalue);
	}

	bool CCJGW_ExcelLibXL::WriteExcelContent(int row,int col,libxl::CellType ecelltype,void* cellvalue)
	{
		if( !m_ptrBook || !mpSheet || !cellvalue ) return false;

		switch(ecelltype)
		{
		case libxl::CELLTYPE_STRING:
			return mpSheet->writeStr(row,col,(LPCTSTR)cellvalue/*,pformat*/);
		case libxl::CELLTYPE_NUMBER:
			return mpSheet->writeNum(row,col,*((double*)cellvalue));
		case libxl::CELLTYPE_BOOLEAN:
			return mpSheet->writeBool(row,col,(*((int*)cellvalue))==1?true:false);
		}
		return false;
	}

	void CCJGW_ExcelLibXL::CloseExcel()
	{
		m_ptrBook.reset();
		mbOpenExcel = false;
	}

	bool CCJGW_ExcelLibXL::SaveExcel()
	{
		if( !m_ptrBook ) return false;
		return m_ptrBook->save(mstrExcelPath.c_str());
	}

    bool CCJGW_ExcelLibXL::SaveExcel(const std::wstring& strExcelFilePath)
    {
        if( !m_ptrBook ) return false;
        return m_ptrBook->save(strExcelFilePath.c_str());
    }

	/*	int CTF_ExcelLibXL::GetRowCount()
	{
	return mpSheet->lastRow();
	}

	int CTF_ExcelLibXL::GetColCount()
	{
	return mpSheet->lastCol();
	}*/

	std::wstring CCJGW_ExcelLibXL::ReadExcelContent(int row,int col)
	{
		if( !m_ptrBook || !mpSheet ) return L"";
		libxl::CellType cellType = mpSheet->cellType(row, col);
		std::wstring strTemp;
		double num = 0;

		switch (cellType)
		{//! 空
		case libxl::CELLTYPE_EMPTY:
			break;//! 字符串
		case libxl::CELLTYPE_STRING:
			strTemp = mpSheet->readStr(row, col);
			break;//! 数值
		case libxl::CELLTYPE_NUMBER:
			if( mpSheet->isDate(row, col) )
			{
				SYSTEMTIME systime;
				num = mpSheet->readNum(row, col);
				VariantTimeToSystemTime(num, &systime);
				FormatWString(strTemp,L"%04d/%02d/%02d %02d:%02d:%02d",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wMilliseconds);
			}
			else
			{
				num = mpSheet->readNum(row, col);
				FormatWString(strTemp,L"%.2lf",num);
				JGW_ReplaceStringW(strTemp,_T(".00"),_T(""));
				//strTemp.replace()
			}
			break;//! bool变量
		case libxl::CELLTYPE_BOOLEAN:
			strTemp = mpSheet->readBool(row,col)?L"1":L"0";
			break;//! 合并
		case libxl::CELLTYPE_BLANK:
			int rowFirst,rowLast,colFirst,colLast;
			mpSheet->getMerge(row,col,&rowFirst,&rowLast,&colFirst,&colLast);
			if ( libxl::CELLTYPE_BLANK == mpSheet->cellType(rowFirst, colFirst) ) return _T("");
			strTemp = ReadExcelContent(rowFirst,colFirst);
			break;
		case libxl::CELLTYPE_ERROR:
			break;
		default:
			break;
		}
		return strTemp;
	}

	const char* CCJGW_ExcelLibXL::GetErrorMsg()
	{
		if( !m_ptrBook || !mpSheet ) return "not open excel";
		return m_ptrBook->errorMessage();
	}
}