#pragma once
#include <JGW_TestRecord/CJGW_TestRecordImpl.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_TestRecordConfig:public CCJGW_TestRecordImpl
    {
    public:
        CCJGW_TestRecordConfig(void);
        ~CCJGW_TestRecordConfig(void);
    private:
        //! 加载测试记录文档
        virtual bool LoadTestRecordConfig(const wchar_t* ModuleFolder,const wchar_t* strRecordConfigName);
        //!
        virtual bool GetIsSaveTestLog();
        //!
        virtual void SetIsSaveTestLog(bool isSave);
        //! 获取PASS数值
        virtual size_t GetTestRecordPassCount();
        //! 获取总数
        virtual size_t GetTestRecordCount();
        //! 更新测试成功记录
        virtual void UpdateTestRecordToTestPass();
        //! 更新失败记录
        virtual void UpdateTestRecordToTestFail();
        //! 清空测试记录
        virtual void ClearTestRecord();

    private:
        bool mbSaveTestLog;
        size_t mnTestPassCount;
        size_t mnTestCounts;
        CCJGW_ConfigIni mConfigIni;
    };

}

