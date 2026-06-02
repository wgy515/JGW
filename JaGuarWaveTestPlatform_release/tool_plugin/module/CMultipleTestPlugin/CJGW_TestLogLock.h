#pragma once
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
    class CCJGW_TestLogLock
    {
    public:
        CCJGW_TestLogLock(void);
        ~CCJGW_TestLogLock(void);
    public:
        bool IsHaveCurrntTestLog();
        std::wstring GetCurrntTestLog();
        void AppendText(const std::wstring& strLog);
        void ClearText();
        std::wstring GetAllTestLog();
    private:
        std::wstring mstrTestLog;
        std::wstring mstrCurrentTestLog;
        CCJGW_CriticalSectionLock mcCriticalSectionLock;
    };
}