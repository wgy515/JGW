#include "StdAfx.h"
#include "TSE_IPQ40xxGuLossCal.h"
#include "JGW_IPQ40xxGuTestData.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CTSE_IPQ40xxGuLossCal::CTSE_IPQ40xxGuLossCal(void) : mnPowerLevel(15),mstrTxTestParamPathName(L"COMMON_PARAM"),mbInitParam(false),mnSingleTestErrorRetryCount(3),mnNumAverages(3),mfExpectPower(INVAILD_WLAN_RESULT),mbTestEvm(true),mbTestMask(true),mbSaveGuTestData(false)
    {
        memset(&msTxTestingStandard,0x00,sizeof(S_TX_TestingStandard));
    }


    CTSE_IPQ40xxGuLossCal::~CTSE_IPQ40xxGuLossCal(void)
    {
    }

    const wchar_t* CTSE_IPQ40xxGuLossCal::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_IPQ40xxGuLossCal\",\
                \"TestName\": \"IPQ40xx Gu Loss Calibration\",\
                \"PowerLevel\": \"Tx power in dBm\",\
                \"ExpectPower\": \"Tx Expect power in dBm\",\
                \"TxTestParamPathName\": \"tx test param path name\",\
                \"TxTestingStandardPathName\": \"tx Testing Standard path name\",\
                \"Channel\": \"channel split ,\",\
                \"TxChainMask\": \"tx chain mask\",\
                \"ChannelBondingState\": \"Channel Bonding State \",\
                \"Rate\": \"Data Rate\",\
                \"WlanRate\": \"wlan Rate WLAN Mode Enums\",\
                \"SNEnvrionment\": \"Serial Number Envrinment\",\
                \"SaveGuTestData\": \"Save Gu Test Data\",\
                \"SaveGuCalDataFileEnv\": \"Save Gu Cal Test Data File Envrinment\",\
                \"WlanTxEvmTest\": \"is test Wlan Tx EvmT est\",\
                \"WlanTxVerifyMaskTestSegmentMargins\": \"is test Wlan Tx Verify Mask Test Segment Margins\",\
                \"SingleTestErrorRetryCount\": \"Single Test Error Retry Count\"\
                }";
    }

    bool CTSE_IPQ40xxGuLossCal::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Channel",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvChannel.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                if (i == 0 && vTemp[i] >= 2412 && vTemp[i] <= 2472)
                {
                    CJGW_IPQ40xxGuTestData::GetInstance()->UpdateIPQ40xxGuTest24ChannelInfo(strParamValue);
                }
                if (i == 0 && vTemp[i] >= 4920 && vTemp[i] <= 5920)
                {
                    CJGW_IPQ40xxGuTestData::GetInstance()->UpdateIPQ40xxGuTest5ChannelInfo(strParamValue);
                }
                mvChannel.push_back(vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxChainMask",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvTxChainMask.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvTxChainMask.push_back((WLAN_CHAIN_MASK2)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxTestParamPathName",strParamName))
        {
            mstrTxTestParamPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxTestingStandardPathName",strParamName))
        {
            mstrTxTestingStandardPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SNEnvrionment",strParamName))
        {
            mstrSNEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveGuCalDataFileEnv",strParamName))
        {
            mstrSaveGuCalDataFileEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveGuTestData",strParamName))
        {
            mbSaveGuTestData = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ChannelBondingState",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvChannelBondingState.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChannelBondingState.push_back((WLAN_Gen6_ChannelBondingState)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Rate",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvDataRate.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvDataRate.push_back((WLAN_DataRate)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanRate",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvWlanRate.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvWlanRate.push_back((WLAN_RATE)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PowerLevel",strParamName))
        {
            mnPowerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ExpectPower",strParamName))
        {
            mfExpectPower = _ttof(strParamValue);
        }
		else if (TSE_PARAM_NAME_EQUAL(L"NumAverages",strParamName))
		{
			mnNumAverages = _ttoi(strParamValue);
		}
        else if (TSE_PARAM_NAME_EQUAL(L"SingleTestErrorRetryCount",strParamName))
        {
            mnSingleTestErrorRetryCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxEvmTest",strParamName))
        {
            mbTestEvm = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxVerifyMaskTestSegmentMargins",strParamName))
        {
            mbTestMask = 1 == _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ40xxGuLossCal::TSE_Init()
    {
        msTxTestParam.mnChannel = 5500;
        msTxTestParam.meChannelBondingState = primary20;
        msTxTestParam.meRate = RATE_AC_MCS_8_20;
        msTxTestParam.meWlanRate = RATE_11AC_HT20_MCS8;
        msTxTestParam.meTxChain = WLAN_CHAIN_1;
        msTxTestParam.mfPowerLevel = mnPowerLevel;
        mbInitParam = true;
        mbInitParam &= GetWlanConfigFile()->GetTxParamToPatnName(mstrTxTestParamPathName,msTxTestParam);
        mbInitParam &= GetWlanConfigFile()->GetTestingStandardToPatnName(mstrTxTestingStandardPathName,msTxTestingStandard);
        msTxTestingStandard.mbTestEvm = true;
        LogI_F(L"Evm:%d,AmplErr:%d,DataRateMB:%d,SymbolClockError:%d,FreqErr:%d,PhaseErr:%d,LoLeakage:%d,AvgTxPowerDelta:%d",msTxTestingStandard.mbTestEvm?1:0,msTxTestingStandard.mbTestAmplErr?1:0,msTxTestingStandard.mbTestDataRateMB?1:0,msTxTestingStandard.mbTestSymbolClockError?1:0,msTxTestingStandard.mbTestFreqErr?1:0,msTxTestingStandard.mbTestPhaseErr?1:0,msTxTestingStandard.mbTestLoLeakage?1:0,msTxTestingStandard.mbTestAvgTxPowerDelta?1:0);
        mfExpectPower = (mfExpectPower == INVAILD_WLAN_RESULT) ? mnPowerLevel : mfExpectPower;
        return mbInitParam;
    }

    bool CTSE_IPQ40xxGuLossCal::TSE_Run()
    {
        if (!mbInitParam)
        {
            LogE_F("Get Tx Test Param To Path Name Fail");
            return false;
        }

        if (mvDataRate.size() != mvWlanRate.size() 
            && mvWlanRate.size() != mvChannelBondingState.size())
        {
            LogE("Wlan Tx Test Param Error");
            return false;
        }

        double evmDb = INVAILD_WLAN_RESULT, evmRateMbps = INVAILD_WLAN_RESULT,avgTxPowerDbm = INVAILD_WLAN_RESULT,carrierFreqErrorPpm = INVAILD_WLAN_RESULT, phaseNoiseDb = INVAILD_WLAN_RESULT,symbolClockErrorPpm = INVAILD_WLAN_RESULT, ampImbDb = INVAILD_WLAN_RESULT,loLeakageDbc = INVAILD_WLAN_RESULT,phaseImbDeg = INVAILD_WLAN_RESULT;
        bool maskPass = false;
        double marginsViolationPct = INVAILD_WLAN_RESULT;
		LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Test Project\tMin\tTest\tMax\tResult");
        //! double marginsViolationPct,  avgTxPowerDbm,  double[][] marginsDbr,  double[][] marginsFreqHz;
        for (size_t channelIndex = 0;channelIndex != mvChannel.size();channelIndex++)
        {
            msTxTestParam.mnChannel = mvChannel[channelIndex];
            for (size_t txChainMaskIndex = 0; txChainMaskIndex != mvTxChainMask.size();txChainMaskIndex++)
            {
                msTxTestParam.meTxChain = (WLAN_CHAIN_MASK2)mvTxChainMask[txChainMaskIndex];
				GETINSTRUMENT->SwitchLossPath(true,PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain));
                for (size_t txRateIndex = 0;txRateIndex < mvChannelBondingState.size();txRateIndex ++)
                {
                    msTxTestParam.mfPowerLevel = mnPowerLevel;
                    msTxTestParam.meChannelBondingState = mvChannelBondingState[txRateIndex];
                    msTxTestParam.meWlanRate = mvWlanRate[txRateIndex];
                    msTxTestParam.meRate = mvDataRate[txRateIndex];
                    double txPower;
                    bool result = false;

                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"-------------------------------------------------------------");
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Channel\t---\t%u\t---\t---",msTxTestParam.mnChannel);
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Chain\t---\t%d\t---\t---",msTxTestParam.meTxChain);

					if (!PHONE->SetUpTxDetail(msTxTestParam.mnChannel,msTxTestParam.mnChannel2,msTxTestParam.meChannelBondingState,msTxTestParam.meRate,msTxTestParam.mePwrMode,msTxTestParam.meTxMode,msTxTestParam.meTxChain,msTxTestParam.mfPowerLevel,msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,msTxTestParam.muPayLoadSize,msTxTestParam.mbShort11b_nGuard,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,msTxTestParam.mbBroadcastMode,msTxTestParam.muPacket,msTxTestParam.muiAIFSN,msTxTestParam.mbScramblerDisable,txPower))
					{
						LogE(L"Set Up Tx Detail Fail");
						return false;
					}
#if 1
                    for (int count = 0;count < mnSingleTestErrorRetryCount;count ++)
                    {
                        Sleep(msTxTestParam.muDutTxSettlingDelayMs);
                        GETINSTRUMENT->MeasureEVM(PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain),msTxTestParam.meWlanRate,msTxTestParam.mnChannel,/*msTxTestParam.mfPowerLevel*/mfExpectPower,evmDb,evmRateMbps,avgTxPowerDbm,mbTestEvm,mbTestMask,mnNumAverages);
                        if (!CheckAvgTxPower(avgTxPowerDbm,mfExpectPower))
                        {
                            //PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                            continue;
                        }
                        CJGW_IPQ40xxGuTestData::GetInstance()->UpdateIPQ40xxGuTestData(txChainMaskIndex,msTxTestParam.mnChannel,avgTxPowerDbm);
                        //! 
                        /*if (mbTestEvm)
                        {
                            GETINSTRUMENT->GetEVMTxQuality(carrierFreqErrorPpm,phaseNoiseDb,symbolClockErrorPpm,ampImbDb,loLeakageDbc,phaseImbDeg);
                            if (!(CheckEVM(evmDb) 
                                && CheckDataRateMB(evmRateMbps)
                                && CheckFreqErr(carrierFreqErrorPpm)
                                && CheckSymbolClockError(symbolClockErrorPpm)
                                && CheckAmplErr(ampImbDb)
                                && CheckLoLeakager(loLeakageDbc)
                                && CheckPhaseErr(phaseImbDeg)))
                            {
                                //PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                                continue;
                            }
                        }
                        //! 
                        if (mbTestMask)
                        {
                            GETINSTRUMENT->GetMeasureMask(maskPass,marginsViolationPct);
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Pass/Fail WLAN Mask\t---\t%s\t---\t%s",maskPass ? L"PASS" : L"FAIL",maskPass ? L"PASS" : L"FAIL");
                            if (!maskPass)
                            {
                                //PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                                continue;
                            }
                            if (0 <= marginsViolationPct && marginsViolationPct <= 1)
                            {
                                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"margins Violation Pct\t0\t%.2f\t1\t%s",marginsViolationPct,L"PASS");
                            }
                            else
                            {
                                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"margins Violation Pct\t0\t%.2f\t1\t%s",marginsViolationPct,L"FAIL");
                                //PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                                continue;
                            }
                        }*/
                        result = true;
                        break;
                    }
					PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                    if (!result)
                    {
                        return false;
                    }
#else
                    for (int count = 0;count < 3;count ++)
                    {
                        if (!PHONE->SetUpTxDetail(msTxTestParam.mnChannel,msTxTestParam.mnChannel2,msTxTestParam.meChannelBondingState,msTxTestParam.meRate,msTxTestParam.mePwrMode,msTxTestParam.meTxMode,msTxTestParam.meTxChain,msTxTestParam.mfPowerLevel,msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,msTxTestParam.muPayLoadSize,msTxTestParam.mbShort11b_nGuard,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,msTxTestParam.mbBroadcastMode,msTxTestParam.muPacket,msTxTestParam.muiAIFSN,msTxTestParam.mbScramblerDisable,txPower))
                        {
                            LogE(L"Set Up Tx Detail Fail");
                            return false;
                        }
                        Sleep(msTxTestParam.muDutTxSettlingDelayMs);
                        GETINSTRUMENT->MeasureEVM(PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain),msTxTestParam.meWlanRate,msTxTestParam.mnChannel,msTxTestParam.mfPowerLevel,evmDb,evmRateMbps,avgTxPowerDbm,mbTestEvm,mbTestMask,mnNumAverages);
                        if (avgTxPowerDbm == INVAILD_WLAN_RESULT || avgTxPowerDbm > 999)
                        {
                            continue;
                        }
                        break;
                    }
                    /*if (!PHONE->SetUpTxDetail(msTxTestParam.mnChannel,msTxTestParam.mnChannel2,msTxTestParam.meChannelBondingState,msTxTestParam.meRate,msTxTestParam.mePwrMode,msTxTestParam.meTxMode,msTxTestParam.meTxChain,msTxTestParam.mfPowerLevel,msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,msTxTestParam.muPayLoadSize,msTxTestParam.mbShort11b_nGuard,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,msTxTestParam.mbBroadcastMode,msTxTestParam.muPacket,msTxTestParam.muiAIFSN,msTxTestParam.mbScramblerDisable,txPower))
                    {
                    LogE(L"Set Up Tx Detail Fail");
                    return false;
                    }
                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"-------------------------------------------------------------");
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Channel\t---\t%u\t---\t---",msTxTestParam.mnChannel);
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Chain\t---\t%d\t---\t---",msTxTestParam.meTxChain);
                    Sleep(msTxTestParam.muDutTxSettlingDelayMs);
                    GETINSTRUMENT->MeasureEVM(PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain),msTxTestParam.meWlanRate,msTxTestParam.mnChannel,msTxTestParam.mfPowerLevel,evmDb,evmRateMbps,avgTxPowerDbm,mbTestEvm,mbTestMask,mnNumAverages);*/

                    if (!CheckAvgTxPower(avgTxPowerDbm))
                    {
                        PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                        return false;
                    }
                    if (mbTestEvm)
                    {
                        GETINSTRUMENT->GetEVMTxQuality(carrierFreqErrorPpm,phaseNoiseDb,symbolClockErrorPpm,ampImbDb,loLeakageDbc,phaseImbDeg);
                        if (!(CheckEVM(evmDb) 
                            && CheckDataRateMB(evmRateMbps)
                            && CheckFreqErr(carrierFreqErrorPpm)
                            && CheckSymbolClockError(symbolClockErrorPpm)
                            && CheckAmplErr(ampImbDb)
                            && CheckLoLeakager(loLeakageDbc)
                            && CheckPhaseErr(phaseImbDeg)))
                        {
                            PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                            return false;
                        }
                    }
                    if (mbTestMask)
                    {
                        GETINSTRUMENT->GetMeasureMask(maskPass,marginsViolationPct);
                        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Pass/Fail WLAN Mask\t---\t%s\t---\t%s",maskPass ? L"PASS" : L"FAIL",maskPass ? L"PASS" : L"FAIL");
                        if (!maskPass)
                        {
                            PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                            return false;
                        }
                        if (0 <= marginsViolationPct && marginsViolationPct <= 1)
                        {
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"margins Violation Pct\t0\t%.2f\t1\t%s",marginsViolationPct,L"PASS");
                        }
                        else
                        {
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"margins Violation Pct\t0\t%.2f\t1\t%s",marginsViolationPct,L"FAIL");
                            PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                            return false;
                        }
                    }
                    PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
#endif
                }
            }
        }

        if (mbSaveGuTestData)
        {
            std::wstring strSaveGuCalDataFile = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrSaveGuCalDataFileEnv).c_str());
            std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvrionment);
            std::wstring strSaveGuCalDataFolder = JGW_GetFileFolderToFilePath(strSaveGuCalDataFile.c_str());

            if (!JGW_PathFileExists(strSaveGuCalDataFolder.c_str()))
            {
                JGW_CreateDirectory(strSaveGuCalDataFile.c_str(),true);
            }
           
            CJGW_IPQ40xxGuTestData::GetInstance()->SaveIPQ40xxGuTestData(strSN,strSaveGuCalDataFile);
        }

        return true;
    }

    bool CTSE_IPQ40xxGuLossCal::CheckAvgTxPower(double val,double fExpectPower)
    {
        if (!msTxTestingStandard.mbTestAvgTxPowerDelta) return true;
        double vAvgTxPower[COLLARS_ARRAY_SIZE] = {msTxTestingStandard.mvAvgTxPowerDelta[E_MIN_Collars] + fExpectPower,msTxTestingStandard.mvAvgTxPowerDelta[E_MAX_Collars] + fExpectPower};
        bool result = vAvgTxPower[E_MIN_Collars] <= val && val <= vAvgTxPower[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Avg Power\t%.2f\t%.2f\t%.2f\t%s",vAvgTxPower[E_MIN_Collars],val,vAvgTxPower[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

}
