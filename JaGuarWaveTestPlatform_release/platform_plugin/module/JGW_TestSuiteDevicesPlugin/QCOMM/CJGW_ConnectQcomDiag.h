#pragma once
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteImpl.h>

namespace JGW
{
    class CCJGW_ConnectQcomDiag:public CCJGW_TestSuiteImpl
    {
    public:
        CCJGW_ConnectQcomDiag(void);
    private:
        bool CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool Run();
    private:
        unsigned char   m_bUserQpst;
        int             mnPort;
        size_t          mnTimeOut;
    };
};


