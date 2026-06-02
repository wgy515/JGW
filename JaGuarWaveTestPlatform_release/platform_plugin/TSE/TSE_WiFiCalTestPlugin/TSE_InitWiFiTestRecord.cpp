#include "StdAfx.h"
#include "TSE_InitWiFiTestRecord.h"

namespace JGW
{
    CTSE_InitWiFiTestRecord::CTSE_InitWiFiTestRecord(void) : mstrSaveWiFiTestResultRecordEnvironment(L"TSE_WIFI_TEST_RESULT_RECORD")
    {
    }


    CTSE_InitWiFiTestRecord::~CTSE_InitWiFiTestRecord(void)
    {
    }

    const wchar_t* CTSE_InitWiFiTestRecord::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_InitWiFiTestRecord::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SaveWiFiTestResultRecordEnvironment",strParamName))
        {
            mstrSaveWiFiTestResultRecordEnvironment =  strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_InitWiFiTestRecord::TSE_Run()
    {
        GetGlobalEnvironment()->PutInt(mstrSaveWiFiTestResultRecordEnvironment,(int)&mcWiFiTestResultRecord);
        mcWiFiTestResultRecord.ClearTRXTestData();
        return true;
    }
}
