#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_QcommDownloadSerialPort.h"

namespace JGW
{
    class CTSE_OpenSaharaDownloadPort : public CTSE_TestBase
    {
    public:
        CTSE_OpenSaharaDownloadPort(void);
        ~CTSE_OpenSaharaDownloadPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    //private:
    //    bool OpenSaharaDownloadPort(const std::wstring& strComPort);
    private:
        int mnTimeOutSec;
        std::wstring mstrPortEnv;
        std::wstring mstrSaharaSerialClassEnv;
        CCJGW_QcommDownloadSerialPort mcSerialPort;
    };
}