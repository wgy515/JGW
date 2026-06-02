#include "stdafx.h"
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    CTSE_TestBase::CTSE_TestBase(): mnErrorRetryCount(3),mnIntervalTimeMS(0) 
    {

    }

    CTSE_TestBase::~CTSE_TestBase()
    {

    }

    const wchar_t* CTSE_TestBase::TSE_GetParamDescription()
    {
        return L"{\"RetryCount\":\"失败后重测次数\",\"TestName\":\"测试名称\",\"IntervalTimeMS\":\"测试失败间隔测试时间,某些测试可能需要等待测试完成时间(需要测试项内部调用)\"}";
    }

    bool CTSE_TestBase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        GetLogServices()->DebugLogFormat(_T("%s    %s"),strParamName,strParamValue);
        if(TSE_PARAM_NAME_EQUAL(TSE_CONFIG_RETRYCOUNT,strParamName))
        {
            mnErrorRetryCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(TSE_CONFIG_TEST_NAME,strParamName))
        {
            mstrTestName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(TSE_CONFIG_INTERVALTIMEMS,strParamName))
        {
            mnIntervalTimeMS = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_TestBase::TSE_Init()
    {
        return true;
    }

    bool CTSE_TestBase::TSE_Run()
    {
        return true;
    }

    bool CTSE_TestBase::TSE_Exit()
    {
        return true;
    }

    size_t CTSE_TestBase::TSE_GetErrorRetryTestCount()
    {
        return mnErrorRetryCount > 0 ? mnErrorRetryCount : 1;
    }

    std::wstring& CTSE_TestBase::TSE_GetTestName()
    {
        return mstrTestName;
    }
}