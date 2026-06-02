#include "StdAfx.h"
#include "TSE_QCA639xRxTest.h"
#include "TSE_QCA639xTxTest.h"
namespace JGW
{
    CTSE_QCA639xRxTest::CTSE_QCA639xRxTest(void) : mnPacketCount(100),mnSingleTestErrorRetryCount(3),mfSignalLeveldBm(-50),meLinkDirection(Up),mePpduType(SingleUser),mbldpcRate(true),mePhyID(PHY_A0)
    {
    }


    CTSE_QCA639xRxTest::~CTSE_QCA639xRxTest(void)
    {
    }

    const wchar_t* CTSE_QCA639xRxTest::TSE_GetParamDescription()
    {
        //! \"PERPowerUL\": \"Power at which reported PER is measured\",
        //! \"RssiUL\": \"RSSI at Per Power;\",
        return L"{\
                \"RealName\": \"CTSE_QCA639xRxTest\",\
                \"TestName\": \"IPQ4019 Rx Cal Test\",\
                \"RxChainMask\": \"rx chain mask\",\
                \"Channel\": \"channel split ,\",\
                \"ChannelBondingState\": \"Channel Bonding State\",\
                \"Rate\": \"Data Rate\",\
                \"PERUL\": \"PER at Per Power; must define Upper Limit\",\
                \"RssiLL\": \"RSSI at Per Power;\",\
                \"SignalLeveldBm\": \"PER Signal Level dBm\",\
                \"PacketCount\": \"number of packets for PER coarse search loop\"\
                }";
    }

    bool CTSE_QCA639xRxTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"ChainMask",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvChainMask2.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChainMask2.push_back((WLAN_CHAIN_MASK2)vTemp[i]);
            }
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
        else if (TSE_PARAM_NAME_EQUAL(L"PhyID",strParamName))
        {
            mePhyID = (WLAN_UniversalPhyID)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SingleTestErrorRetryCount",strParamName))
        {
            mnSingleTestErrorRetryCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"msecDelaybeforePktSend",strParamName))
        {
            mnPacketCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SignalLeveldBm",strParamName))
        {
            mfSignalLeveldBm = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PERUL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvPERUL);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RssiLL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvRssiLL);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QCA639xRxTest::TSE_Init()
    {
        //msRxCalTestParam.mnChannel = 2442;
        //msRxCalTestParam.meChannelBondingState = primaryHigh;
        //msRxCalTestParam.meRxChain = WLAN_CHAIN_2;
        //msRxCalTestParam.meRate = RATE_11AC_HT40_MCS9;
        //msRxCalTestParam.mePromiscuousMode = PromiscuousMode;
        //msRxCalTestParam.mnChannel2 = 0;
        //msRxCalTestParam.meAntennaSetting = ANTENNA0;

        ////msRxCalTestParam.mfStartPower = -50;
        ////msRxCalTestParam.mfStopPower = -70;
        ////msRxCalTestParam.mnCoursePktCount = 200;
        ////msRxCalTestParam.mdFineStepSize = 0.5;
        ////msRxCalTestParam.mnCoursePktCount = 200;
        ////msRxCalTestParam.mdCoarseStepSize = 2.0;
        //msRxCalTestParam.mbLDPC = false;
        //msRxCalTestParam.mbSTBC = false;
        return true;
    }

    bool CTSE_QCA639xRxTest::TSE_Run()
    {
#if 1
        if (0 == mvChannel.size() 
            || mvUniversalRateBW.size() != mvChannelBondingState.size() || 0 == mvUniversalRateBW.size()
            || 0 == mvUniversalNSS.size()
            || 0 == mvUniversalDataRate.size()
            || 0 == mvChainMask2.size()
            || mvPERUL.size() != mvRssiLL.size() || mvChainMask2.size() < mvPERUL.size())
        {
            LogE("Rx Test Param Error");
            return false;
        }

        S_SCPI_PARAM sScpiParam = {0};
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
                            uint numPkts = mnPacketCount;
                            uint dutPortMask = PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)mvChainMask2[chainMaskIndex]);
                            GETINSTRUMENT->SwitchLossPath(false,dutPortMask);
                            PHONE->SetRxPhyId((PhyID)mePhyID);
                            //if (mvChainMask2[chainMaskIndex] >= 2412 && mvChainMask2[chainMaskIndex] <= 2472)
                            //    mbldpcRate = false;
                            //else
                            //    mbldpcRate = true;
                            sScpiParam.nss = mvUniversalNSS[nssIndex];
                            sScpiParam.link = meLinkDirection == Down?"DOWN":(meLinkDirection == Up?"UP":"Direct");
                            //! nBandWidth mcs strWlanStand
                            CTSE_QCA639xTxTest::ConvertBandWidthMCSStanderToRata(mvUniversalRateBW[modeIndex],mvUniversalDataRate[mcsIndex],mvChannelBondingState[modeIndex],sScpiParam,mePpduType);
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
                            

                            bool result = false;
                            for (int count = 0;count < mnSingleTestErrorRetryCount;count ++)
                            {
                                if (!PHONE->ResetRxPacketCounts2(2,mePhyID,mvChannel[channelIndex],0
                                    ,mvChannelBondingState[modeIndex],mvUniversalDataRate[mcsIndex],mvUniversalRateBW[modeIndex],mvUniversalNSS[nssIndex],mvChainMask2[chainMaskIndex]))
                                {
                                    LogE(L"Rx Cal Test SetUpRx Fail");
                                    continue;
                                }
                                GETINSTRUMENT->RxWlanMeasurementConfiguration((WLAN_SINGLE_CHAIN_MASK)mvChainMask2[chainMaskIndex],mvChannel[channelIndex],sScpiParam.nBandWidth,sScpiParam.mcs,sScpiParam.strWlanStand,mvUniversalNSS[nssIndex],mfSignalLeveldBm,mnPacketCount,sScpiParam.ltft,sScpiParam.gint,sScpiParam.link,sScpiParam.strPPDU,sScpiParam.strFEC,sScpiParam.dcm,0);
                                //! => 初校准 + 细校准
                                //! TD0:INSTRUMENT => start -> stop
                                uint totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount;
                                int evms, pilotevms, rssiPerChain20Mhz,tPut;
                                uint chainMask,bandwidth,num20MHz,numChain;
                                double rssiDbm;
                                if (GETINSTRUMENT->CheckVSGSentDone())
                                { 
                                    Sleep(500);
                                    PHONE->GetRxRept(mvChainMask2[chainMaskIndex],totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount,rssiDbm,evms,pilotevms,chainMask,bandwidth,num20MHz,numChain,rssiPerChain20Mhz,tPut);
                                    uint NumberOfBadPkts = 0;
                                    double PacketErrorRatio = 0.00;
                                    if (numPkts > totalMacPktCount)
                                    {
                                        NumberOfBadPkts = numPkts - totalMacPktCount;
                                        PacketErrorRatio = (double)NumberOfBadPkts / (double)numPkts * 100.0;
                                    }
                                    else
                                    {
                                        NumberOfBadPkts = totalPktCount - totalMacPktCount;
                                        PacketErrorRatio = (double)NumberOfBadPkts / (double)totalPktCount * 100.0;
                                        numPkts = totalPktCount;
                                    }    
                                    //double PacketErrorRatio = (double)NumberOfBadPkts / (double)numPkts * 100.0;
                                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Freq\trate\tchain\tPower\tPER\tBad/Total\trssi");
                                    LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%d\t%d\t%d\t%.2f\t%.2f\t%d/%d\t%.2f",mvChannel[channelIndex],mvUniversalRateBW[modeIndex],mvChainMask2[chainMaskIndex],/*msRxCalTestParam.mfStartPower*/mfSignalLeveldBm,PacketErrorRatio,NumberOfBadPkts,numPkts,rssiDbm);

                                    if (PacketErrorRatio > mvPERUL[chainMaskIndex]
                                    || rssiDbm < mvRssiLL[chainMaskIndex])
                                    {
                                        LogE_F(L"Rx Test PER:%.2f,RSSI:%.2f Fail",PacketErrorRatio,rssiDbm);
                                        GETINSTRUMENT->StopRx((WLAN_SINGLE_CHAIN_MASK)mvChainMask2[chainMaskIndex]);
                                        continue;
                                    }
                                }
                                result = true;
                                GETINSTRUMENT->StopRx((WLAN_SINGLE_CHAIN_MASK)mvChainMask2[chainMaskIndex]);
                                break;
                            }
                            if (!result)
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }

#else
        if (0 == mvRxChainMask.size() || 0 == mvChannel.size() || 0 == mvChannelBondingState.size())
        {
            LogE("Rx Cal Test Param Error");
            return false;
        }
        if (mvChannelBondingState.size() != mvRate.size() || mvRate.size() != mvPERUL.size() || mvPERUL.size() != mvRssiLL.size())
        {
            LogE("Rx Cal Test Param Error");
            return false;
        }

        //msRxCalTestParam.mfStartPower = mfStartPower;
        //msRxCalTestParam.mfStopPower = mfStopPower;
        for (size_t rxChainMaskIndex = 0;rxChainMaskIndex < mvRxChainMask.size();rxChainMaskIndex ++)
        {
            msRxCalTestParam.meRxChain = (WLAN_CHAIN_MASK2)mvRxChainMask[rxChainMaskIndex];
			uint dutPortMask = PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)mvRxChainMask[rxChainMaskIndex]);
			GETINSTRUMENT->SwitchLossPath(false,dutPortMask);
            for (size_t channelIndex = 0;channelIndex < mvChannel.size();channelIndex ++)
            {
                msRxCalTestParam.mnChannel = mvChannel[channelIndex];
                for (size_t channelBondingStateIndex = 0;channelBondingStateIndex < mvChannelBondingState.size();channelBondingStateIndex ++)
                {
                    msRxCalTestParam.meChannelBondingState = (WLAN_Gen6_ChannelBondingState)mvChannelBondingState[channelBondingStateIndex];
                    msRxCalTestParam.meRate = (WLAN_RATE)mvRate[channelBondingStateIndex];

                   
#if 1
                    bool result = false;
                    for (int count = 0;count < mnSingleTestErrorRetryCount;count ++)
                    {
                        uint numPkts = mnPacketCount;
                        GETINSTRUMENT->InitVsgModulation(dutPortMask,msRxCalTestParam.meRate,msRxCalTestParam.mnChannel,/*msRxCalTestParam.mfStartPower*/mfSignalLeveldBm,msRxCalTestParam.mbLDPC,msRxCalTestParam.mbSTBC);
                        if (!PHONE->SetUpRx(msRxCalTestParam))
                        {
                            LogE(L"Rx Cal Test SetUpRx Fail");
                            continue;
                        }

                        GETINSTRUMENT->SetVsgRfEnable(dutPortMask,true,numPkts);
                        //! => 初校准 + 细校准
                        //! TD0:INSTRUMENT => start -> stop
                        uint totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount;
                        double rssiDbm;
                        if (GETINSTRUMENT->CheckVSGSentDone())
                        {
                            PHONE->GetRxReport(msRxCalTestParam.meRxChain,totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount,rssiDbm);
                            uint NumberOfBadPkts = numPkts - totalMacPktCount;
                            double PacketErrorRatio = (double)NumberOfBadPkts / (double)numPkts * 100.0;
                            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Freq\trate\tchain\tPower\tPER\tBad/Total\trssi");
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%d\t%d\t%d\t%.2f\t%.2f\t%d/%d\t%.2f",msRxCalTestParam.mnChannel,mvRate[channelBondingStateIndex],mvRxChainMask[rxChainMaskIndex],/*msRxCalTestParam.mfStartPower*/mfSignalLeveldBm,PacketErrorRatio,NumberOfBadPkts,numPkts,rssiDbm);

                            if (PacketErrorRatio > mvPERUL[channelBondingStateIndex]
                            || rssiDbm < mvRssiLL[channelBondingStateIndex])
                            {
                                LogE_F(L"Rx Test PER:%.2f,RSSI:%.2f Fail",PacketErrorRatio,rssiDbm);
                                continue;
                            }
                        }
                        result = true;
                        break;
                    }
                    if (!result)
                    {
                        return false;
                    }
#else
                    uint numPkts = mnPacketCount;
                    GETINSTRUMENT->InitVsgModulation(dutPortMask,msRxCalTestParam.meRate,msRxCalTestParam.mnChannel,/*msRxCalTestParam.mfStartPower*/mfSignalLeveldBm,msRxCalTestParam.mbLDPC,msRxCalTestParam.mbSTBC);
                    if (!PHONE->SetUpRx(msRxCalTestParam))
                    {
                        LogE(L"Rx Cal Test SetUpRx Fail");
                        return false;
                    }
                    GETINSTRUMENT->SetVsgRfEnable(dutPortMask,true,numPkts);
                    //! => 初校准 + 细校准
                    //! TD0:INSTRUMENT => start -> stop
                    uint totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount;
                    double rssiDbm;
                    if (GETINSTRUMENT->CheckVSGSentDone())
                    {
                        PHONE->GetRxReport(msRxCalTestParam.meRxChain,totalPktCount,totalMacPktCount,crcErrPktCount,secErrPktCount,rssiDbm);
                        uint NumberOfBadPkts = numPkts - totalMacPktCount;
                        double PacketErrorRatio = (double)NumberOfBadPkts / (double)numPkts * 100.0;
                        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Freq\trate\tchain\tPower\tPER\tBad/Total\trssi");
                        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%d\t%d\t%d\t%.2f\t%.2f\t%d/%d\t%.2f",msRxCalTestParam.mnChannel,mvRate[channelBondingStateIndex],mvRxChainMask[rxChainMaskIndex],/*msRxCalTestParam.mfStartPower*/mfSignalLeveldBm,PacketErrorRatio,NumberOfBadPkts,numPkts,rssiDbm);

                        if (PacketErrorRatio > mvPERUL[channelBondingStateIndex]
                        || rssiDbm < mvRssiLL[channelBondingStateIndex])
                        {
                            LogE_F(L"Rx Test PER:%.2f,RSSI:%.2f Fail",PacketErrorRatio,rssiDbm);
                            return false;
                        }
                    }
#endif
                }
            }
        }
#endif
        return true;
    }
}
