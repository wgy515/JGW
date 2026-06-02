#pragma once
#include "sti_export_func_decl.h"
#include <string>
#include <vector>
#include "CJGW_RFCalVFSConfig.h"

#define XTT_TIMEOUT				"100"
#define XTT_TIMEOUT_OFFSET		"5"

namespace JGW
{
	enum E_RFCAL_QSEQ_RECORD_LOG
	{
		E_RFCAL_QSEQ_DEBUG_CAL = 1,
		E_RFCAL_QSEQ_LOG_CAL,
		E_RFCAL_QSEQ_ERROR_CAL,
		E_RFCAL_QSEQ_TPL3_CAL,
		E_RFCAL_QSEQ_HTML_CAL,
	};

	class CCJGW_RFQualcommInterface
	{
	public:
		CCJGW_RFQualcommInterface(std::wstring& wstrDllSrc);
		~CCJGW_RFQualcommInterface(void);
		//! 初始化校准接口
		bool			InitQCServerModule();
		//! 
		void			InitQCServerInterface(const std::string  filename,unsigned char bUseQPST,std::string& strEquipPad,std::string& strEquipName);
		//xtt文件名
		bool			LoadRefXtt(const std::string  filename/*, const stm_assembly_info & ai*/);
		bool			LoadRefXml(const std::string  filename/*, const stm_assembly_info & ai*/);
		//! 获取测试项目数
		size_t			GetXttTestCount();
		//! 注册测试函数log
		static void		DutTestEventHandler(void * context, int type, const char * msg);
		//! 运行XTT
		bool			RunXtt();
		bool			InitTestListCtrl();
		bool			ExecuteSuiteIndex(int nIndex);
		void			DisConnect(){try{if(m_iface && m_sti_func_decl.sti_close_handle)m_sti_func_decl.sti_close_handle(m_iface);}catch(...){}}
		void			SetErrorReTestCount(int nErrorReTest){m_nErrorRetest = nErrorReTest;}
		void			SetErrorStop(bool errorstop){m_bErrorStop = errorstop;}
	private:
		bool						 ExecuteTestAll();
		bool						 ExecuteDestoryTest();
		HMODULE						 LoadCalDll();
		void						 FreeCalDll();
	private:
		std::string					 m_strSn;
		bool						 m_bErrorStop;
		int							 m_nErrorRetest;
		CCJGW_RFCalVFSConfig		 m_rfPareseConfig;
		sti_export_func_decl		 m_sti_func_decl;
		HMODULE						 m_dll_ptr;
		std::wstring&				 m_wstrDllSrc;
		size_t						 m_nXttTestCount;
		size_t						 m_nCurrentIndex;
		void*						 m_iface;
		bool						 m_bFristLoadInterfaceModule;
		std::vector<size_t>			 m_vDestory;
	};
}
