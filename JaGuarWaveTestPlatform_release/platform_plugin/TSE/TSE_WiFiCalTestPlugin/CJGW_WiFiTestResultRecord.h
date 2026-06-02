#pragma once
#include "CJGW_IPQ807xTestDataDefine.h"
namespace JGW
{
    class CCJGW_WiFiTestResultRecord
    {
    public:
        CCJGW_WiFiTestResultRecord(void);
        ~CCJGW_WiFiTestResultRecord(void);
    public:
        void ClearTRXTestData();
        LPS_TRX_TEST_DATA GetTRXTestDataRecordToWiFiInfo(WLAN_UniversalPhyID ePhyID,WLAN_CHAIN_MASK2 eChainMask,uint uChannel,WLAN_UniversalDataRate eDataRate,WLAN_UniversalRateBW eRateBw,WLAN_Gen6_ChannelBondingState eChannelBondingState);
        std::vector<S_TRX_TEST_DATA>* GetTRxTestData();
    private:
        std::vector<S_TRX_TEST_DATA> mvsTestDatas;
    };
}
