#include "StdAfx.h"
#include "TSE_ConnectDutUsingUserTransportDLL.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "JGW_Phone.h"

namespace JGW
{
    CTSE_ConnectDutUsingUserTransportDLL::CTSE_ConnectDutUsingUserTransportDLL(void) : meType(QLIB_TARGET_TYPE_APQ),mstrWlanID("4019"),mstrComPort("192.168.1.1:2391"),mstrUserTransPortDll(JGW_RealativePathToAbsPathA("QMSL_WLAN_Transport.dll")),mbReloadUserDefineDll(true)
    {
    }


    CTSE_ConnectDutUsingUserTransportDLL::~CTSE_ConnectDutUsingUserTransportDLL(void)
    {
    }

    const wchar_t* CTSE_ConnectDutUsingUserTransportDLL::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectDutUsingUserTransportDLL\",\
                \"TestName\": \"Connect Dut Using UserTransportDLL\",\
                \"Type\": \"Target Type,def 1 :APQ QLIB_TARGET_TYPE_MSM_MDM=0;QLIB_TARGET_TYPE_APQ=1;QLIB_TARGET_TYPE_MAX_INVALID=2\",\
                \"WlanID\": \"WlanID def:4019\",\
				\"ReloadUserDefineDll\": \"reload user define dll def:1\",\
                \"ComPort\": \"Com Port number, AUTO for auto detect, 127.0.0.1 for user TransportDLL,def:192.168.1.1:2391\",\
                \"WlanID\": \"User Defined Transport DLL name with folder path,def:QMSL_WLAN_Transport.dll\"\
                }";
    }

    bool CTSE_ConnectDutUsingUserTransportDLL::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Type",strParamName))
        {
            meType = (QLIB_TargetType_Enum)(_ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanID",strParamName))
        {
            mstrWlanID = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ComPort",strParamName))
        {
            mstrComPort = JGW_W2A(strParamValue);
        }
		else if (TSE_PARAM_NAME_EQUAL(L"ReloadUserDefineDll",strParamName))
		{
			mbReloadUserDefineDll = (1 == _ttoi(strParamValue));
		}
        else if (TSE_PARAM_NAME_EQUAL(L"UserTransPortDll",strParamName))
        {
            mstrUserTransPortDll = JGW_RealativePathToAbsPathA(JGW_W2A(strParamValue));
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectDutUsingUserTransportDLL::TSE_Run()
    {
        //PHONE->SetLibraryMode(meType);
        //! QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);
        QLIB_SetTargetType((unsigned char)QLIB_TARGET_TYPE_APQ);
        return PHONE->LoadUserDefinedTransportLibraryAndConnectDUT(mstrComPort,mstrUserTransPortDll,mbReloadUserDefineDll);
    }
}
