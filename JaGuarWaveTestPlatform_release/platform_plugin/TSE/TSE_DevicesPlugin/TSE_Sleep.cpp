#include "StdAfx.h"
#include "TSE_Sleep.h"

namespace JGW
{
    CTSE_Sleep::CTSE_Sleep(void) : mSleepMs(0)
    {
    }


    CTSE_Sleep::~CTSE_Sleep(void)
    {
    }

    const wchar_t* CTSE_Sleep::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_Sleep\",\
                \"TestName\": \"–›√ﬂ\",\
                \"SleepMS\": \"–›√ﬂ ±º‰∫¡√Î\"\
                }";
    }

    bool CTSE_Sleep::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SleepMS"))
        {
            mSleepMs = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_Sleep::TSE_Run()
    {
        Sleep(mSleepMs);
        return true;
    }
}
