#include "StdAfx.h"
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    CTSE_FTM_TestBase::CTSE_FTM_TestBase() : mTimeOutSec(10)
    {

    }

    CTSE_FTM_TestBase::~CTSE_FTM_TestBase(void)
    {
    }

    const wchar_t* CTSE_FTM_TestBase::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckKey\",\
                \"TestName\": \"FTM测试基类(LYNX FTM)\",\
                \"TimeOutSec\": \"发送命令超时时间,以S为单位\"\
                }";
    }

    bool CTSE_FTM_TestBase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mTimeOutSec = _wtoi(strParamValue);
            return true;
        }
        return false;
    }
}