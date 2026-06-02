#pragma once
#include "CJGW_QIARFInterface.h"
#include <map>

namespace JGW
{
    class CCJGW_QIARFTest : public CJGW_QIARFInterface
    {
    public:
        CCJGW_QIARFTest(void);
        ~CCJGW_QIARFTest(void);
    public:
        //! 设置全局变量
        virtual void SetGlobalVariable(const std::string& gvName,const std::string& value);
        //! 获取全局变量
        virtual std::string GetGlobalVariable(const std::string& gvName);
        //! 清空当前全局变量
        virtual void ClearGlobalVariable();
    public:
        //! 执行QIA RF测试
        virtual bool ExecuteQIARFTest(void* gStmInterface,stm_assembly_info* pstmAssemblyInfo,const std::string& strXttFilePath,sti_sys_evt_handler pstmSysEvtHandler,void* pstmParam);
        virtual void StopQIARFTest();
    private:
        bool mbOpenedXtt;
        bool mbStopQIARFTest;
        ITestTreePtr mptrTestTree;
        std::string mstrXttFilePath;
        std::map<std::string,std::string> mmapGlobalVariable;
    };
}
