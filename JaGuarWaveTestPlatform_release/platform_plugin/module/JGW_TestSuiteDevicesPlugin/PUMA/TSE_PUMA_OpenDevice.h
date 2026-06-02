#pragma once
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>

namespace JGW
{
    class CTSE_PUMA_OpenDevice:public CCJGW_TestSuiteImpl
    {
    public:
        CTSE_PUMA_OpenDevice(void);

    private:
        bool CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool Run(void);
    private:
        int mnPort;
        size_t mnTimeOut;
    };
}

