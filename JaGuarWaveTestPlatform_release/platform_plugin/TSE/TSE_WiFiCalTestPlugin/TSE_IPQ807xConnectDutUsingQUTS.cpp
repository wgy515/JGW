#include "StdAfx.h"
#include "TSE_IPQ807xConnectDutUsingQUTS.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xConnectDutUsingQUTS::CTSE_IPQ807xConnectDutUsingQUTS(void) : meType(QLIB_TARGET_TYPE_APQ),meWlanDut(IPQ807x),mstrComPort("192.168.1.1"),mhResourceContext(NULL)
    {
    }


    CTSE_IPQ807xConnectDutUsingQUTS::~CTSE_IPQ807xConnectDutUsingQUTS(void)
    {
    }

    const wchar_t* CTSE_IPQ807xConnectDutUsingQUTS::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_IPQ807xConnectDutUsingQUTS\",\
                \"TestName\": \"connect dut using quts\",\
                \"Type\": \"Target Type 0 :QLIB_TARGET_TYPE_MSM_MDM 1:QLIB_TARGET_TYPE_APQ\",\
                \"ComPort\": \"Com Port number, AUTO for auto detect COM port or IP Address from QUTS GUI\"\
                }";
    }

    bool CTSE_IPQ807xConnectDutUsingQUTS::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("Type"),strParamName))
        {
            meType = (QLIB_TargetType_Enum)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("ComPort"),strParamName))
        {
            mstrComPort = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }


    bool CTSE_IPQ807xConnectDutUsingQUTS::TSE_Run()
    {
        CCJGW_IPQ807xPhone::GetIPQ807xPhone()->InitQMSLLibrary(meType);
        for (int index = 0;index < 30 * 5;index ++)
        {
            if (CCJGW_IPQ807xPhone::GetIPQ807xPhone()->FindConnectAvailablePhonesHandleID(mstrComPort))
            {
                LogI_F(L"ConnectQUTS, connect successfully:   %s",JGW_A2W(mstrComPort).c_str());
                return true;
            }
            Sleep(200);
        }
        LogE_F(L"Could not connect to:  %s",JGW_A2W(mstrComPort).c_str());
        return false;
    }
}

