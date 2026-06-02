#pragma once
#include <string>
#include "StmInterface.h"
//#import "C:\Program Files (x86)\Qualcomm\QDART\bin\QSPRSchedulerWrapper.tlb" no_namespace named_guids
namespace JGW
{
    typedef enum 
    {
        Unknown = 0,
        Passed = 1,
        Failed = 2,
        Executing = 4,
        Waiting = 5 // waiting for a Run/Result command
    }ExecutionStatus;


    typedef enum 
    {
        NotSet = 0,
        TestPassed = 1,
        TestFailed = 2,
        Exception = 3
    }QIARFTestStatus;

    typedef enum 
    {
        E_RFCAL_QSEQ_DEBUG_CAL = 1,
        E_RFCAL_QSEQ_LOG_CAL,
        E_RFCAL_QSEQ_ERROR_CAL,
        E_RFCAL_QSEQ_TPL3_CAL,
        E_RFCAL_QSEQ_HTML_CAL,
    }E_RFCAL_QSEQ_RECORD_LOG;

    typedef void(*sti_sys_evt_handler)(void * context, int type, const char * msg);

    class CJGW_QIARFInterface
    {
    public:
        //! 设置全局变量
        virtual void SetGlobalVariable(const std::string& gvName,const std::string& value) = 0;
        //! 获取全局变量
        virtual std::string GetGlobalVariable(const std::string& gvName) = 0;
        //! 清空当前全局变量
        virtual void ClearGlobalVariable() = 0;
    public:
        //! 执行QIA RF测试
        virtual bool ExecuteQIARFTest(void* gStmInterface,stm_assembly_info* pstmAssemblyInfo,const std::string& strXttFilePath,sti_sys_evt_handler pstmSysEvtHandler,void* pstmParam) = 0;
    };
}