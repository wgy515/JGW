#include "StdAfx.h"
#include "TSE_IPQ807xWlanRxCalibrationFWControlled2.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanRxCalibrationFWControlled2::CTSE_IPQ807xWlanRxCalibrationFWControlled2(void) : mstrBssId("020000C0FFEE"),mstrRxMacAddress("010000C0FFEE"),mePhyId(PHY_A0),mePromiscuousMode(PromiscuousMode),meBand(_5G_6G_BAND)
    {
    }


    CTSE_IPQ807xWlanRxCalibrationFWControlled2::~CTSE_IPQ807xWlanRxCalibrationFWControlled2(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanRxCalibrationFWControlled2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanRxCalibrationFWControlled2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyId = (WLAN_UniversalPhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("ChainToCalibrate"),strParamName))
        {
            mvChainToCalibrate.clear();
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            for (size_t i = 0;i < vTemp.size();i++)
            {
                mvChainToCalibrate.push_back((size_t)vTemp[i]);
            }
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("Band"),strParamName))
        {
            meBand = (BAND)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("PromiscuousMode"),strParamName))
        {
            mePromiscuousMode = (WLAN_Action)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RxMacAddress"),strParamName))
        {
            mstrRxMacAddress = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("BssId"),strParamName))
        {
            mstrBssId = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanRxCalibrationFWControlled2::TSE_Run()
    {
        for (size_t i = 0;i < mvChainToCalibrate.size();i ++)
        {
            size_t chainMask = 1 << mvChainToCalibrate[i];
            GETINSTRUMENT->SwitchLossPath(false,chainMask);
            bool status = true;
            if (!CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanRxCalibrationFWControlled2(mePhyId,mvChainToCalibrate[i],meBand,mePromiscuousMode,status,mstrRxMacAddress,mstrBssId))
            {
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Wlan Rx Calibration2 Fail");
                return false;
            }
        }
        return true;
    }
}
