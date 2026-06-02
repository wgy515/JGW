#pragma once
/*
测试记录接口
*/
namespace JGW
{
    class CCJGW_TestRecordImpl
    {
    public:
        virtual ~CCJGW_TestRecordImpl(){}
        //! 加载测试记录文档
        virtual bool LoadTestRecordConfig(const wchar_t* ModuleFolder,const wchar_t* strRecordConfigName) = 0;
        //!
        virtual bool GetIsSaveTestLog() = 0;
        //!
        virtual void SetIsSaveTestLog(bool isSave) = 0;
        //! 获取PASS数值
        virtual size_t GetTestRecordPassCount() = 0;
        //! 获取总数
        virtual size_t GetTestRecordCount() = 0;
        //! 更新测试成功记录
        virtual void UpdateTestRecordToTestPass() = 0;
        //! 更新失败记录
        virtual void UpdateTestRecordToTestFail() = 0;
        //! 清空测试记录
        virtual void ClearTestRecord() = 0;
    };
}