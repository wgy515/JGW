#include "StdAfx.h"
#include "CJGW_TestLogLock.h"

namespace JGW
{
    CCJGW_TestLogLock::CCJGW_TestLogLock(void)
    {
        mstrTestLog.reserve(8 * 1024);
    }


    CCJGW_TestLogLock::~CCJGW_TestLogLock(void)
    {
    }

    bool CCJGW_TestLogLock::IsHaveCurrntTestLog()
    {
        return !mstrCurrentTestLog.empty();
    }

    std::wstring CCJGW_TestLogLock::GetCurrntTestLog()
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mcCriticalSectionLock);
        std::wstring strTemp = mstrCurrentTestLog;
        mstrCurrentTestLog = L"";
        return strTemp;
    }

    void CCJGW_TestLogLock::AppendText(const std::wstring& strLog)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mcCriticalSectionLock);
        mstrTestLog += strLog;
        mstrCurrentTestLog += strLog;
    }

    void CCJGW_TestLogLock::ClearText()
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mcCriticalSectionLock);
        mstrTestLog = L"";
        mstrCurrentTestLog = L"";
    }

    std::wstring CCJGW_TestLogLock::GetAllTestLog()
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mcCriticalSectionLock);
        return mstrTestLog;
    }
}
