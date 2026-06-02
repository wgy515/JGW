#pragma once
#include <string>
namespace JGW
{
    typedef struct
    {
        bool mbInitSequenceTestOk;
        HWND mhMainWnd;
        std::wstring mstrSuiteXmlConfigPath;
    }S_TSE_TEST_CONFIG_PARAM,*PS_TSE_TEST_CONFIG_PARAM;
}