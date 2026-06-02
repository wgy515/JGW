#include "StdAfx.h"
#include "TSE_IPQ807xWlanNoiseFloorCalGenBDF.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanNoiseFloorCalGenBDF::CTSE_IPQ807xWlanNoiseFloorCalGenBDF(void) : mePhyId(PhyID_A),mbUpdateNfToBdf(false),meChainMask(WLAN_CHAIN_NONE),meChainMaskH(WLAN_CHAIN_NONE)
    {
    }


    CTSE_IPQ807xWlanNoiseFloorCalGenBDF::~CTSE_IPQ807xWlanNoiseFloorCalGenBDF(void) 
    {
    }

    const wchar_t* CTSE_IPQ807xWlanNoiseFloorCalGenBDF::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanNoiseFloorCalGenBDF::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyId = (PhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("UpdateNfToBdf"),strParamName))
        {
            mbUpdateNfToBdf = 1 == _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("ChainMask"),strParamName))
        {
            meChainMask = (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("ChainMaskH"),strParamName))
        {
            meChainMaskH = (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("Channel"),strParamName))
        {
            mvChannel.clear();
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChannel.push_back(static_cast<unsigned short>(vTemp[i]));
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanNoiseFloorCalGenBDF::TSE_Run()
    {
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanNoiseFloorCalGenBDF(mePhyId,mvChannel,meChainMask,meChainMaskH,mbUpdateNfToBdf);
    }
}
