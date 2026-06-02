#pragma once
#include "JGW_TestLoggingPlugin_Define.h"
#include <string>
namespace JGW
{
    class JGW_TESTLOGGINGPLUGIN_CLASS CCJGW_SaveTestLog
    {
    public:
        CCJGW_SaveTestLog(void);
        ~CCJGW_SaveTestLog(void);
    public:
        //! save folder base folde\\yy_mm_dd\\p_snxxx.log
        static bool SaveTestLog(const std::wstring& strBaseFolder,const std::wstring& strBarcode,bool bTestResult,const std::wstring& strTestLog);
        static bool SaveTestLogGetFilePath(const std::wstring& strBaseFolder,const std::wstring& strBarcode,bool bTestResult,const std::wstring& strTestLog,std::wstring& strTestLogPath);
        static std::wstring GetBarcodeToTestLog(const std::wstring& strTestLog,const std::wstring& strStartBarcode,const std::wstring& strEndBarcode,const std::wstring& strDefaultBarcode = L"");
    };
}


