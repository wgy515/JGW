#include "StdAfx.h"
#include "TSE_ScanRKDevice.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_ScanRKDevice::CTSE_ScanRKDevice(void) : mnScanDeviceTimeoutSec(30)
    {
    }


    CTSE_ScanRKDevice::~CTSE_ScanRKDevice(void)
    {
    }

    const wchar_t* CTSE_ScanRKDevice::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ScanRKDevice\",\
                \"TestName\": \"Scan RK Device\",\
                \"ScanDeviceTimeoutSec\": \"扫描设备超时时间单位秒\",\
                \"RKLayerIDEnvironment\": \"RK Device Layer ID\",\
                }";
    }

    bool CTSE_ScanRKDevice::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ScanDeviceTimeoutSec"))
        {
            mnScanDeviceTimeoutSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RKLayerIDEnvironment"))
        {
            mstrLayerIDEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ScanRKDevice::TSE_Run()
    {
        jgw_timer jt;
        int dwLayerID = GetGlobalEnvironment()->GetInt(mstrLayerIDEnvironment);
        LogI_F(L"Scan RK Device Layer ID(%s) : %d - %d",mstrLayerIDEnvironment.c_str(),dwLayerID,(!mstrLayerIDEnvironment.empty() && dwLayerID > 0) ? 1 : 0);

        while (jt.elapsed() < mnScanDeviceTimeoutSec)
        {
            if (!mstrLayerIDEnvironment.empty() && dwLayerID > 0)
            {
                if (1 == CCJGW_RKDeviceHelp::GetRKDeviceInfoToLayerID(dwLayerID))
                {
                    return true;
                }
            }
            else
            {
                if (CCJGW_RKDeviceHelp::ScanRKDevice() >= 1)
                {
                    return true;
                }
            }
            Sleep(100);
        }
        return false;
    }
}
