#include "StdAfx.h"
#include "TSE_OpenInstrumentVISA.h"
#include "CJGW_TSEIntrument.h"
namespace JGW
{
    CTSE_OpenInstrumentVISA::CTSE_OpenInstrumentVISA(void) : mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT")
    {
    }


    CTSE_OpenInstrumentVISA::~CTSE_OpenInstrumentVISA(void)
    {
        //CCJGW_TSEIntrument::CloseIEquipCommTL();
    }

    const wchar_t* CTSE_OpenInstrumentVISA::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenInstrumentVISA\",\
                \"TestName\": \"打开仪器VISA连接\",\
                \"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\",\
                \"VisaAddressEnvionment\": \"仪器VISA地址环境变量\"\
                }";
    }

    bool CTSE_OpenInstrumentVISA::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("VisaAddressEnvionment"),strParamName))
        {
            mstrVisaAddressEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_OpenInstrumentVISA::TSE_Run()
    {
         GetGlobalEnvironment()->PutInt(mstrInstrumentEnvironment,int(&mcTSEIntrument));
        if (mstrVisaAddress.empty() || NULL != mstrVisaAddress.compare(GetGlobalEnvironment()->GetString(mstrVisaAddressEnvionment)) || 1 != mcTSEIntrument.GetIEquipCommTLType())
        {
            mstrVisaAddress = GetGlobalEnvironment()->GetString(mstrVisaAddressEnvionment);
            mcTSEIntrument.CloseIEquipCommTL();
        }
        return NULL != mcTSEIntrument.OpenIEquipCommTLToVisa(JGW_W2A(mstrVisaAddress));
    }
}