#pragma once
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>
#include "jgw_wr_nv_api.h"

namespace JGW
{
    class JGWQMSLFUNCPLUGIN_API CCJGWWiFiQMSL:public jgw_wr_nv_api
    {
    public:
        CCJGWWiFiQMSL(void* hResourceContent = NULL,bool bUseQPST = false);
        ~CCJGWWiFiQMSL(void);
        /*
        // Storage type -DataNone=0, DataFlash=1, DataEeprom=2,DataOtp=3, DataDontLoad=4,
        // DataFile=5, DataDram=6
        // SSID = 0x3e, this is afixed id for QCA61x4 chip
        */
        bool LoadWiFiDut(const char *boardDataFName = "",const char *DevDLLname = "qc6174",int iNVMem = 5,unsigned int ssid = 62);

        bool UnLoadWiFiDut();

        bool StartWiFiTx(int nTxChain,float fPower,int nChannel,int nHtMode,int nDataRate,int nTxMode = 3,int nTpcm = 0);

        bool DisableWiFiTx();

        bool StartWiFiRx(int nRxChain,int nChannel,int nHtMode,int nDataRate);
        
        bool GetWiFiRxData(float& rssi,int& txbTotalPackets,int& txbGoodPackets);
    private:
        bool DefaultWiFiTxParam();
    };

}

