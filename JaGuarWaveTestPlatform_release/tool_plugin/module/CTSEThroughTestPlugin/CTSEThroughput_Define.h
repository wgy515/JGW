#pragma once

namespace JGW
{
    typedef struct
    {
        bool mbInitSequenceTestOk;
        HWND mhMainWnd;
        int mnTestTime; //! 每个周期测试时间
        int mnTestTimeIndex; //! 测试时间索引
        bool mbMultThread; //! 是否是多路TCP测试
        std::streamoff mnSeekgLog;
        std::wstring mstrSuiteXmlConfigPath;
    }S_TSE_THROUGHPUT_TEST_CONFIG_PARAM,*PS_TSE_THROUGHPUT_TEST_CONFIG_PARAM;
}
