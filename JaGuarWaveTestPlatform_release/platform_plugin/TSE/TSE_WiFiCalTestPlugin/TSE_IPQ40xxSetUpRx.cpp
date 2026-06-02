#include "StdAfx.h"
#include "TSE_IPQ40xxSetUpRx.h"

namespace JGW
{
    CTSE_IPQ40xxSetUpRx::CTSE_IPQ40xxSetUpRx(void) : meRxChainMask(WLAN_CHAIN_1),meChannelBondingState(WLAN_Gen6_ChannelBondingState_none),meRate(RATE_11N_HT20_MCS0),mePromiscuousMode(PromiscuousMode),mnChannel(5500),mnChannel2(0),mstrRxMacAddress("010000C0FFEE")
    {
    }


    CTSE_IPQ40xxSetUpRx::~CTSE_IPQ40xxSetUpRx(void)
    {
    }

    const wchar_t* CTSE_IPQ40xxSetUpRx::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_IPQ40xxSetUpRx\",\
                \"TestName\": \"IPQ40xx Set Up Rx\",\
                \"Channel\": \"Rx Channel number\",\
                \"ChannelBondingState\": \"Channel Bonding State\",\
                \"RxChainMask\": \"rx chain\",\
                \"Rate\": \"Rx Data rate\",\
                \"Channel2\": \"Rx Channel for 80p80\",\
                \"PromiscuousMode\": \"0: promiscuous Mode; 1: filter Mode\"\
                }";
    }

    bool CTSE_IPQ40xxSetUpRx::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Channel",strParamName))
        {
            mnChannel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ChannelBondingState",strParamName))
        {
            meChannelBondingState = (WLAN_Gen6_ChannelBondingState)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RxChainMask",strParamName))
        {
            meRxChainMask = (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Rate",strParamName))
        {
            meRate = (WLAN_RATE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PromiscuousMode",strParamName))
        {
            mePromiscuousMode = (WLAN_Action)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Channel2",strParamName))
        {
            mnChannel2 = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ40xxSetUpRx::TSE_Run()
    {
        msRxCalTestParam.mnChannel = mnChannel;
        msRxCalTestParam.mnChannel2 = mnChannel2;
        msRxCalTestParam.meChannelBondingState = meChannelBondingState;
        msRxCalTestParam.meRxChain = meRxChainMask;
        msRxCalTestParam.meRate = meRate;
        msRxCalTestParam.mePromiscuousMode = mePromiscuousMode;

        return PHONE->SetUpRx(msRxCalTestParam);
    }
}
