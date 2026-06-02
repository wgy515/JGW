#pragma once

namespace JGW
{
    enum E_TEST_STATUS
    {
        E_INIT_TEST_STATUS = 0,
        E_RUN_TEST_STATUS,//! 运行
        E_RUNING_TEST_STATUS, //! 正在运行
        E_FAIL_TEST_STATUS,
        E_PASS_TEST_STATUS
    };
    const wchar_t gTestStatus[][25] = {L"E_INIT_TEST_STATUS",L"E_RUN_TEST_STATUS",L"E_RUNING_TEST_STATUS",L"E_FAIL_TEST_STATUS",L"E_PASS_TEST_STATUS"};
    class CTSE_TestStatusListener
    {
    public:
        //! 监听测试状态变化
        virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus) {}
        virtual void OnTestIndexChange(size_t index,int subIndex = -1){}
    };
}