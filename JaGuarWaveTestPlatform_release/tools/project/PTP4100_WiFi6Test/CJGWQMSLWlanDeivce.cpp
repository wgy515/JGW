#include "StdAfx.h"
#include "CJGWQMSLWlanDeivce.h"
#include "QLibDemo_WLAN_Core.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

//#define START_LOG 
char (*pQLIB_FTM_WLAN_TLV_Create)(HANDLE hResourceContext,unsigned char opCode);
char (*pQLIB_FTM_WLAN_TLV_Create16)(HANDLE hResourceContext,unsigned short opCode);
char (*pQLIB_FTM_WLAN_TLV_AddParam)(HANDLE hResourceContext, char *pKey, char *pData);
char (*pQLIB_FTM_WLAN_TLV_Complete)(HANDLE hResourceContext);
char (*pQLIB_FTM_WLAN_TLV_GetRspParam)(HANDLE hResourceContext,char *pKey, char *pData);
char (*pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2)(HANDLE hResourceContext, int phyId, int calScheme, double _1ptRangeDbUL, double _1ptRangeDbLL, int totalNumOfChains, int ultiChainMask, int band, int numAverages, void *pAsyncSetChainHandlerCB, void *pAsyncPMHandlerCB2, void *pAsyncRFSHandlerCB);
char (*pQLIB_FTM_WLAN_Atheros_Save_BDF)(HANDLE hResourceContext, unsigned int optio, char * fname);

namespace JGW
{
    CCJGWQMSLWlanDeivce::CCJGWQMSLWlanDeivce(void) : mstrQMSL_Log_File("C:\\Qualcomm\\Log\\QlibDemo_QMSL_Log"),mstrCom("2500"),mhResourceContext(NULL),mbLoadedDut(false)
    {
        pQLIB_FTM_WLAN_TLV_Create = &QLIB_FTM_WLAN_TLV2_Create;
        pQLIB_FTM_WLAN_TLV_Create16 = &QLIB_FTM_WLAN_TLV2_Create16;
        pQLIB_FTM_WLAN_TLV_AddParam = &QLIB_FTM_WLAN_TLV2_AddParam;
        pQLIB_FTM_WLAN_TLV_Complete = &QLIB_FTM_WLAN_TLV2_Complete;
        pQLIB_FTM_WLAN_TLV_GetRspParam = &QLIB_FTM_WLAN_TLV2_GetRspParam;
        pQLIB_FTM_WLAN_Atheros_Tx_FW_CAL2 = &QLIB_FTM_WLAN_Atheros_Tx_FW_CAL2;
        pQLIB_FTM_WLAN_Atheros_Save_BDF = &QLIB_FTM_WLAN_Atheros_Save_BDF;
    }


    CCJGWQMSLWlanDeivce::~CCJGWQMSLWlanDeivce(void)
    {
    }

    bool CCJGWQMSLWlanDeivce::ConnectStandaloneWlanCardQUTS(const std::string& strCom)
    {
        const unsigned int maxListSize = 10;
        unsigned int iNumListSize = maxListSize;
        long long devHandleIdList[maxListSize];
        long long protocolIdList[maxListSize];
        unsigned int index, i;

        char* pDescriptionList[maxListSize];

        unsigned int iDescriptionLength = 200;
        unsigned int iErrorCode = 0;
        for (unsigned int i = 0; i < iNumListSize; i++)
        {
            pDescriptionList[i] = new char[iDescriptionLength];
            if (pDescriptionList[i])
            {
                memset(pDescriptionList[i], 0, sizeof(char)*iDescriptionLength);
            }
        }
        mstrCom = strCom;
        QLIB_SetLibraryMode(QLIB_LIB_MODE_QUTS);
        QLIB_SetTargetType(QLIB_TARGET_TYPE_APQ);

        unsigned char bOK =  QLIB_GetAvailablePhonesHandleIDList((QUTS_PROTOCOL_TYPE)0, &iNumListSize,
            devHandleIdList,
            protocolIdList,
            &iDescriptionLength, pDescriptionList,
            &iErrorCode);

        if (bOK)
        {
            for (i = 0; i < iNumListSize; i++)
            {
                std::string description(pDescriptionList[i]);
                if(description.find(strCom) != std::string::npos)
                { index = i;
                break;
                }
            }

            mhResourceContext = QLIB_ConnectServerWithWaitByHandleID((QUTS_PROTOCOL_TYPE)0, devHandleIdList[index], protocolIdList[index], 2000);
        }

        if (mhResourceContext == NULL)
            return false;

        CaptureQMSLTraceLog();

        return true;
    }

    bool CCJGWQMSLWlanDeivce::IsConnectWlanDeivce()
    {
        return TRUE == QLIB_IsPhoneConnected(mhResourceContext);
    }

    void CCJGWQMSLWlanDeivce::CaptureQMSLTraceLog(void)
    {
#ifdef START_LOG
        CreateDirectory (L"C:\\Qualcomm\\Log", NULL);

        // Automatically start the text log
        unsigned short _iLogFlags = LOG_FN | LOG_RET | LOG_ERR | LOG_C_HIGH_LEVEL_START | LOG_C_HIGH_LEVEL_STOP | LOG_ASYNC | LOG_IO;
        QLIB_SetLogFlags(mhResourceContext, _iLogFlags);
        char LogFile[110];
        sprintf_s(LogFile,110,"%s-%d.txt", mstrQMSL_Log_File.c_str(), mstrCom.c_str());
        QLIB_StartLogging(mhResourceContext, LogFile);
        printf("Open QMSL log file: %s\n\n", LogFile);
#endif
    }

    bool CCJGWQMSLWlanDeivce::LoadDut(char* dllID /* = QCN9224_DLLID */, char* bin_file /* = "" */, unsigned int chipID /* = QCN9224_CHIPID */)
    {
        int LoadOption = 0;

        if (mhResourceContext == NULL)
            return false;

        unsigned char bRet = QLIB_FTM_WLAN_Atheros_LoadDUT(mhResourceContext, (unsigned char*)dllID, (unsigned char*)bin_file, 5, chipID);
        mbLoadedDut = bRet == TRUE;
        return (bRet ? true : false);
    }

    void CCJGWQMSLWlanDeivce::CloseDut()
    {
        if (mbLoadedDut)
        {
            QLIB_FTM_WLAN_Atheros_UNLoadDUT(mhResourceContext);
            mbLoadedDut = false;
        }    
    }

    extern "C" {
        QLIB_API unsigned char QLIB_FTM_WLAN_SetInstance(HANDLE hResourceContext, unsigned int instance);
    }

    void CCJGWQMSLWlanDeivce::SetWlanInstance(int channel)
    {
#if 0
        if (channel < 5000) // 2G
            QLIB_FTM_WLAN_SetInstance(mhResourceContext, WLANINSTANCE_2G);
        else if ((channel > 5000) && (channel < 5925)) // 5G
            QLIB_FTM_WLAN_SetInstance(mhResourceContext, WLANINSTANCE_5G);
        else
            QLIB_FTM_WLAN_SetInstance(mhResourceContext, WLANINSTANCE_6G);
#else
        if (channel > 5925)
        {
            QLIB_FTM_WLAN_SetInstance(mhResourceContext, WLANINSTANCE_6G);
        }
        else
        {
            QLIB_FTM_WLAN_SetInstance(mhResourceContext, WLANINSTANCE_2G);
        }
#endif
    }

    unsigned char override_rxStation_addr[7] = { 0x00, 0x03, 0x7F, 0x44, 0x55, 0x71, 0x00 };
    unsigned char override_txStation_addr[7] = { 0x00, 0x03, 0x7F, 0x44, 0x55, 0x72, 0x00 };
    unsigned char override_txBSSID[7] = { 0x00, 0x03, 0x7F, 0x44, 0x55, 0x75, 0x00 };

    bool CCJGWQMSLWlanDeivce::WlanSetDutTxMacAddressBssId()
    {
        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext,_OP_TX);
        // 00:03:7F:44:55:71
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"rxStation",(char*)override_rxStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txStation",(char*)override_txStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"bssid",(char*)override_txBSSID);
        //return (TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext));
        return true;
    }

    void CCJGWQMSLWlanDeivce::StopTx(int channel /* = 6135 */, PhyID phyId /* = PHY_B */)
    {
        char buf[32];

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP_TX);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel", _itoa(channel, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txMode", "0");   //0 = xmit off
        pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
    }

    void CCJGWQMSLWlanDeivce::DisconnectStandaloneWlanCard()
    {
#ifdef START_LOG
        char LogFile[110];
        sprintf_s(LogFile,110, "%s-%d.txt", mstrQMSL_Log_File, mstrCom);
        QLIB_StopLogging(mhResourceContext);
        printf("Close QMSL log file: %s\n\n", LogFile);
#endif

        //80-WL400-11 Refer Section "Disconnect - standalone targets"
        if (mhResourceContext)
            QLIB_DisconnectServer(mhResourceContext);
        mhResourceContext = NULL;
    }

    bool CCJGWQMSLWlanDeivce::StartTx(int txPower0,RFMode phyRFMode /* = DBS_PhyA5G_PhyB2G */, PhyID phyId /* = PHY_B */, WLAN_TX_Mode txMode /* = ContTx99 */, int channel /* = 6135 */, int channel2 /* = 0 */, WLAN_UniversalDataRate rateBitIndex0 /* = RATE_MCS_0 */, WLAN_Gen6_ChannelBondingState wlanMode /* = BW320 */, int pktLen0 /* = 1500 */, WLAN_CHAIN_MASK2 txChain0 /* = WLAN_CHAIN_1 */, int shortGuard /* = 0 */, int numPackets /* = 0 */, int broadcast /* = 0 */, int flags /* = 0x18 */, int gainIdx /* = 5 */, int dacGain /* = 5 */, int paConfig /* = 5 */, TCMD_TPC_TYPE tpcm /* = TPC_TX_PWR */, WLAN_UniversalWiFtStandard wifiStd /* = WiFiStandard_Legacy11AX */, int bandWidth /* = 0 */, WLAN_UniversalRateBW rateBw /* = RateBW_11BE_EHT320 */, WLAN_UniversalNSS nss /* = _1Stream */, WLAN_UniversalGuardInterval gI /* = LTF_Mode4_GI_800 */)
    {
#if 0
        char buf[32];

        unsigned char override_rxStation_addr[7] = { 0xA0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txStation_addr[7] = { 0xB0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txBSSID[7] = { 0xC0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };

        SetWlanInstance(channel);

        StopTx(channel, phyId);

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP_TX);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel", _itoa(channel, buf, 10));
        if (channel2 != 0)
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel2", _itoa(channel2, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txMode", _itoa(txMode, buf, 10));                  //3 = Tx99
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBitIndex0", _itoa(rateBitIndex0, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wlanMode", _itoa(wlanMode, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "pktLen0", _itoa(pktLen0, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txChain0", _itoa(txChain0, buf, 10));             //1(ch0),2(ch1),3(ch01) etc.
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "shortGuard", _itoa(shortGuard, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "numPackets", _itoa(numPackets, buf, 10));         //0 = continuous, otherwise number of packets
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "broadcast", broadcast ? "1" : "0");

        //flags = 0x18;
        //if (ldpcRate)  flags |= DESC_LDPC_ENA_MASK;  // 0x00000002
        //if (dpdMode)   flags |= PAPRD_ENA_MASK;      // 0x00000004
        //if (STBCmode)  flags |= DESC_STBC_ENA_MASK;  // 0x00000001
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "flags", _itoa(flags, buf, 10));

        // tpcm TX_PWR=0, FORCED_GAIN=1, TGT_PWR=2, TX_FORCED_GAIN=3, FORCED_GAINIDX=4, FORCED_TGTPWR=5
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "tpcm", _itoa(tpcm, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "gainIdx", _itoa(gainIdx, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "dacGain", _itoa(dacGain, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "paConfig", _itoa(paConfig, buf, 10));

        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", _itoa(wifiStd, buf, 10));  // For now = 2
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bandwidth", _itoa(bandWidth, buf, 10));   // cbState
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBw", _itoa(rateBw, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "nss", _itoa(nss, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "gI", _itoa(gI, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "agg", "1");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "dutyCycle", "10");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rxStation", (char*)override_rxStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txStation", (char*)override_txStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bssid", (char*)override_txBSSID);

        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "scramblerOff", "0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "shortGuard", "0");
        //nPattern
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txPattern", "0");

        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "txPower0", _itoa(txPower0, buf, 10));

        return TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
#else
        bool bResult = false;
        char buf[32] = {0};
        StopTx(channel,phyId);
        QLIB_GetTimeOut(mhResourceContext,20000);

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP_TX);
        sprintf_s(buf,31,"%d",bandWidth);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"bandwidth",buf);
        sprintf_s(buf,31,"%d",wifiStd);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"wifiStandard",buf);
        sprintf_s(buf,31,"%d",phyId);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"phyId",buf);
        sprintf_s(buf,31,"%d",nss);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"nss",buf);
        sprintf_s(buf,31,"%d",gI);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"gI",buf);
        sprintf_s(buf,31,"%d",channel);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"channel",buf);
        sprintf_s(buf,31,"%d",txMode);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txMode",buf);
        sprintf_s(buf,31,"%d",rateBitIndex0);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"rateBitIndex0",buf);
        sprintf_s(buf,31,"%d",rateBw);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"rateBw",buf);

        sprintf_s(buf,31,"%d",wlanMode);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"wlanMode",buf);
        sprintf_s(buf,31,"%d",pktLen0);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"pktLen0",buf);
        sprintf_s(buf,31,"%d",txChain0);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txChain0",buf);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"agg","1");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "shortGuard", "0");
        sprintf_s(buf,31,"%d",numPackets);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "numPackets", buf);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "broadcast", broadcast ? "1" : "0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"rxStation",(char*)override_rxStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txStation",(char*)override_txStation_addr);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"bssid",(char*)override_txBSSID);
        /*pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"rxStation","00:03:7F:44:55:71");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txStation","00:03:7F:44:55:72");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"bssid","00:03:7F:44:55:75");*/
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"aifsn","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"scramblerOff","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"flags","24");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txPattern","0");
        sprintf_s(buf,31,"%d",gI);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"gI",buf);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"ctrlFlag","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"dutyCycle","10");
        sprintf_s(buf,31,"%d",tpcm);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"tpcm",buf);
        sprintf_s(buf,31,"%d",txPower0);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"txPower0",buf);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"ppduType","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"dcm","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"scanProfile","0");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,"puncBwMask","0");
        bResult = TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
        QLIB_GetTimeOut(mhResourceContext,20000);
        return bResult;
#endif
    }
#if 0
    bool CCJGWQMSLWlanDeivce::StartRx(RFMode phyRFMode /* = DBS_PhyA5G_PhyB2G */, PhyID phyId /* = PHY_B */, int channel /* = 6135 */, int channel2 /* = 0 */, WLAN_RX_MODE rxMode /* = ALL_FRAMES */, WLAN_UniversalDataRate rateBitIndex0 /* = RATE_MCS_0 */, WLAN_CHAIN_MASK2 rxChain /* = WLAN_CHAIN_1 */, WLAN_Gen6_ChannelBondingState wlanMode /* = BW320 */, WLAN_UniversalWiFtStandard wifiStd /* = WiFiStandard_Legacy11AX */, int bandWidth /* = 0 */, WLAN_UniversalRateBW rateBw /* = RateBW_11BE_EHT320 */, WLAN_UniversalNSS nss /* = _1Stream */)
    {
        char buf[32];
        unsigned char rx_mac_addr[6] = { 1, 0, 0, 0xC0, 0xFF, 0xEE }; //0x010000C0FFEE;
        unsigned char rx_bssid[6] = { 1, 0, 0, 0xC0, 0xFF, 0xEE };       //0x010000C0FFEE

        SetWlanInstance(channel);

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP2_SETPHYRFMODE);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyRFMode", _itoa(phyRFMode, buf, 10));
        pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP_RX);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel", _itoa(channel, buf, 10));
        if (channel2 != 0)
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel2", _itoa(channel2, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rxMode", _itoa(rxMode, buf, 10));        //1 = filter mode
        if (rxMode == 1)
        {
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "staAddr", (char*)rx_mac_addr);
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bssid", (char*)rx_bssid);
        }
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rxChain", _itoa(rxChain, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wlanMode", _itoa(wlanMode, buf, 10));

        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", _itoa(wifiStd, buf, 10));  // For now = 2
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bandwidth", _itoa(bandWidth, buf, 10));   // cbState
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBw", _itoa(rateBw, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "nss", _itoa(nss, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBitIndex0", _itoa(rateBitIndex0, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "flags", _itoa(0x30, buf, 10));

        return TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
    }

#else
    bool CCJGWQMSLWlanDeivce::StartRx(WLAN_TLV2_OPCODE_NEW wlanTlv2Opcode/* = _OP2_RX_NEW */, PhyID phyId /* = PHY_B */, int channel /* = 6135 */, int channel2 /* = 0 */, WLAN_RX_MODE rxMode /* = ALL_FRAMES */, WLAN_UniversalDataRate rateBitIndex0 /* = RATE_MCS_0 */, WLAN_CHAIN_MASK2 rxChain /* = WLAN_CHAIN_1 */, WLAN_Gen6_ChannelBondingState wlanMode /* = BW320 */, WLAN_UniversalWiFtStandard wifiStd /* = WiFiStandard_Legacy11AX */, int bandWidth /* = 0 */, WLAN_UniversalRateBW rateBw /* = RateBW_11BE_EHT320 */, WLAN_UniversalNSS nss /* = _1Stream */,WLAN_Antenna antenna /* = Antenna1 */)
    {
        char buf[32];
        unsigned char rx_mac_addr[6] = { 1, 0, 0, 0xC0, 0xFF, 0xEE }; //0x010000C0FFEE;
        unsigned char rx_bssid[6] = { 1, 0, 0, 0xC0, 0xFF, 0xEE };       //0x010000C0FFEE

        SetWlanInstance(channel);

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, wlanTlv2Opcode);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", _itoa(wifiStd, buf, 10));  // For now = 2
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));

        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel", _itoa(channel, buf, 10));
        if (channel2 != 0)
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "channel2", _itoa(channel2, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rxMode", _itoa(rxMode, buf, 10));        //1 = filter mode
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBitIndex0", _itoa(rateBitIndex0, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rateBw", _itoa(rateBw, buf, 10));

         pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "nss", _itoa(nss, buf, 10));
         pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bandwidth", _itoa(bandWidth, buf, 10));   // cbState
        if (rxMode == 1)
        {
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "staAddr", (char*)rx_mac_addr);
            pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "bssid", (char*)rx_bssid);
        }
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wlanMode", _itoa(wlanMode, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "antenna", _itoa(antenna, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "enANI","1");
        
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "rxChain", _itoa(rxChain, buf, 10));
        //pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", _itoa(wifiStd, buf, 10));  // For now = 2
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "flags", _itoa(0x30, buf, 10));

        return TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
    }
#endif
    void CCJGWQMSLWlanDeivce::StopRxWithReport(PhyID phyId /* = PHY_B */)
    {
        char strData[128];
        char buf[32];

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP2_RXSTATUS);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", "3");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "LinkDir", "1");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "stopRx", "1");
        pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);

        printf("==============================\n");
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "totalPkt", strData);
        printf("Rx Report : totalPkt    = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "goodPackets", strData);
        printf("Rx Report : goodPackets = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "crcErrPkt", strData);
        printf("Rx Report : crcErrPkt   = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "secErrPkt", strData);
        printf("Rx Report : secErrPkt   = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "evm0", strData);
        printf("Rx Report : evm0        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "rssi", strData);
        printf("Rx Report : rssi        = %4s\n", strData);
    }

    bool CCJGWQMSLWlanDeivce::GetRxReport(size_t& uTotalPackets,size_t& uGoodPackets,int& nRssi,float& fEvm,PhyID phyId /* = PHY_B */)
    {
        char strData[128];
        char buf[32];
        float fEvmScaleFactor = 10.0f;

        pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, _OP2_RXSTATUS);
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "phyId", _itoa(phyId, buf, 10));
        //pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "wifiStandard", "3");
        //pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "LinkDir", "1");
        pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext, "stopRx", "1");
        pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext);

        //printf("==============================\n");
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "totalPkt", strData);
        //printf("Rx Report : totalPkt    = %4s\n", strData);
        uTotalPackets = atoi(strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "goodPackets", strData);
        //printf("Rx Report : goodPackets = %4s\n", strData);
        uGoodPackets = atoi(strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "crcErrPkt", strData);
        //printf("Rx Report : crcErrPkt   = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "secErrPkt", strData);
        //printf("Rx Report : secErrPkt   = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "evmScaleFactor", strData);
        //printf("Rx Report : evmScaleFactor   = %4s\n", strData);
        fEvmScaleFactor = static_cast<float>(atof(strData));
        if (0 == fEvmScaleFactor) fEvmScaleFactor = 10.0f;

        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "evm0", strData);
        //printf("Rx Report : evm0        = %4s\n", strData);
        fEvm = static_cast<float>(atof(strData)) / fEvmScaleFactor;

        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "evm1", strData);
        //printf("Rx Report : evm1        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "evm2", strData);
        //printf("Rx Report : evm2        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "endTime", strData);
        //printf("Rx Report : endTime        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "startTime", strData);
        //printf("Rx Report : startTime        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "byteCount", strData);
        //printf("Rx Report : byteCount        = %4s\n", strData);
        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "dontCount", strData);
        //printf("Rx Report : dontCount        = %4s\n", strData);

        pQLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext, "rssi", strData);
        //printf("Rx Report : rssi        = %4s\n", strData);
        nRssi = atoi(strData);

        return true;
    }

    bool CCJGWQMSLWlanDeivce::ExecTlv2ConfigFlow(std::vector<S_TLV2_CONFIG>& vsTlv2Configs)
    {
        bool result = true;
        for (std::vector<S_TLV2_CONFIG>::iterator it = vsTlv2Configs.begin();it != vsTlv2Configs.end();++it)
        {
            if (it->mnopCode > 0)
            {
                pQLIB_FTM_WLAN_TLV_Create(mhResourceContext, it->mnopCode);
            }

            for (std::vector<S_PARAM_NAME_VALUE>::iterator itVec = it->mvParamNameValues.begin();itVec != it->mvParamNameValues.end();++itVec)
            {
                if (NULL == JGW_StrCaseStr(itVec->mstrParamValue.c_str(),"MAC:"))
                {
                    pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,const_cast<char*>(itVec->mstrParamName.c_str()),const_cast<char*>(itVec->mstrParamValue.c_str()));
                }
                else
                {
                    std::string strRead = itVec->mstrParamValue;
                    char szTemp[128] = {0};
                    JGW_ReplaceStringA(strRead,"MAC:","");
                    JGW_ReplaceStringA(strRead,":","");

                    for (size_t x = 1;x < strRead.length();x += 2)
                    {
                        szTemp[x/2] = hexCharToInt((char)strRead[x]) + hexCharToInt((char)strRead[x-1]) * 16;
                    }
                    pQLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,const_cast<char*>(itVec->mstrParamName.c_str()),szTemp);
                }   
            }

            if (it->mbIsCompleted)
            {
                result &= (TRUE == pQLIB_FTM_WLAN_TLV_Complete(mhResourceContext));
            }
            QLIB_GetTimeOut(mhResourceContext,20000);
        }
        return result;
    }
}
