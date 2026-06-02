#include "StdAfx.h"
#include "TSE_CloseInstrument.h"
#include "CJGW_TSEIntrument.h"

namespace JGW
{
    CTSE_CloseInstrument::CTSE_CloseInstrument(void) : mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT")
    {
    }


    CTSE_CloseInstrument::~CTSE_CloseInstrument(void)
    {
    }

    const wchar_t* CTSE_CloseInstrument::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseInstrument\",\
                \"TestName\": \"¹Ø±ÕÒÇÆ÷Á¬½Ó\",\
                \"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\"\
                }";
    }

    bool CTSE_CloseInstrument::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CloseInstrument::TSE_Run()
    {
        CCJGW_TSEIntrument* pTSEIntrument = (CCJGW_TSEIntrument*)(GetGlobalEnvironment()->GetInt(mstrInstrumentEnvironment));
        if (NULL != pTSEIntrument)
        {
            pTSEIntrument->CloseIEquipCommTL();
        }
        return true;
    }
}