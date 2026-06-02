#pragma once
#include <libxl/include_cpp/libxl.h>
#include <string>
#include <memory>

namespace JGW
{
	class CCJGW_ExcelLibXL
	{
	public:
		CCJGW_ExcelLibXL(void);
		~CCJGW_ExcelLibXL(void);
		//! 打开Excel文档
		bool OpenExcel(const std::wstring& strExcelPath,int nsheet = 0);
		//! 打开Excel文档 打开相应的sheet名称
		bool OpenExcel(const std::wstring& strExcelPath,const std::wstring& sheetname);
		//!
		bool IsOpenExcel( int nsheet = 0 );

		bool IsOpenExcel( const std::wstring& sheetname );
		//!
		bool CheckExcelIsOpen();
		//! 选择操作的工作表
		bool SetExcelSheet(int nsheet);
        //!
        bool SetExcelSheet(const std::wstring& sheetname);
		//! 最大的行
		size_t GetSheetMaxRow();
		//! 当前SHEET列
		size_t GetSheetMaxCol();
		//! 创建EXCEL
		bool CreateExcelSheet(const std::wstring& strExcelPath,const wchar_t* strSheetName );
		//! 写EXCEL表头
		bool WriteExcelHeader(int col,libxl::CellType ecelltype,void* cellvalue);
		//! 写EXCEL内容
		bool WriteExcelContent(int row,int col,libxl::CellType ecelltype,void* cellvalue);
		//! 设置单元格格式
		bool WriteExcelColNumFormat(int colfrist,int collast,int width,libxl::NumFormat numformat);
		//! 读取Excel内容
		std::wstring ReadExcelContent(int row,int col);
		//! 关闭excel文档
		void CloseExcel();
		//! 保存Excel文档
		bool SaveExcel();
        //!
        bool SaveExcel(const std::wstring& strExcelFilePath);
		const char* GetErrorMsg();
	private:
		std::shared_ptr<libxl::Book>	m_ptrBook;
		libxl::Sheet*					mpSheet;
		std::wstring					mstrExcelPath;
		bool							mbOpenExcel;
	};
}
