#include "StdAfx.h"
#include "TSE_QCA639xTxTest.h"

namespace JGW
{
    CTSE_QCA639xTxTest::CTSE_QCA639xTxTest(void) : mnPowerLevel(15),mstrTxTestParamPathName(L"COMMON_PARAM"),mbInitParam(false),mbTestEvm(true),mbTestMask(true),mnSingleTestErrorRetryCount(3),mnNumAverages(3),mfExpectPower(INVAILD_WLAN_RESULT),mePhyID(PHY_A0),meLinkDirection(Down),mePpduType(SingleUser),mbldpcRate(true),mfEvmLL(-99),mfEvmUL(-32),mnTestIntervals(2 * 1000)
    {
        memset(&msTxTestingStandard,0x00,sizeof(S_TX_TestingStandard));
        //memset(&msTxTestParam,0x00,sizeof(S_TxTestParam));
    }


    CTSE_QCA639xTxTest::~CTSE_QCA639xTxTest(void)
    {
    }

    const wchar_t* CTSE_QCA639xTxTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QCA639xTxTest\",\
                \"TestName\": \"QCA639x Tx Test\",\
                \"PowerLevel\": \"Tx power in dBm\",\
                \"ExpectPower\": \"Tx Expect power in dBm\",\
                \"TxTestParamPathName\": \"tx test param path name\",\
                \"TxTestingStandardPathName\": \"tx Testing Standard path name\",\
                \"Channel\": \"channel split ,\",\
                \"TxChainMask\": \"tx chain mask\",\
                \"ChannelBondingState\": \"Channel Bonding State \",\
                \"UniversalRateBW\": \"Channel Bonding State \",\
                \"Rate\": \"Data Rate\",\
                \"WlanRate\": \"wlan Rate WLAN Mode Enums\",\
                \"EvmLL\": \"Minimum EVM out of all numAverages times requested\",\
                \"EvmUL\": \"Maximum EVM out of all numAverages times requested\",\
                \"SingleTestErrorRetryCount\": \"Single Test Error Retry Count\",\
                \"WlanTxEvmTest\": \"is test Wlan Tx EvmT est\",\
                \"WlanTxVerifyMaskTestSegmentMargins\": \"is test Wlan Tx Verify Mask Test Segment Margins\"\
                }";
    }

    bool CTSE_QCA639xTxTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Channel",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvChannel.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChannel.push_back(vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxChainMask",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvChainMask2.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChainMask2.push_back((WLAN_CHAIN_MASK2)vTemp[i]);
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
        else if (TSE_PARAM_NAME_EQUAL(L"UniversalRateBW",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvUniversalRateBW.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvUniversalRateBW.push_back((WLAN_UniversalRateBW)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"UniversalDataRate",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvUniversalDataRate.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvUniversalDataRate.push_back((WLAN_UniversalDataRate)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"UniversalNSS",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvUniversalNSS.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvUniversalNSS.push_back((WLAN_UniversalNSS)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PowerLevel",strParamName))
        {
            mnPowerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PhyID",strParamName))
        {
            mePhyID = (WLAN_UniversalPhyID)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ExpectPower",strParamName))
        {
            mfExpectPower = _ttof(strParamValue);
        }
		else if (TSE_PARAM_NAME_EQUAL(L"NumAverages",strParamName))
		{
			mnNumAverages = _ttoi(strParamValue);
		}
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxEvmTest",strParamName))
        {
            mbTestEvm = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxVerifyMaskTestSegmentMargins",strParamName))
        {
            mbTestMask = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SingleTestErrorRetryCount",strParamName))
        {
            mnSingleTestErrorRetryCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TestIntervals",strParamName))
        {
            mnTestIntervals = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"EvmLL",strParamName))
        {
            //JGW_ParserFloatW(strParamValue,L",",mvfEvmLL);
            mfEvmLL = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"EvmUL",strParamName))
        {
            //JGW_ParserFloatW(strParamValue,L",",mvfEvmUL);
            mfEvmUL = static_cast<float>(_ttof(strParamValue));
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QCA639xTxTest::TSE_Init()
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

    bool CTSE_QCA639xTxTest::TSE_Run()
    {
        if (!mbInitParam)
        {
            LogE_F("Get Tx Test Param To Path Name Fail");
            return false;
        }

        if (0 == mvChannel.size() 
            || mvUniversalRateBW.size() != mvChannelBondingState.size() || 0 == mvUniversalRateBW.size()
            || 0 == mvUniversalNSS.size()
            || 0 == mvUniversalDataRate.size()
            || 0 == mvChainMask2.size())
        {
            LogE("Wlan Tx Test Param Error");
            return false;
        }
        double evmDb = INVAILD_WLAN_RESULT, evmRateMbps = INVAILD_WLAN_RESULT,avgTxPowerDbm = INVAILD_WLAN_RESULT,carrierFreqErrorPpm = INVAILD_WLAN_RESULT, phaseNoiseDb = INVAILD_WLAN_RESULT,symbolClockErrorPpm = INVAILD_WLAN_RESULT, ampImbDb = INVAILD_WLAN_RESULT,loLeakageDbc = INVAILD_WLAN_RESULT,phaseImbDeg = INVAILD_WLAN_RESULT;
        bool maskPass = false;
        double marginsViolationPct = INVAILD_WLAN_RESULT;
        S_SCPI_PARAM sScpiParam = {0};
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Test Project\tMin\tTest\tMax\tResult");
        //! //! channel -> mode -> nss -> mcs -> chain mask -> power
        for (size_t channelIndex = 0;channelIndex < mvChannel.size();channelIndex ++)
        {
            //! mvUniversalRateBW  mvChannelBondingState
            for (size_t modeIndex = 0;modeIndex < mvUniversalRateBW.size();modeIndex ++)
            {
                for (size_t nssIndex = 0; nssIndex < mvUniversalNSS.size();nssIndex ++)
                {
                    for (size_t mcsIndex = 0;mcsIndex < mvUniversalDataRate.size();mcsIndex ++)
                    {
                        for (size_t chainMaskIndex = 0;chainMaskIndex < mvChainMask2.size();chainMaskIndex ++)
                        {
                            double txPower = -999.00;
                            bool result = false;
                            msTxTestingStandard.mvEvm[E_MIN_Collars] = mfEvmLL;
                            msTxTestingStandard.mvEvm[E_MAX_Collars] = mfEvmUL;

                            if (mvChainMask2[chainMaskIndex] >= 2412 && mvChainMask2[chainMaskIndex] <= 2472)
                                mbldpcRate = false;
                            else
                                mbldpcRate = true;
                            sScpiParam.nss = mvUniversalNSS[nssIndex];
                            sScpiParam.nRefPower = static_cast<int>(mfExpectPower);
                            sScpiParam.averCount = mnNumAverages;
                            sScpiParam.trigLev = -25;
                            sScpiParam.link = meLinkDirection == Down?"Down":(meLinkDirection == Up?"Up":"Direct");
                            //! nBandWidth mcs strWlanStand
                            ConvertBandWidthMCSStanderToRata(mvUniversalRateBW[modeIndex],mvUniversalDataRate[mcsIndex],mvChannelBondingState[modeIndex],sScpiParam,mePpduType);
                            switch (mePpduType)
                            {
                            case SingleUser:
                                sScpiParam.strPPDU = "SU";
                                break;
                            case MultipleUsers:
                                sScpiParam.strPPDU = "MU";
                                break;
                            case ExtRangeSingleUser:
                                sScpiParam.strPPDU = "ERSU";
                                break;
                            case Trigger:
                                sScpiParam.strPPDU = "TRIG";
                                break;
                            }
                            sScpiParam.strFEC = mbldpcRate?"LDPC":"BCC";
                            sScpiParam.dcm = 0;
                            sScpiParam.gint = 0.8f;
                            sScpiParam.ltft = 4;
                            
                            msTxTestParam.mnChannel = mvChannel[channelIndex];
                            msTxTestParam.meTxChain = (WLAN_CHAIN_MASK2)mvChainMask2[chainMaskIndex];
                            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"-------------------------------------------------------------");
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Channel\t---\t%u\t---\t---",msTxTestParam.mnChannel);
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Chain\t---\t%d\t---\t---",msTxTestParam.meTxChain);

                            GETINSTRUMENT->SwitchLossPath(true,PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain));
                            Sleep(mnTestIntervals);
                            //PHONE->WlanSetPhyRFMode((PhyID)mePhyID,mePhyID == PHY_A0 ? PhyA_only :PhyB_only,-1);
                            if (!PHONE->SetUpDutTxDetails2(mePhyID,mvChannel[channelIndex],0,
                                mvChannelBondingState[modeIndex],
                                mvUniversalDataRate[mcsIndex],mvUniversalRateBW[modeIndex],
                                mvUniversalNSS[nssIndex],
                                msTxTestParam.mePwrMode,msTxTestParam.meTxMode,mvChainMask2[chainMaskIndex],
                                WLAN_CHAIN_NONE,/*mfExpectPower*/mnPowerLevel,
                                msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,
                                msTxTestParam.muPayLoadSize,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,
                                msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,
                                msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,
                                msTxTestParam.muPacket,msTxTestParam.mbBroadcastMode,msTxTestParam.muiAIFSN,msTxTestParam.muDutTxSettlingDelayMs,msTxTestParam.mbScramblerDisable,msTxTestParam.mbackRequest,msTxTestParam.meGuardInterval,SingleUser,false,txPower))
                            {
                                LogE(L"Set Up Tx Detail Fail");
                                return false;
                            }

                            for (int count = 0;count < mnSingleTestErrorRetryCount;count ++)
                            {
                                Sleep(msTxTestParam.muDutTxSettlingDelayMs);
                                GETINSTRUMENT->TxWlanMeasurementConfiguration(PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)msTxTestParam.meTxChain)
                                    ,mvChannel[channelIndex],evmDb,evmRateMbps,avgTxPowerDbm
                                    ,sScpiParam.nBandWidth,sScpiParam.mcs
                                    ,sScpiParam.nss,sScpiParam.gint
                                    ,sScpiParam.ltft,sScpiParam.link
                                    ,sScpiParam.strPPDU,sScpiParam.strFEC
                                    ,sScpiParam.dcm,sScpiParam.nRefPower
                                    ,sScpiParam.trigLev,sScpiParam.averCount
                                    ,sScpiParam.strWlanStand,mbTestEvm,true,mbTestMask);
                                if (!CheckAvgTxPower(avgTxPowerDbm,mnPowerLevel))
                                {
                                    //PHONE->TurnOffTX(msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                                    continue;
                                }
                                //! 
                                if (mbTestEvm)
                                {
                                    GETINSTRUMENT->GetEVMTxQuality2(carrierFreqErrorPpm,phaseNoiseDb,symbolClockErrorPpm,ampImbDb,loLeakageDbc,phaseImbDeg);
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
                                    GETINSTRUMENT->GetMeasureMask2(maskPass,marginsViolationPct);
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
                                }
                                result = true;
                                break;
                            }
                            PHONE->TurnOffTX2(mePhyID,msTxTestParam.mnChannel,msTxTestParam.meTxChain);
                            if (!result)
                            {
                                return false;
                            }
                        //    if (!PHONE->SetUpDutTxDetails2(mePhyID,mvChannel[channelIndex],0,
                        //        mvChannelBondingState[modeIndex],
                        //        mvUniversalDataRate[mcsIndex],mvUniversalRateBW[modeIndex],
                        //        mvUniversalNSS[nssIndex],
                        //        msTxTestParam.mePwrMode,msTxTestParam.meTxMode,mvChainMask2[chainMaskIndex],
                        //        WLAN_CHAIN_NONE,/*mfExpectPower*/mnPowerLevel,
                        //        msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,
                        //        msTxTestParam.muPayLoadSize,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,
                        //        msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,
                        //        msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,
                        //        msTxTestParam.muPacket,msTxTestParam.mbBroadcastMode,msTxTestParam.muiAIFSN,msTxTestParam.muDutTxSettlingDelayMs,msTxTestParam.mbScramblerDisable,msTxTestParam.mbackRequest,msTxTestParam.meGuardInterval,SingleUser,false,txPower))
                        //    {
                        //        LogE(L"Set Up Tx Detail Fail");
                        //        return false;
                        //    }
                        }
                    }
                }
            }
        }
#if 0
        if (mvDataRate.size() != mvWlanRate.size() 
            && mvWlanRate.size() != mvChannelBondingState.size()
            && mvChannelBondingState.size() != mvfEvmLL.size()
            && mvfEvmUL.size() != mvfEvmLL.size())
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
                    msTxTestingStandard.mvEvm[E_MIN_Collars] = mvfEvmLL[txRateIndex];
                    msTxTestingStandard.mvEvm[E_MAX_Collars] = mvfEvmUL[txRateIndex];
                    msTxTestParam.mfPowerLevel = mnPowerLevel;
                    msTxTestParam.meChannelBondingState = mvChannelBondingState[txRateIndex];
                    msTxTestParam.meWlanRate = mvWlanRate[txRateIndex];
                    msTxTestParam.meRate = mvDataRate[txRateIndex];
                    double txPower = -999.00;
                    bool result = false;

                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"-------------------------------------------------------------");
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Channel\t---\t%u\t---\t---",msTxTestParam.mnChannel);
                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Chain\t---\t%d\t---\t---",msTxTestParam.meTxChain);

					//if (!PHONE->SetUpTxDetail(msTxTestParam.mnChannel,msTxTestParam.mnChannel2,msTxTestParam.meChannelBondingState,msTxTestParam.meRate,msTxTestParam.mePwrMode,msTxTestParam.meTxMode,msTxTestParam.meTxChain,msTxTestParam.mfPowerLevel,msTxTestParam.muGain,msTxTestParam.mnDigitalGain,msTxTestParam.muPASetting,msTxTestParam.muPayLoadSize,msTxTestParam.mbShort11b_nGuard,msTxTestParam.mbLdpcRate,msTxTestParam.mbSTBCmode,msTxTestParam.muAggregation,msTxTestParam.meDpdMode,msTxTestParam.meHC_Enable,msTxTestParam.mnIfs,msTxTestParam.mnDutyCycle,msTxTestParam.mePattern,msTxTestParam.mvPattern,msTxTestParam.mbBroadcastMode,msTxTestParam.muPacket,msTxTestParam.muiAIFSN,msTxTestParam.mbScramblerDisable,txPower))
                    if (!PHONE->SetUpDutTxDetails2(mePhyID,msTxTestParam.mnChannel,msTxTestParam.mnChannel2,msTxTestParam.meChannelBondingState,msTxTestParam.meWlanRate))
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
                        //! 
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
                        }
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
#endif
        return true;
    }

    bool CTSE_QCA639xTxTest::CheckEVM(double val)
    {
        if (!msTxTestingStandard.mbTestEvm) return true;
        bool result = msTxTestingStandard.mvEvm[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvEvm[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"EVM\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvEvm[E_MIN_Collars],val,msTxTestingStandard.mvEvm[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckAmplErr(double val)
    {
        if (!msTxTestingStandard.mbTestAmplErr) return true;
        bool result = msTxTestingStandard.mvAmplErr[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvAmplErr[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Amplitude Imbalance Error\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvAmplErr[E_MIN_Collars],val,msTxTestingStandard.mvAmplErr[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckDataRateMB(double val)
    {
        if (!msTxTestingStandard.mbTestDataRateMB) return true;
        bool result = msTxTestingStandard.mvDataRateMB[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvDataRateMB[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Data Rate\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvDataRateMB[E_MIN_Collars],val,msTxTestingStandard.mvDataRateMB[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckSymbolClockError(double val)
    {
        if (!msTxTestingStandard.mbTestSymbolClockError) return true;
        bool result = msTxTestingStandard.mvSymbolClockError[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvSymbolClockError[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Symbol Clock Error\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvSymbolClockError[E_MIN_Collars],val,msTxTestingStandard.mvSymbolClockError[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckFreqErr(double val)
    {
        if (!msTxTestingStandard.mbTestFreqErr) return true;
        bool result = msTxTestingStandard.mvFreqErr[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvFreqErr[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Frequency Error\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvFreqErr[E_MIN_Collars],val,msTxTestingStandard.mvFreqErr[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckPhaseErr(double val)
    {
        if (!msTxTestingStandard.mbTestPhaseErr) return true;
        bool result = msTxTestingStandard.mvPhaseErr[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvPhaseErr[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Phase Imbalance Error\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvPhaseErr[E_MIN_Collars],val,msTxTestingStandard.mvPhaseErr[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::CheckAvgTxPower(double val,double fExpectPower)
    {
        if (!msTxTestingStandard.mbTestAvgTxPowerDelta) return true;
        double vAvgTxPower[COLLARS_ARRAY_SIZE] = {msTxTestingStandard.mvAvgTxPowerDelta[E_MIN_Collars] + fExpectPower,msTxTestingStandard.mvAvgTxPowerDelta[E_MAX_Collars] + fExpectPower};
        bool result = vAvgTxPower[E_MIN_Collars] <= val && val <= vAvgTxPower[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Avg Power\t%.2f\t%.2f\t%.2f\t%s",vAvgTxPower[E_MIN_Collars],val,vAvgTxPower[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    /* bool CTSE_QCA639xTxTest::CheckAvgTxPowerDelta(double val)
    {
    bool result = mvAvgTxPowerDelta[E_MIN_Collars] <= val && val <= mvAvgTxPowerDelta[E_MAX_Collars];
    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Power Level Delta\t%.2f\t%.2f\t%.2f\t%s",mvAvgTxPowerDelta[E_MIN_Collars],val,mvAvgTxPowerDelta[E_MAX_Collars],result ? L"PASS" : L"FAIL");
    return result;
    }*/

    bool CTSE_QCA639xTxTest::CheckLoLeakager(double val)
    {
        if (!msTxTestingStandard.mbTestLoLeakage) return true;
        bool result = msTxTestingStandard.mvLoLeakage[E_MIN_Collars] <= val && val <= msTxTestingStandard.mvLoLeakage[E_MAX_Collars];
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"LO Leakage\t%.2f\t%.2f\t%.2f\t%s",msTxTestingStandard.mvLoLeakage[E_MIN_Collars],val,msTxTestingStandard.mvLoLeakage[E_MAX_Collars],result ? L"PASS" : L"FAIL");
        return result;
    }

    bool CTSE_QCA639xTxTest::ConvertBandWidthMCSStanderToRata(WLAN_UniversalRateBW rateBw,WLAN_UniversalDataRate dataRate,WLAN_Gen6_ChannelBondingState cbState,S_SCPI_PARAM& sScpiPara,PPDU_TYPE& ePpduType)
    {
        WLAN_UniversalRateBW val3 = rateBw;
        WLAN_STANDARD wlanStandrd;
        switch ((int)val3)
        {
        case 0:
            switch ((int)dataRate)
            {
            case 0:
                wlanStandrd = WLAN_STANDARD_RATE_11B_LONG_1_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 1:
                wlanStandrd = WLAN_STANDARD_RATE_11B_LONG_2_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 2:
                wlanStandrd = WLAN_STANDARD_RATE_11B_LONG_5_5_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 3:
                wlanStandrd = WLAN_STANDARD_RATE_11B_LONG_11_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 4:
                wlanStandrd = WLAN_STANDARD_RATE_11B_SHORT_2_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 5:
                wlanStandrd = WLAN_STANDARD_RATE_11B_SHORT_5_5_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            case 6:
                wlanStandrd = WLAN_STANDARD_RATE_11B_SHORT_11_MBPS;
                sScpiPara.strWlanStand = "WIFI_11B";
                break;
            default:
                //LogE_F("ConvertDutToScpiChannelWaveformParameters() failed: the '" + ((object)0).ToString() + "' does not support '" + ((object)rate).ToString() + "'");
                return false;
            }
            sScpiPara.mcs = 999;
            sScpiPara.nBandWidth  = 22;
            ePpduType = SingleUser;
            break;
        case 1:
            {
                sScpiPara.strWlanStand = "WIFI_11G";
                switch (dataRate - 10)
                {
                case 0:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_6_MBPS;
                    break;
                case 1:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_9_MBPS;
                    break;
                case 2:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_12_MBPS;
                    break;
                case 3:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_18_MBPS;
                    break;
                case 4:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_24_MBPS;
                    break;
                case 5:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_36_MBPS;
                    break;
                case 6:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_48_MBPS;
                    break;
                case 7:
                    wlanStandrd = WLAN_STANDARD_RATE_11A_54_MBPS;
                    break;
                default:
                    //LogError("ConvertDutToScpiChannelWaveformParameters() failed: the '" + ((object)1).ToString() + "' does not support '" + ((object)rate).ToString() + "'");
                    return false;
                }
                sScpiPara.mcs = 999;
                sScpiPara.nBandWidth = 20;
                if ((int)cbState == 50)
                {
                    sScpiPara.nBandWidth = 10;
                }
                if ((int)cbState == 51)
                {
                    sScpiPara.nBandWidth = 5;
                }
                ePpduType = SingleUser;
                break;
            }
        case 2:
        case 3:
            {
                sScpiPara.strWlanStand = "WIFI_11N";
                sScpiPara.mcs = dataRate - 20;
                wlanStandrd = WLAN_STANDARD_RATE_11N;
                sScpiPara.nBandWidth = (((int)rateBw == 2) ? 20 : 40);
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 50)
                {
                    sScpiPara.nBandWidth = 10;
                }
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 51)
                {
                    sScpiPara.nBandWidth = 5;
                }
                break;
            }
        case 4:
        case 5:
        case 6:
        case 7:
        case 20:
        case 21:
            {
                //wlanStandrd = (((object)rateBw).ToString().Contains("80P80") ? WLAN_STANDARD.RATE_11AC_80P80 : WLAN_STANDARD.RATE_11AC);
                sScpiPara.mcs = dataRate - 20;
                sScpiPara.strWlanStand = "WIFI_11AC";
                wlanStandrd = WLAN_STANDARD_RATE_11AC;
                sScpiPara.nBandWidth = (((int)rateBw == 4) ? 20 : (((int)rateBw == 5) ? 40 : (((int)rateBw == 20) ? 160 : (((int)rateBw == 21) ? 165 : 80))));
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 50)
                {
                    sScpiPara.nBandWidth = 10;
                }
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 51)
                {
                    sScpiPara.nBandWidth = 5;
                }
                break;
            }
        case 8:
        case 9:
        case 10:
        case 11:
        case 16:
        case 18:
        case 22:
            {
                //string text = ((object)rateBw).ToString().ToUpper();
                //wLAN_STANDARD = (text.Contains("80P80") ? WLAN_STANDARD.RATE_11AX_80P80 : WLAN_STANDARD.RATE_11AX);
                wlanStandrd = WLAN_STANDARD_RATE_11AX;
                sScpiPara.mcs = dataRate - 20;
                sScpiPara.strWlanStand = "WIFI_11AX";
                sScpiPara.nBandWidth = (((int)rateBw == 8) ? 20 : (((int)rateBw == 9) ? 40 : (((int)rateBw == 16) ? 160 : (((int)rateBw == 18) ? 165 : (((int)rateBw == 22) ? 320 : 80)))));
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 50)
                {
                    sScpiPara.nBandWidth = 10;
                }
                if (sScpiPara.nBandWidth == 20 && (int)cbState == 51)
                {
                    sScpiPara.nBandWidth = 5;
                }
                break;
            }
        default:
            return false;
        }



        return true;
    }
}
