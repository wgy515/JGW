#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin/CJGW_WCNTestIPQ8070Dut.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <algorithm>
#include <functional>
#include <vector>
#include <fstream>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestBoxInterface.h>
namespace JGW
{
    _FREQ_CAP CCJGW_WCNTestIPQ8070Dut::mFreqCap[16];
    CCJGW_WCNTestBoxInterface* gpWCNTestBoxInterface = NULL;
    int CCJGW_WCNTestIPQ8070Dut::m_fc_index = 0;
    uint CCJGW_WCNTestIPQ8070Dut::m_rate = 17;
    uint CCJGW_WCNTestIPQ8070Dut::m_txChainMask = 1;
    WLAN_CAL_SCHEME CCJGW_WCNTestIPQ8070Dut::mCalScheme = CAL_OLPC2P;
    POWER_MEAS_TYPE CCJGW_WCNTestIPQ8070Dut::m_pwrMeasType = POWER_MEAS_AVG;
    DutTxDetailSettings2 txSettings2;


    CCJGW_WCNTestIPQ8070Dut::CCJGW_WCNTestIPQ8070Dut(void) : mbInitialized(false) , m_hResourceContext(NULL)
    {
        for (int i = 0;i < 7;i ++)
        {
            mszRxStationAddr[i] = '\0';
            mszTxStationAddr[i] = '\0';
            mszTxBSSID[i] = '\0';
        }
    }


    CCJGW_WCNTestIPQ8070Dut::~CCJGW_WCNTestIPQ8070Dut(void)
    {
    }

    void CCJGW_WCNTestIPQ8070Dut::SetWCNTestBoxInstance(CCJGW_WCNTestBoxInterface* pWCNTestBoxInterface)
    {
        gpWCNTestBoxInterface = pWCNTestBoxInterface;
    }

    bool CCJGW_WCNTestIPQ8070Dut::ConnectToServerAutoDetect(uint comPort, uint cOMPortAutoDetectTimeout)
    {
        m_hResourceContext = NULL;
        unsigned short piPhysicalPort = 0;

        __try
        {
            if (comPort != QLIB_COM_AUTO_DETECT)
            {
                m_hResourceContext = QLIB_ConnectServer(comPort);
            }
            else
            {
                m_hResourceContext = QLIB_ConnectServerWithWait(comPort, (unsigned long)(cOMPortAutoDetectTimeout * 1000u));
            }
            QLIB_GetComPortNumber(m_hResourceContext, &piPhysicalPort);

            if (NULL != m_hResourceContext)
            {
                return (TRUE == QLIB_IsPhoneConnected(m_hResourceContext));
            }
        }
        __except( EXCEPTION_EXECUTE_HANDLER )
        {
            PELOG4WW(L"ConnectDut Exception");
        }
        return false;
    }
    // 
    //     public void ConnectToServer(int comPortNum, TargetType targetType, DiagType diagType)
    //     {
    //         uint cOMPortAutoDetectTimeout = 60u;
    //         QLIB_SetTargetType((byte)targetType);
    //         QLIB_SetDiagType((byte)diagType);
    //         this.connectToServerAutoDetect((uint)comPortNum, cOMPortAutoDetectTimeout);
    //     }

    // Token: 0x0200000A RID: 10
    //public enum TargetType
    //{
    //    // Token: 0x040000AC RID: 172
    //    QLIB_TARGET_TYPE_MSM_MDM,
    //    // Token: 0x040000AD RID: 173
    //    QLIB_TARGET_TYPE_APQ,
    //    // Token: 0x040000AE RID: 174
    //    QLIB_TARGET_TYPE_MAX_INVALID
    //}

    bool CCJGW_WCNTestIPQ8070Dut::ConnectDut(QLIB_TargetType_Enum type /* = QLIB_TARGET_TYPE_APQ */,int WlanID /* = 8070 */,const std::string& strComPort /* = "COM30001" */, bool useQPST /* = true */)
    {
        unsigned int cOMPortAutoDetectTimeout = 60u;
        std::string strComPortTemp = strComPort;


        JGW_MakeStringToUpper(strComPortTemp);
        if (!mbInitialized)
        {
            QLIB_SetLibraryMode(useQPST ? eQPSTMode : eQPhoneMS);
            QLIB_SetTargetType(type);
            QLIB_SetDiagType(QLIB_DIAG_TYPE_STANDARD);
            mbInitialized = true;
        }

        //! mDeviceApi.DisconnectServer();

        int result = 0;
        if (0 == strComPortTemp.compare("AUTO"))
        {
            result = 65535;
        }
        else
        {
            JGW_ReplaceStringA(strComPortTemp,"COM","");
            result = static_cast<unsigned short>(atoi(strComPortTemp.c_str()));
        }

        jgw_timer jt;
        while (jt.elapsed() < 30)
        {
            if (ConnectToServerAutoDetect(result,cOMPortAutoDetectTimeout))
            {
                return true;
            }
            Sleep(1000);
        }

        return false;
        //if (!mbInitialized)
        //{
        //    QLIB_SetLibraryMode(useQPST ? eQPSTMode : eQPhoneMS);
        //    ///QLIB_DIAG_TYPE_STANDARD,
        //    // QLIB_DIAG_TYPE_LITE,
        //    // QLIB_DIAG_TYPE_MAX_INVALID
        //    QLIB_SetTargetType(type);
        //    QLIB_SetDiagType(QLIB_DIAG_FULL);
        //    mbInitialized = true;
        //}

        //JGW_MakeStringToUpper(strComPortTemp);
        //JGW_ReplaceStringA(strComPortTemp,"COM","");
        //unsigned short comPort = static_cast<unsigned short>(atoi(strComPortTemp.c_str()));




        ////Phone phoneInstance = PhoneInstance;
        //QLIB_FTM_WLAN_GEN6_START(m_hResourceContext,WlanID);
        //logMessage("FTM_WLAN_GEN6_START(" + WlanID + ")");


        //mDeviceApi.DisconnectServer();
        ////! 8070
        //if (comPort == QLIB_COM_AUTO_DETECT)
        //{
        //    mDeviceApi.SetResourceContex(QLIB_ConnectServerWithWait(QLIB_COM_AUTO_DETECT,20 * 1000));
        //    return m_hResourceContext != NULL;
        //}

        //return mDeviceApi.ConnectPhone(cOMPortAutoDetectTimeout,comPort > 0 ?comPort:QLIB_COM_AUTO_DETECT);
    }

    bool CCJGW_WCNTestIPQ8070Dut::DutLog(bool logOn, const std::string& fileName /* = "C:logdut_log.txt" */)
    {
        if (logOn)
        {
            //mDeviceApi.StartLogging(fileName.c_str());
            QLIB_SetLogFlags( m_hResourceContext,LOG_DEFAULT );
            QLIB_StartLogging( m_hResourceContext, const_cast<char*>(fileName.c_str()) );
        }
        else
        {
            //mDeviceApi.StopLogging();
            QLIB_StopLogging( m_hResourceContext );
        }
        return true;
    }

    void QmslMsgCB(unsigned int type,char *strMsg)
    {
        std::wstring msg(JGW_A2W_A(strMsg));
        switch (type)
        {
        case 0:
            //QTALogMgr.Error(strMsg);
            PELOG4WW(msg.c_str());
            break;
        case 1:
            //logMessage(strMsg);
            PDLOG4WW(msg.c_str());
            break;
        default:
            PDLOG4WW(msg.c_str());
            //DebugMessage.Write("Debug", strMsg);
            break;
        }
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanLoadDut(const std::string& eepFName, int ssid /* = "0x40" */, const std::string& DevDLLName /* = "ipq8070" */, const std::string& refDesign /* = "ipq8070" */, NVMEM_OPTION iNVMemOption /* = DataFile */)
    {
#ifdef _DEBUG
        //asyncQmslMessageCB qmslMsgCB = QmslMsgCB;
        QLIB_FTM_WLAN_Atheros_Qmsl_Msg_Callback(m_hResourceContext,QmslMsgCB);
#endif
        return (TRUE == QLIB_FTM_WLAN_SetModuleType(m_hResourceContext,FTM_WLAN_ModuleType_Atheros) && TRUE == QLIB_FTM_WLAN_Atheros_LoadDUT(m_hResourceContext,(unsigned char*)(DevDLLName.c_str()),(unsigned char*)(eepFName.c_str()),iNVMemOption,ssid));;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanSetInstance(WLAN_INSTANCE instance /* = WLAN0 */)
    {
        return (TRUE == QLIB_FTM_WLAN_SetInstance(m_hResourceContext,instance));
    }

    void CopyMacToCharArray(char* byteAddr,int start,const std::string& macAddr)
    {
        std::string temp = "",data = macAddr;
        JGW_ReplaceStringA(data,":","");
        JGW_ReplaceStringA(data,".","");

        if (12 == data.length())
        {
            for (int i = 0;i < 6;i++)
            {
                temp = data.substr(i * 2, 2);
                byteAddr[start + i] = stoi(temp, nullptr, 16);
            }
        }
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanSetDutTxMacAddressBssId(const std::string& rxStation /* = "00.03.7F.44.55.68" */,const std::string& txStation /* = "00.03.7F.44.55.69" */,const std::string& bssId /* = "00.03.7F.44.55.70" */,unsigned int deviceIndex /* = 0 */)
    {
        CopyMacToCharArray(mszRxStationAddr,0,rxStation);
        CopyMacToCharArray(mszTxStationAddr,0,txStation);
        CopyMacToCharArray(mszTxBSSID,0,bssId);
        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanSetLocalMac(const std::string& macAddr1 /* = "000000000000" */, const std::string& macAddr2 /* = "000000000000" */, const std::string& macAddr3 /* = "000000000000" */, const std::string& macAddr4 /* = "000000000000" */, const std::string& macAddr5 /* = "000000000000" */, const std::string& macAddr6 /* = "000000000000" */, const std::string& macAddr7 /* = "000000000000" */, const std::string& macAddr8 /* = "000000000000" */, const std::string& macAddr9 /* = "000000000000" */)
    {
        bool result = true;
        char byteAddr[55] = {0};
        char array[1025] = {0};

        CopyMacToCharArray(byteAddr,0,macAddr1);
        CopyMacToCharArray(byteAddr,6,macAddr2);
        CopyMacToCharArray(byteAddr,12,macAddr3);
        CopyMacToCharArray(byteAddr,18,macAddr4);
        CopyMacToCharArray(byteAddr,24,macAddr5);
        CopyMacToCharArray(byteAddr,30,macAddr6);
        CopyMacToCharArray(byteAddr,36,macAddr7);
        CopyMacToCharArray(byteAddr,42,macAddr8);
        CopyMacToCharArray(byteAddr,48,macAddr9);

        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,_OP2_SETLOCALMAC));
        //result &= (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(m_hResourceContext,"localMac", byteAddr));
        result &= tlv2AddRspParam("localMac", byteAddr);
        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext));
        //result &= (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(m_hResourceContext,"status",array));
        tlv2GetRspParam("status");

        return result;
    }

    bool CCJGW_WCNTestIPQ8070Dut::SetUpDutTxDetails2(double* txPower, WLAN_UniversalPhyID phyId /* = PHY_A0 */, double channel /* = 5500 */, double channel2 /* = 0 */, WLAN_Gen6_ChannelBondingState cbState /* = primary20 */, WLAN_UniversalDataRate rate /* = RATE_MCS_0 */, WLAN_UniversalRateBW rateBw /* = RateBW_11AC_VHT20 */, WLAN_UniversalNSS nss /* = _1Stream */, TxPowerMode pwrMode /* = TxPowerForce_CLPC */, WLAN_TX_Mode txMode /* = ContTx99 */, WLAN_CHAIN_MASK2 chainMask /* = WLAN_CHAIN_1 */, WLAN_CHAIN_MASK2 chainMaskH /* = WLAN_CHAIN_NONE */, double powerLevel /* = 0 */, unsigned int gain /* = 0 */, int digitalGain /* = 0 */, unsigned int PAcfg /* = 5 */, unsigned int payLoadSize /* = 1500 */, bool ldpcRate /* = false */, bool stbcMode /* = false */, unsigned int aggregation /* = 1 */, DPDSetting dpdMode /* = DPD_Disabled */, HCSetting HC_Enable /* = HCS_Disabled */, int ifs /* = 1 */, unsigned int dutyCycle /* = 10 */, TxDataPattern nPattern /* = ZEROES_PATTERN */, char* pattern /* = "" */, unsigned int nPacket /* = 0 */, bool broadcastMode /* = false */, unsigned int AIFSN /* = 0 */, int dutTxSettlingDelayMs /* = 500 */, bool scramblerDisable /* = false */, bool ackRequest /* = false */, WLAN_UniversalGuardInterval guardInterval /* = LTF_Mode0_GI_800 */, PPDU_TYPE ppduType /* = SingleUser */, bool dualCarrierModulation /* = false */)
    {
        std::string temp;
        bool result = true;
        unsigned int wifiStandard = 2;
        unsigned int chainMask2 = (unsigned int)((int)chainMask | ((int)chainMaskH << 4));

        SetUpTxDetail2(wifiStandard, (uint)phyId, channel, channel2, cbState, rate, rateBw, nss, pwrMode, txMode, chainMask2, powerLevel, gain, digitalGain, PAcfg, payLoadSize, ldpcRate, stbcMode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, nPacket, broadcastMode, AIFSN, scramblerDisable, ackRequest, guardInterval, (uint)ppduType, dualCarrierModulation, txPower);

        return result;
    }

    bool CCJGW_WCNTestIPQ8070Dut::GetTxReport2(WLAN_UniversalPhyID phyId, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, uint& TxRGdPkt, uint& TxRput, uint& TxRTemp0, uint& TxRTemp1, double& rssi, uint& TxRGainIdx, uint& TxRDacGain, uint& TxRPAcfg, uint& TxRpdadc)
    {
        uint TxRTemp2 = 0;
        std::string data;
        TxRGdPkt = (TxRTemp0 = (TxRTemp1 = (TxRput = (TxRGainIdx = (TxRDacGain = (TxRPAcfg = (TxRpdadc = 0)))))));
        rssi = 0.0;
        uint txChain = (uint)((int)chainMask | ((int)chainMaskH << 4));

        //!dut.SetPhyId(phyId);
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,_OP2_SYNC);
        //data = Convert.ToString((int)newPhyId);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyId", data);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);

        GetTxReport((int)txChain, TxRGdPkt, TxRTemp2, TxRput, rssi, TxRGainIdx, TxRDacGain, TxRPAcfg, TxRpdadc);

        TxRTemp0 = (TxRTemp2 & 0xFF);
        TxRTemp1 = ((TxRTemp2 >> 8) & 0xFF);

        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanXtalCalInit2(WLAN_UniversalPhyID phyId /* = PHY_A0 */, int dXtalTargetPPM /* = 0 */, uint dXtalTolerancePPM /* = 1 */, WLAN_XTAL_STORAGE_OTP dXtalCalOtpSaveOption /* = XTAL_OTP_DISABLE */, uint settleTime /* = 1000 */, uint delayTime /* = 2000 */)
    {
        bool result = true;
        int xtalCapDelta = 0;

        mXtalTolerancePPM = dXtalTolerancePPM;
        mXtalTargetPPM = dXtalTargetPPM;
        SetPhyId(phyId);
        result &= (TRUE == QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(m_hResourceContext,dXtalTargetPPM, dXtalTolerancePPM, xtalCapDelta, dXtalCalOtpSaveOption, settleTime, delayTime));
        //XtalCalInit2(uint phyId, int XtalTargetPPM, uint XtalTolerancePPM, int XtalCapDelta, WLAN_XTAL_STORAGE_OTP dXtalCalSaveOption, uint settleTime, uint delayTime)
        Xtal_Cal_Pre();
        //XtalCalInit2((uint)phyId, dXtalTargetPPM, dXtalTolerancePPM, xtalCapDelta, dXtalCalOtpSaveOption, settleTime, delayTime);
        return result;
    }

    bool Xtal_Cal_FC(uint cVal, double freqErr)
    {
        if (CCJGW_WCNTestIPQ8070Dut::m_fc_index >= 16)
        {
            return false;
        }
        CCJGW_WCNTestIPQ8070Dut::mFreqCap[CCJGW_WCNTestIPQ8070Dut::m_fc_index].m_capVal = cVal;
        CCJGW_WCNTestIPQ8070Dut::mFreqCap [CCJGW_WCNTestIPQ8070Dut::m_fc_index].m_freqErr = freqErr;
        CCJGW_WCNTestIPQ8070Dut::m_fc_index++;
        return true;
    }

    //FreqErr:-2.16   ChannelFreq: 5500    txchainMask: 1    capOutValue:176    capInValue:560
    //FreqErr:39.01   ChannelFreq: 5500    txchainMask: 1    capOutValue:0    capInValue:384
    //FreqErr:7.13   ChannelFreq: 5500    txchainMask: 1    capOutValue:128    capInValue:512
    //FreqErr:-5.32   ChannelFreq: 5500    txchainMask: 1    capOutValue:192    capInValue:576
    //FreqErr:0.72   ChannelFreq: 5500    txchainMask: 1    capOutValue:160    capInValue:544

    double gTestFreqErrr[5] = {-2.16,39.01,7.13,-5.32,0.72};
    int gIndex = 0;
    double asyncFreqErrCB2(uint capInValue, uint capOutValue, uint freq, double powerLevel, uint avgNum)
    {
        double* array = new double[avgNum];
        double num = 0.0;
        int num2 = 0;
        for (uint i = 0; i < avgNum; i++)
        {
#if 0
            array[i] = gpWCNTestBoxInterface->getCWFreqErr(freq, powerLevel) / (double)freq;
#else
            array[i] = gTestFreqErrr[gIndex];
#endif
            if (array[i] > -999.0)
            {
                num += array[i];
                num2++;
            }
        }
        gIndex ++;
        if (5 == gIndex)
        {
            gIndex = 0;
        }
        num = ((num2 <= 0) ? (-999.0) : (num / (double)num2));
        //DebugMessage.Write("Debug", "FreqErr:" + $"{num:0.00}" + "   ChannelFreq: " + freq + "    txchainMask: " + m_txChainMask + "    capOutValue:" + capOutValue + "    capInValue:" + capInValue);
        PDLOG4WW_F(L"avgNum: %d,FreqErr:%.2f   ChannelFreq: %d    txchainMask: %d    capOutValue:%d    capInValue:%d",avgNum,num,freq,CCJGW_WCNTestIPQ8070Dut::m_txChainMask,capOutValue,capInValue);
        Xtal_Cal_FC(capOutValue, num);
        delete[] array;
        return num;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanXtalCalibration2(uint& xtalCapIn, uint& xtalCapOut, double& freqErrPPM,WLAN_UniversalPhyID phyId /* = PHY_A0 */, uint channel /* = 5500 */, WLAN_SINGLE_CHAIN_MASK txChainMask /* = WLAN_CHAIN_1 */, uint gainIndex /* = 28 */, int dacGain /* = 0 */, int paCfg /* = 5 */, uint numAverages /* = 3 */,bool ppmCurveDetection /* = false */)
    {
        bool flag = true;
        std::string data;
        uint value = channel;

        xtalCapIn = (xtalCapOut = 0u);
        freqErrPPM = -999.0;
        gIndex = 0;

        SetPhyId(phyId);
        //flag = XtalCalibration2(xtalCapIn, xtalCapOut, freqErrPPM,channel, txChainMask, gainIndex, dacGain, paCfg, numAverages,  ppmCurveDetection);

        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }
        m_txChainMask = (uint)txChainMask;
        QLIB_FTM_WLAN_TLV_Create(m_hResourceContext,3);
        //string data = Convert.ToString(value);
        JGW_FormatString(data,"%d",value);
        tlvAddRspParam("channel", data);
        WLAN_TX_Mode value2 = EnablingContinuousModulatedTX;
        //data = Convert.ToString((uint)value2);
        JGW_FormatString(data,"%d",value2);
        tlvAddRspParam("txMode", data);
        TCMD_WLAN_MODE value3 = TCMD_WLAN_MODE_NOHT;
        //data = Convert.ToString((uint)value3);
        JGW_FormatString(data,"%d",value3);
        tlvAddRspParam("wlanMode", data);
        //data = Convert.ToString((uint)txChainMask);
        JGW_FormatString(data,"%d",txChainMask);
        tlvAddRspParam("txChain0", data);
        //data = Convert.ToString(4u);
        tlvAddRspParam("tpcm", "4");
        JGW_FormatString(data,"%d",gainIndex);
        tlvAddRspParam("gainIdx", data.c_str());
        JGW_FormatString(data,"%d",dacGain);
        tlvAddRspParam("dacGain", data.c_str());
        JGW_FormatString(data,"%d",paCfg);
        tlvAddRspParam("paConfig", data.c_str());

        QLIB_FTM_WLAN_TLV_Complete(m_hResourceContext);
        ///*flag &= (TRUE == */QLIB_FTM_WLAN_Atheros_xtal_CAL2(m_hResourceContext,asyncFreqErrCB2, numAverages, ppmCurveDetection)/*)*/;
        data = tlvGetRspParam("XTAL_CAPIN_VALUE");
        xtalCapIn = atoi(data.c_str());
        data = tlvGetRspParam("XTAL_CAPOUT_VALUE");
        xtalCapOut = atoi(data.c_str());
        data = tlvGetRspParam("XTAL_FREQERRPPM");
        freqErrPPM = atoi(data.c_str());

        if (ppmCurveDetection)
        {
            flag &= Xtal_Cal_Post(true);
        }

        if (freqErrPPM < (double)(mXtalTargetPPM - mXtalTolerancePPM) || freqErrPPM > (double)(mXtalTargetPPM + mXtalTolerancePPM))
        {
            flag = false;
        }

        if (!flag)
        {
            QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,10);
            tlv2AddRspParam("stopTx", "1");
            tlv2AddRspParam("needReport", "0");
            QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        }

        return flag;
    }

    bool SetChainMessageCBFunc(int chainMask)
    {
        return true;
    }

    bool RFSMessageCBFunc(int chainMask)
    {
        /*setRFSwitch(chainMask);
        m_txChainMask = Convert.ToUInt32(chainMask);*/
        return true;
    }
// 
//     void mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2 = 0u)
//     {
//        
//             //default:
//             //throw new WLANCustomException("Phone WLAN Rate does not exist");
//         }
//     }

    bool PMMessageCB2Func(unsigned int gain, unsigned int freq, unsigned int chainMask,double *powerLevel,unsigned int numAvg,unsigned int iTriggerType,int iTriggerLevel,double *oPower)
    {
        double array[8] = {0.00};
        double oPower2[8] = {0.00};
        //double array2[8] = {0.00};
        std::vector<double> vPowerLevelDbm;

        //!CallRfSwitchIfAvailble(qcmbrPort, freq, m_txChainMask);
        //!Marshal.Copy(powerLevel1, array2, 0, array2.Length);
        for (int i = 0;i < 8;i ++)
        {
            //array2[i] = powerLevel[i];
            if (powerLevel[i] == -999.0)
            {
                //array2[i] = powerLevel[0];
                vPowerLevelDbm.push_back(powerLevel[0]);
            }
            else
            {
                vPowerLevelDbm.push_back(powerLevel[i]);
            }
        }

        if (CCJGW_WCNTestIPQ8070Dut::mCalScheme == CAL_SCPC_PSATADJUST)
        {
            switch (gain)
            {
            case 0u:
                CCJGW_WCNTestIPQ8070Dut::m_pwrMeasType = POWER_MEAS_PEAK;
                break;
            case 1u:
                CCJGW_WCNTestIPQ8070Dut::m_pwrMeasType = POWER_MEAS_AVG;
                break;
            }
        }
        else
        {
            CCJGW_WCNTestIPQ8070Dut::m_pwrMeasType = POWER_MEAS_AVG;
        }
        //getPower2(freq, chainMask, (WLAN_DataRate)m_rate, array2, numAvg, out oPower2);
        //! getPower2
        {
            WLAN_RATE rate2;
            WLAN_Gen6_PHYDBG_PreambleRate preabmbleRate;
            double powerAvgEachBurst[8] = {0};
            double powerPkEachBurst[8] = {0};
            double pathLossDb[8] = {0};
            //double vsaTriggerLevelOffsetDbr[1] = {0};
            std::vector<double> vsaTriggerLevelOffsetDbr;

            gpWCNTestBoxInterface->mapWlanRate((WLAN_DataRate)CCJGW_WCNTestIPQ8070Dut::m_rate,false,rate2,preabmbleRate);
            //vsaTriggerLevelOffsetDbr[0] = -25.0;
            vsaTriggerLevelOffsetDbr.push_back(-25.0);
            //gpWCNTestBoxInterface->GetPower2(freq,chainMask,(WLAN_DataRate)CCJGW_WCNTestIPQ8070Dut::m_rate,array2,numAvg,oPower2);
            gpWCNTestBoxInterface->MeasurePower(rate2,freq,vPowerLevelDbm,(ChainMask)chainMask,Edge,pathLossDb,vsaTriggerLevelOffsetDbr,false,powerAvgEachBurst,powerPkEachBurst,numAvg);

            if (CCJGW_WCNTestIPQ8070Dut::m_pwrMeasType == POWER_MEAS_PEAK)
            {
                for (int i = 0;i < 8;i ++)
                {
                    oPower2[i] = powerPkEachBurst[i];
                }
            }
            else
            {
                for (int i = 0;i < 8;i ++)
                {
                    oPower2[i] = powerAvgEachBurst[i];
                }
            }
        }

        int num = 0;
        int num2 = 1;
        while (num < 8)
        {
            array[num] = -999.0;
            if ((num2 & chainMask) != 0)
            {
                array[num] = oPower2[num];
                //DebugMessage.Write("Debug", "Goal: " + $"{array2[num]:0.00}" + "    MeasPwr:" + $"{array[num]:0.00}" + "   gain: " + gain + "   lFreq: " + freq + "    chainMask: " + (chainMask & num2));
                PDLOG4WW_F(L"Goal: %.2f    MeasPwr:%.2f   gain: %d   lFreq: %d    chainMask: %d",vPowerLevelDbm[num],array[num],gain,freq,chainMask & num2);
            }
            num++;
            num2 <<= 1;
        }
        for (int i = 0;i < 8;i ++)
        {
            oPower[i] = array[i];
        }
        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId /* = PHY_A0 */, WLAN_TpcCalScheme calScheme /* = ALL_POINTS */, uint totalNumOfChains /* = 4 */, uint multiChainMask /* = 1 */, uint numAverages /* = 1 */, bool VsaOnePort /* = false */, double rangeDb1ptCalUL /* = 3.0 */, double rangeDb1ptCalLL /* = -3.0 */)
    {
        //TxCalibrationFWControlled2(phyId, (uint)calScheme, rangeDb1ptCalUL, rangeDb1ptCalLL, totalNumOfChains, multiChainMask, numAverages);
        m_rate = 33u;
        return (TRUE == QLIB_FTM_WLAN_Atheros_Tx_FW_CAL2(m_hResourceContext,(uint)phyId, calScheme, rangeDb1ptCalUL, rangeDb1ptCalLL, totalNumOfChains, multiChainMask,0, numAverages, SetChainMessageCBFunc,PMMessageCB2Func, RFSMessageCBFunc));
    }

    int asyncSGCB(int inputSignalStrength_dBm,unsigned int freq,unsigned int rate,unsigned int numPackets,unsigned int chain)
    {
        int chainMask = 1 << (int)chain;
        //CallRfSwitchIfAvailble(qcmbrPort, freq, (uint)chainMask);
        return gpWCNTestBoxInterface->sigGenTransmit(inputSignalStrength_dBm, freq, (WLAN_DataRate)rate, numPackets, chain);
    }

    bool asyncRxGainCalDoneCB(uint status)
    {
        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, WLAN_SYS_BAND band /* = WLAN_SYS_5G */, WLAN_Action PromiscuousMode /* = PromiscuousMode */, const std::string& rxMacAddress /* = "010000C0FFEE" */, const std::string& bssId /* = "020000C0FFEE" */)
    {
        unsigned char rx_mac_addr[7] = {0};
        unsigned char bssid[7] = {0};
        //QLIB_FTM_WLAN_Atheros_Rx_FW_CAL2
        CopyMacToCharArray((char*)rx_mac_addr,0,rxMacAddress);
        CopyMacToCharArray((char*)bssid,0,bssId);

        /*Phone phoneInstance = PhoneInstance;
        asyncHandlerSigGenCallBack asyncHandlerSigGenCallBack = null;
        asyncHandlerRxGainCalDoneCallBack asyncHandlerRxGainCalDoneCallBack = null;*/
        //StoreDutRxMacAddressBssid(rx_mac_addr, bssid);
        //WlanTestSharedInfo.PathNameIndex = WlanTestSharedInfo.GetSISOChainNumber(WLAN_CHAIN_MASK.WLAN_CHAIN_1) - 1;
        //asyncHandlerSigGenCallBack = base.asyncSGCB;
        //asyncHandlerRxGainCalDoneCallBack = base.asyncRxGainCalDoneCB;
        return (TRUE == QLIB_FTM_WLAN_Atheros_Rx_FW_CAL2(m_hResourceContext,asyncSGCB, asyncRxGainCalDoneCB, phyId, chainToCalibrate, band, (uint)PromiscuousMode, rx_mac_addr, bssid));
    }

    /*void NfGetMasks(double& mask, double& divisor)
    {
    mask = 1024.0;
    divisor = 2.0;
    }*/

    bool CCJGW_WCNTestIPQ8070Dut::WlanNoiseFloorCal2(double NFCAL_Values[8],WLAN_UniversalPhyID PhyId /* = PHY_A0 */, WLAN_CHAIN_MASK2 chainMask /* = WLAN_CHAIN_1234 */, WLAN_CHAIN_MASK2 chainMaskH /* = WLAN_CHAIN_NONE */, unsigned short channel /* = 5745 */,SYNTH_MODE Mode /* = Regular */, const std::string& pathName /* = "c:Log" */, const std::string& fileName /* = "NF.txt" */)
    {
        uint chain = (uint)((int)chainMask | ((int)chainMaskH << 4));
        uint nfcal_values[8] = {0};
        double mask = 1024.0;
        double divisor = 2.0;
        //! NfGetMasks(mask,divisor);
        NfCal2(PhyId, chain, channel, nfcal_values);
        for (int i = 0; i < 8; i++)
        {
            if (nfcal_values[i] >= 924)
            {
                nfcal_values[i] = 0u;
            }
            else if (nfcal_values[i] != 0)
            {
                NFCAL_Values[i] = ((double)nfcal_values[i] - mask) / divisor;
            }
            else
            {
                NFCAL_Values[i] = -999.0;
            }
            PDLOG4WW_F(L"freq:%d,chainMask:%d,NFCAL_Values[%d]:%.2f",channel,chain,i,NFCAL_Values[i]);
        }

        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanNoiseFloorCalGenBDF(const std::string& strChannel /* = "5180 ,5320,5500,5745"*/,WLAN_UniversalPhyID PhyId/* = PHY_A0 */, WLAN_CHAIN_MASK2 chainMask /* = WLAN_CHAIN_1234 */, WLAN_CHAIN_MASK2 chainMaskH /* = WLAN_CHAIN_NONE */, bool updateNfToBdf /* = false */)
    {
        std::vector<int> vChannel;
        JGW_ParserIntA(strChannel.c_str(),",",vChannel);
        uint chain = (uint)((int)chainMask | ((int)chainMaskH << 4));
        double mask = 1024.0;
        double divisor = 2.0;

        size_t num = 0;
        size_t num2 = 0;
        uint nfcal_values[8] = {0};
        unsigned short array[8][8] = {0};
        short array2[8][8] = {0};
        double array3[8][8] = {0};
        //short[,] array2 = new short[vChannel.size(), 8];
        //double[,] array3 = new double[vChannel.size(), 8];
        for (num = 0; num < vChannel.size(); num++)
        {
            for (num2 = 0; num2 < 8; num2++)
            {
                array3[num][num2] = (array2[num][num2] = -999);
                array[num][num2] = (unsigned short)0;
            }
        }

        for (num = 0; num < vChannel.size(); num++)
        {
            if (vChannel[num] != 0)
            {
                //dut.NfGetMasks(out mask, out divisor);
                NfCal2(PhyId, chain, vChannel[num], nfcal_values);
                for (num2 = 0; num2 < 8; num2++)
                {
                    if (nfcal_values[num2] >= 924)
                    {
                        array2[num][num2] = 0;
                        array3[num][num2] = 0.0;
                        PELOG4WW_F(L"freq:%dChainIdx:%dNFdB2:%dNFdB:%dNFReg%d",vChannel[num],num2,array2[num][num2],array3[num][num2],array[num][num2]);
                        //QTALogMgr.Debug("freq:" + channel[num] + "ChainIdx:" + num2 + "NFdB2:" + array2[num, num2] + "NFdB:" + array3[num, num2] + "NFReg" + array[num, num2]);
                    }
                    else if (nfcal_values[num2] != 0)
                    {
                        array2[num][num2] = (short)((double)nfcal_values[num2] - mask);
                        array3[num][num2] = (double)array2[num][num2] / divisor;
                        array[num][num2] = (unsigned short)nfcal_values[num2];
                        //QTALogMgr.Debug("freq:" + channel[num] + "ChainIdx:" + num2 + "NFdB2:" + array2[num, num2] + "NFdB:" + array3[num, num2] + "NFReg" + array[num, num2]);
                        PELOG4WW_F(L"freq:%dChainIdx:%dNFdB2:%dNFdB:%dNFReg%d",vChannel[num],num2,array2[num][num2],array3[num][num2],array[num][num2]);
                    }
                }
            }
        }
        //if (updateNfToBdf)
        //{
        //    //NfCalSave(PhyId, channel, array);
        //}

        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::BoardDataGetSize(std::string& BoardDataSize)
    {
        BoardDataSize = "";
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,_OP2_BDGETSIZE);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        BoardDataSize = tlv2GetRspParam("bdSize");
        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::BoardDataRead(uint boardDataSize,const std::string& filePath)
    {
        uint num = 0u;
        uint num2 = 4096u;
        char data[4096] = {0};
        uint num3 = boardDataSize;

        JGW_CreateDirectoryA(filePath.c_str(),true);
        std::ofstream fileStream;

        fileStream.imbue( std::locale("") );
        fileStream.open(filePath.c_str(),std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
        if (!fileStream.is_open())
        {
            PELOG4WW_F(L"open %s file fail!!!",JGW_A2W(filePath).c_str());
            return false;
        }

        while (num + num2 <= num3)
        {
            bdRead(num3, num, num2, data);
            //fileStream.Write(data, 0, (int)num2);
            fileStream.write(data,num2);
            num += 4096;
            if (num > num3)
            {
                break;
            }
            if (num + num2 > num3)
            {
                num2 = num3 - num;
            }
        }
        fileStream.flush();
        fileStream.close();

        return true;
    }

    bool CCJGW_WCNTestIPQ8070Dut::WlanSaveBDF2(BDF_OPTION saveOption /* = BDF_DataFlash */, const std::string& bdfFileName /* = "bdf.bin" */)
    {
        return (TRUE == QLIB_FTM_WLAN_Atheros_Save_BDF(m_hResourceContext,saveOption,(char*)bdfFileName.c_str()));
    }

    bool CCJGW_WCNTestIPQ8070Dut::DisconnectWlanTester()
    {
        QLIB_DisconnectAllServers();
        return true;
    }

    void CCJGW_WCNTestIPQ8070Dut::GetTxReport(int txChain, uint& TxRGdPkt, uint& TxRTemp, uint& TxRput, double& rssiDbm, uint& TxRGainIdx, uint& TxRDacGain, uint& TxRPAcfg, uint& TxRpdadc)
    {
        TxRGdPkt = (TxRTemp = (TxRput = (TxRGainIdx = (TxRDacGain = (TxRPAcfg = (TxRpdadc = 0u))))));
        rssiDbm = 0.0;

        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,10);
        tlv2AddRspParam("stopTx", "1");
        tlv2AddRspParam("needReport", "1");
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        std::string text = tlv2GetRspParam("goodPackets");
        if (text.length() > 0)
        {
            TxRGdPkt = atoi(text.c_str());
        }
        text = tlv2GetRspParam("thermCal");
        if (text.length() > 0)
        {
            TxRTemp = atoi(text.c_str());
        }
        text = tlv2GetRspParam("TxThrPut");
        if (text.length() > 0)
        {
            TxRput = atoi(text.c_str());
        }
        text = tlv2GetRspParam("rssi");
        if (text.length() > 0)
        {
            rssiDbm = atoi(text.c_str());
        }
        text = tlv2GetRspParam("gainIdx");
        if (text.length() > 0)
        {
            TxRGainIdx = atoi(text.c_str());
        }
        text = tlv2GetRspParam("dacGain");
        if (text.length() > 0)
        {
            TxRDacGain = atoi(text.c_str());
        }
        text = tlv2GetRspParam("paCfg");
        if (text.length() > 0)
        {
            TxRPAcfg = atoi(text.c_str());
        }
        text = tlv2GetRspParam("pdadc");
        if (text.length() > 0)
        {
            TxRpdadc = atoi(text.c_str());
        }
        //! FTM_WLAN_Atheros_Tx_Report(goodPackets 1890, thermCal chain0 0, thermCal chain1 0, Throughput 0, rssi 0, gainIdx 0, dacGain 0, paCfg 0, pdadc 0)
        //logMessage("FTM_WLAN_Atheros_Tx_Report(goodPackets " + TxRGdPkt + ", thermCal chain0 " + (TxRTemp & 0xFF) + ", thermCal chain1 " + ((TxRTemp >> 8) & 0xFF) + ", Throughput " + TxRput + ", rssi " + rssiDbm + ", gainIdx " + TxRGainIdx + ", dacGain " + TxRDacGain + ", paCfg " + TxRPAcfg + ", pdadc " + TxRpdadc + ")");
        PDLOG4WW_F(L"FTM_WLAN_Atheros_Tx_Report(goodPackets %d, thermCal chain0 %d, thermCal chain1 %d, Throughput %d, rssi %d, gainIdx %d, dacGain %d, paCfg %d, pdadc %d)",TxRGdPkt,(TxRTemp & 0xFF),((TxRTemp >> 8) & 0xFF),TxRput,rssiDbm,TxRGainIdx,TxRDacGain,TxRPAcfg,TxRpdadc);
    }

    void CCJGW_WCNTestIPQ8070Dut::SetUpTxDetail2(uint wifiStandard, uint phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint chainMask, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, uint nPacket, bool broadcastMode, uint AIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, uint ppduType, bool dualCarrierModulation, double* txPower)
    {
        std::string data;
        bool result = true;
        setUpTxDetailCommon2(wifiStandard, phyId, (uint)channel, (uint)channel2, cbState, rate, rateBw, (uint)nss, pwrMode, txMode, chainMask, powerLevel, gain, digitalGain, PAcfg, payLoadSize, /*short11b_nGuard:*/ false, ldpcRate, stbcMode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, AIFSN, scramblerDisable, ackRequest, guardInterval);

        JGW_FormatString(data,"%d",ppduType);
        result = tlv2AddRspParam("ppduType",data);
        result = tlv2AddRspParam("dcm",dualCarrierModulation ? "1" : "0");
        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext));

        if (pwrMode == TxPowerAuto)
        {
            uint tgtPwr = 0u;
            tgtPwr = GetTgtPwr2((uint)channel, (uint)rate, (uint)rateBw, (WLAN_UniversalPhyID)phyId);
            if (tgtPwr != 0)
            {
                txPower[0] = (double)tgtPwr / 4.0f;
            }
            else
            {
                txPower[0] = 0.0f;
            }
        }
    }

    uint CCJGW_WCNTestIPQ8070Dut::GetTgtPwr2(uint freq, uint rate, uint rateBw, WLAN_UniversalPhyID phyId)
    {
        uint tgtPwr = 0;
        std::string data;
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,_OP2_XTALCALPROCRSP);
        //phoneInstance.FTM_WLAN_TLV2_Create(132);
        //string data = Convert.ToString((int)phyId);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyId", data);
        //data = Convert.ToString(freq);
        JGW_FormatString(data,"%d",freq);
        tlv2AddRspParam("freq", data);
        //data = Convert.ToString(0);
        tlv2AddRspParam("userMode", "0");
        //data = Convert.ToString(rate);
        JGW_FormatString(data,"%d",rate);
        tlv2AddRspParam("rateBit", data);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        data = tlv2GetRspParam("tgtPwr");
        if (data.length() > 0)
        {
            tgtPwr = atoi(data.c_str());
        }
        return tgtPwr;
    }

    void CCJGW_WCNTestIPQ8070Dut::setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        SaveTxSettings2(wifiStandard, phyId, channel, channel2, cbState, rate, rateBw, nss, pwrMode, txMode, txChainMask, powerLevel, gain, digitalGain, PASetting, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, ackRequest, guardInterval);
        TurnOffTX2(phyId, channel, txChainMask);

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
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,1);
        std::string data;
        switch (cbState)
        {
        case Half_Rate:
            //data = Convert.ToString(50u);
            tlv2AddRspParam("bandwidth", "50");
            cbState = WLAN_Gen6_ChannelBondingState_none;
            break;
        case Quarter_Rate:
            //data = Convert.ToString(51u);
            tlv2AddRspParam("bandwidth", "51");
            cbState = WLAN_Gen6_ChannelBondingState_none;
            break;
        default:
            tlv2AddRspParam("bandwidth", "0");
            break;
        }
        SetUpTxDefault2(wifiStandard, phyId, centFreqMHz, centFreq2MHz, cbState, rate, rateBw, nss, txMode, txChainMask, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, scramblerDisable, ackRequest, guardInterval);
        //data = Convert.ToString(dutyCycle);
        JGW_FormatString(data,"%d",dutyCycle);
        tlv2AddRspParam("dutyCycle", data);
        switch (pwrMode)
        {
        case ForcedGain:
        case ForcedDesiredGain:
            //data = Convert.ToString(4u);
            tlv2AddRspParam("tpcm", "4");
            //data = Convert.ToString(gain);
            JGW_FormatString(data,"%d",gain);
            tlv2AddRspParam("gainIdx", data);
            //data = Convert.ToString(digitalGain);
            JGW_FormatString(data,"%d",digitalGain);
            tlv2AddRspParam("dacGain", data);
            //data = Convert.ToString(PASetting);
            JGW_FormatString(data,"%d",PASetting);
            tlv2AddRspParam("paConfig", data);
            break;
        case ForcedGLUTIndex:
            //data = Convert.ToString(6u);
            tlv2AddRspParam("tpcm", "6");
            //data = Convert.ToString(gain);
            JGW_FormatString(data,"%d",gain);
            tlv2AddRspParam("gainIdx", data);
            break;
        case TxPowerAuto:
            //data = Convert.ToString(5u);
            tlv2AddRspParam("tpcm", "5");
            break;
        case TxPowerForce_CLPC:
        case TxPowerForce_SCPC:
        case TxPowerForce_OLPC:
            //data = Convert.ToString(0u);
            tlv2AddRspParam("tpcm", "0");
            //data = Convert.ToString(powerLevel);
            JGW_FormatString(data,"%f",powerLevel);
            tlv2AddRspParam("txPower0", data);
            break;
        default:
            PELOG4WW(L"IPQ807x doesn't support selected pwrMode");
        }
    }

    void CCJGW_WCNTestIPQ8070Dut::SetUpTxDefault2(uint wifiStandard, uint phyId, uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, WLAN_TX_Mode txMode, uint txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        uint num = 1u;
        uint num2 = 2u;
        uint num3 = 4u;
        uint num4 = 128u;
        std::string data;
        //! Convert.ToString(wifiStandard)
        JGW_FormatString(data,"%d",wifiStandard);
        tlv2AddRspParam("wifiStandard", data);
        //data = Convert.ToString(phyId);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyId", data);
        //data = Convert.ToString(nss);
        JGW_FormatString(data,"%d",nss);
        tlv2AddRspParam("nss", data);
        JGW_FormatString(data,"%d",guardInterval);
        //data = Convert.ToString((uint)guardInterval);
        tlv2AddRspParam("gI", data);
        //data = Convert.ToString(centFreqMHz);
        JGW_FormatString(data,"%d",centFreqMHz);
        tlv2AddRspParam("channel", data);
        if (centFreq2MHz != 0)
        {
            //data = Convert.ToString(centFreq2MHz);
            JGW_FormatString(data,"%d",centFreq2MHz);
            tlv2AddRspParam("channel2", data);
        }
        //! data = Convert.ToString((uint)txMode);
        JGW_FormatString(data,"%d",txMode);
        tlv2AddRspParam("txMode", data);
        JGW_FormatString(data,"%d",rate);
        //data = Convert.ToString((uint)rate);
        tlv2AddRspParam("rateBitIndex0", data);
        JGW_FormatString(data,"%d",rateBw);
        //data = Convert.ToString((uint)rateBw);
        tlv2AddRspParam("rateBw", data);
        JGW_FormatString(data,"%d",cbState);
        //data = Convert.ToString((uint)cbState);
        tlv2AddRspParam("wlanMode", data);
        //data = Convert.ToString(payLoadSize);
        JGW_FormatString(data,"%d",payLoadSize);
        tlv2AddRspParam("pktLen0", data);
        //data = Convert.ToString(txChainMask);
        JGW_FormatString(data,"%d",txChainMask);
        tlv2AddRspParam("txChain0", data);
        JGW_FormatString(data,"%d",aggregation);
        //data = Convert.ToString(aggregation);
        tlv2AddRspParam("agg", data);
        data = ((!short11b_nGuard) ? "0" : "1");
        tlv2AddRspParam("shortGuard", data);
        //data = Convert.ToString(nPacket);
        JGW_FormatString(data,"%d",nPacket);
        tlv2AddRspParam("numPackets", data);
        data = ((!broadcastMode) ? "0" : "1");
        tlv2AddRspParam("broadcast", data);
        /*tlv2AddRspParam("rxStation", WlanTestSharedInfo.override_rxStation_addr, 6);
        tlv2AddRspParam("txStation", WlanTestSharedInfo.override_txStation_addr, 6);
        tlv2AddRspParam("bssid", WlanTestSharedInfo.override_txBSSID, 6);*/
        tlv2AddRspParam("rxStation", mszRxStationAddr);
        tlv2AddRspParam("txStation", mszTxStationAddr);
        tlv2AddRspParam("bssid", mszTxBSSID);
        //data = Convert.ToString(iAIFSN);
        JGW_FormatString(data,"%d",iAIFSN);
        tlv2AddRspParam("aifsn", data);
        data = ((!scramblerDisable) ? "0" : "1");
        tlv2AddRspParam("scramblerOff", data);
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
        //data = Convert.ToString(num5);
        JGW_FormatString(data,"%d",num5);
        tlv2AddRspParam("flags", data);
        uint num6 = (uint)nPattern;
        JGW_FormatString(data,"%d",num6);
        tlv2AddRspParam("txPattern", data);
        //data = Convert.ToString((uint)guardInterval);
        JGW_FormatString(data,"%d",guardInterval);
        tlv2AddRspParam("gI", data);
    }

    std::string CCJGW_WCNTestIPQ8070Dut::tlv2GetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(m_hResourceContext,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    std::string CCJGW_WCNTestIPQ8070Dut::tlvGetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(m_hResourceContext,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    bool CCJGW_WCNTestIPQ8070Dut::tlv2AddRspParam(const std::string& key,const std::string& data)
    {
        return (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(m_hResourceContext,(char*)key.c_str(), (char*)data.c_str()));
    }

    bool CCJGW_WCNTestIPQ8070Dut::tlv2AddRspParam(const std::string& key,char data[])
    {
        return (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(m_hResourceContext,(char*)key.c_str(),data));
    }

    bool CCJGW_WCNTestIPQ8070Dut::tlvAddRspParam(const std::string& key, const std::string& data)
    {
        return (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,(char*)key.c_str(), (char*)data.c_str()));
    }

    bool CCJGW_WCNTestIPQ8070Dut::tlvAddRspParam(const std::string& key, char data[])
    {
        return (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,(char*)key.c_str(), data));
    }

    void CCJGW_WCNTestIPQ8070Dut::TurnOffTX2(uint phyId, uint channel, uint txChain)
    {
        uint value = channel;
        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }

        std::string data;
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,1);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyid", data);
        //data = Convert.ToString(value);
        JGW_FormatString(data,"%d",value);
        tlv2AddRspParam("channel", data);
        //data = Convert.ToString(0u);
        tlv2AddRspParam("txMode", "0");
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
    }

    int CCJGW_WCNTestIPQ8070Dut::getChanFreqMhz(uint chanID)
    {
        int array[15] = {
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
        int array2[54] = 
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
        int array3[54] = 
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
        PELOG4WW(L"Invalid channel index");
        return -1; 
    }

    void CCJGW_WCNTestIPQ8070Dut::SaveTxSettings2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState,WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, char* pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        txSettings2.wifiStandard = wifiStandard;
        txSettings2.phyId = phyId;
        txSettings2.channel = channel;
        txSettings2.channel2 = channel2;
        txSettings2.cbState = cbState;
        txSettings2.rate = rate;
        txSettings2.rateBw = rateBw;
        txSettings2.nss = nss;
        txSettings2.pwrMode = pwrMode;
        txSettings2.txMode = txMode;
        txSettings2.txChainMask = txChainMask;
        txSettings2.powerLevel = powerLevel;
        txSettings2.gain = gain;
        txSettings2.digitalGain = digitalGain;
        txSettings2.PASetting = PASetting;
        txSettings2.payLoadSize = payLoadSize;
        txSettings2.short11b_nGuard = short11b_nGuard;
        txSettings2.ldpcRate = ldpcRate;
        txSettings2.bSTBCmode = bSTBCmode;
        txSettings2.aggregation = aggregation;
        txSettings2.dpdMode = dpdMode;
        txSettings2.ifs = ifs;
        txSettings2.dutyCycle = dutyCycle;
        txSettings2.nPattern = nPattern;
        txSettings2.pattern = pattern;
        txSettings2.broadcastMode = broadcastMode;
        txSettings2.nPacket = nPacket;
        txSettings2.iAIFSN = iAIFSN;
        txSettings2.ackRequest = ackRequest;
        txSettings2.guardInterval = guardInterval;
    }

    void CCJGW_WCNTestIPQ8070Dut::SetPhyId(WLAN_UniversalPhyID newPhyId)
    {
        //         Phone phoneInstance = PhoneInstance;
        //         PHYID = newPhyId;
        std::string data;
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,_OP2_SYNC);
        // = Convert.ToString((int)newPhyId);
        JGW_FormatString(data,"%d",newPhyId);
        tlv2AddRspParam("phyId", data);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
    }

    void CCJGW_WCNTestIPQ8070Dut::Xtal_Cal_Pre()
    {
        for (int i = 0; i < 16; i++)
        {
            mFreqCap[i].m_capVal = 999u;
            mFreqCap[i].m_freqErr = -999.0;
        }
        m_fc_index = 0;
    }

    int CCJGW_WCNTestIPQ8070Dut::GetSISOChainNumber(WLAN_SINGLE_CHAIN_MASK mask)
    {
        switch (mask)
        {
        case WLAN_SINGLE_CHAIN_1:
            return 1;
        case WLAN_SINGLE_CHAIN_2:
            return 2;
        case WLAN_SINGLE_CHAIN_3:
            return 3;
        case WLAN_SINGLE_CHAIN_4:
            return 4;
        case WLAN_SINGLE_CHAIN_5:
            return 5;
        case WLAN_SINGLE_CHAIN_6:
            return 6;
        case WLAN_SINGLE_CHAIN_7:
            return 7;
        case WLAN_SINGLE_CHAIN_8:
            return 8;
        default:
            PELOG4WW(L"Chain mask is not mapped");
        }
        return 1;
    }

    int FreqCapCompare(_FREQ_CAP fc1, _FREQ_CAP fc2)
    {
        if (fc1.m_capVal < fc2.m_capVal)
        {
            return -1;
        }
        else if (fc1.m_capVal > fc2.m_capVal)
        {
            return 1;
        }

        return 0;
    }

    bool CCJGW_WCNTestIPQ8070Dut::Xtal_Cal_Post(bool bDescending)
    {
        if (m_fc_index == 0)
        {
            return true;
        }

        _FREQ_CAP freqCap[16] = {0};
        std::vector<_FREQ_CAP> vFreqCap;
        for (int i = 0;i < 16;i ++)
        {
            _FREQ_CAP freqCap;
            freqCap.m_capVal = mFreqCap[i].m_capVal;
            freqCap.m_freqErr = mFreqCap[i].m_freqErr;
            vFreqCap.push_back(freqCap);
        }
        //  一个有符号数字，该数字指示此实例与 value 的相对值。返回值说明小于零此实例小于 value。零此实例等于 value。大于零此实例大于 value。
        //std::sort(vFreqCap.begin(),vFreqCap.end(),FreqCapCompare);
        std::sort(vFreqCap.begin(),vFreqCap.end(),[] (_FREQ_CAP fc1, _FREQ_CAP fc2) -> int 
        { 
            if (fc1.m_capVal < fc2.m_capVal)
            {
                return -1;
            }
            else if (fc1.m_capVal > fc2.m_capVal)
            {
                return 1;
            }
            return 0;
        });

        //!Array.Sort(m_freqCap, (_FREQ_CAP fc1, _FREQ_CAP fc2) => fc1.m_capVal.CompareTo(fc2.m_capVal));
        for (int i = 0;i < 16;i ++)
        {
            mFreqCap[i].m_capVal = vFreqCap[i].m_capVal;
            mFreqCap[i].m_freqErr = vFreqCap[i].m_freqErr;
        }

        if (bDescending)
        {
            for (int i = 1; i < m_fc_index; i++)
            {
                if (mFreqCap[i - 1].m_freqErr + 1.0 < mFreqCap[i].m_freqErr)
                {
                    return false;
                }
            }
        }
        else
        {
            for (int j = 1; j < m_fc_index; j++)
            {
                if (mFreqCap[j - 1].m_freqErr - 1.0 > mFreqCap[j].m_freqErr)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void CCJGW_WCNTestIPQ8070Dut::NfCal2(WLAN_UniversalPhyID phyId, uint chainMask, unsigned short freq, uint nfcal_values[8])
    {
        std::string data ;
        //Phone phoneInstance = PhoneInstance;
        //nfcal_values = new uint[8];
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,/*202*/_OP2_NOISEFLOORREAD);
        //string data = Convert.ToString((int)phyId);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyId", data);
        //data = Convert.ToString(chainMask);
        JGW_FormatString(data,"%d",chainMask);
        tlv2AddRspParam("chainMask", data);
        //data = Convert.ToString(freq);
        JGW_FormatString(data,"%d",freq);
        tlv2AddRspParam("freq", data);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        for (int i = 0; i < 8; i++)
        {
            JGW_FormatString(data,"nfcal_values%d",i);
            nfcal_values[i] = atoi(tlv2GetRspParam(data).c_str());
        }
    }

    void CCJGW_WCNTestIPQ8070Dut::bdRead(uint bdsize, uint offset, uint size, char data[])
    {
        //data = new byte[4096];
        std::string text = "";
        QLIB_FTM_WLAN_TLV2_Create(m_hResourceContext,/*207*/_OP2_BDREAD);
        //text = Convert.ToString(bdsize);
        JGW_FormatString(text,"%d",bdsize);
        tlv2AddRspParam("bdSize", text);
        //text = Convert.ToString(offset);
        JGW_FormatString(text,"%d",offset);
        tlv2AddRspParam("offset", text);
        //text = Convert.ToString(size);
        JGW_FormatString(text,"%d",size);
        tlv2AddRspParam("size", text);
        QLIB_FTM_WLAN_TLV2_Complete(m_hResourceContext);
        //data = tlv2GetRspParamRaw("data");
        QLIB_FTM_WLAN_TLV2_GetRspParam(m_hResourceContext,"data",data);
    }

    /*void CCJGW_WCNTestIPQ8070Dut::setRFSwitch(WLAN_CHAIN_MASK2 chainmask)
    {
    if (WlanTestSharedInfo.WlanRfSwitchBoxEnabled)
    {
    if (chainmask == (WLAN_CHAIN_MASK)0)
    {
    resetRFSwitch();
    return;
    }
    QTALogMgr.Debug("RFSwitch to Chain : " + (int)chainmask);
    try
    {
    IWCNRFSwitchBox iWCNRFSwitchBox = ResourceManager.GetAnInstrument(WlanTestSharedInfo.WlanRfSwitchBoxResourceID, this) as IWCNRFSwitchBox;
    if (iWCNRFSwitchBox == null)
    {
    throw new WLANCustomException("Null ." + WlanTestSharedInfo.WlanRfSwitchBoxResourceID);
    }
    iWCNRFSwitchBox.ConnectToSwitch();
    iWCNRFSwitchBox.SetWLANPath((ChainMask)chainmask);
    iWCNRFSwitchBox.DisconnectFromSwitch();
    }
    catch (Exception ex)
    {
    throw new WCNCustomException(" setRFSwitch failed : " + ex.ToString());
    }
    }
    }*/
}
