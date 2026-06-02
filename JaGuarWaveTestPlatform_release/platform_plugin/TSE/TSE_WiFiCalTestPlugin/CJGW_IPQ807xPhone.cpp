#include "StdAfx.h"
#include "CJGW_IPQ807xPhone.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <regex>
#include "JGW_Phone.h"
namespace JGW
{
    CCJGW_IPQ807xPhone::CCJGW_IPQ807xPhone(void) : mhResourceContext(NULL),mXtalTolerancePPM(1),mXtalTargetPPM(0),m_fc_index(0)
    {
        PHONE->m_txChainMask = 1;
    }


    CCJGW_IPQ807xPhone::~CCJGW_IPQ807xPhone(void)
    {
    }

    CCJGW_IPQ807xPhone* CCJGW_IPQ807xPhone::GetIPQ807xPhone()
    {
        static CCJGW_IPQ807xPhone gIPQ807xPhone;
        return &gIPQ807xPhone;
    }

    void CCJGW_IPQ807xPhone::InitQMSLLibrary(QLIB_TargetType_Enum type)
    {
        QLIB_SetLibraryMode(QLIB_LIB_MODE_QUTS);
        QLIB_SetTargetType(type);
    }

    bool CCJGW_IPQ807xPhone::FindConnectAvailablePhonesHandleID(const std::string& strComPort)
    {
        mhResourceContext = NULL;
        GetPhoneInstance()->SetResourceContent(mhResourceContext);
        const unsigned int maxListSize = 10;
        unsigned int iNumListSize = maxListSize;
        long long devHandleIdList[maxListSize];
        long long protocolIdList[maxListSize];
        unsigned int index = 9999, i;
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

        unsigned char bOK =  QLIB_GetAvailablePhonesHandleIDList((QUTS_PROTOCOL_TYPE)0, &iNumListSize,
            devHandleIdList,
            protocolIdList,
            &iDescriptionLength, pDescriptionList,
            &iErrorCode);
        if (bOK)
        {
            if (0 == JGW_StrComparenoCaseStr(strComPort.c_str(),"AUTO"))
            {
                for (i = 0; i < iNumListSize; i++)
                {
                    std::string description(pDescriptionList[i]);
                    if(description.find("COM") != std::string::npos)
                    { 
                        index = i;
                        break;
                    }
                    std::regex ipPattern("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
                    if (regex_search("2500->::ffff:192.168.1.1", ipPattern))
                    {
                        index = i;
                        break;
                    }
                }
                //if (array3[k].Contains("COM"))
                //{
                //    int num5 = array3[k].IndexOf("COM");
                //    int num6 = array3[k].IndexOf(")", num5);
                //    comPort = array3[k].Substring(num5, num6 - num5);
                //    num2 = k;
                //    break;
                //}
                //Uri result;
                //IPAddress address;
                //if (Uri.TryCreate(string.Format("http://{0}", array3[k]), UriKind.Absolute, out result) && IPAddress.TryParse(result.Host, out address))
                //{
                //    comPort = array3[k];
                //    num2 = k;
                //    break;
                //}
            }
            else
            {
                for (i = 0; i < iNumListSize; i++)
                {
                    std::string description(pDescriptionList[i]);
                    if (description.find(strComPort) != std::string::npos)
                    { 
                        index = i;
                        break;
                    }
                    //if (description.find(strComPort) != std::string::npos)
                    //{ 
                    //    index = i;
                    //    break;
                    //}
                }
            }
            if (index < iNumListSize && index >= 0)
            {
                mhResourceContext = QLIB_ConnectServerWithWaitByHandleID((QUTS_PROTOCOL_TYPE)0, devHandleIdList[index], protocolIdList[index], 2000);
            } 
        }

        for (unsigned int i = 0; i < iNumListSize; i++)
        {
            delete []pDescriptionList[i];
            //pDescriptionList[i] = new char[iDescriptionLength];
            //if (pDescriptionList[i])
            //{
            //    memset(pDescriptionList[i], 0, sizeof(char)*iDescriptionLength);
            //}
        }
        GetPhoneInstance()->SetResourceContent(mhResourceContext);
        return NULL != mhResourceContext;
    }

    void CCJGW_IPQ807xPhone::DisConnectDut()
    {
        if (NULL != mhResourceContext)
        {
            QLIB_DisconnectServer(mhResourceContext);
            mhResourceContext = NULL;
            GetPhoneInstance()->SetResourceContent(mhResourceContext);
        }
    }

    bool CCJGW_IPQ807xPhone::WlanSetInstance(WLAN_INSTANCE instance)
    {
        return TRUE == QLIB_FTM_WLAN_SetInstance(mhResourceContext,instance);
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

    bool CCJGW_IPQ807xPhone::WlanSetLocalMac(const std::string& macAddr1 /* = "000000000000" */, const std::string& macAddr2 /* = "000000000000" */, const std::string& macAddr3 /* = "000000000000" */, const std::string& macAddr4 /* = "000000000000" */, const std::string& macAddr5 /* = "000000000000" */, const std::string& macAddr6 /* = "000000000000" */, const std::string& macAddr7 /* = "000000000000" */, const std::string& macAddr8 /* = "000000000000" */, const std::string& macAddr9 /* = "000000000000" */)
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

        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,_OP2_SETLOCALMAC));
        //result &= (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(m_hResourceContext,"localMac", byteAddr));
        result &= tlv2AddRspParam("localMac", byteAddr);
        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Complete(mhResourceContext));
        //result &= (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(m_hResourceContext,"status",array));
        tlv2GetRspParam("status");

        return result;
    }

    std::string CCJGW_IPQ807xPhone::tlv2GetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(mhResourceContext,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    std::string CCJGW_IPQ807xPhone::tlvGetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContext,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    bool CCJGW_IPQ807xPhone::tlv2AddRspParam(const std::string& key,const std::string& data)
    {
        return (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContext,(char*)key.c_str(), (char*)data.c_str()));
    }

    bool CCJGW_IPQ807xPhone::tlv2AddRspParam(const std::string& key,char data[])
    {
        return (TRUE == QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContext,(char*)key.c_str(),data));
    }

    bool CCJGW_IPQ807xPhone::tlvAddRspParam(const std::string& key, const std::string& data)
    {
        return (TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,(char*)key.c_str(), (char*)data.c_str()));
    }

    bool CCJGW_IPQ807xPhone::tlvAddRspParam(const std::string& key, char data[])
    {
        return (TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContext,(char*)key.c_str(), data));
    }

    bool CCJGW_IPQ807xPhone::SetPhyId(PhyID newPhyId)
    {
        bool result = true;
        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,_OP2_SYNC));
        std::string data;
        JGW_FormatString(data,"%d",newPhyId);
        result &= tlv2AddRspParam("phyId", data);
        result &= (TRUE == QLIB_FTM_WLAN_TLV2_Complete(mhResourceContext));

        return result;
        //Phone phoneInstance = Phone.get_PhoneInstance();
        //PHYID = newPhyId;
        //phoneInstance.FTM_WLAN_TLV2_Create((byte)100);
        //string data = Convert.ToString((int)newPhyId);
        //tlv2AddRspParam("phyId", data);
        //phoneInstance.FTM_WLAN_TLV2_Complete();
    }


    bool CCJGW_IPQ807xPhone::WlanXtalCalInit2(PhyID phyId, int dXtalTargetPPM, size_t dXtalTolerancePPM, WLAN_XTAL_STORAGE_OTP dXtalCalOtpSaveOption, size_t settleTime /* = 1000 */, size_t delayTime /* = 2000 */,bool pmic/* = false*/)
    {
        
        mXtalTargetPPM = dXtalTargetPPM;
        mXtalTolerancePPM = dXtalTolerancePPM;
        if (!SetPhyId(phyId))
        {
            LogE(L"Set Wlan PhyID Fail");
            return false;
        }
        bool result = true;
        XTAL_ALGORITHM XtalAlgorithm = SEARCH_BINARY;
        uint XtalStep = 0;
        result &= TRUE == QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT2(mhResourceContext,dXtalTargetPPM, dXtalTolerancePPM,XtalAlgorithm,XtalStep,dXtalCalOtpSaveOption, settleTime, delayTime,pmic);

        for (int i = 0; i < 16; i++)
        {
            m_freqCap[i].m_capVal = 999;
            m_freqCap[i].m_freqErr = -999.0;
        }
        m_fc_index = 0;
        return result;
    }

    static bool Xtal_Cal_FC(uint cVal, double freqErr)
    {
        if (CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_fc_index >= 16)
        {
            return false;
        }
        CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_freqCap[CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_fc_index].m_capVal = cVal;
        CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_freqCap[CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_fc_index].m_freqErr = freqErr;
        CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_fc_index++;
        return true;
    }
    //! (uint capInValue, uint capOutValue, uint freq, double powerLevel, uint avgNum)
    double WlanXtalCalibration2_asyncCWFreqErrMessageCB2(unsigned int iCapInValue,unsigned int iCapOutValue,unsigned int iCwFreqMHz,double dPowerLevel,unsigned int iNumAvg)
    {
        //double[] array = new double[avgNum];
        std::vector<double> array;
        double num = 0.0;
        double cfFreqPowerDbm = -999.0;
        unsigned int i = 0;
        int num2 = 0;
        for (; i < iNumAvg; i++)
        {
            //array[i] = getCWFreqErr(freq, powerLevel, out cfFreqPowerDbm);
            double val = INVAILD_WLAN_RESULT;
            PHONE->mWlanDutWrapper.GetCWFreqErrPower(iCwFreqMHz,dPowerLevel,val,cfFreqPowerDbm);
            if (val > -999.0)
            {
                num += val;
                num2++;
            }
        }
        num = ((num2 <= 0) ? (-999.0) : (num / (double)num2));
        
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"FreqErr:%.2f   Power: %.2f   ChannelFreq: %u    txchainMask:%d     capOutValue:%u    capInValue:%u",num,cfFreqPowerDbm,iCwFreqMHz,PHONE->m_txChainMask,iCapOutValue,iCapInValue);
        //DebugMessage.Write("Debug", "FreqErr:" + string.Format("{0:0.00}", num) + "   Power: " + string.Format("{0:0.00}", cfFreqPowerDbm) + "   ChannelFreq: " + freq + "    txchainMask: " + PHONE->m_txChainMask + "    capOutValue:" + iCapOutValue + "    capInValue:" + iCapInValue);
        Xtal_Cal_FC(iCapOutValue, num);
        return num;
#if 0
        double freqErr = INVAILD_WLAN_RESULT;
        double power = INVAILD_WLAN_RESULT;

        std::vector<double> vTestValue;
        //! 测试数据有效数
        int validCount = 0;
        double sum = 0,avg = INVAILD_WLAN_RESULT;

        for (size_t i = 0;i < iNumAvg;i ++)
        {
            //int pathNameIndex = GetSISOChainNumber(m_txChainMask);
            double val = INVAILD_WLAN_RESULT;
            PHONE->mWlanDutWrapper.GetCWFreqErrPower(iCwFreqMHz,dPowerLevel,val,power);
            vTestValue.push_back(val);
            if (val > INVAILD_WLAN_RESULT)
            {
                sum += val;
                validCount ++;
            } 
        }

        if (validCount > 0)
        {
            avg = sum/validCount;
        }
        freqErr = avg;
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"FreqErr:%.2f\tPower: %.2f\tChannelFreq: %d\tTxchainMask: %d\tcapOutValue:%d\tcapInValue:%d",freqErr,power,iCwFreqMHz,CCJGW_IPQ807xPhone::GetIPQ807xPhone()->m_txChainMask,iCapOutValue,iCapInValue);

        return freqErr;
#endif
        //std::vector<double> vTestValue;
        ////! 测试数据有效数
        //int validCount = 0;
        //double sum = 0,avg = -999.0;

        //for (size_t i = 0;i < avgNum;i ++)
        //{
        //    //int pathNameIndex = GetSISOChainNumber(m_txChainMask);
        //    double val = PHONE->mWlanDutWrapper.GetCWFreqErr(freq,powerLevel);
        //    vTestValue.push_back(val);
        //    if (val > INVAILD_WLAN_RESULT)
        //    {
        //        sum += val;
        //        validCount ++;
        //    } 
        //}

        //if (validCount > 0)
        //{
        //    avg = sum/validCount;
        //}
        ////! "FreqErr:",
        ////! string.Format("{0:0.00}", num),"   ChannelFreq: ",freq,"    txchainMask: ",m_txChainMask"    capOutValue:",capValue
        //LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%d\t%.2f\t%.2f\t%d\t%d",freq,powerLevel,avg,PHONE->m_txChainMask,capValue);
        //return avg;
    }

    bool CCJGW_IPQ807xPhone::WlanXtalCalibration2(PhyID phyId, size_t channel, WLAN_SINGLE_CHAIN_MASK txChainMask, size_t gainIndex, int dacGain, int paCfg, size_t numAverages, size_t& xtalCapIn, size_t& xtalCapOut, double& freqErrPPM, bool& status, bool ppmCurveDetection /* = false */,bool pmic/* = false*/)
    {
        if (!SetPhyId(phyId))
        {
            LogE(L"Set Wlan PhyID Fail");
            return false;
        }

        std::string data;
        PHONE->m_txChainMask = (uint)(int)txChainMask;
        QLIB_FTM_WLAN_TLV_Create(mhResourceContext,(byte)3);

        JGW_FormatString(data,"%d",channel);
        tlvAddRspParam("channel", data);

        WLAN_TX_Mode val2 = (WLAN_TX_Mode)EnablingContinuousUnmodulatedTX;
        JGW_FormatString(data,"%d",val2);
        tlvAddRspParam("txMode", data);

        WLAN_Gen6_ChannelBondingState val3 = primary20;
        JGW_FormatString(data,"%d",val3);
        tlvAddRspParam("wlanMode", data);

        JGW_FormatString(data,"%d",txChainMask);
        tlvAddRspParam("txChain0", data);

        tlvAddRspParam("tpcm", "4");
        JGW_FormatString(data,"%d",gainIndex);
        tlvAddRspParam("gainIdx", data);
        JGW_FormatString(data,"%d",dacGain);
        tlvAddRspParam("dacGain", data);
        JGW_FormatString(data,"%d",paCfg);
        tlvAddRspParam("paConfig", data);
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContext);
        QLIB_FTM_WLAN_Atheros_xtal_CAL2(mhResourceContext,WlanXtalCalibration2_asyncCWFreqErrMessageCB2, numAverages, ppmCurveDetection,pmic);
        data = tlvGetRspParam("XTAL_CAPIN_VALUE");
        xtalCapIn = atoi(data.c_str());
        data = tlvGetRspParam("XTAL_CAPOUT_VALUE");
        xtalCapOut = atoi(data.c_str());
        data = tlvGetRspParam("XTAL_FREQERRPPM");
        freqErrPPM = atof(data.c_str());

        //double min = mXtalTargetPPM - (int)mXtalTolerancePPM;
        //double max = (double)(mXtalTargetPPM + mXtalTolerancePPM);
        //bool result = freqErrPPM < (double)(mXtalTargetPPM - mXtalTolerancePPM);
        //result = freqErrPPM > (double)(mXtalTargetPPM + mXtalTolerancePPM);
        if (freqErrPPM < (double)(mXtalTargetPPM - (int)mXtalTolerancePPM) || freqErrPPM > (double)(mXtalTargetPPM + (int)mXtalTolerancePPM))
        {
            return false;
        }
        //！ FreqErr:-8.28   Power: -9.14   ChannelFreq: 5500    txchainMask: 1    capOutValue:176    capInValue:560
        return true;
    }

    static bool asyncSetChainCB2(int chainMask)
    {
        PHONE->m_txChainMask = chainMask;
        PHONE->mWlanDutWrapper.SwitchLossPathToChainMask(true,chainMask);
        printf("asyncSetChainCB chainMask = %2.2Xh\n",chainMask);
        return true;  
       // return true;
    }


    static bool asyncRFSCB2(int chainMask)
    {
        printf("asyncRFSCB chainMask     = %2.2Xh\n",chainMask);
        return true;
    }
    //static bool asyncPMCB2(unsigned int gain, unsigned int freq, unsigned int chainMask,
    //double* powerLevel1, unsigned int numAvg, unsigned int iTriggerType, int iTriggerLevel, double* oPower1)
    //double asyncPMCB(unsigned int iGain
    //    ,unsigned int iFreq
    //    ,double dPowerLevel
    //    ,unsigned int iNumAvg
    //    ,unsigned int iTriggerType
    //    //! for measure CW tone, set iTriggerLevel = -1000
    //    ,int iTriggerLevel)
    static bool asyncPMCB2(unsigned int iGain, unsigned int iFreq, unsigned int chainMask,
        double* powerLevel1, unsigned int iNumAvg, unsigned int iTriggerType, int iTriggerLevel, double* oPower1)
    {
#if 1
        double avgVal = 999.0,dPowerLevel = *powerLevel1;
        std::vector<double> vdTestValue;
        //for (int i =0; i < 8; i++)
        //{
        //    *(oPower1+i)=*(powerLevel1+i);
        //    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%.2f - %.2f",*(powerLevel1+i),*(oPower1+i));
        //    //double val = *(powerLevel1+i);
        //}
        for (size_t i = 0;i < iNumAvg;i ++)
        {
            if (iTriggerLevel <= -1000 && iTriggerType == 0u)
            {
                if (PHONE->mCalScheme == CAL_SCPC || PHONE->mCalScheme == CAL_SCPC_PSATADJUST)
                {
                    vdTestValue.push_back(PHONE->mWlanDutWrapper.GetAvgPowerDelegate((WLAN_RATE)PHONE->mnWlanRate, iFreq, dPowerLevel,iTriggerLevel,PHONE->mnVsaAverages));
                    PHONE->m_pwrMeasType = POWER_MEAS_PEAK;
                }
                else
                {
                    vdTestValue.push_back(PHONE->mWlanDutWrapper.GetCWAvgPowerDelegate(iFreq, dPowerLevel,iTriggerLevel,PHONE->mnVsaAverages));
                    PHONE->m_pwrMeasType = POWER_MEAS_AVG;
                }
            }
            else
            {
                if (PHONE->mCalScheme == CAL_SCPC_PSATADJUST)
                {
                    if (0 == iGain)
                    {
                        PHONE->m_pwrMeasType = POWER_MEAS_PEAK;
                    }
                    else if (1 == iGain)
                    {
                        PHONE->m_pwrMeasType = POWER_MEAS_AVG;
                    }
                }
                else
                {
                    PHONE->m_pwrMeasType = POWER_MEAS_AVG;
                }
                if (0 == i)
                {
                    vdTestValue.push_back(PHONE->mWlanDutWrapper.GetAvgPowerDelegate((WLAN_RATE)PHONE->mnWlanRate, iFreq, dPowerLevel,iTriggerLevel,PHONE->mnVsaAverages));
                }
                else
                {
                    vdTestValue.push_back(PHONE->mWlanDutWrapper.GetCurrentConfigAvgPowerDelegate());
                }
            }
        }
        avgVal = AveragePowResult(vdTestValue);
        double tmpAvgVal = avgVal;
        //switch (iGain)
        //{
        //case 18:
        //    avgVal = 4.34;
        //    break;
        //case 20:
        //    avgVal = 6.27;
        //    break;
        //case 21:
        //    avgVal = 7.24;
        //    break;
        //case 22:
        //    avgVal = 8.51;
        //    break;
        //case 23:
        //    avgVal = 9.40;
        //    break;
        //case 24 :
        //    avgVal = 10.31;
        //}

        //avgVal = GetFineTuningPower(iGain,iFreq,dPowerLevel,avgVal);
        if (iTriggerLevel <= -1000 && iTriggerType == 0u)
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"CWGoal:%.2f,MeasPwr:%.2f,gain:%d,ChannelFreq:%d,txchainMask:%d(%.2f)",dPowerLevel,avgVal,iGain,iFreq,(int)PHONE->m_txChainMask,tmpAvgVal);
        }
        else
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Goal:%.2f,MeasPwr:%.2f,gain:%d,ChannelFreq:%d,txchainMask:%d(%.2f)",dPowerLevel,avgVal,iGain,iFreq,(int)PHONE->m_txChainMask,tmpAvgVal);
        }
        *oPower1 = avgVal;
        return true;
#endif
#if 0
        for (int i =0; i < 8; i++)
            *(oPower1+i)=*(powerLevel1+i);    // oPower is actual measured power, upto 8 depends on chainmask setting etc.

        printf("asyncPMCB2 Frequency=%d, chainMask=%2.2Xh, Target Power=%2.2f dBm, Measured Power=%2.2f dBm\n",iFreq, chainMask, *powerLevel1, *oPower1);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"asyncPMCB2 Frequency=%d, chainMask=%2.2Xh, Target Power=%2.2f dBm, Measured Power=%2.2f dBm\n",iFreq, chainMask, *powerLevel1, *oPower1);
        return true;
#endif
#if 0
        //IL_00b2: Unknown result type (might be due to invalid IL or missing references)
        //IL_00b8: Invalid comparison between Unknown and I4
        double[] array = new double[8];
        double[] oPower2 = new double[8];
        double[] array2 = new double[8];
        CallRfSwitchIfAvailble(qcmbrPort, freq, m_txChainMask);
        Marshal.Copy(powerLevel1, array2, 0, array2.Length);
        if (freq >= 5945 && !string.IsNullOrEmpty(WlanTestSharedInfo.testerConfigFileName6) && !WlanTestSharedInfo.KsCiuSwitched)
        {
            WlanTestSharedInfo.KsCiuSwitched = true;
            DebugMessage.Write("Debug", "InitializeWlanTester for 6GHz");
            instanceTestSuite = QC.CTE.WLANTestSuite.WLANTestSuite.tsInst;
            instanceDutWrapper = QC.CTE.WlanDutWrapper.WlanDutWrapper.drInst;
            string testerConfigFileName = WlanTestSharedInfo.testerConfigFileName6;
            string stationCalFileName = WlanTestSharedInfo.stationCalFileName6;
            string wlanTesterResID = WlanTestSharedInfo.wlanTesterResID6;
            string txStationCalPathName = WlanTestSharedInfo.TxStationCalPathName6;
            string rxStationCalPathName = WlanTestSharedInfo.RxStationCalPathName6;
            instanceTestSuite.InitializeWlanTester(testerConfigFileName, stationCalFileName, wlanTesterResID, txStationCalPathName, rxStationCalPathName);
        }
        if ((int)mCalScheme == 6)
        {
            switch (gain)
            {
            case 0u:
                m_pwrMeasType = POWER_MEAS_TYPE.POWER_MEAS_PEAK;
                break;
            case 1u:
                m_pwrMeasType = POWER_MEAS_TYPE.POWER_MEAS_AVG;
                break;
            }
        }
        else
        {
            m_pwrMeasType = POWER_MEAS_TYPE.POWER_MEAS_AVG;
        }
        getPower2(freq, chainMask, m_rate, array2, numAvg, out oPower2);
        int num = 0;
        int num2 = 1;
        while (num < 8)
        {
            array[num] = -999.0;
            if ((num2 & chainMask) != 0)
            {
                array[num] = oPower2[num];
                DebugMessage.Write("Debug", "Goal: " + string.Format("{0:0.00}", array2[num]) + "    MeasPwr:" + string.Format("{0:0.00}", array[num]) + "   gain: " + gain + "   lFreq: " + freq + "    chainMask: " + (chainMask & num2));
            }
            num++;
            num2 <<= 1;
        }
        Marshal.Copy(array, 0, oPower1, array.Length);
        return true;
#endif
    }


    bool CCJGW_IPQ807xPhone::WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, WLAN_TpcCalScheme calScheme, uint totalNumOfChains, uint multiChainMask, WLAN_TpcBand band, uint numAverages, bool& status, bool VsaOnePort /* = true */, double rangeDb1ptCalUL /* = 3.0 */, double rangeDb1ptCalLL /* = -3.0 */)
    {
        //! :QCOM:WLAN:SENS:CONF:MOD 1,1;:QCOM:WLAN:SENS:CONF:POW 1,1;:QCOM:WLAN:SENS:CONF:SMAS 1,0;:QCOM:WLAN:SENS:PAR 1,'WIFI_11AC_HT20_MCS5',1;:QCOM:WLAN:SENS:FREQ 1,5180000000;:QCOM:WLAN:SENS:POW:REF 1,10;:QCOM:WLAN:SENS:TRIG:LEV 1,-25;:QCOM:WLAN:SENS:AVER:COUN 1,1;:QCOM:WLAN:ERR?
        char bStatus= 0;
        PHONE->mmapFreqS_XtalCalibration_Result.clear();
        PHONE->mnWlanRate = RATE_11AC_HT20_MCS5;
        PHONE->mbResult = true;
        PHONE->mnVsaAverages = numAverages;
        bStatus = QLIB_FTM_WLAN_Atheros_Tx_FW_CAL2(mhResourceContext, phyId, calScheme, rangeDb1ptCalUL, rangeDb1ptCalLL, totalNumOfChains, multiChainMask, band, numAverages, asyncSetChainCB2, asyncPMCB2, asyncRFSCB2);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Test_FTM_QCA_WLAN_TPC1Point = %s\n",bStatus == 0 ? L"Failed" : L"Success");
        return TRUE == bStatus && PHONE->mbResult;
    }

    bool CCJGW_IPQ807xPhone::WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, BAND band, WLAN_Action PromiscuousMode,bool& status, const std::string& rxMacAddress /* = "010000C0FFEE" */, const std::string& bssId /* = "020000C0FFEE" */)
    {
        char rxMacAddressByte[6] = {0};
        char bssIdByte[6] = {0};
        PHONE->mbResult = false;
        CopyMacToCharArray(rxMacAddressByte,0,rxMacAddress);
        CopyMacToCharArray(bssIdByte,0,bssId);
        return TRUE == QLIB_FTM_WLAN_Atheros_Rx_FW_CAL2(mhResourceContext,asyncSGCB, asyncRxGainCalDoneCB, phyId, chainToCalibrate, band, (uint)PromiscuousMode, (unsigned char*)rxMacAddressByte, (unsigned char*)bssIdByte) && PHONE->mbResult;
    }

    void CCJGW_IPQ807xPhone::NfCal2(PhyID phyId, uint chainMask, unsigned short freq, uint* nfcal_values)
    {
        std::string data;
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,202);
        JGW_FormatString(data,"%d",phyId);
        tlv2AddRspParam("phyId", data);
        JGW_FormatString(data,"%d",chainMask);
        tlv2AddRspParam("chainMask", data);
        JGW_FormatString(data,"%d",freq);
        tlv2AddRspParam("freq", data);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContext);
        for (int i = 0; i < 8; i++)
        {
            JGW_FormatString(data,"nfcal_values%d",i);
            std::string value = tlv2GetRspParam(data);
            if (value.size() > 0)
            {
                nfcal_values[i] = atoi(value.c_str());
            }
            else
            {
                nfcal_values[i] = 0u;
            }
        }
    }

    void CCJGW_IPQ807xPhone::tlv2AddRspParam(std::string key, const std::vector<unsigned short>& data, int dataLength)
    {
        byte array[128] = {0};
        for (int i = 0; i < dataLength; i++)
        {
            array[i * 2] = (byte)(data[i] & 0xFF);
            array[i * 2 + 1] = (byte)((data[i] >> 8) & 0xFF);
        }
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContext,(char*)key.c_str(), (char*)array);
    }

    void CCJGW_IPQ807xPhone::NfCalSave(PhyID phyId, const std::vector<unsigned short>& vChannel, unsigned short NFdB2_Value[16][16])
    {
        uint num = 0;
        int num2 = 0;
        int num3 = vChannel.size();
        bool _b6G = false;
        unsigned short array[16] = {0};
        std::vector<unsigned short> vTemp;
        if (_b6G)
        {
            for (num = 0; num < vChannel.size(); num++)
            {
                for (num2 = 0; num2 < 16; num2++)
                {
                    array[num2 * 16 + num] = NFdB2_Value[num][num2];
                }
            }
            if (num3 > 16)
            {
                num3 = 16;
            }
        }
        else
        {
            for (num = 0; num < vChannel.size(); num++)
            {
                for (num2 = 0; num2 < 8; num2++)
                {
                    array[num2 * 8 + num] = NFdB2_Value[num][num2];
                }
            }
            if (num3 > 8)
            {
                num3 = 8;
            }
        }
        for (size_t i = 0;i < vChannel.size();i ++)
        {
            vTemp.push_back(array[i]);
        }

        QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,222);
        std::string data;
        JGW_FormatString(data,"%d",(int)phyId);
        tlv2AddRspParam("phyId", data);
        JGW_FormatString(data,"%d",num3);
        tlv2AddRspParam("itemNum", data);
        tlv2AddRspParam("freqs", vChannel, (int)num3);
        tlv2AddRspParam("nfcal_values", vTemp, (int)vTemp.size());
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContext);
        data = tlv2GetRspParam("status");
    }

    bool CCJGW_IPQ807xPhone::WlanNoiseFloorCalGenBDF(PhyID PhyId, const std::vector<unsigned short>& vChannel, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, bool updateNfToBdf)
    {
        double mask = 1024.0;
        double divisor = 2.0;
        unsigned short NFdB2_Value[16][16] = {0};
        uint nfcal_values[16] = {0};
        uint num3 = (uint)((int)chainMask | ((int)chainMaskH << 4));
        uint num4 = num3;
        uint num5 = 0u;
        uint num = 0;
        uint num2 = 0;
        num = 0;
        while (num4 != 0)
        {
            num5++;
            num4 &= num4 - 1;
            num++;
        }
        short array2[16][16] = {0};
        double array3[16][16] = {0};
        //short[,] array2 = new short[channel.Length, 8];
        //double[,] array3 = new double[channel.Length, 8];
        for (num = 0; num < vChannel.size(); num++)
        {
            for (num2 = 0; num2 < num5; num2++)
            {
                array3[num][num2] = (array2[num][num2] = -999);
                NFdB2_Value[num][num2] = 0;
            }
        }

        for (size_t num = 0;num < vChannel.size();num++)
        {
            if (0 != vChannel[num])
            {
                memset(nfcal_values,0x00,16 * sizeof(uint));
                NfCal2(PhyId,num3,vChannel[num],nfcal_values);
                for (num2 = 0; num2 < 8; num2++)
                {
                    if (1 == ((num3 >> num2) & 1))
                    {
                        if (nfcal_values[num2] >= 924)
                        {
                            array2[num][num2] = 0;
                            array3[num][num2] = 0.0;
                        }
                        else if (nfcal_values[num2] != 0)
                        {
                            array2[num][num2] = (short)((double)nfcal_values[num2] - mask);
                            array3[num][num2] = (double)array2[num][num2] / divisor;
                            NFdB2_Value[num][num2] = (unsigned short)nfcal_values[num2];
                        }
                        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"freq: %d, ChainIdx: %d, NFdB2: %d, NFdB: %d, NFReg: %d",vChannel[num],num2,array2[num][num2],array3[num][num2],NFdB2_Value[num][num2]);
                        //QTALogMgr.Debug("freq: " + channel[num] + ", ChainIdx: " + num2 + ", NFdB2: " + array2[num, num2] + ", NFdB: " + array3[num, num2] + ", NFReg: " + array[num, num2]);
                    }
                }
            }
        }
        if (updateNfToBdf)
        {
            NfCalSave(PhyId, vChannel, NFdB2_Value);
        }

        return true;
    }

    bool CCJGW_IPQ807xPhone::BoardDataGetSize(std::string& BoardDataSize)
    {
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,205);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContext);
        BoardDataSize = tlv2GetRspParam("bdSize");
        return !BoardDataSize.empty();
    }

    bool CCJGW_IPQ807xPhone::WlanSaveBDF2(BDF_OPTION saveOption, const std::string& bdfFileName)
    {
        return TRUE == QLIB_FTM_WLAN_Atheros_Save_BDF(mhResourceContext,saveOption, (char*)bdfFileName.c_str());
    }

    bool CCJGW_IPQ807xPhone::WlanSetDutTxMacAddressBssId(const std::string& rxStation, const std::string& txStation, const std::string& bssId, uint deviceIndex /* = 0 */)
    {
        PHONE->m_override_rxStation_addr = rxStation;
        PHONE->m_override_txStation_addr = txStation;
        PHONE->m_override_txBSSID = bssId;

        char override_rxStation_addr[7] = {0};
        char override_txStation_addr[7] = {0};
        char override_txBSSID[7] = {0};
        CopyMacToCharArray(override_rxStation_addr,0,rxStation);
        CopyMacToCharArray(override_txStation_addr,0,txStation);
        CopyMacToCharArray(override_txBSSID,0,bssId);

        QLIB_FTM_WLAN_TLV2_Create(mhResourceContext,1);
        tlv2AddRspParam("rxStation", override_rxStation_addr);
        tlv2AddRspParam("txStation", override_txStation_addr);
        tlv2AddRspParam("bssid", override_txBSSID);

        return true;
    }
}
