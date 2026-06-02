#include "StdAfx.h"
#include "TSE_ParseContinousRxResult.h"

namespace JGW
{
    CTSE_ParseContinousRxResult::CTSE_ParseContinousRxResult(void)
    {
    }


    CTSE_ParseContinousRxResult::~CTSE_ParseContinousRxResult(void)
    {
    }
    
    const wchar_t* CTSE_ParseContinousRxResult::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ParseContinousRxResult::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        return false;
    }

    bool CTSE_ParseContinousRxResult::TSE_Run()
    {
        return false;
    }
}
