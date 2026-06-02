#pragma once
#include "CJGW_FTM_JsonSocket.h"
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_FTM_JSONCommand.h"


namespace JGW
{
    class CTSE_FTM_TestBase : public CTSE_TestBase
    {
    public:
        CTSE_FTM_TestBase();
        virtual ~CTSE_FTM_TestBase(void);
    public:
        virtual const wchar_t* TSE_GetParamDescription();

        virtual bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
    protected:
        size_t mTimeOutSec;
    };
}