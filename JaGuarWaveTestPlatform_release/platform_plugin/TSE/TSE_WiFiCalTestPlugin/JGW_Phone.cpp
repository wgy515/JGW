#include "StdAfx.h"
#include "JGW_Phone.h"
#include "CJGW_EquipBase.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include <vector>
#include <algorithm>
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>
namespace JGW
{


    CJGW_Phone::CJGW_Phone(void) : mhUserDefinedDll(NULL),mOpenUserDefinedTransport(NULL),mCloseUserDefined(NULL),mhResourceContent(NULL),muserDefinedSend(NULL),muserDefinedReceive(NULL),muserDefinedFlushTxRx(NULL),mCalScheme(CAL_OLPC2P),mbResult(false),mnVsaAverages(1),mnpuncBwMask(0)
    {
        //! 46:46:2E:46:46:2E
        m_override_rxStation_addr = "FF:FF:FF:FF:FF:FF";
        m_override_txStation_addr = "20:22:22:22:22:02";
        m_override_txBSSID = "50:55:55:55:55:05";
    }

    CJGW_Phone::~CJGW_Phone(void)
    {
        if (mhUserDefinedDll)
        {
            DisconnectDut();
            FreeLibrary(mhUserDefinedDll);
            //mhUserDefinedDll = NULL;
            //mOpenUserDefinedTransport = NULL;
            //mCloseUserDefined = NULL;
            //muserDefinedSend = NULL;
            //muserDefinedReceive = NULL;
            //muserDefinedFlushTxRx = NULL;
        }
    }

    bool CJGW_Phone::WlanSetPhyRFMode(PhyID phyId, RFMode mode,int wlan_instance)
    {
        //QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,WCN_OP_TX);
        //Tlv2AddRspParam("rxStation", "D0:C0:C0:C0:00:00"/*m_override_rxStation_addr*/);
        //Tlv2AddRspParam("txStation", "E0:C0:C0:C0:00:00"/*m_override_txStation_addr*/);
        //Tlv2AddRspParam("bssid", "F0:C0:C0:C0:00:00"/*m_override_txBSSID*/);

        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,_OP_SETPHYRFMODE);
        //string data = Convert.ToString((int)Mode);
        Tlv2AddRspParam("phyRFMode", mode);
        //data = Convert.ToString((int)phyId);
        Tlv2AddRspParam("phyId", phyId);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        if (atoi(tlv2GetRspParam("status").c_str()) != 0)
        {
            return false;
        }

        if (-1 == wlan_instance)
        {
            return true;
        }

        return (TRUE == QLIB_FTM_WLAN_SetInstance(mhResourceContent,wlan_instance));
    }

    void CJGW_Phone::SetResourceContent(HANDLE hResourceContent)
    {
        mhResourceContent = hResourceContent;
    }

    void CJGW_Phone::SetLibraryMode(QLIB_TargetType_Enum type)
    {
        QLIB_SetLibraryMode(type);
    }

    bool CJGW_Phone::IsMobileConnected()
    {
        if ( !mhResourceContent ) return false;

        for (int i = 0;i < 10 && mhResourceContent;i ++)
        {
            if( QLIB_IsPhoneConnected( mhResourceContent ) )
            {
                return true;
            }
            Sleep(50);
        }
        return false;
    }

	void CJGW_Phone::FreeUserDefinedDll()
	{
		if (NULL == mhUserDefinedDll && !mstrUserDefinedDllFilePath.empty())
		{
			mhUserDefinedDll = GetModuleHandleA(mstrUserDefinedDllFilePath.c_str());
		}
		if (NULL != mhUserDefinedDll)
		{
			mOpenUserDefinedTransport = NULL;
			mCloseUserDefined = NULL;
			muserDefinedSend = NULL;
			muserDefinedReceive = NULL;
			muserDefinedFlushTxRx = NULL;
			FreeLibrary(mhUserDefinedDll);
			LogI_F(L"Free Library:%s",JGW_A2W(mstrUserDefinedDllFilePath).c_str());
			mhUserDefinedDll = NULL;
		}
	}

	bool CJGW_Phone::LoadUserDefinedTransportLibraryAndConnectDUT(const std::string& resourceId, const std::string& userDefinedDll,bool reloadUserdefineDll)
	{
		mstrUserDefinedDllFilePath = userDefinedDll;
		//! 断开DUT连接
		DisconnectDut();
		//! 重新加载userdefine dll
		if (reloadUserdefineDll)
		{
			FreeUserDefinedDll();
		}

		if (NULL == mhUserDefinedDll)
		{
			//! check userdefine dll is load
			mhUserDefinedDll = GetModuleHandleA(userDefinedDll.c_str());
			if (NULL == mhUserDefinedDll)
			{
				mhUserDefinedDll = ::LoadLibraryA(userDefinedDll.c_str());
			}
			if (NULL == mhUserDefinedDll)
			{
				LogE_F(L"Load User Defined Transport dll Fail(%s)",JGW_A2W(userDefinedDll).c_str());
				return false;
			}
			LogI_F(L"Load User Defined Transport dll:%s",JGW_A2W(mstrUserDefinedDllFilePath).c_str());
			//! QLIB_ConnectServer_UserDefinedTransport
			mOpenUserDefinedTransport = (OpenUserDefinedTransportFn)GetProcAddress(mhUserDefinedDll,"OpenUserDefinedTransport");
			muserDefinedSend = (userDefinedSend)GetProcAddress(mhUserDefinedDll,"UserDefinedSend");
			muserDefinedReceive = (userDefinedReceive)GetProcAddress(mhUserDefinedDll,"UserDefinedReceive");
			muserDefinedFlushTxRx = (userDefinedFlushTxRx)GetProcAddress(mhUserDefinedDll,"UserDefinedFlushTxRx");
			mCloseUserDefined = (CloseUserDefinedFn)GetProcAddress(mhUserDefinedDll,"CloseUserDefinedTransport");

			if(NULL == mhUserDefinedDll 
				|| NULL == mOpenUserDefinedTransport 
				|| NULL == muserDefinedSend
				|| NULL == muserDefinedReceive 
				|| NULL == muserDefinedFlushTxRx
				|| NULL == mCloseUserDefined)
			{
				mstrErrorMsg = "Error: Loaded User Defined Transport dll does not contain right function calls: ";
				mstrErrorMsg += userDefinedDll;
				LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,JGW_A2W(mstrErrorMsg).c_str());
				FreeLibrary(mhUserDefinedDll);
				mhUserDefinedDll = NULL;
				return false;
			}
		}

		int num = 0;
		char pIpPort[100] = {0};
		strcpy_s(pIpPort,100,resourceId.c_str());
		if ((num = mOpenUserDefinedTransport(pIpPort)) != 0)
		{
			LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,JGW_GetFormatWString(L"OpenUserDefinedTransport command failed; Return value: %d",num).c_str());
			return false;
		}
		LogI_F(L"Open User Defined Dll:%s",JGW_A2W(resourceId).c_str());
		mstrResourceId = resourceId;
		mhResourceContent = QLIB_ConnectServer_UserDefinedTransport((HANDLE)1024,muserDefinedSend,muserDefinedReceive,muserDefinedFlushTxRx,true,true);

		return mhResourceContent != NULL;
	}

    bool CJGW_Phone::DisconnectDut()
    {
        if (mhResourceContent)
        {
            QLIB_FTM_WLAN_Atheros_UNLoadDUT(mhResourceContent);
            QLIB_DisconnectServer(mhResourceContent);
			QLIB_DisconnectAllServers();
            mhResourceContent = NULL;
            if (NULL != mCloseUserDefined && !mstrResourceId.empty())
            {
                mCloseUserDefined(const_cast<char*>(mstrResourceId.data()));
				LogI_F(L"Close User Defined :%s",JGW_A2W(mstrResourceId).c_str());
            }
            mstrResourceId = "";
        }
        return true;
    }

    void CJGW_Phone::StartLogging(const std::string& strFileName)
    {
        QLIB_StartLogging(mhResourceContent,const_cast<char*>(strFileName.data()));
    }

    void CJGW_Phone::StopLogging()
    {
        QLIB_StopLogging(mhResourceContent);
    }

    void CJGW_Phone::FTM_WLAN_Atheros_Qmsl_Msg_Callback(asyncQmslMessageCB cbMsg)
    {
        QLIB_FTM_WLAN_Atheros_Qmsl_Msg_Callback(mhResourceContent,cbMsg);
    }
    
    bool CJGW_Phone::FTM_WLAN_Atheros_LoadDUT(const std::string& strDLLname, const std::string& BroadDataFName, int eNvmemOption,size_t ssid)
    {
        //!QLIB_FTM_WLAN_Atheros_UNLoadDUT(mhResourceContent);
        return (TRUE == QLIB_FTM_WLAN_Atheros_LoadDUT(mhResourceContent,(unsigned char*)(strDLLname.c_str()),(unsigned char*)(BroadDataFName.c_str()),eNvmemOption,ssid));;
    }

    bool CJGW_Phone::FTM_WLAN_Atheros_xtal_CAL_INIT(int iXtalTargetPPM, uint iXtalTolerancePPM, int iXtalCapDelta, WLAN_XTAL_STORAGE_OTP dXtalCalSaveOption, uint delayCapSettleMs /* = 1000 */, uint delayPostXtalMs /* = 2000 */)
    {
        return (TRUE == QLIB_FTM_WLAN_Atheros_xtal_CAL_INIT(mhResourceContent,iXtalTargetPPM,iXtalTolerancePPM,iXtalCapDelta,dXtalCalSaveOption,delayCapSettleMs,delayPostXtalMs));
    }

    bool CJGW_Phone::SetMacAddress(unsigned char* aiMAC_Address)
    {
        bool result = true;
        result &= TRUE == QLIB_FTM_WLAN_TLV_Create(mhResourceContent,16);
        result &= TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"NVMEM_actions", "6");
        result &= TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"NVMEM_MAC", (char*)aiMAC_Address);
        result &= TRUE == QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
        return result;
    }

    bool CJGW_Phone::TxCalMemCommit(byte itemplate, byte iWriteOption, byte iMemSize, byte iCompress, byte iOverwrite, byte* section)
    {
        uint iMemSize2 = 0u;
        if (iMemSize == 0)
        {
            iMemSize2 = 0u;
        }
        else if (iMemSize == 1)
        {
            iMemSize2 = 1024u;
        }
        else if (iMemSize == 2)
        {
            iMemSize2 = 2048u;
        }
        else if (iMemSize == 3)
        {
            iMemSize2 = 4096u;
        }
        else if (iMemSize == 4)
        {
            iMemSize2 = 8192u;
        }
        int num = 0;
        int num2 = 0;
        while (num2 < 32 && section[num2] != 0)
        {
            num |= 1 << (int)section[num2];
            num2++;
        }
        uint iSectionMask = (uint)((unsigned short)num);
        return TRUE == QLIB_FTM_WLAN_Atheros_MEM_COMMIT(mhResourceContent,itemplate, iWriteOption, iMemSize2, iCompress, iOverwrite, iSectionMask);
    }

    int CJGW_Phone::GetSISOChainNumber(WLAN_SINGLE_CHAIN_MASK mask)
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
            LogE(L"Chain mask is not mapped");
        }
        return 1;
    }

    double asyncFreqErrCB(uint capValue, uint freq, double powerLevel, uint avgNum)
    {
        std::vector<double> vTestValue;
        //! 测试数据有效数
        int validCount = 0;
        double sum = 0,avg = -999.0;

        for (size_t i = 0;i < avgNum;i ++)
        {
            //int pathNameIndex = GetSISOChainNumber(m_txChainMask);
            double val = PHONE->mWlanDutWrapper.GetCWFreqErr(freq,powerLevel);
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
        //! "FreqErr:",
        //! string.Format("{0:0.00}", num),"   ChannelFreq: ",freq,"    txchainMask: ",m_txChainMask"    capOutValue:",capValue
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%d\t%.2f\t%.2f\t%d\t%d",freq,powerLevel,avg,PHONE->m_txChainMask,capValue);
        return avg;
    }

    void CJGW_Phone::XtalCalibration(size_t channel, WLAN_SINGLE_CHAIN_MASK txChainMask, size_t gainIndex, int dacGain, int paCfg, uint numAverages, size_t& xtalCap, double& freqErrPPM)
    {
        xtalCap = 0;
        freqErrPPM = -999.0;

        //int pathNameIndex = GetSISOChainNumber(txChainMask);
        m_txChainMask = txChainMask;
        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,3);
        tlvAddRspParam("channel",channel);
        tlvAddRspParam("txMode",EnablingContinuousUnmodulatedTX);
        tlvAddRspParam("wlanMode",TCMD_WLAN_MODE_NOHT);
        tlvAddRspParam("txChain0",m_txChainMask);
        tlvAddRspParam("tpcm","4");
        tlvAddRspParam("gainIdx",gainIndex);
        tlvAddRspParam("dacGain",dacGain);
        tlvAddRspParam("paConfig",paCfg);
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);

		PHONE->mWlanDutWrapper.SwitchLossPathToChainMask(true,txChainMask);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"ChannelFreq\tPowerLevel\tFreqErr\tTxchainMask\tCapOutValue");
        QLIB_FTM_WLAN_Atheros_xtal_CAL(mhResourceContent,asyncFreqErrCB, numAverages);

        char szbuf[1024] = {0};
        QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContent,"XTAL_CAP_VALUE",szbuf);
        xtalCap = atoi(szbuf);

        memset(szbuf,0x00,1024);
        QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContent,"XTAL_FREQERRPPM",szbuf);
        freqErrPPM = atof(szbuf);
    }

    bool asyncSetChainCB(int chainMask)
    {
        PHONE->m_txChainMask = chainMask;
		PHONE->mWlanDutWrapper.SwitchLossPathToChainMask(true,chainMask);
        return true;
    }

    double AveragePowResult(std::vector<double> vdTestValue)
    {
        if (vdTestValue.empty())
        {
            PHONE->mbResult = false;
            return -999.9;
        }
        double averageResult = 0.0;
        double num = 20.0;
        int num2 = 0;
        for (size_t i = 0; i < vdTestValue.size(); i++)
        {

            if (vdTestValue[i] > -999.0 && (i != 0 || vdTestValue.size() <= 1 || std::abs(vdTestValue[0] - vdTestValue[1]) <= 3.0))
            {
                num2++;
                averageResult += std::pow(10.0, vdTestValue[i] / num);
            }
        }

        if (num2 <= 0)
        {
            averageResult = -999.0;
            return averageResult;
        }
        averageResult /= (double)num2;
        if (0.0 != averageResult)
        {
            averageResult = num * std::log10(averageResult);
            return averageResult;
        }
        averageResult = 0.0;
        return averageResult;
    }

    double GetFineTuningPower(unsigned int iGain,unsigned int iFreq,double dPowerLevel,double avgVal) 
    {
        //FINE_TUNING_POWER
        std::map<unsigned int,std::vector<S_XtalCalibration_Result>>::iterator it = PHONE->mmapFreqS_XtalCalibration_Result.find(iFreq);
        if (it == PHONE->mmapFreqS_XtalCalibration_Result.end())
        {
            std::vector<S_XtalCalibration_Result> vsXtalCalibration_Result;
            S_XtalCalibration_Result sXtalCalibration_Result = {true,dPowerLevel,avgVal,avgVal,iGain,(int)PHONE->m_txChainMask};
            vsXtalCalibration_Result.push_back(sXtalCalibration_Result);
            PHONE->mmapFreqS_XtalCalibration_Result.insert(std::make_pair(iFreq,vsXtalCalibration_Result));
            return avgVal;
        }
        for (std::vector<S_XtalCalibration_Result>::iterator itvec = it->second.begin();
            itvec != it->second.end();
            ++ itvec)
        {
            //int meastmp = itvec->mfMeasPower;
            //int avgValTmp = avgVal;
            if (itvec->mnGain == iGain && itvec->mnTxChainMask == (int)PHONE->m_txChainMask && std::abs(itvec->mfMeasPower - avgVal) < 0.2)
            {
                int i = rand() % 40;
                if (i > 20)
                {
                    return itvec->mfMeasPower - FINE_TUNING_POWER * (i - 20);
                }
                else
                {
                    return itvec->mfMeasPower + FINE_TUNING_POWER * i;
                }
                //itvec->mfChangeMeasPower = itvec->mfMeasPower;
                // 查过0.5范围
                //if (itvec->mfChangeMeasPower < itvec->mfMeasPower - 0.2)
                //{
                //    itvec->mbAutoDecrement = false;
                //}
                //if (itvec->mfChangeMeasPower > itvec->mfMeasPower + 0.2)
                //{
                //    itvec->mbAutoDecrement = true;
                //}
                //if (itvec->mbAutoDecrement)
                //{
                //    itvec->mfChangeMeasPower -= FINE_TUNING_POWER;
                //}
                //else
                //{
                //    itvec->mfChangeMeasPower += FINE_TUNING_POWER;
                //}
                //return itvec->mfChangeMeasPower;
            }
        }
        //it->second.clear();
        S_XtalCalibration_Result sXtalCalibration_ResultTmp = {true,dPowerLevel,avgVal,avgVal,iGain,(int)PHONE->m_txChainMask};
        it->second.push_back(sXtalCalibration_ResultTmp);
        return sXtalCalibration_ResultTmp.mfMeasPower;
    }
        
        
    double asyncPMCB(unsigned int iGain
        ,unsigned int iFreq
        ,double dPowerLevel
        ,unsigned int iNumAvg
        ,unsigned int iTriggerType
        //! for measure CW tone, set iTriggerLevel = -1000
        ,int iTriggerLevel)
    {
        double avgVal = 999.0;
        std::vector<double> vdTestValue;
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
        //avgVal = GetFineTuningPower(iGain,iFreq,dPowerLevel,avgVal);
        if (iTriggerLevel <= -1000 && iTriggerType == 0u)
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"CWGoal:%.2f,MeasPwr:%.2f,gain:%d,ChannelFreq:%d,txchainMask:%d(%.2f)",dPowerLevel,avgVal,iGain,iFreq,(int)PHONE->m_txChainMask,tmpAvgVal);
        }
        else
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Goal:%.2f,MeasPwr:%.2f,gain:%d,ChannelFreq:%d,txchainMask:%d(%.2f)",dPowerLevel,avgVal,iGain,iFreq,(int)PHONE->m_txChainMask,tmpAvgVal);
        }
        return avgVal;
    }

    bool asyncRFSCB(int chainMask)
    {
        //! m_txChainMask = chainMask;
        return true;
    }

    WLAN_tlvDataRate getTLVdataRate(WLAN_DataRate rate, bool short11b_nGuard, TCMD_WLAN_MODE& wlanMode)
    {
        wlanMode = TCMD_WLAN_MODE_HT20;
        if (rate <= RATE_11Mbps)
        {
            wlanMode = TCMD_WLAN_MODE_CCK;
            if (rate == RATE_1Mbps)
            {
                return tlvRATE_1Mbps;
            }
            if (rate == RATE_2Mbps)
            {
                if (!short11b_nGuard)
                {
                    return tlvRATE_2Mbps_L;
                }
                return tlvRATE_2Mbps_S;
            }
            else if (rate == RATE_5_5Mbps)
            {
                if (!short11b_nGuard)
                {
                    return tlvRATE_5_5Mbps_L;
                }
                return tlvRATE_5_5Mbps_S;
            }
            else if (rate == RATE_11Mbps)
            {
                if (!short11b_nGuard)
                {
                    return tlvRATE_11Mbps_L;
                }
                return tlvRATE_11Mbps_S;
            }
        }
        if (rate >= RATE_6Mbps && rate <= RATE_54Mbps)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
            return (WLAN_tlvDataRate)(rate - 4 + 8);
        }
        if (rate >= RATE_MCS_0_20 && rate <= RATE_MCS_7_20)
        {
            wlanMode = TCMD_WLAN_MODE_HT20;
            return (WLAN_tlvDataRate)(rate - 12 + 16);
        }
        if (rate >= RATE_MCS_0_40 && rate <= RATE_MCS_7_40)
        {
            wlanMode = TCMD_WLAN_MODE_HT40PLUS;
            return (WLAN_tlvDataRate)(rate - 20 + 24);
        }
        if (rate >= RATE_MCS_8_20 && rate <= RATE_MCS_15_20)
        {
            wlanMode = TCMD_WLAN_MODE_HT20;
            return (WLAN_tlvDataRate)(rate - 58 + 32);
        }
        if (rate >= RATE_MCS_8_40 && rate <= RATE_MCS_15_40)
        {
            wlanMode = TCMD_WLAN_MODE_HT40PLUS;
            return (WLAN_tlvDataRate)(rate - 66 + 40);
        }
        if (rate >= RATE_MCS_16_20 && rate <= RATE_MCS_23_20)
        {
            wlanMode = TCMD_WLAN_MODE_HT20;
            return (WLAN_tlvDataRate)(rate - 104 + 48);
        }
        if (rate >= RATE_MCS_24_20 && rate <= RATE_MCS_31_20)
        {
            wlanMode = TCMD_WLAN_MODE_HT20;
            return (WLAN_tlvDataRate)(rate - 220 + 256);
        }
        if (rate >= RATE_MCS_16_40 && rate <= RATE_MCS_23_40)
        {
            wlanMode = TCMD_WLAN_MODE_HT40PLUS;
            return (WLAN_tlvDataRate)(rate - 112 + 56);
        }
        if (rate >= RATE_MCS_24_40 && rate <= RATE_MCS_31_40)
        {
            wlanMode = TCMD_WLAN_MODE_HT40PLUS;
            return (WLAN_tlvDataRate)(rate - 228 + 264);
        }
        if (rate >= RATE_AC_MCS_0_20 && rate <= RATE_AC_MCS_9_20)
        {
            wlanMode = TCMD_WLAN_MODE_VHT20;
            return (WLAN_tlvDataRate)(rate - 28 + 64);
        }
        if (rate >= RATE_AC_MCS_0_40 && rate <= RATE_AC_MCS_9_40)
        {
            wlanMode = TCMD_WLAN_MODE_VHT40PLUS;
            return (WLAN_tlvDataRate)(rate - 38 + 76);
        }
        if (rate >= RATE_AC_MCS_0_80 && rate <= RATE_AC_MCS_9_80)
        {
            wlanMode = TCMD_WLAN_MODE_VHT80_0;
            return (WLAN_tlvDataRate)(rate - 48 + 88);
        }
        if (rate >= RATE_AC_MCS_10_20 && rate <= RATE_AC_MCS_19_20)
        {
            wlanMode = TCMD_WLAN_MODE_VHT20;
            return (WLAN_tlvDataRate)(rate - 74 + 100);
        }
        if (rate >= RATE_AC_MCS_10_40 && rate <= RATE_AC_MCS_19_40)
        {
            wlanMode = TCMD_WLAN_MODE_VHT40PLUS;
            return (WLAN_tlvDataRate)(rate - 84 + 112);
        }
        if (rate >= RATE_AC_MCS_10_80 && rate <= RATE_AC_MCS_19_80)
        {
            wlanMode = TCMD_WLAN_MODE_VHT80_0;
            return (WLAN_tlvDataRate)(rate - 94 + 124);
        }
        if (rate >= RATE_AC_MCS_20_20 && rate <= RATE_AC_MCS_29_20)
        {
            wlanMode = TCMD_WLAN_MODE_VHT20;
            return (WLAN_tlvDataRate)(rate - 120 + 136);
        }
        if (rate >= RATE_AC_MCS_20_40 && rate <= RATE_AC_MCS_29_40)
        {
            wlanMode = TCMD_WLAN_MODE_VHT40PLUS;
            return (WLAN_tlvDataRate)(rate - 130 + 148);
        }
        if (rate >= RATE_AC_MCS_20_80 && rate <= RATE_AC_MCS_29_80)
        {
            wlanMode = TCMD_WLAN_MODE_VHT80_0;
            return (WLAN_tlvDataRate)(rate - 140 + 160);
        }
        if (rate >= RATE_AC_MCS_30_20 && rate <= RATE_AC_MCS_39_20)
        {
            wlanMode = TCMD_WLAN_MODE_VHT20;
            return (WLAN_tlvDataRate)(rate - 150 + 192);
        }
        if (rate >= RATE_AC_MCS_30_40 && rate <= RATE_AC_MCS_39_40)
        {
            wlanMode = TCMD_WLAN_MODE_VHT40PLUS;
            return (WLAN_tlvDataRate)(rate - 160 + 202);
        }
        if (rate >= RATE_AC_MCS_30_80 && rate <= RATE_AC_MCS_39_80)
        {
            wlanMode = TCMD_WLAN_MODE_VHT80_0;
            return (WLAN_tlvDataRate)(rate - 170 + 212);
        }
        if (rate >= RATE_AC_MCS_0_160 && rate <= RATE_AC_MCS_19_160)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
            return (WLAN_tlvDataRate)(rate - 180 + 224);
        }
        if (rate >= RATE_P_1p5Mbps_5 && rate <= RATE_P_13p5Mbps_5)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
            return (WLAN_tlvDataRate)(rate - 1004 + 8);
        }
        if (rate >= RATE_P_3Mbps_10 && rate <= RATE_P_27Mbps_10)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
            return (WLAN_tlvDataRate)(rate - 1104 + 8);
        }
        if (rate >= RATE_P_6Mbps_20 && rate <= RATE_P_54Mbps_20)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
            return (WLAN_tlvDataRate)(rate - 1204 + 8);
        }
        ///throw new WLANCustomException("Unknown rate: " + rate.ToString());
        LogE_F(L"Unknown rate:%d",(int)rate);
        PHONE->mbResult = false;
        return (WLAN_tlvDataRate)(rate - 12 + 16);
    }

    bool CJGW_Phone::TxCalibrationFWControlled(bool AllChainOn, bool DebugInfo, std::vector<int>& iMaxIteration, size_t numAverages,int nVsaAverages)
    {
        mmapFreqS_XtalCalibration_Result.clear();
        mbResult = true;
        mnWlanRate = RATE_11A_6_MBPS;
        mnVsaAverages = nVsaAverages;
        return (TRUE == QLIB_FTM_WLAN_Atheros_Tx_FW_CAL(mhResourceContent,asyncSetChainCB, asyncPMCB, numAverages, AllChainOn, asyncRFSCB, DebugInfo)) && mbResult;
    }

    void CJGW_Phone::GetTgtPwr(size_t freq, size_t rateBitIdx, size_t userMode, PhyID phyId, size_t& tgtPwr)
    {
        tgtPwr = 0u;
        TCMD_WLAN_MODE tcmd_WLAN_MODE;
        WLAN_tlvDataRate tlvdataRate = getTLVdataRate((WLAN_DataRate)rateBitIdx, false, tcmd_WLAN_MODE);

        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,132);
        tlvAddRspParam("freq",freq);
        tlvAddRspParam("rateBit",tlvdataRate);
        tlvAddRspParam("userMode",userMode);
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);

        char szbuf[1024] = {0};
        QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContent,"tgtPwr",szbuf);
        tgtPwr = atoi(szbuf);
    }

    void CJGW_Phone::SaveTxSettings(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN)
    {
        txSettings.channel = channel;
        txSettings.channel2 = channel2;
        txSettings.cbState = cbState;
        txSettings.rate = rate;
        txSettings.pwrMode = pwrMode;
        txSettings.txMode = txMode;
        txSettings.txChainMask = txChainMask;
        txSettings.powerLevel = powerLevel;
        txSettings.gain = gain;
        txSettings.digitalGain = digitalGain;
        txSettings.PASetting = PASetting;
        txSettings.payLoadSize = payLoadSize;
        txSettings.short11b_nGuard = short11b_nGuard;
        txSettings.ldpcRate = ldpcRate;
        txSettings.bSTBCmode = bSTBCmode;
        txSettings.aggregation = aggregation;
        txSettings.dpdMode = dpdMode;
        txSettings.HC_Enable = HC_Enable;
        txSettings.ifs = ifs;
        txSettings.dutyCycle = dutyCycle;
        txSettings.nPattern = nPattern;
		txSettings.antenna = 0;
        //txSettings.pattern = pattern;
        for (size_t i = 0;i < pattern.size();i++)
        {
            txSettings.pattern.push_back(pattern[i]);
        }

        txSettings.broadcastMode = broadcastMode;
        txSettings.nPacket = nPacket;
        txSettings.iAIFSN = iAIFSN;
    }

    bool asyncRxGainCalDoneCB(unsigned int status)
    {
        //PHONE->mbResult = (TRUE == status);
        //return PHONE->mbResult;
        return true;
    }

    int asyncSGCB(int inputSignalStrength_dBm, unsigned int freq, unsigned int rate, unsigned int numPackets, unsigned int chain)
    {
        int chainMask = 1 << (int)chain;
        //! uint sigGenTransmit(int inputSignalStrength_dBm, uint freq, Phone.WLAN_DataRate rate, uint numPackets, uint chain)
        bool short11b_nGuard = true;
        WLAN_RATE rate2;
        WLAN_Gen6_PHYDBG_PreambleRate wlan_Gen6_PHYDBG_PreambleRate;
        mapWlanRate((WLAN_DataRate)rate, short11b_nGuard,  rate2, wlan_Gen6_PHYDBG_PreambleRate, 0);
        GETINSTRUMENT->InitRxCalibration(chainMask,rate2,freq,inputSignalStrength_dBm,numPackets);

        int num2 = 3;
        while (!GETINSTRUMENT->CheckVSGSentDone())
        {
            Sleep(10);
            num2--;
            if (num2 <= 0)
            {
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Error: timed out waiting for VSG to idle");
                PHONE->mbResult = false;
                //! throw new WLANCustomException("Error: timed out waiting for VSG to idle");
            }
        }
        return 0;
    }

    bool CJGW_Phone::RxCalibrationFWControlled(uint radioId, uint chainToCalibrate, uint band)
    {
        return (TRUE == QLIB_FTM_WLAN_Atheros_Rx_FW_CAL(mhResourceContent,asyncSGCB,asyncRxGainCalDoneCB,radioId,chainToCalibrate,band) && PHONE->mbResult);
    }

    /* void CJGW_Phone::TurnOffTX(uint channel, WLAN_CHAIN_MASK2 txChain)
    {
    uint value = channel;
    QLIB_FTM_WLAN_TLV_Create(mhResourceContent,1);
    std::string temp;
    JGW_FormatString(temp,"%d",value);
    QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"channel",const_cast<char*>(temp.data()));

    JGW_FormatString(temp,"%d",0);
    QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"txMode",const_cast<char*>(temp.data()));

    QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
    }*/

    bool isVHT80Rate(WLAN_DataRate wlanRate)
    {
        return (wlanRate >= RATE_AC_MCS_0_80 && wlanRate <= RATE_AC_MCS_9_80) || (wlanRate >= RATE_AC_MCS_10_80 && wlanRate <= RATE_AC_MCS_19_80) || (wlanRate >= RATE_AC_MCS_20_80 && wlanRate <= RATE_AC_MCS_29_80) || (wlanRate >= RATE_AC_MCS_30_80 && wlanRate <= RATE_AC_MCS_39_80);
    }

    bool isVHT40Rate(WLAN_DataRate wlanRate)
    {
        return (wlanRate >= RATE_AC_MCS_0_40 && wlanRate <= RATE_AC_MCS_9_40) || (wlanRate >= RATE_AC_MCS_10_40 && wlanRate <= RATE_AC_MCS_19_40) || (wlanRate >= RATE_AC_MCS_20_40 && wlanRate <= RATE_AC_MCS_29_40) || (wlanRate >= RATE_AC_MCS_30_40 && wlanRate <= RATE_AC_MCS_39_40);
    }

    // Token: 0x0600088F RID: 2191 RVA: 0x00073C69 File Offset: 0x00071E69
    bool isVHT20Rate(WLAN_DataRate wlanRate)
    {
        return (wlanRate >= RATE_AC_MCS_0_20 && wlanRate <= RATE_AC_MCS_9_20) || (wlanRate >= RATE_AC_MCS_10_20 && wlanRate <= RATE_AC_MCS_19_20) || (wlanRate >= RATE_AC_MCS_20_20 && wlanRate <= RATE_AC_MCS_29_20) || (wlanRate >= RATE_AC_MCS_30_20 && wlanRate <= RATE_AC_MCS_39_20);
    }

    bool isHT40Rate(WLAN_DataRate wlanRate)
    {
        return (wlanRate >= RATE_MCS_0_40 && wlanRate <= RATE_MCS_7_40) || (wlanRate >= RATE_MCS_8_40 && wlanRate <= RATE_MCS_15_40) || (wlanRate >= RATE_MCS_16_40 && wlanRate <= RATE_MCS_23_40) || (wlanRate >= RATE_MCS_24_40 && wlanRate <= RATE_MCS_31_40);
    }

    bool isLegacyCCK(WLAN_DataRate wlanRate)
    {
        return wlanRate >= RATE_1Mbps && wlanRate <= RATE_11Mbps;
    }

    bool CJGW_Phone::mapWlanMode(WLAN_DataRate wlanRate, WLAN_Gen6_ChannelBondingState cbState, TCMD_WLAN_MODE& wlanMode)
    {
        wlanMode = TCMD_WLAN_MODE_HT20;
        if (cbState == BW160 || cbState == BW80p80 || cbState == Primary_1st_20_in_BW80p80 || cbState == Primary_2nd_20_in_BW80p80 || cbState == Primary_3rd_20_in_BW80p80 || cbState == Primary_4th_20_in_BW80p80 || cbState == Primary_5th_20_in_BW80p80 || cbState == Primary_6th_20_in_BW80p80 || cbState == Primary_7th_20_in_BW80p80 || cbState == Primary_8th_20_in_BW80p80 || cbState == Primary_1st_20_in_BW160 || cbState == Primary_2nd_20_in_BW160 || cbState == Primary_3rd_20_in_BW160 || cbState == Primary_4th_20_in_BW160 || cbState == Primary_5th_20_in_BW160 || cbState == Primary_6th_20_in_BW160 || cbState == Primary_7th_20_in_BW160 || cbState == Primary_8th_20_in_BW160)
        {
            wlanMode = (TCMD_WLAN_MODE)cbState;
        }
        else if (cbState == BW80_20_40GHigh_40_80High || cbState == BW80_20_40High_40_80Low || cbState == BW80_20_40Low_40_80High || cbState == BW80_20_40Low_40_80Low)
        {
            wlanMode = (TCMD_WLAN_MODE)(cbState + 1);
        }
        else if (cbState == primaryHigh || cbState == primaryLow)
        {
            if (isVHT80Rate(wlanRate))
            {
                LogE(L"VHT80 rate can't has 40MHz mode");
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"VHT80 rate can't has 40MHz mode");
                mbResult = false;
            }
            if (isVHT40Rate(wlanRate) || isVHT20Rate(wlanRate))
            {
                if (cbState == primaryHigh)
                {
                    wlanMode = TCMD_WLAN_MODE_VHT40PLUS;
                }
                else
                {
                    wlanMode = TCMD_WLAN_MODE_VHT40MINUS;
                }
            }
            else if (cbState == primaryHigh)
            {
                wlanMode = TCMD_WLAN_MODE_HT40PLUS;
            }
            else
            {
                wlanMode = TCMD_WLAN_MODE_HT40MINUS;
            }
        }
        else if (cbState == primary20)
        {
            if (isVHT80Rate(wlanRate) || isVHT40Rate(wlanRate) || isHT40Rate(wlanRate))
            {
                LogE(L"VHT80/VHT40/HT40 rate can't has 20MHz mode");
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"VHT80/VHT40/HT40 rate can't has 20MHz mode");
                mbResult = false;
            }
            if (isVHT20Rate(wlanRate))
            {
                wlanMode = TCMD_WLAN_MODE_VHT20;
            }
            else
            {
                wlanMode = TCMD_WLAN_MODE_HT20;
            }
        }
        else if (cbState == WLAN_Gen6_ChannelBondingState_none)
        {
            if (isLegacyCCK(wlanRate))
            {
                wlanMode = TCMD_WLAN_MODE_CCK;
            }
            else
            {
                wlanMode = TCMD_WLAN_MODE_NOHT;
            }
        }
        else if (cbState == Quarter_Rate || cbState == Half_Rate)
        {
            wlanMode = TCMD_WLAN_MODE_NOHT;
        }
        return true;
    }

    void CJGW_Phone::SetUpTxDefault(uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate,WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable)
    {
        uint num = 1u;
        uint num2 = 2u;
        uint num3 = 4u;
        uint num4 = 128u;
		TCMD_WLAN_MODE value = TCMD_WLAN_MODE_HT20;

        tlvAddRspParam("channel",centFreqMHz);
        if (centFreq2MHz != 0u)
        {
            tlvAddRspParam("channel2",centFreq2MHz);
        }

        tlvAddRspParam("txMode",txMode);

		mapWlanMode(rate, cbState, value);
		tlvAddRspParam("wlanMode",value);

		std::string data;
		if (cbState == Half_Rate)
		{
			tlvAddRspParam("bandwidth","50");
			cbState = WLAN_Gen6_ChannelBondingState_none;
		}
		else if (cbState == Quarter_Rate)
		{
			tlvAddRspParam("bandwidth","51");
			cbState = WLAN_Gen6_ChannelBondingState_none;
		}
		else
		{
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"bandwidth","0");
		}

		switch (txSettings.pwrMode)
		{
		case ForcedDesiredGain:
		case ForcedGain:
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"tpcm","4");
			tlvAddRspParam("gainIdx",txSettings.gain);
			tlvAddRspParam("dacGain",txSettings.digitalGain);
			tlvAddRspParam("paConfig",txSettings.PASetting);
			break;
		case ForcedGLUTIndex:
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"tpcm","6");
			tlvAddRspParam("gainIdx",txSettings.gain);
			break;
		case TxPowerAuto:
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"tpcm","2");
			break;
		case TxPowerForce_OLPC:
		case TxPowerForce_CLPC:
		case TxPowerForce_SCPC:
			JGW_FormatString(data,"%.0f",txSettings.powerLevel);
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"txPower0",const_cast<char*>(data.data()));
			QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"tpcm","0");
			break;
		default:
			mbResult = false;
			LogE(L"QC99xx doesn't support selected pwrMode");
			LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"QC99xx doesn't support selected pwrMode");
		}

       
        WLAN_tlvDataRate tlvdataRate = getTLVdataRate(rate, short11b_nGuard, value);
        tlvAddRspParam("rateBitIndex0",tlvdataRate);
		tlvAddRspParam("enANI",0);
		QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"scramblerOff",scramblerDisable?"1":"0");
        tlvAddRspParam("aifsn",iAIFSN);
		tlvAddRspParam("agg",aggregation);
		tlvAddRspParam("dutyCycle",dutyCycle);
        tlvAddRspParam("pktLen0",payLoadSize);
		tlvAddRspParam("antenna", txSettings.antenna );
        tlvAddRspParam("txChain0",txChainMask);
        QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"broadcast",broadcastMode?"1":"0");
        tlvAddRspParam("shortGuard",short11b_nGuard?1:0);
        tlvAddRspParam("numPackets",nPacket);
        
        //QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"rxStation",const_cast<char*>(m_override_rxStation_addr.data()));
        //QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"txStation",const_cast<char*>(m_override_txStation_addr.data()));
       // QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,"bssid",const_cast<char*>(m_override_txBSSID.data()));
        /*this.tlvAddRspParam("rxStation", WlanTestSharedInfo.override_rxStation_addr);
        this.tlvAddRspParam("txStation", WlanTestSharedInfo.override_txStation_addr);
        this.tlvAddRspParam("bssid", WlanTestSharedInfo.override_txBSSID);*/
       
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
        if (HC_Enable == HCS_Enabled)
        {
            num5 |= num4;
        }
		tlvAddRspParam("txPattern",nPattern);
        tlvAddRspParam("flags",num5);
    }

    void CJGW_Phone::setUpTxDetailCommon(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable)
    {
        SaveTxSettings(channel, channel2, cbState, rate, pwrMode, txMode, txChainMask, powerLevel, gain, digitalGain, PASetting, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN);
        //TurnOffTX(channel, txChainMask);

        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,1);
        
        SetUpTxDefault(channel, channel2, cbState, rate, txMode, txChainMask, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, scramblerDisable);
    }

    bool CJGW_Phone::SetUpTxDetail(uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_DataRate rate, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, double& txPower)
    {
        txPower = powerLevel;
        mbResult = true;

        setUpTxDetailCommon(channel, channel2, cbState, rate, pwrMode, txMode, txChainMask, powerLevel, gain, digitalGain, PASetting, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, scramblerDisable);
        mbResult = TRUE == QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
        if (pwrMode == TxPowerAuto)
        {
            uint num = 0u;
            GetTgtPwr(channel, (uint)rate, 0u, PhyID_A,num);
            txPower = num / 2.0;
        }
        return mbResult;
    }

    bool CJGW_Phone::TurnOffTX(uint channel, WLAN_CHAIN_MASK2 txChain)
    {
        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,1);
        tlvAddRspParam("channel",channel);
        tlvAddRspParam("txMode","0");
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
        return true;
    }

    void mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2 /*= 0u*/)
    {
        bool flag = (channel2 != 0) ? true : false;
        switch (wlanRate)
        {
        case RATE_1Mbps:
        case RATE_2Mbps:
        case RATE_5_5Mbps:
        case RATE_11Mbps:
            if (short11b_nGuard)
            {
                preambleType = PREAMBLE_SHORTB_11B;
            }
            else
            {
                preambleType = PREAMBLE_LONGB_11B;
            }
            break;
        case RATE_6Mbps:
        case RATE_9Mbps:
        case RATE_12Mbps:
        case RATE_18Mbps:
        case RATE_24Mbps:
        case RATE_36Mbps:
        case RATE_48Mbps:
        case RATE_54Mbps:
            preambleType = PREAMBLE_OFDM_11G;
            break;
        default:
            preambleType = PREAMBLE_MIXED_11N;
            break;
        }
        switch (wlanRate)
        {
        case RATE_1Mbps:
            rate = RATE_11B_LONG_1_MBPS;
            break;
        case RATE_2Mbps:
            if (short11b_nGuard)
            {
                rate = RATE_11B_SHORT_2_MBPS;
            }
            else
            {
                rate = RATE_11B_LONG_2_MBPS;
            }
            break;
        case RATE_5_5Mbps:
            if (short11b_nGuard)
            {
                rate = RATE_11B_SHORT_5_5_MBPS;
            }
            else
            {
                rate = RATE_11B_LONG_5_5_MBPS;
            }
            break;
        case RATE_11Mbps:
            if (short11b_nGuard)
            {
                rate = RATE_11B_SHORT_11_MBPS;
            }
            else
            {
                rate = RATE_11B_LONG_11_MBPS;
            }
            break;
        case RATE_6Mbps:
            rate = RATE_11A_6_MBPS;
            break;
        case RATE_9Mbps:
            rate = RATE_11A_9_MBPS;
            break;
        case RATE_12Mbps:
            rate = RATE_11A_12_MBPS;
            break;
        case RATE_18Mbps:
            rate = RATE_11A_18_MBPS;
            break;
        case RATE_24Mbps:
            rate = RATE_11A_24_MBPS;
            break;
        case RATE_36Mbps:
            rate = RATE_11A_36_MBPS;
            break;
        case RATE_48Mbps:
            rate = RATE_11A_48_MBPS;
            break;
        case RATE_54Mbps:
            rate = RATE_11A_54_MBPS;
            break;
        case RATE_MCS_0_20:
            rate = RATE_11N_HT20_MCS0;
            break;
        case RATE_MCS_1_20:
            rate = RATE_11N_HT20_MCS1;
            break;
        case RATE_MCS_2_20:
            rate = RATE_11N_HT20_MCS2;
            break;
        case RATE_MCS_3_20:
            rate = RATE_11N_HT20_MCS3;
            break;
        case RATE_MCS_4_20:
            rate = RATE_11N_HT20_MCS4;
            break;
        case RATE_MCS_5_20:
            rate = RATE_11N_HT20_MCS5;
            break;
        case RATE_MCS_6_20:
            rate = RATE_11N_HT20_MCS6;
            break;
        case RATE_MCS_7_20:
            rate = RATE_11N_HT20_MCS7;
            break;
        case RATE_MCS_8_20:
            rate = RATE_11N_HT20_MCS8;
            break;
        case RATE_MCS_9_20:
            rate = RATE_11N_HT20_MCS9;
            break;
        case RATE_MCS_10_20:
            rate = RATE_11N_HT20_MCS10;
            break;
        case RATE_MCS_11_20:
            rate = RATE_11N_HT20_MCS11;
            break;
        case RATE_MCS_12_20:
            rate = RATE_11N_HT20_MCS12;
            break;
        case RATE_MCS_13_20:
            rate = RATE_11N_HT20_MCS13;
            break;
        case RATE_MCS_14_20:
            rate = RATE_11N_HT20_MCS14;
            break;
        case RATE_MCS_15_20:
            rate = RATE_11N_HT20_MCS15;
            break;
        case RATE_MCS_16_20:
            rate = RATE_11N_HT20_MCS16;
            break;
        case RATE_MCS_17_20:
            rate = RATE_11N_HT20_MCS17;
            break;
        case RATE_MCS_18_20:
            rate = RATE_11N_HT20_MCS18;
            break;
        case RATE_MCS_19_20:
            rate = RATE_11N_HT20_MCS19;
            break;
        case RATE_MCS_20_20:
            rate = RATE_11N_HT20_MCS20;
            break;
        case RATE_MCS_21_20:
            rate = RATE_11N_HT20_MCS21;
            break;
        case RATE_MCS_22_20:
            rate = RATE_11N_HT20_MCS22;
            break;
        case RATE_MCS_23_20:
            rate = RATE_11N_HT20_MCS23;
            break;
        case RATE_MCS_0_40:
            rate = RATE_11N_HT40_MCS0;
            break;
        case RATE_MCS_1_40:
            rate = RATE_11N_HT40_MCS1;
            break;
        case RATE_MCS_2_40:
            rate = RATE_11N_HT40_MCS2;
            break;
        case RATE_MCS_3_40:
            rate = RATE_11N_HT40_MCS3;
            break;
        case RATE_MCS_4_40:
            rate = RATE_11N_HT40_MCS4;
            break;
        case RATE_MCS_5_40:
            rate = RATE_11N_HT40_MCS5;
            break;
        case RATE_MCS_6_40:
            rate = RATE_11N_HT40_MCS6;
            break;
        case RATE_MCS_7_40:
            rate = RATE_11N_HT40_MCS7;
            break;
        case RATE_MCS_8_40:
            rate = RATE_11N_HT40_MCS8;
            break;
        case RATE_MCS_9_40:
            rate = RATE_11N_HT40_MCS9;
            break;
        case RATE_MCS_10_40:
            rate = RATE_11N_HT40_MCS10;
            break;
        case RATE_MCS_11_40:
            rate = RATE_11N_HT40_MCS11;
            break;
        case RATE_MCS_12_40:
            rate = RATE_11N_HT40_MCS12;
            break;
        case RATE_MCS_13_40:
            rate = RATE_11N_HT40_MCS13;
            break;
        case RATE_MCS_14_40:
            rate = RATE_11N_HT40_MCS14;
            break;
        case RATE_MCS_15_40:
            rate = RATE_11N_HT40_MCS15;
            break;
        case RATE_MCS_16_40:
            rate = RATE_11N_HT40_MCS16;
            break;
        case RATE_MCS_17_40:
            rate = RATE_11N_HT40_MCS17;
            break;
        case RATE_MCS_18_40:
            rate = RATE_11N_HT40_MCS18;
            break;
        case RATE_MCS_19_40:
            rate = RATE_11N_HT40_MCS19;
            break;
        case RATE_MCS_20_40:
            rate = RATE_11N_HT40_MCS20;
            break;
        case RATE_MCS_21_40:
            rate = RATE_11N_HT40_MCS21;
            break;
        case RATE_MCS_22_40:
            rate = RATE_11N_HT40_MCS22;
            break;
        case RATE_MCS_23_40:
            rate = RATE_11N_HT40_MCS23;
            break;
        case RATE_AC_MCS_0_20:
            rate = RATE_11AC_HT20_MCS0;
            break;
        case RATE_AC_MCS_1_20:
            rate = RATE_11AC_HT20_MCS1;
            break;
        case RATE_AC_MCS_2_20:
            rate = RATE_11AC_HT20_MCS2;
            break;
        case RATE_AC_MCS_3_20:
            rate = RATE_11AC_HT20_MCS3;
            break;
        case RATE_AC_MCS_4_20:
            rate = RATE_11AC_HT20_MCS4;
            break;
        case RATE_AC_MCS_5_20:
            rate = RATE_11AC_HT20_MCS5;
            break;
        case RATE_AC_MCS_6_20:
            rate = RATE_11AC_HT20_MCS6;
            break;
        case RATE_AC_MCS_7_20:
            rate = RATE_11AC_HT20_MCS7;
            break;
        case RATE_AC_MCS_8_20:
            rate = RATE_11AC_HT20_MCS8;
            break;
        case RATE_AC_MCS_9_20:
            rate = RATE_11AC_HT20_MCS9;
            break;
        case RATE_AC_MCS_10_20:
            rate = RATE_11AC_HT20_MCS10;
            break;
        case RATE_AC_MCS_11_20:
            rate = RATE_11AC_HT20_MCS11;
            break;
        case RATE_AC_MCS_12_20:
            rate = RATE_11AC_HT20_MCS12;
            break;
        case RATE_AC_MCS_13_20:
            rate = RATE_11AC_HT20_MCS13;
            break;
        case RATE_AC_MCS_14_20:
            rate = RATE_11AC_HT20_MCS14;
            break;
        case RATE_AC_MCS_15_20:
            rate = RATE_11AC_HT20_MCS15;
            break;
        case RATE_AC_MCS_16_20:
            rate = RATE_11AC_HT20_MCS16;
            break;
        case RATE_AC_MCS_17_20:
            rate = RATE_11AC_HT20_MCS17;
            break;
        case RATE_AC_MCS_18_20:
            rate = RATE_11AC_HT20_MCS18;
            break;
        case RATE_AC_MCS_19_20:
            rate = RATE_11AC_HT20_MCS19;
            break;
        case RATE_AC_MCS_20_20:
            rate = RATE_11AC_HT20_MCS20;
            break;
        case RATE_AC_MCS_21_20:
            rate = RATE_11AC_HT20_MCS21;
            break;
        case RATE_AC_MCS_22_20:
            rate = RATE_11AC_HT20_MCS22;
            break;
        case RATE_AC_MCS_23_20:
            rate = RATE_11AC_HT20_MCS23;
            break;
        case RATE_AC_MCS_24_20:
            rate = RATE_11AC_HT20_MCS24;
            break;
        case RATE_AC_MCS_25_20:
            rate = RATE_11AC_HT20_MCS25;
            break;
        case RATE_AC_MCS_26_20:
            rate = RATE_11AC_HT20_MCS26;
            break;
        case RATE_AC_MCS_27_20:
            rate = RATE_11AC_HT20_MCS27;
            break;
        case RATE_AC_MCS_28_20:
            rate = RATE_11AC_HT20_MCS28;
            break;
        case RATE_AC_MCS_29_20:
            rate = RATE_11AC_HT20_MCS29;
            break;
        case RATE_AC_MCS_30_20:
            rate = RATE_11AC_HT20_MCS30;
            break;
        case RATE_AC_MCS_31_20:
            rate = RATE_11AC_HT20_MCS31;
            break;
        case RATE_AC_MCS_32_20:
            rate = RATE_11AC_HT20_MCS32;
            break;
        case RATE_AC_MCS_33_20:
            rate = RATE_11AC_HT20_MCS33;
            break;
        case RATE_AC_MCS_34_20:
            rate = RATE_11AC_HT20_MCS34;
            break;
        case RATE_AC_MCS_35_20:
            rate = RATE_11AC_HT20_MCS35;
            break;
        case RATE_AC_MCS_36_20:
            rate = RATE_11AC_HT20_MCS36;
            break;
        case RATE_AC_MCS_37_20:
            rate = RATE_11AC_HT20_MCS37;
            break;
        case RATE_AC_MCS_38_20:
            rate = RATE_11AC_HT20_MCS38;
            break;
        case RATE_AC_MCS_39_20:
            rate = RATE_11AC_HT20_MCS39;
            break;
        case RATE_AC_MCS_0_40:
            rate = RATE_11AC_HT40_MCS0;
            break;
        case RATE_AC_MCS_1_40:
            rate = RATE_11AC_HT40_MCS1;
            break;
        case RATE_AC_MCS_2_40:
            rate = RATE_11AC_HT40_MCS2;
            break;
        case RATE_AC_MCS_3_40:
            rate = RATE_11AC_HT40_MCS3;
            break;
        case RATE_AC_MCS_4_40:
            rate = RATE_11AC_HT40_MCS4;
            break;
        case RATE_AC_MCS_5_40:
            rate = RATE_11AC_HT40_MCS5;
            break;
        case RATE_AC_MCS_6_40:
            rate = RATE_11AC_HT40_MCS6;
            break;
        case RATE_AC_MCS_7_40:
            rate = RATE_11AC_HT40_MCS7;
            break;
        case RATE_AC_MCS_8_40:
            rate = RATE_11AC_HT40_MCS8;
            break;
        case RATE_AC_MCS_9_40:
            rate = RATE_11AC_HT40_MCS9;
            break;
        case RATE_AC_MCS_10_40:
            rate = RATE_11AC_HT40_MCS10;
            break;
        case RATE_AC_MCS_11_40:
            rate = RATE_11AC_HT40_MCS11;
            break;
        case RATE_AC_MCS_12_40:
            rate = RATE_11AC_HT40_MCS12;
            break;
        case RATE_AC_MCS_13_40:
            rate = RATE_11AC_HT40_MCS13;
            break;
        case RATE_AC_MCS_14_40:
            rate = RATE_11AC_HT40_MCS14;
            break;
        case RATE_AC_MCS_15_40:
            rate = RATE_11AC_HT40_MCS15;
            break;
        case RATE_AC_MCS_16_40:
            rate = RATE_11AC_HT40_MCS16;
            break;
        case RATE_AC_MCS_17_40:
            rate = RATE_11AC_HT40_MCS17;
            break;
        case RATE_AC_MCS_18_40:
            rate = RATE_11AC_HT40_MCS18;
            break;
        case RATE_AC_MCS_19_40:
            rate = RATE_11AC_HT40_MCS19;
            break;
        case RATE_AC_MCS_20_40:
            rate = RATE_11AC_HT40_MCS20;
            break;
        case RATE_AC_MCS_21_40:
            rate = RATE_11AC_HT40_MCS21;
            break;
        case RATE_AC_MCS_22_40:
            rate = RATE_11AC_HT40_MCS22;
            break;
        case RATE_AC_MCS_23_40:
            rate = RATE_11AC_HT40_MCS23;
            break;
        case RATE_AC_MCS_24_40:
            rate = RATE_11AC_HT40_MCS24;
            break;
        case RATE_AC_MCS_25_40:
            rate = RATE_11AC_HT40_MCS25;
            break;
        case RATE_AC_MCS_26_40:
            rate = RATE_11AC_HT40_MCS26;
            break;
        case RATE_AC_MCS_27_40:
            rate = RATE_11AC_HT40_MCS27;
            break;
        case RATE_AC_MCS_28_40:
            rate = RATE_11AC_HT40_MCS28;
            break;
        case RATE_AC_MCS_29_40:
            rate = RATE_11AC_HT40_MCS29;
            break;
        case RATE_AC_MCS_30_40:
            rate = RATE_11AC_HT40_MCS30;
            break;
        case RATE_AC_MCS_31_40:
            rate = RATE_11AC_HT40_MCS31;
            break;
        case RATE_AC_MCS_32_40:
            rate = RATE_11AC_HT40_MCS32;
            break;
        case RATE_AC_MCS_33_40:
            rate = RATE_11AC_HT40_MCS33;
            break;
        case RATE_AC_MCS_34_40:
            rate = RATE_11AC_HT40_MCS34;
            break;
        case RATE_AC_MCS_35_40:
            rate = RATE_11AC_HT40_MCS35;
            break;
        case RATE_AC_MCS_36_40:
            rate = RATE_11AC_HT40_MCS36;
            break;
        case RATE_AC_MCS_37_40:
            rate = RATE_11AC_HT40_MCS37;
            break;
        case RATE_AC_MCS_38_40:
            rate = RATE_11AC_HT40_MCS38;
            break;
        case RATE_AC_MCS_39_40:
            rate = RATE_11AC_HT40_MCS39;
            break;
        case RATE_AC_MCS_0_80:
            rate = RATE_11AC_HT80_MCS0;
            break;
        case RATE_AC_MCS_1_80:
            rate = RATE_11AC_HT80_MCS1;
            break;
        case RATE_AC_MCS_2_80:
            rate = RATE_11AC_HT80_MCS2;
            break;
        case RATE_AC_MCS_3_80:
            rate = RATE_11AC_HT80_MCS3;
            break;
        case RATE_AC_MCS_4_80:
            rate = RATE_11AC_HT80_MCS4;
            break;
        case RATE_AC_MCS_5_80:
            rate = RATE_11AC_HT80_MCS5;
            break;
        case RATE_AC_MCS_6_80:
            rate = RATE_11AC_HT80_MCS6;
            break;
        case RATE_AC_MCS_7_80:
            rate = RATE_11AC_HT80_MCS7;
            break;
        case RATE_AC_MCS_8_80:
            rate = RATE_11AC_HT80_MCS8;
            break;
        case RATE_AC_MCS_9_80:
            rate = RATE_11AC_HT80_MCS9;
            break;
        case RATE_AC_MCS_10_80:
            rate = RATE_11AC_HT80_MCS10;
            break;
        case RATE_AC_MCS_11_80:
            rate = RATE_11AC_HT80_MCS11;
            break;
        case RATE_AC_MCS_12_80:
            rate = RATE_11AC_HT80_MCS12;
            break;
        case RATE_AC_MCS_13_80:
            rate = RATE_11AC_HT80_MCS13;
            break;
        case RATE_AC_MCS_14_80:
            rate = RATE_11AC_HT80_MCS14;
            break;
        case RATE_AC_MCS_15_80:
            rate = RATE_11AC_HT80_MCS15;
            break;
        case RATE_AC_MCS_16_80:
            rate = RATE_11AC_HT80_MCS16;
            break;
        case RATE_AC_MCS_17_80:
            rate = RATE_11AC_HT80_MCS17;
            break;
        case RATE_AC_MCS_18_80:
            rate = RATE_11AC_HT80_MCS18;
            break;
        case RATE_AC_MCS_19_80:
            rate = RATE_11AC_HT80_MCS19;
            break;
        case RATE_AC_MCS_20_80:
            rate = RATE_11AC_HT80_MCS20;
            break;
        case RATE_AC_MCS_21_80:
            rate = RATE_11AC_HT80_MCS21;
            break;
        case RATE_AC_MCS_22_80:
            rate = RATE_11AC_HT80_MCS22;
            break;
        case RATE_AC_MCS_23_80:
            rate = RATE_11AC_HT80_MCS23;
            break;
        case RATE_AC_MCS_24_80:
            rate = RATE_11AC_HT80_MCS24;
            break;
        case RATE_AC_MCS_25_80:
            rate = RATE_11AC_HT80_MCS25;
            break;
        case RATE_AC_MCS_26_80:
            rate = RATE_11AC_HT80_MCS26;
            break;
        case RATE_AC_MCS_27_80:
            rate = RATE_11AC_HT80_MCS27;
            break;
        case RATE_AC_MCS_28_80:
            rate = RATE_11AC_HT80_MCS28;
            break;
        case RATE_AC_MCS_29_80:
            rate = RATE_11AC_HT80_MCS29;
            break;
        case RATE_AC_MCS_30_80:
            rate = RATE_11AC_HT80_MCS30;
            break;
        case RATE_AC_MCS_31_80:
            rate = RATE_11AC_HT80_MCS31;
            break;
        case RATE_AC_MCS_32_80:
            rate = RATE_11AC_HT80_MCS32;
            break;
        case RATE_AC_MCS_33_80:
            rate = RATE_11AC_HT80_MCS33;
            break;
        case RATE_AC_MCS_34_80:
            rate = RATE_11AC_HT80_MCS34;
            break;
        case RATE_AC_MCS_35_80:
            rate = RATE_11AC_HT80_MCS35;
            break;
        case RATE_AC_MCS_36_80:
            rate = RATE_11AC_HT80_MCS36;
            break;
        case RATE_AC_MCS_37_80:
            rate = RATE_11AC_HT80_MCS37;
            break;
        case RATE_AC_MCS_38_80:
            rate = RATE_11AC_HT80_MCS38;
            break;
        case RATE_AC_MCS_39_80:
            rate = RATE_11AC_HT80_MCS39;
            break;
        case RATE_AC_MCS_0_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS0 : RATE_11AC_HT160_MCS0);
            break;
        case RATE_AC_MCS_1_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS1 : RATE_11AC_HT160_MCS1);
            break;
        case RATE_AC_MCS_2_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS2 : RATE_11AC_HT160_MCS2);
            break;
        case RATE_AC_MCS_3_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS3 : RATE_11AC_HT160_MCS3);
            break;
        case RATE_AC_MCS_4_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS4 : RATE_11AC_HT160_MCS4);
            break;
        case RATE_AC_MCS_5_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS5 : RATE_11AC_HT160_MCS5);
            break;
        case RATE_AC_MCS_6_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS6 : RATE_11AC_HT160_MCS6);
            break;
        case RATE_AC_MCS_7_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS7 : RATE_11AC_HT160_MCS7);
            break;
        case RATE_AC_MCS_8_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS8 : RATE_11AC_HT160_MCS8);
            break;
        case RATE_AC_MCS_9_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS9 : RATE_11AC_HT160_MCS9);
            break;
        case RATE_AC_MCS_10_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS10 : RATE_11AC_HT160_MCS10);
            break;
        case RATE_AC_MCS_11_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS11 : RATE_11AC_HT160_MCS11);
            break;
        case RATE_AC_MCS_12_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS12 : RATE_11AC_HT160_MCS12);
            break;
        case RATE_AC_MCS_13_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS13 : RATE_11AC_HT160_MCS13);
            break;
        case RATE_AC_MCS_14_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS14 : RATE_11AC_HT160_MCS14);
            break;
        case RATE_AC_MCS_15_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS15 : RATE_11AC_HT160_MCS15);
            break;
        case RATE_AC_MCS_16_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS16 : RATE_11AC_HT160_MCS16);
            break;
        case RATE_AC_MCS_17_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS17 : RATE_11AC_HT160_MCS17);
            break;
        case RATE_AC_MCS_18_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS18 : RATE_11AC_HT160_MCS18);
            break;
        case RATE_AC_MCS_19_160:
            rate = (flag ? RATE_11AC_HT80P80_MCS19 : RATE_11AC_HT160_MCS19);
            break;
        case RATE_P_1p5Mbps_5:
            rate = RATE_11P_BW5_1p5_MBPS;
            break;
        case RATE_P_2Mbps_5:
            rate = RATE_11P_BW5_2p25_MBPS;
            break;
        case RATE_P_3Mbps_5:
            rate = RATE_11P_BW5_3_MBPS;
            break;
        case RATE_P_4p5Mbps_5:
            rate = RATE_11P_BW5_4p5_MBPS;
            break;
        case RATE_P_6Mbps_5:
            rate = RATE_11P_BW5_6_MBPS;
            break;
        case RATE_P_9Mbps_5:
            rate = RATE_11P_BW5_9_MBPS;
            break;
        case RATE_P_12Mbps_5:
            rate = RATE_11P_BW5_12_MBPS;
            break;
        case RATE_P_13p5Mbps_5:
            rate = RATE_11P_BW5_13p5_MBPS;
            break;
        case RATE_P_3Mbps_10:
            rate = RATE_11P_BW10_3_MBPS;
            break;
        case RATE_P_4p5Mbps_10:
            rate = RATE_11P_BW10_4p5_MBPS;
            break;
        case RATE_P_6Mbps_10:
            rate = RATE_11P_BW10_6_MBPS;
            break;
        case RATE_P_9Mbps_10:
            rate = RATE_11P_BW10_9_MBPS;
            break;
        case RATE_P_12Mbps_10:
            rate = RATE_11P_BW10_12_MBPS;
            break;
        case RATE_P_18Mbps_10:
            rate = RATE_11P_BW10_18_MBPS;
            break;
        case RATE_P_24Mbps_10:
            rate = RATE_11P_BW10_24_MBPS;
            break;
        case RATE_P_27Mbps_10:
            rate = RATE_11P_BW10_27_MBPS;
            break;
        case RATE_P_6Mbps_20:
            rate = RATE_11P_BW20_6_MBPS;
            break;
        case RATE_P_9Mbps_20:
            rate = RATE_11P_BW20_9_MBPS;
            break;
        case RATE_P_12Mbps_20:
            rate = RATE_11P_BW20_12_MBPS;
            break;
        case RATE_P_18Mbps_20:
            rate = RATE_11P_BW20_18_MBPS;
            break;
        case RATE_P_24Mbps_20:
            rate = RATE_11P_BW20_24_MBPS;
            break;
        case RATE_P_36Mbps_20:
            rate = RATE_11P_BW20_36_MBPS;
            break;
        case RATE_P_48Mbps_20:
            rate = RATE_11P_BW20_48_MBPS;
            break;
        case RATE_P_54Mbps_20:
            rate = RATE_11P_BW20_54_MBPS;
            break;
        case RATE_MCS_24_40:
            rate = RATE_11N_HT40_MCS24;
            break;
        case RATE_MCS_25_40:
            rate = RATE_11N_HT40_MCS25;
            break;
        case RATE_MCS_26_40:
            rate = RATE_11N_HT40_MCS26;
            break;
        case RATE_MCS_27_40:
            rate = RATE_11N_HT40_MCS27;
            break;
        case RATE_MCS_28_40:
            rate = RATE_11N_HT40_MCS28;
            break;
        case RATE_MCS_29_40:
            rate = RATE_11N_HT40_MCS29;
            break;
        case RATE_MCS_30_40:
            rate = RATE_11N_HT40_MCS30;
            break;
        case RATE_MCS_31_40:
            rate = RATE_11N_HT40_MCS31;
            break;
        case RATE_MCS_24_20:
            rate = RATE_11N_HT20_MCS24;
            break;
        case RATE_MCS_25_20:
            rate = RATE_11N_HT20_MCS25;
            break;
        case RATE_MCS_26_20:
            rate = RATE_11N_HT20_MCS26;
            break;
        case RATE_MCS_27_20:
            rate = RATE_11N_HT20_MCS27;
            break;
        case RATE_MCS_28_20:
            rate = RATE_11N_HT20_MCS28;
            break;
        case RATE_MCS_29_20:
            rate = RATE_11N_HT20_MCS29;
            break;
        case RATE_MCS_30_20:
            rate = RATE_11N_HT20_MCS30;
            break;
        case RATE_MCS_31_20:
            rate = RATE_11N_HT20_MCS31;
            break;
        }
    }

    uint SetUpRate(uint rate)
    {
        if (rate >= 1007u && rate <= 1014u)
        {
            return rate - 1000u;
        }
        if (rate >= 1107u && rate <= 1114u)
        {
            return rate - 1100u;;
        }
        return rate;
    }

    void mapbStateToWLAN_MODE(WLAN_Gen6_ChannelBondingState channelBonding, WLAN_RATE rate, TCMD_WLAN_MODE& cbState)
    {
        cbState = TCMD_WLAN_MODE_VHT20;
        switch (channelBonding)
        {
        case WLAN_Gen6_ChannelBondingState_none:
            if (rate >= RATE_11B_LONG_1_MBPS && rate <= RATE_11B_SHORT_11_MBPS)
            {
                cbState = TCMD_WLAN_MODE_CCK;
                return;
            }
            cbState = TCMD_WLAN_MODE_NOHT;
            return;
        case primaryLow:
            if ((rate >= RATE_11AC_HT40_MCS0 && rate <= RATE_11AC_HT40_MCS9) || (rate >= RATE_11AC_HT40_MCS10 && rate <= RATE_11AC_HT40_MCS19) || (rate >= RATE_11AC_HT40_MCS20 && rate <= RATE_11AC_HT40_MCS29))
            {
                cbState = TCMD_WLAN_MODE_VHT40MINUS;
                return;
            }
            cbState = TCMD_WLAN_MODE_HT40MINUS;
            return;
        case primary20:
            cbState = TCMD_WLAN_MODE_HT20;
            return;
        case primaryHigh:
            if ((rate >= RATE_11AC_HT40_MCS0 && rate <= RATE_11AC_HT40_MCS9) || (rate >= RATE_11AC_HT40_MCS10 && rate <= RATE_11AC_HT40_MCS19) || (rate >= RATE_11AC_HT40_MCS20 && rate <= RATE_11AC_HT40_MCS29))
            {
                cbState = TCMD_WLAN_MODE_VHT40PLUS;
                return;
            }
            cbState = TCMD_WLAN_MODE_HT40PLUS;
            return;
        case BW80_20_40Low_40_80Low:
            cbState = TCMD_WLAN_MODE_VHT80_0;
            return;
        case BW80_20_40High_40_80Low:
            cbState = TCMD_WLAN_MODE_VHT80_1;
            return;
        case BW80_20_40Low_40_80High:
            cbState = TCMD_WLAN_MODE_VHT80_2;
            return;
        case BW80_20_40GHigh_40_80High:
            cbState = TCMD_WLAN_MODE_VHT80_3;
            return;
        case BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80p80;
            return;
        case BW160:
            cbState = TCMD_WLAN_MODE_VHT160;
            return;
        case Primary_1st_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_0;
            return;
        case Primary_2nd_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_1;
            return;
        case Primary_3rd_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_2;
            return;
        case Primary_4th_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_3;
            return;
        case Primary_5th_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_4;
            return;
        case Primary_6th_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_5;
            return;
        case Primary_7th_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_6;
            return;
        case Primary_8th_20_in_BW80p80:
            cbState = TCMD_WLAN_MODE_VHT80P80_20_7;
            return;
        case Primary_1st_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_0;
            return;
        case Primary_2nd_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_1;
            return;
        case Primary_3rd_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_2;
            return;
        case Primary_4th_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_3;
            return;
        case Primary_5th_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_4;
            return;
        case Primary_6th_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_5;
            return;
        case Primary_7th_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_6;
            return;
        case Primary_8th_20_in_BW160:
            cbState = TCMD_WLAN_MODE_VHT160_20_7;
            return;
        case Half_Rate:
        case Quarter_Rate:
            cbState = TCMD_WLAN_MODE_NOHT;
            return;
        }
        cbState = TCMD_WLAN_MODE_VHT20;
    }

    bool CJGW_Phone::tlvAddRspParam(const std::string& strKey,const std::string& strData)
    {
        return TRUE == QLIB_FTM_WLAN_TLV_AddParam(mhResourceContent,const_cast<char*>(strKey.data()),const_cast<char*>(strData.data()));
    }

    bool CJGW_Phone::tlvAddRspParam(const std::string& strKey,size_t nData)
    {
        std::string data;
        JGW_FormatString(data,"%u",nData);
        return tlvAddRspParam(strKey,data);
    }

    bool CJGW_Phone::Tlv2AddRspParam(const std::string& strKey,const std::string& strData)
    {
        return TRUE == QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,const_cast<char*>(strKey.data()),const_cast<char*>(strData.data()));
    }

    bool CJGW_Phone::Tlv2AddRspParam(const std::string& strKey,size_t nData)
    {
        std::string data;
        JGW_FormatString(data,"%u",nData);
        return Tlv2AddRspParam(strKey,data);
    }

    bool CJGW_Phone::SetUpRx(const S_RxCalTestParam& msRxCalTestParam)
    {
        size_t rate = SetUpRate(msRxCalTestParam.meRate);

        uint value = msRxCalTestParam.mnChannel;
        uint num = msRxCalTestParam.mnChannel2;

        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,2);
        TCMD_WLAN_MODE value3;
        mapbStateToWLAN_MODE(msRxCalTestParam.meChannelBondingState, (WLAN_RATE)rate, value3);

        std::string data;
        tlvAddRspParam("wlanMode", (size_t)value3);

        if (msRxCalTestParam.meChannelBondingState == Half_Rate)
        {
            tlvAddRspParam("bandwidth", "50");
        }
        else if (msRxCalTestParam.meChannelBondingState == Quarter_Rate)
        {
            tlvAddRspParam("bandwidth", "51");
        }
        else
        {
            tlvAddRspParam("bandwidth", "0");
        }
        tlvAddRspParam("rxMode", (size_t)msRxCalTestParam.mePromiscuousMode);
        tlvAddRspParam("channel", value);
        if (num != 0u)
        {
            tlvAddRspParam("channel2", num);
        }
        uint value2 = rate;
        if (rate == 2u)
        {
            value2 = 3u;
        }
        else if (rate == 5u)
        {
            value2 = 4u;
        }
        else if (rate == 4u)
        {
            value2 = 2u;
        }
        else if (rate == 3u)
        {
            value2 = 5u;
        }
        tlvAddRspParam("rateMask", value2);
        tlvAddRspParam("rxChain", (uint)msRxCalTestParam.meRxChain);
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);

        return true;
    }

    double GetAdjQuarterRateFreq(WLAN_Gen6_ChannelBondingState cbState)
    {
        if (cbState == Quarter_Rate)
        {
            return 0.5;
        }
        return 0.0;
    }

    std::string CJGW_Phone::tlv2GetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV2_GetRspParam(mhResourceContent,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    std::string CJGW_Phone::tlvGetRspParam(const std::string& key)
    {
        char rsp[1025] = {0};
        if (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(mhResourceContent,(char*)key.c_str(),rsp))
        {
            return rsp;
        }
        return "";
    }

    void CJGW_Phone::GetRxReport(int rxChain, uint& totalPktCount, uint& totalMacPktCount, uint& crcErrPktCount,uint& secErrPktCount,double& rssiDbm)
    {
        int evm0 = 0,  evm1 = 0,  evm2 = 0,  tPut = 0;
        totalPktCount = (totalMacPktCount = (crcErrPktCount = (secErrPktCount = 0u)));
        rssiDbm = 0.0;
        evm0 = (evm1 = (evm2 = 0));

        int num = 0;
        int num2 = 0;
        tPut = 0;
        QLIB_FTM_WLAN_TLV_Create(mhResourceContent,11);
        QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
        std::string text = tlvGetRspParam("totalPkt");
        if (text.size() > 0)
        {
            totalPktCount = atoi(text.c_str());
        }
        text = tlvGetRspParam("goodPackets");
        if (text.size() > 0)
        {
            totalMacPktCount = atoi(text.c_str());
        }
        text = tlvGetRspParam("crcErrPkt");
        if (text.size() > 0)
        {
            crcErrPktCount = atoi(text.c_str());
        }
        text = tlvGetRspParam("secErrPkt");
        if (text.size() > 0)
        {
            secErrPktCount = atoi(text.c_str());
        }
        text = tlvGetRspParam("rssi");
        if (text.size() > 0)
        {
            rssiDbm = atof(text.c_str());
        }
        text = tlvGetRspParam("evm0");
        if (text.size() > 0)
        {
            evm0 = atoi(text.c_str());
        }
        text = tlvGetRspParam("evm1");
        if (text.size() > 0)
        {
            evm1 = atoi(text.c_str());
        }
        text = tlvGetRspParam("evm2");
        if (text.size() > 0)
        {
            evm2 = atoi(text.c_str());
        }
        text = tlvGetRspParam("endTime");
        if (text.size() > 0)
        {
            num = atoi(text.c_str());
        }
        text = tlvGetRspParam("startTime");
        if (text.size() > 0)
        {
            num -= atoi(text.c_str());
        }
        text = tlvGetRspParam("byteCount");
        if (text.size() > 0)
        {
            num2 = atoi(text.c_str());
        }
        text = tlvGetRspParam("dontCount");
        if (text.size() > 0)
        {
            num2 -= atoi(text.c_str());
        }
        if (num > 0 && num2 > 0)
        {
            tPut = (int)((double)(num2 * 8) / (0.001 * (double)num));
        }
        LogI_F(L"totalPkt:%d,Throughput:%d,rssi:%.0f,evm0:%d,evm1:%d,evm2:%d,crcErrPkt:%d,secErrPktCount:%d",totalPktCount,tPut,rssiDbm,evm0,evm1,evm2,crcErrPktCount,secErrPktCount);
    }

    bool CJGW_Phone::AutoDetectChip(std::string& strChipInfo)
    {
        bool result = TRUE == QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,200);
        result &= TRUE == QLIB_FTM_WLAN_TLV_Complete(mhResourceContent);
        strChipInfo = tlv2GetRspParam("ver_info");
        return result;
    }

    /*
    28:19:065	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_TLV2_Create
    28:19:065	,LOG_FN,FTM_WLAN_TLV2_Create 1
    28:19:066	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:066	,LOG_FN,FTM_WLAN_TLV2_AddParam bandwidth <==> 0
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam wifiStandard <==> 2
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam phyId <==> 0
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam nss <==> 1
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam gI <==> 50
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam channel <==> 5210
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam txMode <==> 3
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam rateBitIndex0 <==> 20
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam rateBw <==> 8
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam wlanMode <==> 2
    28:19:067	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:067	,LOG_FN,FTM_WLAN_TLV2_AddParam pktLen0 <==> 4000
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam txChain0 <==> 1
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam agg <==> 1
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam shortGuard <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam numPackets <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam broadcast <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam rxStation <==> D0:C0:C0:C0:00:00
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam txStation <==> E0:C0:C0:C0:00:00
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam bssid <==> F0:C0:C0:C0:00:00
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam aifsn <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam scramblerOff <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam flags <==> 24
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam txPattern <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam gI <==> 50
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam ctrlFlag <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam dutyCycle <==> 10
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam tpcm <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam txPower0 <==> 10
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam ppduType <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam dcm <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam scanProfile <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_tlv2_AddParam
    28:19:068	,LOG_FN,FTM_WLAN_TLV2_AddParam puncBwMask <==> 0
    28:19:068	,LOG_C_HIGH_LEVEL_START,QLIB_FTM_WLAN_TLV2_Complete
    */
    bool CJGW_Phone::SetUpTxDetail2(int nPowerLevel,double& txPower,const S_TxTestParam& sTxTestParam,PhyID ePhyID /* = PhyID_A */,WLAN_UniversalRateBW eRateBw /* = RateBW_11AX_HE80 */,WLAN_CHAIN_MASK2 eWlanChainMask /* = WLAN_CHAIN_1 */)
    {
        bool result = true;
        result &= TRUE == QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,WCN_OP_TX);
        std::string data;
        //! default
        unsigned int wifiStandard = 2;
        WLAN_Gen6_ChannelBondingState eWLanGen6ChannelBondingState = sTxTestParam.meChannelBondingState;

        if (eWLanGen6ChannelBondingState == Half_Rate)
        {
            result &= Tlv2AddRspParam("bandwidth","50");
            eWLanGen6ChannelBondingState = WLAN_Gen6_ChannelBondingState_none;
        }
        else if (eWLanGen6ChannelBondingState == Quarter_Rate)
        {
            result &= Tlv2AddRspParam("bandwidth","51");
            eWLanGen6ChannelBondingState = WLAN_Gen6_ChannelBondingState_none;
        }
        else
        {
            result &= Tlv2AddRspParam("bandwidth","0");
        }
        Tlv2AddRspParam("wifiStandard",wifiStandard);
        Tlv2AddRspParam("phyId",ePhyID);
        Tlv2AddRspParam("nss",sTxTestParam.meNss);
        Tlv2AddRspParam("gI",sTxTestParam.meGuardInterval);
        Tlv2AddRspParam("channel",sTxTestParam.mnChannel);
        if (sTxTestParam.mnChannel2 != 0u)
        {
            tlvAddRspParam("channel2",sTxTestParam.mnChannel2);
        }
        Tlv2AddRspParam("txMode",sTxTestParam.meTxMode);

        TCMD_WLAN_MODE value = TCMD_WLAN_MODE_HT20;
        mapWlanMode(sTxTestParam.meRate, eWLanGen6ChannelBondingState, value);
        //tlvAddRspParam("wlanMode",value);
        WLAN_tlvDataRate tlvdataRate = getTLVdataRate(sTxTestParam.meRate, sTxTestParam.mbShort11b_nGuard, value);
        Tlv2AddRspParam("rateBitIndex0",tlvdataRate);
        Tlv2AddRspParam("rateBw",eRateBw);
        Tlv2AddRspParam("wlanMode",value);
        Tlv2AddRspParam("pktLen0",sTxTestParam.muPayLoadSize);
        Tlv2AddRspParam("txChain0",eWlanChainMask);
        Tlv2AddRspParam("agg",sTxTestParam.muAggregation);
        Tlv2AddRspParam("shortGuard",sTxTestParam.mbShort11b_nGuard?1:0);
        Tlv2AddRspParam("numPackets",sTxTestParam.muPacket);
        Tlv2AddRspParam("broadcast",sTxTestParam.mbBroadcastMode ? 1 : 0);
        //Tlv2AddRspParam("rxStation",m_override_rxStation_addr);
        //Tlv2AddRspParam("txStation",m_override_txStation_addr);
        //Tlv2AddRspParam("bssid",m_override_txBSSID);

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

        if (ePhyID == 1)  // phyB
        {
            override_rxStation_addr = override_rxStation_addr2g;
            override_txStation_addr = override_txStation_addr2g;
            override_txBSSID        = override_txBSSID2g;
        }
        else {
            override_rxStation_addr = override_rxStation_addr5g;
            override_txStation_addr = override_txStation_addr5g;
            override_txBSSID        = override_txBSSID5g;
        }
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"rxStation", (char *)override_rxStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"txStation", (char *)override_txStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"bssid", (char *)override_txBSSID);

        Tlv2AddRspParam("aifsn",sTxTestParam.muiAIFSN);
        Tlv2AddRspParam("scramblerOff",sTxTestParam.mbScramblerDisable ? 1 : 0);
        uint num = 1u;
        uint num2 = 2u;
        uint num3 = 4u;
        uint num4 = 128u;
        uint num5 = 24u;
        if (sTxTestParam.mbLdpcRate)
        {
            num5 |= num2;
        }
        if ( sTxTestParam.meDpdMode == DPD_Enabled)
        {
            num5 |= num3;
        }
        if (sTxTestParam.mbSTBCmode)
        {
            num5 |= num;
        }
        if (sTxTestParam.meHC_Enable == HCS_Enabled)
        {
            num5 |= num4;
        }
        Tlv2AddRspParam("flags",num5);
        Tlv2AddRspParam("txPattern",sTxTestParam.mePattern);
        Tlv2AddRspParam("gI",sTxTestParam.meGuardInterval);
        Tlv2AddRspParam("ctrlFlag",0);
        Tlv2AddRspParam("dutyCycle",sTxTestParam.mnDutyCycle);
        switch (sTxTestParam.mePwrMode)
        {
        case ForcedDesiredGain:
        case ForcedGain:
            Tlv2AddRspParam("tpcm","4");
            Tlv2AddRspParam("gainIdx",sTxTestParam.muGain);
            Tlv2AddRspParam("dacGain",sTxTestParam.mnDigitalGain);
            Tlv2AddRspParam("paConfig",sTxTestParam.muPASetting);
            break;
        case ForcedGLUTIndex:
            Tlv2AddRspParam("tpcm","6");
            Tlv2AddRspParam("gainIdx",sTxTestParam.muGain);
            break;
        case TxPowerAuto:
            Tlv2AddRspParam("tpcm","2");
            break;
        case TxPowerForce_OLPC:
        case TxPowerForce_CLPC:
        case TxPowerForce_SCPC:
            Tlv2AddRspParam("txPower0",nPowerLevel);
            Tlv2AddRspParam("tpcm","0");
            break;
        default:
            mbResult = false;
            LogE(L"QCA639xx doesn't support selected pwrMode");
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"QCA639x doesn't support selected pwrMode");
        }
        Tlv2AddRspParam("ppduType",0);
        Tlv2AddRspParam("dcm",0);
        Tlv2AddRspParam("scanProfile",0);
        Tlv2AddRspParam("puncBwMask",0);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);

        if (sTxTestParam.mePwrMode == TxPowerAuto)
        {
            uint num = 0u;
            GetTgtPwr(sTxTestParam.mnChannel, (uint)sTxTestParam.meRate, 0u, PhyID_A,num);
            txPower = num / 2.0;
        }
        return true;
    }

    bool CJGW_Phone::SetUpDutTxDetails2(WLAN_UniversalPhyID phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, uint nPacket, bool broadcastMode, uint AIFSN, int dutTxSettlingDelayMs, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, PPDU_TYPE ppduType, bool dualCarrierModulation,double& txPower, PHY165Mode phy165Mode /* = Mode_0 */, const std::string& puncBwMask /* = "0x00" */)
    {
        m_override_rxStation_addr = "D0:C0:C0:C0:00:00";
        m_override_txStation_addr = "E0:C0:C0:C0:00:00";
        m_override_txBSSID = "F0:C0:C0:C0:00:00";
        uint wifiStandard = 2u;
        txPower = powerLevel;
        uint chainMask2 = (uint)(int)(chainMask | ((long)chainMaskH << 4));
        try
        {
            LogI_F(L"Freq: %.0f freq2: %.0f Rate: %d ChainMask: %d",channel,channel2,rate,chainMask);
            //QTALogMgr.Debug("freq: " + channel.ToString() + " freq2: " + channel2.ToString() + " Rate: " + ((int)rate).ToString() + " ChainMask: " + ((int)chainMask).ToString());
            switch (pwrMode)
            {
            case ForcedGain:
            case ForcedDesiredGain:
                //QTALogMgr.Debug("gain: " + gain.ToString() + " digitalGain: " + digitalGain.ToString() + " PAcfg: " + PAcfg.ToString());
                LogI_F(L"gain: %u digitalGain: %d PAcfg: %u",gain,digitalGain,PAcfg);
                break;
            case ForcedGLUTIndex:
                //QTALogMgr.Debug("gain: " + gain.ToString());
                LogI_F(L"gain: %u",gain);
                break;
            default:
                LogI_F(L"powerLevel: ",powerLevel);
                //QTALogMgr.Debug("powerLevel: " + powerLevel.ToString());
                break;
            }
            if (dutyCycle < 0 || dutyCycle > 100)
            {
                LogE(L"dutyCycle parameter should be set within 0 to 100 precent");
                //throw new PhoneException("dutyCycle parameter should be set within 0 to 100 precent");
            }
            unsigned short num = 0;
            //puncBwMask = puncBwMask.Trim();
            char* str;
            num = static_cast<unsigned short>(strtol(puncBwMask.c_str(), &str, 16));
            //if (puncBwMask.StartsWith("0x", true, null))
            //{
            //    puncBwMask = puncBwMask.Remove(0, 2);
            //    num = Convert.ToUInt16(puncBwMask, 16);
            //}
            //else
            //{
            //    num = Convert.ToUInt16(puncBwMask);
            //}
            //dut.setPuncBwMask(num);
            mnpuncBwMask = num;
            return SetUpTxDetail2(wifiStandard, (uint)(int)phyId, channel, channel2, cbState, (uint)phy165Mode, rate, rateBw, nss, pwrMode, txMode, chainMask2, powerLevel, gain, digitalGain, PAcfg, payLoadSize, ldpcRate, stbcMode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, nPacket, broadcastMode, AIFSN, scramblerDisable, ackRequest, guardInterval, (uint)ppduType, dualCarrierModulation, txPower);
            mnpuncBwMask = 0;
            //dut.setPuncBwMask(0);
            //QTALogMgr.Debug("Waiting for dutTxSettlingDelayMs = " + dutTxSettlingDelayMs + "ms");
            //Thread.Sleep(dutTxSettlingDelayMs);
        }
        catch (...)
        {
            LogE(L"SetUpDutTxDetails2() failed");
            return false;
        }
        return true;
    }

    bool CJGW_Phone::SetUpTxDetail2(uint wifiStandard, uint phyId, double channel, double channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, WLAN_UniversalNSS nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint chainMask, double powerLevel, uint gain, int digitalGain, uint PAcfg, uint payLoadSize, bool ldpcRate, bool stbcMode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, uint nPacket, bool broadcastMode, uint AIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval, uint ppduType, bool dualCarrierModulation, double& txPower)
    {
        bool result = setUpTxDetailCommon2(wifiStandard, phyId, (uint)channel, (uint)channel2, cbState, phy165Mode, rate, rateBw, (uint)(int)nss, pwrMode, txMode, chainMask, powerLevel, gain, digitalGain, PAcfg, payLoadSize, false, ldpcRate, stbcMode, aggregation, dpdMode, HC_Enable, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, AIFSN, scramblerDisable, ackRequest, guardInterval);

        uint ScanProfile = 0;
        Tlv2AddRspParam("ppduType", ppduType);
        Tlv2AddRspParam("dcm", dualCarrierModulation ? 1 : 0);
        Tlv2AddRspParam("scanProfile", ScanProfile);
        Tlv2AddRspParam("puncBwMask", mnpuncBwMask);
        result &= TRUE == QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        //double[] array = txPower = new double[1];
        if (pwrMode == TxPowerAuto)
        {
            //double[] array2 = txPower = new double[2];
            GetTgtPwr2PerChain(phyId, (uint)channel, (uint)(int)rate, (uint)(int)rateBw, (uint)(int)nss, ppduType,  txPower);
        }
        return result;
    }

    void CJGW_Phone::GetTgtPwr2PerChain(uint phyId, uint freq, uint rate, uint rateBw, uint nss, uint ppduType,  double& tgtPwr)
    {
        //double[] array = tgtPwr = new double[2];
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)132);
        //string data = Convert.ToString((int)phyId);
        Tlv2AddRspParam("phyId", phyId);
        //data = Convert.ToString(freq);
        Tlv2AddRspParam("freq", freq);
        //data = Convert.ToString(2);
        Tlv2AddRspParam("userMode", 2);
        //data = Convert.ToString(rate);
        Tlv2AddRspParam("rateBit", rate);
        //data = Convert.ToString(rateBw);
        Tlv2AddRspParam("rateBw", rateBw);
        //data = Convert.ToString(nss);
        Tlv2AddRspParam("nss", nss);
        //data = Convert.ToString(ppduType);
        Tlv2AddRspParam("ppduType", ppduType);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        std::string data = tlv2GetRspParam("tgtPwr");
        uint num = atoi(data.c_str());
        tgtPwr = ((num != 255) ? ((double)num / 4.0) : 0.0);
        for (int i = 1; i < 2; i++)
        {
            data = tlv2GetRspParam(JGW_GetFormatString("perChainTgtpwr%d", i));
            num = atoi(data.c_str());
            tgtPwr = ((num != 255) ? ((double)num / 4.0) : 0.0);
        }
    }

    bool CJGW_Phone::setUpTxDetailCommon2(uint wifiStandard, uint phyId, uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState, uint phy165Mode, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, TxPowerMode pwrMode, WLAN_TX_Mode txMode, uint txChainMask, double powerLevel, uint gain, int digitalGain, uint PASetting, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, HCSetting HC_Enable, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        ackRequest = ((HC_Enable == HCS_Enabled || ackRequest) ? true : false);
        //SaveTxSettings2(wifiStandard, phyId, channel, channel2, cbState, phy165Mode, rate, rateBw, nss, pwrMode, txMode, txChainMask, powerLevel, gain, digitalGain, PASetting, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, ackRequest, guardInterval);
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
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)WCN_OP_TX);
        //string data;
        if ((int)cbState == 50)
        {
            //data = Convert.ToString(50u);
            Tlv2AddRspParam("bandwidth", 50);
            cbState = WLAN_Gen6_ChannelBondingState_none;
        }
        else if ((int)cbState == 51)
        {
            //data = Convert.ToString(51u);
            Tlv2AddRspParam("bandwidth", 51);
            cbState = WLAN_Gen6_ChannelBondingState_none;
        }
        else
        {
            Tlv2AddRspParam("bandwidth", "0");
        }
        SetUpTxDefault2(wifiStandard, phyId, centFreqMHz, centFreq2MHz, cbState, rate, rateBw, nss, txMode, txChainMask, payLoadSize, short11b_nGuard, ldpcRate, bSTBCmode, aggregation, dpdMode, ifs, dutyCycle, nPattern, pattern, broadcastMode, nPacket, iAIFSN, scramblerDisable, ackRequest, guardInterval);
        //data = Convert.ToString(phy165Mode);
        Tlv2AddRspParam("ctrlFlag", phy165Mode);
        //data = Convert.ToString(dutyCycle);
        Tlv2AddRspParam("dutyCycle", dutyCycle);
        switch (pwrMode)
        {
        case ForcedGain:
        case ForcedDesiredGain:
            //data = Convert.ToString(4u);
            Tlv2AddRspParam("tpcm", 4);
            //data = Convert.ToString(gain);
            Tlv2AddRspParam("gainIdx", gain);
            //data = Convert.ToString(digitalGain);
            Tlv2AddRspParam("dacGain", digitalGain);
            //data = Convert.ToString(PASetting);
            Tlv2AddRspParam("paConfig", PASetting);
            break;
        case ForcedGLUTIndex:
            //data = Convert.ToString(6u);
            Tlv2AddRspParam("tpcm", 6);
            //data = Convert.ToString(gain);
            Tlv2AddRspParam("gainIdx", gain);
            break;
        case TxPowerAuto:
            //data = Convert.ToString(5u);
            Tlv2AddRspParam("tpcm", 5);
            break;
        case TxForcedPerChain:
            //data = Convert.ToString(7u);
            Tlv2AddRspParam("tpcm", 7);
            break;
        case TxPowerForce_CLPC:
        case TxPowerForce_SCPC:
        case TxPowerForce_OLPC:
            //data = Convert.ToString(0u);
            Tlv2AddRspParam("tpcm", 0);
            //data = Convert.ToString(powerLevel);
            Tlv2AddRspParam("txPower0", JGW_GetFormatString("%.0lf",powerLevel));
            break;
        default:
            LogE(L"IPQ807x doesn't support selected pwrMode");
            return false;
        }
        return true;
    }

    void CJGW_Phone::TurnOffTX2(uint phyId, uint channel, uint txChain)
    {
        uint value = channel;
        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)WCN_OP_TX);
        Tlv2AddRspParam("phyid", phyId);
        //data = Convert.ToString(value);
        Tlv2AddRspParam("channel", value);
        //data = Convert.ToString(0u);
        Tlv2AddRspParam("txMode", 0);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
    }

    int CJGW_Phone::getChanFreqMhz(uint chanID)
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

    void CJGW_Phone::SetUpTxDefault2(uint wifiStandard, uint phyId, uint centFreqMHz, uint centFreq2MHz, WLAN_Gen6_ChannelBondingState cbState, WLAN_UniversalDataRate rate, WLAN_UniversalRateBW rateBw, uint nss, WLAN_TX_Mode txMode, uint txChainMask, uint payLoadSize, bool short11b_nGuard, bool ldpcRate, bool bSTBCmode, uint aggregation, DPDSetting dpdMode, int ifs, uint dutyCycle, TxDataPattern nPattern, const std::vector<byte>& pattern, bool broadcastMode, uint nPacket, uint iAIFSN, bool scramblerDisable, bool ackRequest, WLAN_UniversalGuardInterval guardInterval)
    {
        uint num = 1u;
        uint num2 = 2u;
        uint num3 = 4u;
        uint num4 = 128u;
        //string data = Convert.ToString(wifiStandard);
        Tlv2AddRspParam("wifiStandard", wifiStandard);
        //data = Convert.ToString(phyId);
        Tlv2AddRspParam("phyId", phyId);
        //data = Convert.ToString(nss);
        Tlv2AddRspParam("nss", nss);
        //data = Convert.ToString((uint)(int)guardInterval);
        Tlv2AddRspParam("gI", guardInterval);
        //data = Convert.ToString(centFreqMHz);
        Tlv2AddRspParam("channel", centFreqMHz);
        if (centFreq2MHz != 0)
        {
            //data = Convert.ToString(centFreq2MHz);
            Tlv2AddRspParam("channel2", centFreq2MHz);
        }
        //data = Convert.ToString((uint)(int)txMode);
        Tlv2AddRspParam("txMode", txMode);
        //data = Convert.ToString((uint)(int)rate);
        Tlv2AddRspParam("rateBitIndex0", rate);
        //data = Convert.ToString((uint)(int)rateBw);
        Tlv2AddRspParam("rateBw", rateBw);
        //data = Convert.ToString((uint)(int)cbState);
        Tlv2AddRspParam("wlanMode", cbState);
        //data = Convert.ToString(payLoadSize);
        Tlv2AddRspParam("pktLen0", payLoadSize);
        //data = Convert.ToString(txChainMask);
        Tlv2AddRspParam("txChain0", txChainMask);
        //data = Convert.ToString(aggregation);
        Tlv2AddRspParam("agg", aggregation);
        //data = ((!short11b_nGuard) ? "0" : "1");
        Tlv2AddRspParam("shortGuard", ((!short11b_nGuard) ? "0" : "1"));
        //data = Convert.ToString(nPacket);
        Tlv2AddRspParam("numPackets", nPacket);
        //data = ((!broadcastMode) ? "0" : "1");
        Tlv2AddRspParam("broadcast", ((!broadcastMode) ? "0" : "1"));
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
        else {
            override_rxStation_addr = override_rxStation_addr5g;
            override_txStation_addr = override_txStation_addr5g;
            override_txBSSID        = override_txBSSID5g;
        }
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"rxStation", (char *)override_rxStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"txStation", (char *)override_txStation_addr);
        QLIB_FTM_WLAN_TLV2_AddParam(mhResourceContent,"bssid", (char *)override_txBSSID);
        //Tlv2AddRspParam("rxStation", "D0:C0:C0:C0:00:00"/*m_override_rxStation_addr*/);
        //Tlv2AddRspParam("txStation", "E0:C0:C0:C0:00:00"/*m_override_txStation_addr*/);
        //Tlv2AddRspParam("bssid", "F0:C0:C0:C0:00:00"/*m_override_txBSSID*/);
        //data = Convert.ToString(iAIFSN);
        Tlv2AddRspParam("aifsn", iAIFSN);
        //data = ((!scramblerDisable) ? "0" : "1");
        Tlv2AddRspParam("scramblerOff", ((!scramblerDisable) ? "0" : "1"));
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
        Tlv2AddRspParam("flags", num5);
        uint num6 = (uint)nPattern;
        Tlv2AddRspParam("txPattern", num6);
        //data = Convert.ToString((uint)(int)guardInterval);
        Tlv2AddRspParam("gI", guardInterval);
    }

    bool CJGW_Phone::ResetRxPacketCounts2(uint wifiStandard, uint phyId,uint channel, uint channel2, WLAN_Gen6_ChannelBondingState cbState,WLAN_UniversalDataRate rate,WLAN_UniversalRateBW rateBw,uint nss,int rxChain, uint phy165Mode /* = 0 */, int noisefloor /* = 0 */, int aniMode /* = 0 */, int aniDynaLevel /* = 0 */,WLAN_Action rxFilterMode /* = PromiscuousMode */,PPDU_TYPE ppduType /* = SingleUser */,bool dualCarrierModulation /* = false */,bool enANI /* = false */)
    {
        uint value = channel;
        uint num = channel2;
        if (channel < 2000)
        {
            value = (uint)getChanFreqMhz(channel);
        }
        if (channel2 < 2000)
        {
            num = (uint)getChanFreqMhz(channel2);
        }
        //uint rate = m_rate;
        //RxChain = rxChain;
        //freqMHz = channel;
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)2);
        //string data = Convert.ToString((uint)(int)cbState);
        Tlv2AddRspParam("wlanMode", cbState);
        if ((int)cbState == 50)
        {
            Tlv2AddRspParam("bandwidth", "50");
        }
        else if ((int)cbState == 51)
        {
            Tlv2AddRspParam("bandwidth", "51");
        }
        else
        {
            Tlv2AddRspParam("bandwidth", "0");
        }
        //data = Convert.ToString(rxFilterMode);
        Tlv2AddRspParam("rxMode", rxFilterMode);
        if (rxFilterMode == 1)
        {
            Tlv2AddRspParam("staAddr", m_override_rxStation_addr);
            Tlv2AddRspParam("bssid", m_override_txBSSID);
        }
        //data = Convert.ToString(value);
        Tlv2AddRspParam("channel", value);
        if (num != 0)
        {
            //data = Convert.ToString(num);
            Tlv2AddRspParam("channel2", num);
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
        uint ScanProfile = 0;
        //data = Convert.ToString(num2);
        Tlv2AddRspParam("flags", num2);
        //data = Convert.ToString(m_rate);
        Tlv2AddRspParam("rateBitIndex0", rate);
        //data = Convert.ToString((uint)rxChain);
        Tlv2AddRspParam("rxChain", rxChain);
        //data = Convert.ToString(wifiStandard);
        Tlv2AddRspParam("wifiStandard", wifiStandard);
        //data = Convert.ToString(m_phyId);
        Tlv2AddRspParam("phyId", phyId);
        //data = Convert.ToString(m_nss);
        Tlv2AddRspParam("nss", nss);
        //data = Convert.ToString(m_rateBw);
        Tlv2AddRspParam("rateBw", rateBw);
        //data = Convert.ToString(noisefloor);
        Tlv2AddRspParam("noiseFloor", noisefloor);
        //data = Convert.ToString(m_ppduType);
        Tlv2AddRspParam("ppduType", ppduType);
        //data = Convert.ToString(m_dcm ? 1 : 0);
        Tlv2AddRspParam("dcm", dualCarrierModulation ? 1 : 0);
        //std::string data = Convert.ToString(m_enANI ? 1 : 0);
        Tlv2AddRspParam("enANI", enANI ? 1 : 0);
        //data = Convert.ToString(ScanProfile);
        Tlv2AddRspParam("scanProfile", ScanProfile);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        std::string data = tlv2GetRspParam("status");
        uint num5 = 0u;
        if (data.length() > 0)
        {
            num5 = atoi(data.c_str());
        }
        if (num5 != 0)
        {
            //throw new WLANCustomException("FTM_WLAN_Atheros_Rx() - error code " + num5);
            LogE_F(L"FTM_WLAN_Atheros_Rx() - error code %d",num5);
            return false;
        }
        //logMessage("FTM_WLAN_Atheros_Rx()");
        return true;
    }

    void CJGW_Phone::SetRxPhyId(PhyID newPhyId)
    {
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)100);
        //string data = Convert.ToString((int)newPhyId);
        Tlv2AddRspParam("phyId", newPhyId);
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
    }

    void CJGW_Phone::GetRxRept(int rxChain,uint& totalPktCount,uint& totalMacPktCount,uint& crcErrPktCount,uint& secErrPktCount,double& rssiDbm,int& evms,int& pilotevms,uint& chainMask,uint& bandwidth,uint& num20MHz,uint& numChain,int& rssiPerChain20Mhz,int& tPut)
    {
        totalPktCount = (totalMacPktCount = (crcErrPktCount = (secErrPktCount = 0u)));
        rssiDbm = 0.0;
        chainMask = (bandwidth = (num20MHz = (numChain = 0u)));

        evms = 0;
        pilotevms = 0;
        rssiPerChain20Mhz = 0;
       
        //evms = new int[8];
        //pilotevms = new int[8];
        //for (int i = 0; i < evms.Length; i++)
        //{
        //    evms[i] = 0;
        //}
        //for (int j = 0; j < pilotevms.Length; j++)
        //{
        //    pilotevms[j] = 0;
        //}
        /*rssiPerChain20Mhz = new int[8];
        for (int k = 0; k < rssiPerChain20Mhz.Length; k++)
        {
        rssiPerChain20Mhz[k] = 0;
        }*/
        int num = 0;
        int num2 = 0;
        int aMpdu = 1;
        tPut = 0;
        QLIB_FTM_WLAN_TLV2_Create(mhResourceContent,(byte)11);
        Tlv2AddRspParam("stopRx", "1");
        QLIB_FTM_WLAN_TLV2_Complete(mhResourceContent);
        if (aMpdu == 0)
        {
            aMpdu = 1u;
        }
        std::string text = tlv2GetRspParam("totalPkt");
        totalPktCount = atoi(text.c_str());
        totalPktCount = (((int)totalPktCount > 0) ? (totalPktCount / aMpdu) : 0u);

        text = tlv2GetRspParam("goodPackets");
        totalMacPktCount = atoi(text.c_str());
        totalMacPktCount = (((int)totalMacPktCount > 0) ? (totalMacPktCount / aMpdu) : 0u);

        text = tlv2GetRspParam("crcErrPkt");
        crcErrPktCount = atoi(text.c_str());
        crcErrPktCount = (((int)crcErrPktCount > 0) ? (crcErrPktCount / aMpdu) : 0u);

        text = tlv2GetRspParam("secErrPkt");
        secErrPktCount = atoi(text.c_str());
        secErrPktCount = (((int)secErrPktCount > 0) ? (secErrPktCount / aMpdu) : 0u);

        text = tlv2GetRspParam("rssi");
        rssiDbm = atoi(text.c_str());

        std::string str = "evm";
        for (int i = 0; i < 8; i++)
        {
            text = tlv2GetRspParam(JGW_GetFormatString("%s%d",str.c_str(),i));
            evms = atoi(text.c_str());
        }
        std::string str2 = "pilotevm";
        for (int m = 0; m < 8; m++)
        {
            text = tlv2GetRspParam(JGW_GetFormatString("%s%d",str2.c_str(),m));
            pilotevms = atoi(text.c_str());
        }
        std::string str3 = "rssiPerChain20Mhz";
        for (int n = 0; n < 8; n++)
        {
            text = tlv2GetRspParam(JGW_GetFormatString("%s%d",str2.c_str(),n));
            rssiPerChain20Mhz = atoi(text.c_str());
        }
        text = tlv2GetRspParam("bandwidth");
        bandwidth = atoi(text.c_str());

        text = tlv2GetRspParam("chainMask");
        chainMask = atoi(text.c_str());

        text = tlv2GetRspParam("numChain");
        numChain = atoi(text.c_str());

        text = tlv2GetRspParam("num20MHz");
        num20MHz = atoi(text.c_str());

        text = tlv2GetRspParam("endTime");
        num = atoi(text.c_str());

        text = tlv2GetRspParam("startTime");
        num -= atoi(text.c_str());

        text = tlv2GetRspParam("byteCount");
        num2 = atoi(text.c_str());

        text = tlv2GetRspParam("dontCount");
        num2 -= atoi(text.c_str());

        if (num > 0 && num2 > 0)
        {
            tPut = (int)((double)(num2 * 8) / (0.001 * (double)num));
        }
        //! logMessage("FTM_WLAN_Atheros_Rx_Report(totalPkt " + totalPktCount + ", Throughput " + tPut + ", rssi " + Convert.ToInt32(rssiDbm) + ", evm0 " + evms[0] + ", evm1 " + evms[1] + ", evm2 " + evms[2] + ", evm3 " + evms[3] + ", evm4 " + evms[4] + ", evm5 " + evms[5] + ", evm6 " + evms[6] + ", evm7 " + evms[7] + ", crcErrPkt " + crcErrPktCount + ", secErrPktCount " + secErrPktCount + ")");
        LogI_F(L"totalPktCount:%d,Throughput:%d,rssi:%.0f,evm1:%d,crcErrPkt:%d,secErrPktCount:%d,",totalPktCount,tPut,rssiDbm,evms,crcErrPktCount,secErrPktCount);
    }
}


