#include "StdAfx.h"
#include "TSE_CheckAdapterSpeed.h"
#include <JGW_WindowsFuncPlugin/CJGW_AdapterCommonInfo.h>
namespace JGW
{
    //! dwSpeed = 2500 000 000  
    CTSE_CheckAdapterSpeed::CTSE_CheckAdapterSpeed(void) : mdwbitsPerSecondSpeed(100 * 1000 * 1000),mdwSpeedUnitMbps(1000 * 1000)
    {
    }


    CTSE_CheckAdapterSpeed::~CTSE_CheckAdapterSpeed(void)
    {
    }

    const wchar_t* CTSE_CheckAdapterSpeed::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckAdapterSpeed\",\
                \"TestName\": \"Check The speed of the interface in bits per second\",\
                \"bitsPerSecondSpeed\": \"The speed of the interface in bits per second\",\
                \"SpeedUnitMbps\": \"mbps unit\",\
                \"FindNetDescEnv\": \"Adapter interface Description Environment\",\
                \"IPAddressEnv\": \"IP Address Environment\"\
                }";
    }

    bool CTSE_CheckAdapterSpeed::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"bitsPerSecondSpeed"))
        {
            mdwbitsPerSecondSpeed = _ttoi64(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SpeedUnitMbps"))
        {
            mdwSpeedUnitMbps = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FindNetDescEnv"))
        {
            mstrFindNetDescEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IPAddressEnv"))
        {
            mstrIPAddressEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CheckAdapterSpeed::TSE_Run()
    {
        JGW::CCJGW_AdapterCommonInfo adpater;
        DWORD dwSpeed = 0;
        std::vector<JGW::NetWorkConection>& vNetWorkConection = adpater.GetNetAdaptersInfo();
        
        std::wstring strIPAddress;
        if (!mstrIPAddressEnv.empty()) strIPAddress = GetGlobalEnvironment()->GetString(mstrIPAddressEnv);
        std::string strNetDesc;
        if (!mstrFindNetDescEnv.empty()) strNetDesc = GetGlobalEnvironment()->GetAnsiString(mstrFindNetDescEnv);


        for (size_t i = 0;i < vNetWorkConection.size(); i ++)
        {
            if ((strIPAddress.empty() || NULL == JGW_WStrComparenoCaseWStr(vNetWorkConection[i].ip_address.c_str(),strIPAddress.c_str())) 
                && (strNetDesc.empty() || NULL != JGW_StrCaseStr(vNetWorkConection[i].description.c_str(),strNetDesc.c_str()))
                )
            {
                dwSpeed = adpater.GetNetAdaptersSpeedToDescription(vNetWorkConection[i].description);
                break;
            }
            //if (0 == vNetWorkConection[i].ip_address.compare(L"192.168.1.5"))
            //{
            //    dwSpeed = adpater.GetNetAdaptersSpeedToDescription(vNetWorkConection[i].description);
            //}
        }
        DWORD bitsPerSecondSpeed = static_cast<DWORD>(mdwbitsPerSecondSpeed/1000/1000);
        DWORD speed = dwSpeed / mdwSpeedUnitMbps;
        LogI_F(L"Adapter Speed: %uMbps,Expect Speed:%uMbps",speed,bitsPerSecondSpeed);
        if (bitsPerSecondSpeed == speed)
        {
            return true;
        }
        LogE_F(L"Adapter Speed: %uMbps,Expect Speed:%uMbps Fail!!!",speed,bitsPerSecondSpeed);
        return false;
    }
}

