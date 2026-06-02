#pragma once
#include "JGW_TestLoggingPlugin_Define.h"
#include <string>

namespace JGW
{
    class JGW_TESTLOGGINGPLUGIN_CLASS CCJGW_MoveTestLogging
    {
    public:
        CCJGW_MoveTestLogging(void);
        ~CCJGW_MoveTestLogging(void);
    public:
        void MoveTestLogging(const wchar_t* szTestName,const wchar_t* strSN,const wchar_t* strFilePath,bool bTestStatus = true);       
    };
}