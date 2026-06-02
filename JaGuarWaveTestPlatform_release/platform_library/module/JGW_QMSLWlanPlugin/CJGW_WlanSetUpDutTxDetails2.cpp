#include "StdAfx.h"
#include "CJGW_WlanSetUpDutTxDetails2.h"
#include "JGW_FoundationFunc/JGW_StringFunc.h"

namespace JGW
{
    CCJGW_WlanSetUpDutTxDetails2::CCJGW_WlanSetUpDutTxDetails2(void)
    {
        m_override_rxStation_addr = "FF:FF:FF:FF:FF:FF";
        m_override_txStation_addr = "20:22:22:22:22:02";
        m_override_txBSSID = "50:55:55:55:55:05";
    }


    CCJGW_WlanSetUpDutTxDetails2::~CCJGW_WlanSetUpDutTxDetails2(void)
    {
    }

    bool CCJGW_WlanSetUpDutTxDetails2::tlvAddRspParam(const std::string& strKey,const std::string& strData)
    {
        return TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,const_cast<char*>(strKey.data()),const_cast<char*>(strData.data()));
    }

    bool CCJGW_WlanSetUpDutTxDetails2::tlvAddRspParam(const std::string& strKey,size_t nData)
    {
        std::string data;
        JGW_FormatString(data,"%u",nData);
        return tlvAddRspParam(strKey,data);
    }

    bool CCJGW_WlanSetUpDutTxDetails2::tlv2AddRspParam(const std::string& strKey,const std::string& strData)
    {
        return TRUE == QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,const_cast<char*>(strKey.data()),const_cast<char*>(strData.data()));
    }

    bool CCJGW_WlanSetUpDutTxDetails2::tlv2AddRspParam(const std::string& strKey,size_t nData)
    {
        std::string data;
        JGW_FormatString(data,"%u",nData);
        return tlv2AddRspParam(strKey,data);
    }

    bool CCJGW_WlanSetUpDutTxDetails2::tlv2AddRspParam(const std::string& strKey,int nData)
    {
        std::string data;
        JGW_FormatString(data,"%d",nData);
        return tlv2AddRspParam(strKey,data);
    }

    std::string CCJGW_WlanSetUpDutTxDetails2::tlv2GetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(mhResourceContent,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    std::string CCJGW_WlanSetUpDutTxDetails2::tlvGetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContent,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    uint CCJGW_WlanSetUpDutTxDetails2::GetTgtPwr2(uint phyId, uint freq, uint rate, uint rateBw, uint nss, uint ppduType)
    {
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)_OP2_GETTGTPWR);
        tlv2AddRspParam("phyId", (int)phyId);
        tlv2AddRspParam("freq", freq);
        tlv2AddRspParam("userMode", 2);
        tlv2AddRspParam("rateBit", rate);
        tlv2AddRspParam("rateBw", rateBw);
        tlv2AddRspParam("nss", nss);
        tlv2AddRspParam("ppduType", ppduType);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);

        std::string data = tlv2GetRspParam("tgtPwr");
        return atoi(data.c_str());
    }

    int CCJGW_WlanSetUpDutTxDetails2::getChanFreqMhz(uint chanID)
    {
        int array[] = 
        {
            0,
            2412,
            2417,
            2422,
            2427,
            2432,
            2437,
            2442,
            2447,
            2452,
            2457,
            2462,
            2467,
            2472,
            2484
        };
        int array2[] = 
        {
            36,
            38,
            40,
            42,
            44,
            46,
            48,
            50,
            52,
            54,
            56,
            58,
            60,
            62,
            64,
            100,
            102,
            104,
            106,
            108,
            110,
            112,
            114,
            116,
            118,
            120,
            122,
            124,
            126,
            128,
            130,
            132,
            134,
            136,
            138,
            140,
            142,
            144,
            149,
            151,
            153,
            155,
            157,
            159,
            161,
            163,
            165,
            172,
            174,
            176,
            178,
            180,
            182,
            184
        };
        int array3[] = 
        {
            5180,
            5190,
            5200,
            5210,
            5220,
            5230,
            5240,
            5250,
            5260,
            5270,
            5280,
            5290,
            5300,
            5310,
            5320,
            5500,
            5510,
            5520,
            5530,
            5540,
            5550,
            5560,
            5570,
            5580,
            5590,
            5600,
            5610,
            5620,
            5630,
            5640,
            5650,
            5660,
            5670,
            5680,
            5690,
            5700,
            5710,
            5720,
            5745,
            5755,
            5765,
            5775,
            5785,
            5795,
            5805,
            5815,
            5825,
            5860,
            5870,
            5880,
            5890,
            5900,
            5910,
            5920
        };
        if (chanID > 2000)
        {
            return (int)chanID;
        }
        if (chanID == 0)
        {
            return 0;
        }
        if (chanID >= 1 && chanID <= 14)
        {
            return array[chanID];
        }
        for (int i = 0; i < _countof(array2); i++)
        {
            if (chanID == array2[i])
            {
                return array3[i];
            }
        }
        throw "error channel info";
    }

    void CCJGW_WlanSetUpDutTxDetails2::TurnOffTX2(uint phyId, uint channel, uint txChain)
    {
        uint value = channel;
        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }

        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)1);
        tlv2AddRspParam("phyid", phyId);
        tlv2AddRspParam("channel", value);
        tlv2AddRspParam("txMode", 0);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
    }

    bool CCJGW_WlanSetUpDutTxDetails2::setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, std::vector<unsigned char> pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        bool result = true;
        ackRequest = ((HC_Enable == HCS_Enabled || ackRequest) ? true : false);
        TurnOffTX2(phyId,channel,txChainMask);

        uint centFreqMHz = channel;
        if (channel < 2000)
        {
            centFreqMHz = (uint)getChanFreqMhz(channel);
        }
        uint centFreq2MHz = channel2;
        if (channel2 < 2000)
        {
            centFreq2MHz = (uint)getChanFreqMhz(channel2);
        }
        result &= TRUE == QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)1);
        //! set cb state
        if ((int)cbState == 50)
        {
            tlv2AddRspParam("bandwidth", 50);
            cbState = WLAN_Gen6_ChannelBondingState_none;
        }
        else if ((int)cbState == 51)
        {
            tlv2AddRspParam("bandwidth", 51);
            cbState = WLAN_Gen6_ChannelBondingState_none;
        }
        else
        {
            tlv2AddRspParam("bandwidth", "0");
        }

        /**************************** SetUpTxDefault2 *******************************/
        uint num = 1u;
        uint num2 = 2u;
        uint num3 = 4u;
        uint num4 = 128u;

        tlv2AddRspParam("wifiStandard", 2);
        tlv2AddRspParam("phyId", phyId);
        tlv2AddRspParam("nss", nss);
        tlv2AddRspParam("gI", (uint)(int)guardInterval);
        tlv2AddRspParam("channel", centFreqMHz);
        if (centFreq2MHz != 0)
        {
            tlv2AddRspParam("channel2", centFreq2MHz);
        }
        tlv2AddRspParam("txMode", (uint)(int)txMode);
        tlv2AddRspParam("rateBitIndex0", (uint)(int)rate);
        tlv2AddRspParam("rateBw", (uint)(int)rateBw);
        tlv2AddRspParam("wlanMode", (uint)(int)cbState);
        tlv2AddRspParam("pktLen0", payLoadSize);
        tlv2AddRspParam("txChain0", txChainMask);
        tlv2AddRspParam("agg", aggregation);
        tlv2AddRspParam("shortGuard", (!short11b_nGuard) ? "0" : "1");
        tlv2AddRspParam("numPackets", nPacket);
        tlv2AddRspParam("broadcast", ((!broadcastMode) ? "0" : "1"));

        unsigned char override_rxStation_addr2g[7] = { 0xA0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txStation_addr2g[7] = { 0xB0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txBSSID2g[7]        = { 0xC0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };

        unsigned char override_rxStation_addr5g[7] = { 0xD0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txStation_addr5g[7] = { 0xE0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
        unsigned char override_txBSSID5g[7]        = { 0xF0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00 };

        unsigned char *override_rxStation_addr;
        unsigned char *override_txStation_addr;
        unsigned char *override_txBSSID;

        // QC6390 only has one wlan instance.
        if (phyId == 1)  // phyB
        {
            override_rxStation_addr = override_rxStation_addr2g;
            override_txStation_addr = override_txStation_addr2g;
            override_txBSSID        = override_txBSSID2g;
        }
        else 
        {
            override_rxStation_addr = override_rxStation_addr5g;
            override_txStation_addr = override_txStation_addr5g;
            override_txBSSID        = override_txBSSID5g;
        }
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"rxStation", (char *)override_rxStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"txStation", (char *)override_txStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"bssid", (char *)override_txBSSID);

        tlv2AddRspParam("aifsn", iAIFSN);
        tlv2AddRspParam("scramblerOff", ((!scramblerDisable) ? "0" : "1"));
        uint num5 = 24u;
        if (ldpcRate)
        {
            num5 |= num2;
        }
        if (dpdMode == DPD_Enabled)
        {
            num5 |= num3;
        }
        if (bSTBCmode)
        {
            num5 |= num;
        }
        if (ackRequest)
        {
            num5 |= num4;
        }
        tlv2AddRspParam("flags", num5);
        tlv2AddRspParam("txPattern", (uint)nPattern);
        tlv2AddRspParam("gI", (uint)(int)guardInterval);
        /************************************************************/
        tlv2AddRspParam("ctrlFlag", phy165Mode);
        tlv2AddRspParam("dutyCycle", dutyCycle);
        switch (pwrMode)
        {
        case ForcedGain:
        case ForcedDesiredGain:
            tlv2AddRspParam("tpcm", 4);
            tlv2AddRspParam("gainIdx", gain);
            tlv2AddRspParam("dacGain", digitalGain);
            tlv2AddRspParam("paConfig", PASetting);
            break;
        case ForcedGLUTIndex:
            tlv2AddRspParam("tpcm", 6);
            tlv2AddRspParam("gainIdx", gain);
            break;
        case TxPowerAuto:
            tlv2AddRspParam("tpcm", 5);
            break;
        case TxForcedPerChain:
            tlv2AddRspParam("tpcm", 7);
            break;
        case TxPowerForce_CLPC:
        case TxPowerForce_SCPC:
        case TxPowerForce_OLPC:
            tlv2AddRspParam("tpcm", 0);
            tlv2AddRspParam("txPower0", static_cast<uint>(powerLevel));
            break;
        default:
            mstrErrorMsg = "IPQ807x doesn't support selected pwrMode";
            return false;
        }
        return result;
    }

    bool CCJGW_WlanSetUpDutTxDetails2::SetUpDutTxDetails2(WLAN_UniversalPhyID phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, std::vector<unsigned char> pattern, uint nPacket, bool broadcastMode, uint AIFSN, int dutTxSettlingDelayMs, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, PPDU_TYPE ppduType, bool dualCarrierModulation, PHY165Mode phy165Mode, const std::string& puncBwMask, double* txPower)
    {
        uint wifiStandard = 2u;
        *txPower = powerLevel;
        uint chainMask2 = (uint)(int)(chainMask | ((long)chainMaskH << 4));
        bool result = true;
        unsigned short num = static_cast<unsigned short>(atoi(puncBwMask.c_str()));

        result = setUpTxDetailCommon2(wifiStandard, phyId, (uint)channel, (uint)channel2, cbState, phy165Mode, rate, rateBw, (uint)(int)nss, pwrMode, txMode, chainMask, powerLevel, gain, digitalGain, PAcfg, payLoadSize, false, ldpcRate, stbcMode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, AIFSN, scramblerDisable, ackRequest, guardInterval);
        if (!result)
        {
            return false;
        }

        unsigned int ScanProfile = 0,result_status = 0;
        result &= tlv2AddRspParam("ppduType", ppduType);
        result &= tlv2AddRspParam("dcm", dualCarrierModulation ? 1 : 0);
        result &= tlv2AddRspParam("scanProfile", ScanProfile);
        result &= TRUE == QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);

        std::string data = tlv2GetRspParam("status");
        result_status = atoi(data.c_str());
        if (0 != result_status)
        {
            //! hrow new WLANCustomException("SetUpTxDetail2 - error code " + num);
            JGW_FormatString(mstrErrorMsg,"SetUpTxDetail2 - error code : %d",num);
            return false;
        }
        if (pwrMode == TxPowerAuto)
        {
            uint tgtPwr = GetTgtPwr2(phyId, (uint)channel, (uint)(int)rate, (uint)(int)rateBw, (uint)(int)nss, ppduType);
            if (tgtPwr != 0)
            {
                *txPower = (double)tgtPwr / 4.0;
            }
            else
            {
                *txPower = 0.0;
            }
        }
        Sleep(dutTxSettlingDelayMs);
        return result;
    }

    bool CCJGW_WlanSetUpDutTxDetails2::TurnOnRX2(WLAN_UniversalPhyID phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, AntennaSetting antenna, bool LDPC, bool STBC, WLAN_Action rxFilterMode, const std::string& rxMacAddress, WLAN_UniversalGuardInterval vsgGuardInterval,PPDU_TYPE ppduType, bool dualCarrierModulation /* = false */, bool enANI /* = false */, PHY165Mode phy165Mode /* = Mode_0 */, AniMode aniMode /* = Static */, int aniDynaLevel /* = 0 */, const std::string& puncBwMask /* = "0x00" */)
    {
        uint value = channel;
        uint num = channel2;
        uint rxChain = (uint)(int)(chainMask | ((long)chainMaskH << 4));
        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }
        if (channel2 < 2000)
        {
            num = (uint)getChanFreqMhz(channel2);
        }
        //uint rate = rate;
        //RxChain = rxChain;
        //freqMHz = channel;
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)2);
        tlv2AddRspParam("wlanMode", (uint)(int)cbState);
        if ((int)cbState == 50)
        {
            tlv2AddRspParam("bandwidth", "50");
        }
        else if ((int)cbState == 51)
        {
            tlv2AddRspParam("bandwidth", "51");
        }
        else
        {
            tlv2AddRspParam("bandwidth", "0");
        }
        //data = Convert.ToString(rxFilterMode);
        tlv2AddRspParam("rxMode", rxFilterMode);
        if (rxFilterMode == 1)
        {
            //! 010000C0FFEE
            unsigned char override_rxStation_addr[7] = { 0x01, 0x00, 0x00, 0xC0, 0xFF, 0xEE, 0x00 };
            QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"staAddr", (char *)override_rxStation_addr);
            //tlv2AddRspParam("staAddr", WlanTestSharedInfo.override_rx_mac_addr, WlanTestSharedInfo.override_rx_mac_addr.Length);
            unsigned char override_txBSSID2g[7]        = { 0xC0, 0xC1, 0xC0, 0xC0, 0x00, 0x00, 0x00 };
            QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"bssid",  (char *)override_txBSSID2g);
        }
        tlv2AddRspParam("channel", value);
        if (num != 0)
        {
            tlv2AddRspParam("channel2", num);
        }
        uint num2 = 48u;
        uint num3 = 536870912u;
        uint num4 = 268435456u;
        bool DTIM_mode = false,NFECAL = false;
        if (DTIM_mode)
        {
            num2 |= num4;
        }
        if (NFECAL)
        {
            num2 |= num3;
        }
        tlv2AddRspParam("flags", num2);
        tlv2AddRspParam("rateBitIndex0", rate);
        tlv2AddRspParam("rxChain", rxChain);
        tlv2AddRspParam("wifiStandard", 2);
        tlv2AddRspParam("phyId", phyId);
        tlv2AddRspParam("nss", nss);
        tlv2AddRspParam("rateBw", rateBw);
        int noisefloor = 0;
        tlv2AddRspParam("noiseFloor", noisefloor);
        tlv2AddRspParam("ppduType", ppduType);
        tlv2AddRspParam("dcm", dualCarrierModulation ? 1 : 0);
        tlv2AddRspParam("enANI", enANI ? 1 : 0);
        //data = Convert.ToString(ScanProfile);
        tlv2AddRspParam("scanProfile", 0);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        std::string data = tlv2GetRspParam("status");
        uint num5 = atoi(data.c_str());
        if (num5 != 0)
        {
            JGW_FormatString(mstrErrorMsg,"FTM_WLAN_Atheros_Rx() - error code %d",num5);
            return false;
        }
        return true;
    }

    void CCJGW_WlanSetUpDutTxDetails2::GetRxReport(int rxChain, uint* totalPktCount, uint* totalMacPktCount, uint* crcErrPktCount, uint* secErrPktCount, double* rssiDbm)
    {
        *totalPktCount = (*totalMacPktCount = (*crcErrPktCount = (*secErrPktCount = 0u)));
        *rssiDbm = 0.0;
        int num = 0;
        int num2 = 0;

        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)11);
        tlv2AddRspParam("stopRx", "1");
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);

        int aMpdu = 1;
        std::string text = tlv2GetRspParam("totalPkt");
        if (text.size() > 0)
        {
            *totalPktCount = (uint)atoi(text.c_str());
        }
        *totalPktCount = (((int)*totalPktCount > 0) ? (*totalPktCount / aMpdu) : 0u);
        text = tlv2GetRspParam("goodPackets");
        if (text.size() > 0)
        {
            *totalMacPktCount = atoi(text.c_str());
        }
        *totalMacPktCount = (((int)*totalMacPktCount > 0) ? (*totalMacPktCount / aMpdu) : 0u);
        text = tlv2GetRspParam("crcErrPkt");
        if (text.size() > 0)
        {
            *crcErrPktCount = atoi(text.c_str());
        }
        *crcErrPktCount = (((int)*crcErrPktCount > 0) ? (*crcErrPktCount / aMpdu) : 0u);
        text = tlv2GetRspParam("secErrPkt");
        if (text.size() > 0)
        {
            *secErrPktCount = atoi(text.c_str());
        }
        *secErrPktCount = (((int)*secErrPktCount > 0) ? (*secErrPktCount / aMpdu) : 0u);
        text = tlv2GetRspParam("rssi");
        if (text.size() > 0)
        {
            *rssiDbm = atoi(text.c_str());
        }
        /*
        string str = "evm";
        for (int l = 0; l < 8; l++)
        {
        text = tlv2GetRspParam(str + Convert.ToString(l));
        if (text.Length > 0)
        {
        evms[l] = Convert.ToInt32(text);
        }
        }
        string str2 = "pilotevm";
        for (int m = 0; m < 8; m++)
        {
        text = tlv2GetRspParam(str2 + Convert.ToString(m));
        if (text.Length > 0)
        {
        pilotevms[m] = Convert.ToInt32(text);
        }
        }
        string str3 = "rssiPerChain20Mhz";
        for (int n = 0; n < 8; n++)
        {
        text = tlv2GetRspParam(str3 + Convert.ToString(n));
        if (text.Length > 0)
        {
        rssiPerChain20Mhz[n] = Convert.ToInt32(text);
        }
        }
        text = tlv2GetRspParam("bandwidth");
        if (text.Length > 0)
        {
        bandwidth = Convert.ToUInt32(text);
        }
        text = tlv2GetRspParam("chainMask");
        if (text.Length > 0)
        {
        chainMask = Convert.ToUInt32(text);
        }
        text = tlv2GetRspParam("numChain");
        if (text.Length > 0)
        {
        numChain = Convert.ToUInt32(text);
        }
        text = tlv2GetRspParam("num20MHz");
        if (text.Length > 0)
        {
        num20MHz = Convert.ToUInt32(text);
        }
        text = tlv2GetRspParam("endTime");
        if (text.Length > 0)
        {
        num = Convert.ToInt32(text);
        }
        text = tlv2GetRspParam("startTime");
        if (text.Length > 0)
        {
        num -= Convert.ToInt32(text);
        }
        text = tlv2GetRspParam("byteCount");
        if (text.Length > 0)
        {
        num2 = Convert.ToInt32(text);
        }
        text = tlv2GetRspParam("dontCount");
        if (text.Length > 0)
        {
        num2 -= Convert.ToInt32(text);
        }
        if (num > 0 && num2 > 0)
        {
        tPut = (int)((double)(num2 * 8) / (0.001 * (double)num));
        }
        logMessage("FTM_WLAN_Atheros_Rx_Report(totalPkt " + totalPktCount + ", Throughput " + tPut + ", rssi " + Convert.ToInt32(rssiDbm) + ", evm0 " + evms[0] + ", evm1 " + evms[1] + ", evm2 " + evms[2] + ", evm3 " + evms[3] + ", evm4 " + evms[4] + ", evm5 " + evms[5] + ", evm6 " + evms[6] + ", evm7 " + evms[7] + ", crcErrPkt " + crcErrPktCount + ", secErrPktCount " + secErrPktCount + ")");
        */
    }

    bool CCJGW_WlanSetUpDutTxDetails2::SetUpDutTxDetails2ToParam(std::map<std::string, std::string>& mapParam,double* txPower)
    {
        WLAN_UniversalPhyID phyId = (WLAN_UniversalPhyID)atoi(mapParam["phyID"].c_str()); 
        double channel = atoi(mapParam["channel"].c_str());
        double channel2 = atoi(mapParam["channel2"].c_str());
        WLAN_Gen6_ChannelBondingState cbState = (WLAN_Gen6_ChannelBondingState)atoi(mapParam["cbState"].c_str()); 
        WLAN_UniversalDataRate rate = (WLAN_UniversalDataRate)atoi(mapParam["rate"].c_str()); 
        WLAN_UniversalRateBW rateBw  = (WLAN_UniversalRateBW)atoi(mapParam["rateBw"].c_str());  
        WLAN_UniversalNSS nss = (WLAN_UniversalNSS)atoi(mapParam["nss"].c_str());   
        TxPowerMode pwrMode  = (TxPowerMode)atoi(mapParam["pwrMode"].c_str());   
        WLAN_TX_Mode txMode = (WLAN_TX_Mode)atoi(mapParam["txMode"].c_str());   
        WLAN_CHAIN_MASK2 chainMask = (WLAN_CHAIN_MASK2)atoi(mapParam["chainMask"].c_str());   
        WLAN_CHAIN_MASK2 chainMaskH = (WLAN_CHAIN_MASK2)atoi(mapParam["chainMaskH"].c_str());
        double powerLevel = atof(mapParam["powerLevel"].c_str());
        uint gain = atoi(mapParam["gain"].c_str());
        int digitalGain = atoi(mapParam["digitalGain"].c_str());
        uint PAcfg = atoi(mapParam["PAcfg"].c_str()); 
        uint payLoadSize = atoi(mapParam["payLoadSize"].c_str());
        bool ldpcRate = 1 ==  atoi(mapParam["ldpcRate"].c_str());
        bool stbcMode = 1 ==  atoi(mapParam["stbcMode"].c_str());
        uint aggregation = atoi(mapParam["aggregation"].c_str());
        DPDSetting dpdMode = (DPDSetting)atoi(mapParam["dpdMode"].c_str()); 
        HCSetting HC_Enable = (HCSetting)atoi(mapParam["HC_Enable"].c_str()); 
        int ifs = atoi(mapParam["ifs"].c_str());
        uint dutyCycle = atoi(mapParam["dutyCycle"].c_str()); 
        TxDataPattern nPattern =  (TxDataPattern)atoi(mapParam["nPattern"].c_str()); 
        std::vector<unsigned char> pattern;
        std::vector<int> vTemp;
        JGW_ParserIntA(mapParam["pattern"].c_str(),",",vTemp);
        for (size_t index = 0;index < vTemp.size();index++)
        {
            pattern.push_back(static_cast<unsigned char>(vTemp[index]));
        }
        uint nPacket = atoi(mapParam["nPacket"].c_str()); 
        bool broadcastMode =  1 ==  atoi(mapParam["broadcastMode"].c_str());
        uint AIFSN = atoi(mapParam["AIFSN"].c_str()); 
        int dutTxSettlingDelayMs = atoi(mapParam["dutTxSettlingDelayMs"].c_str()); 
        bool scramblerDisable = 1 ==  atoi(mapParam["scramblerDisable"].c_str());
        bool ackRequest = 1 ==  atoi(mapParam["ackRequest"].c_str());
        WLAN_UniversalGuardInterval guardInterval = (WLAN_UniversalGuardInterval)atoi(mapParam["guardInterval"].c_str()); 
        PPDU_TYPE ppduType = (PPDU_TYPE)atoi(mapParam["ppduType"].c_str());
        bool dualCarrierModulation = 1 ==  atoi(mapParam["dualCarrierModulation"].c_str());
        PHY165Mode phy165Mode = (PHY165Mode)atoi(mapParam["phy165Mode"].c_str());
        std::string puncBwMask = mapParam["puncBwMask"];

        return SetUpDutTxDetails2(phyId,channel,channel2,cbState,rate,rateBw,nss,pwrMode,txMode,chainMask,chainMaskH,powerLevel,gain,digitalGain,PAcfg,payLoadSize,ldpcRate,stbcMode,aggregation,dpdMode,HC_Enable,ifs,dutyCycle,nPattern,pattern,nPacket, 
            broadcastMode, 
            AIFSN, 
            dutTxSettlingDelayMs, 
            scramblerDisable, 
            ackRequest, 
            guardInterval, 
            ppduType, 
            dualCarrierModulation, 
            phy165Mode, 
            puncBwMask,  
            txPower);
    }

    void CCJGW_WlanSetUpDutTxDetails2::TurnOffTX2ToParam(std::map<std::string,std::string>& mapParam)
    {
        uint phyId = atoi(mapParam["phyID"].c_str());
        uint channel = atoi(mapParam["channel"].c_str());
        uint txChain = atoi(mapParam["chainMask"].c_str());   
        TurnOffTX2(phyId,channel,txChain);
    }

    bool CCJGW_WlanSetUpDutTxDetails2::TurnOnRX2ToParam(std::map<std::string, std::string>& mapParam)
    {
        WLAN_UniversalPhyID phyId  = (WLAN_UniversalPhyID)atoi(mapParam["phyID"].c_str()); 
        uint channel = atoi(mapParam["channel"].c_str());
        uint channel2 = atoi(mapParam["channel2"].c_str());
        WLAN_Gen6_ChannelBondingState cbState = (WLAN_Gen6_ChannelBondingState)atoi(mapParam["cbState"].c_str());
        WLAN_UniversalDataRate rate = (WLAN_UniversalDataRate)atoi(mapParam["rate"].c_str()); ;
        WLAN_UniversalRateBW rateBw = (WLAN_UniversalRateBW)atoi(mapParam["rateBw"].c_str());
        WLAN_UniversalNSS nss = (WLAN_UniversalNSS)atoi(mapParam["nss"].c_str());  ;
        WLAN_CHAIN_MASK2 chainMask = (WLAN_CHAIN_MASK2)atoi(mapParam["chainMask"].c_str());
        WLAN_CHAIN_MASK2 chainMaskH = (WLAN_CHAIN_MASK2)atoi(mapParam["chainMaskH"].c_str());
        AntennaSetting antenna = (AntennaSetting)atoi(mapParam["antenna"].c_str());
        bool LDPC = 1 ==  atoi(mapParam["LDPC"].c_str());
        bool STBC = 1 ==  atoi(mapParam["STBC"].c_str());
        WLAN_Action rxFilterMode = (WLAN_Action)atoi(mapParam["rxFilterMode"].c_str());
        std::string rxMacAddress = mapParam["rxMacAddress"];
        WLAN_UniversalGuardInterval vsgGuardInterval = (WLAN_UniversalGuardInterval)atoi(mapParam["vsgGuardInterval"].c_str());   
        PPDU_TYPE ppduType = (PPDU_TYPE)atoi(mapParam["ppduType"].c_str()); 
        bool dualCarrierModulation = 1 == (WLAN_Action)atoi(mapParam["dualCarrierModulation"].c_str());
        bool enANI = 1 == (WLAN_Action)atoi(mapParam["enANI"].c_str());
        PHY165Mode phy165Mode = (PHY165Mode)atoi(mapParam["phy165Mode"].c_str());
        AniMode aniMode = (AniMode)atoi(mapParam["aniMode"].c_str());
        int aniDynaLevel = atoi(mapParam["aniDynaLevel"].c_str());
        std::string puncBwMask = mapParam["puncBwMask"];
        return TurnOnRX2(phyId,channel,channel2,cbState,rate,rateBw,nss,chainMask,chainMaskH,antenna,LDPC,STBC,rxFilterMode,rxMacAddress,vsgGuardInterval,ppduType,dualCarrierModulation,enANI,phy165Mode,aniMode,aniDynaLevel,puncBwMask);
    }

    bool CCJGW_WlanSetUpDutTxDetails2::GetRxResultToParam(std::map<std::string, std::string>& mapParam,uint* totalPktCount, uint* totalMacPktCount, uint* crcErrPktCount,  uint* secErrPktCount,  double* rssiDbm)
    {
        GetRxReport(0,totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount,rssiDbm);
        return true;
    }

    void CCJGW_WlanSetUpDutTxDetails2::SetResourceContent(HANDLE hResourceContent)
    {
        mhResourceContent = hResourceContent;
    }
}
