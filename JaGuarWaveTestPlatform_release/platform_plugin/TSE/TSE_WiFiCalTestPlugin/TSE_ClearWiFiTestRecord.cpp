#include "StdAfx.h"
#include "TSE_ClearWiFiTestRecord.h"
#include "CJGW_WiFiTestResultRecord.h"

namespace JGW
{
    CTSE_ClearWiFiTestRecord::CTSE_ClearWiFiTestRecord(void) : mstrWiFiTestResultRecordEnvironment(L"TSE_WIFI_TEST_RESULT_RECORD")
    {
    }


    CTSE_ClearWiFiTestRecord::~CTSE_ClearWiFiTestRecord(void)
    {
    }

    const wchar_t* CTSE_ClearWiFiTestRecord::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ClearWiFiTestRecord::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"WiFiTestResultRecordEnvironment",strParamName))
        {
            mstrWiFiTestResultRecordEnvironment =  strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }


    bool CTSE_ClearWiFiTestRecord::TES_Run()
    {
        CCJGW_WiFiTestResultRecord* pWiFiTestResultRecord = (CCJGW_WiFiTestResultRecord*)GetGlobalEnvironment()->GetInt(mstrWiFiTestResultRecordEnvironment);
        if (pWiFiTestResultRecord)
        {
            pWiFiTestResultRecord->ClearTRXTestData();
        }
        return true;
    }
}
