#include "StdAfx.h"
#include "CJGW_WiFiTestResultRecord.h"

namespace JGW
{
    CCJGW_WiFiTestResultRecord::CCJGW_WiFiTestResultRecord(void)
    {
    }


    CCJGW_WiFiTestResultRecord::~CCJGW_WiFiTestResultRecord(void)
    {
    }

    void CCJGW_WiFiTestResultRecord::ClearTRXTestData()
    {
        mvsTestDatas.clear();
    }

    std::vector<S_TRX_TEST_DATA>* CCJGW_WiFiTestResultRecord::GetTRxTestData()
    {
        return &mvsTestDatas;
    }

    LPS_TRX_TEST_DATA CCJGW_WiFiTestResultRecord::GetTRXTestDataRecordToWiFiInfo(WLAN_UniversalPhyID ePhyID,WLAN_CHAIN_MASK2 eChainMask,uint uChannel,WLAN_UniversalDataRate eDataRate,WLAN_UniversalRateBW eRateBw,WLAN_Gen6_ChannelBondingState eChannelBondingState)
    {
        for (std::vector<S_TRX_TEST_DATA>::iterator it = mvsTestDatas.begin();
            it != mvsTestDatas.end();
            ++ it)
        {
            if (it[0].meChainMask == eChainMask 
                /*&& it[0].meChannelBondingState == eChannelBondingState*/
                && it[0].mePhyID == ePhyID 
                && it[0].muChannel == uChannel
                && it[0].meRateBw == eRateBw 
                && it[0].meDataRate == eDataRate)
            {
                return &it[0];
            }
        }

        S_TRX_TEST_DATA sTRxTestData = {false,WLAN_Universal_CW,WLAN_CHAIN_NONE,RateBW_CCK,PHY_A0,WLAN_Gen6_ChannelBondingState_none,0,INVAILD_WIFI_RESULT,0,0,0,0,0,0,0,0,INVAILD_WIFI_RESULT,0};
        sTRxTestData.mePhyID = ePhyID;
        sTRxTestData.meChainMask = eChainMask;
        sTRxTestData.muChannel = uChannel;
        sTRxTestData.meDataRate = eDataRate;
        sTRxTestData.meRateBw = eRateBw;
        sTRxTestData.meChannelBondingState = eChannelBondingState;
        sTRxTestData.mnPERPower = INVAILD_WIFI_RESULT;
        sTRxTestData.mfAvgTxPower = INVAILD_WIFI_RESULT;
        mvsTestDatas.push_back(sTRxTestData);
        return &mvsTestDatas[mvsTestDatas.size() - 1];
    }
}

