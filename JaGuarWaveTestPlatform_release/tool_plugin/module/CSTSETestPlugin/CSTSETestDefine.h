#pragma once
#include <string>
#include <map>

namespace JGW
{
    typedef struct
    {
        //! 
        std::wstring mstrCSTSEClientExecPath;
        //!
        std::wstring mstrSuiteXmlConfigPath;
        //! key - value
        std::map<std::wstring,std::wstring> mmapExecParam;
    }S_CSTSE_TEST_CONFIG,*PS_CSTSE_TEST_CONFIG;
}