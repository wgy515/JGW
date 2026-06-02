#include "StdAfx.h"
#include "TSE_IPQ807xWlanTxCalibrationFWControlled2.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanTxCalibrationFWControlled2::CTSE_IPQ807xWlanTxCalibrationFWControlled2(void) : meBand(ALL),mbVsaOnePort(true)
    {
    }


    CTSE_IPQ807xWlanTxCalibrationFWControlled2::~CTSE_IPQ807xWlanTxCalibrationFWControlled2(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanTxCalibrationFWControlled2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanTxCalibrationFWControlled2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyId = (WLAN_UniversalPhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("CalScheme"),strParamName))
        {
            meCalScheme = (WLAN_TpcCalScheme)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("TotalNumOfChains"),strParamName))
        {
            mnTotalNumOfChains = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MultiChainMask"),strParamName))
        {
            mnMultiChainMask = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("NumAverages"),strParamName))
        {
            mnNumAverages = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("VsaOnePort"),strParamName))
        {
            mbVsaOnePort = 1 == _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanTxCalibrationFWControlled2::TSE_Run()
    {
        bool status = false;
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanTxCalibrationFWControlled2(mePhyId,meCalScheme,mnTotalNumOfChains,mnMultiChainMask,meBand,mnNumAverages,status,mbVsaOnePort);
    }
}
