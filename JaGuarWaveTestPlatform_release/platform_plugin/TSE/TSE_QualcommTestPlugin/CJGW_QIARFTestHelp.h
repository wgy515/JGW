#pragma once
#include <memory>
#include <string>
#include "StmInterface.h"
#include "CJGW_QIARFTest.h"
namespace JGW
{
    class CCJGW_QIARFTestHelp
    {
    public:
        CCJGW_QIARFTestHelp(void);
        ~CCJGW_QIARFTestHelp(void);
    public:
        //! 初始化QIA RF
        bool InitializeQIARFTest(bool bIsRFCal,std::string& strAssemblyFilePath);
        //!  执行QIA 引擎测试线程
        bool ExecutionEngineThread(const std::string& strXttFilePath,int nTimeOutSec = 60 * 6);
        //! 停止线程测试
        void StopEngineThread();
        //! 卸载QIA RF
        void UninitializeQIARFTest();
    public:
        //! 设置全局变量
        virtual void SetGlobalVariable(const std::string& gvName,const std::string& value);
        //! 获取全局变量
        virtual std::string GetGlobalVariable(const std::string& gvName);
        //! 清空当前全局变量
        virtual void ClearGlobalVariable();
    public:
        //! 
        void SetEngineThreadIsRuning(bool bRuning = false);
        //! 执行QIA 引擎测试
        void ExecutionEngineTest();
    private:
        //! 创建STM APPLICATION接口
        //! RFCAL RFVERIFY
        void StmCreateApplication(bool bIsRFCal,std::string& strAssemblyFilePath);
        //!
        static void DutTestEventHandler(void * context, int type, const char * msg);
    private:
        //!
        bool mbEngineTestResult;
        //! QIA 引擎线程是否正在运行
        bool mbIsEngineThreadRuning;
        //! 执行引擎线程句柄
        unsigned int m_dThreadID;
        HANDLE mhExecutionEngineThreadHandle;
        //! ptr stm interface
        std::shared_ptr<void> mptrStmInterface;
        //! 
        std::string mstrXttFilePath;
        //! 
        stm_assembly_info msStmAssemblyInfo;
        //!
        CCJGW_QIARFTest mcQIARFTest;
    };

}

