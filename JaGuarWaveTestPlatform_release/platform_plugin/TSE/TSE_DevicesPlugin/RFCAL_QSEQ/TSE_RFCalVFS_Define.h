#pragma once
#include <string>
#include <vector>
#include <tchar.h>
#include <comdef.h>
namespace JGW
{
    struct SParameters_STRUCT
    {
        bstr_t paramName; 
        bstr_t paramVal;
        bstr_t unit;
        bstr_t upper;
        bstr_t lower;
        bstr_t type;
        bstr_t mode;
    };

    class S_QCOMM_FRCALVFS_CONFIG
    {
    public:
        S_QCOMM_FRCALVFS_CONFIG():m_bTest(false)
            ,TestName("")
            ,RealName("")
            ,m_lNumParams(0)
            ,m_bDestory(false)
        {

        }
        bool		m_bTest;
        bool		m_bDestory;
        bstr_t		TestName;
        bstr_t		RealName;
        long		m_lNumParams;
        std::vector<SParameters_STRUCT> m_vParameters;
    };

    enum E_RFCAL_QSEQ_RECORD_LOG
    {
        E_RFCAL_QSEQ_DEBUG_CAL = 1,
        E_RFCAL_QSEQ_LOG_CAL,
        E_RFCAL_QSEQ_ERROR_CAL,
        E_RFCAL_QSEQ_TPL3_CAL,
        E_RFCAL_QSEQ_HTML_CAL,
    };

    typedef struct
    {  
        bool mbIsQSEQCore;//! 是否是QSEQ综测测试
        bool m_bErrorStop;//! 测试失败后是否停止   
        bool mbRecordCPKData;//! 记录CPK数据 
        bool mbSaveTestLogging; //! 是否保存测试LOG
        bool gbQcommTestResult; //! 测试结果
        int	m_nErrorReTest; //! 第一次错误后重测次数
        void* m_iface;//! 实例
        HMODULE	m_dll_ptr;//! 校准或综测DLL句柄
        std::string	mstrSN;
        std::wstring mwstrSN; //! SN
        std::string	m_strXttPath;
        std::wstring mstrRFCalQSEQDllFilePath; //! DLL路径
        sti_export_func_decl m_sti_func_decl; //! DLL导出函数结构体
    }S_RFQUALCOMM_INTERFACE_INFO,*PS_RFQUALCOMM_INTERFACE_INFO;
}