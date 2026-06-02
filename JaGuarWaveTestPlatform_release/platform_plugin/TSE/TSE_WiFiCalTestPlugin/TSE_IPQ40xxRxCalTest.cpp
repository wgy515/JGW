#include "StdAfx.h"
#include "TSE_IPQ40xxRxCalTest.h"
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>
namespace JGW
{
    CTSE_IPQ40xxRxCalTest::CTSE_IPQ40xxRxCalTest(void) : mnPacketCount(200),mfSignalLeveldBm(-60),mnSingleTestErrorRetryCount(3)
    {
    }


    CTSE_IPQ40xxRxCalTest::~CTSE_IPQ40xxRxCalTest(void)
    {
    }

    const wchar_t* CTSE_IPQ40xxRxCalTest::TSE_GetParamDescription()
    {
        //! \"PERPowerUL\": \"Power at which reported PER is measured\",
        //! \"RssiUL\": \"RSSI at Per Power;\",
        return L"{\
                \"RealName\": \"CTSE_IPQ40xxRxCalTest\",\
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

    bool CTSE_IPQ40xxRxCalTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"RxChainMask",strParamName))
        {
            JGW_ParserIntW(strParamValue,L",",mvRxChainMask);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Channel",strParamName))
        {
            JGW_ParserIntW(strParamValue,L",",mvChannel);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ChannelBondingState",strParamName))
        {
            JGW_ParserIntW(strParamValue,L",",mvChannelBondingState);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Rate",strParamName))
        {
            JGW_ParserIntW(strParamValue,L",",mvRate);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PERUL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvPERUL);
        }
        /*else if (TSE_PARAM_NAME_EQUAL(L"PERPowerUL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvPERPowerUL);
        }*/
        else if (TSE_PARAM_NAME_EQUAL(L"RssiLL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvRssiLL);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SingleTestErrorRetryCount",strParamName))
        {
            mnSingleTestErrorRetryCount = _ttoi(strParamValue);
        }
        /*else if (TSE_PARAM_NAME_EQUAL(L"RssiUL",strParamName))
        {
            JGW_ParserFloatW(strParamValue,L",",mvRssiUL);
        }*/
        else if (TSE_PARAM_NAME_EQUAL(L"SignalLeveldBm",strParamName))
        {
            mfSignalLeveldBm = _ttof(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PacketCount",strParamName))
        {
            mnPacketCount = _ttoi(strParamValue);
        }
        /*else if (TSE_PARAM_NAME_EQUAL(L"StartPower",strParamName))
        {
        mfStartPower = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"EndPower",strParamName))
        {
        mfStopPower = static_cast<float>(_ttof(strParamValue));
        }*/
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ40xxRxCalTest::TSE_Init()
    {
        msRxCalTestParam.mnChannel = 2442;
        msRxCalTestParam.meChannelBondingState = primaryHigh;
        msRxCalTestParam.meRxChain = WLAN_CHAIN_2;
        msRxCalTestParam.meRate = RATE_11AC_HT40_MCS9;
        msRxCalTestParam.mePromiscuousMode = PromiscuousMode;
        msRxCalTestParam.mnChannel2 = 0;
        msRxCalTestParam.meAntennaSetting = ANTENNA0;

        //msRxCalTestParam.mfStartPower = -50;
        //msRxCalTestParam.mfStopPower = -70;
        //msRxCalTestParam.mnCoursePktCount = 200;
        //msRxCalTestParam.mdFineStepSize = 0.5;
        //msRxCalTestParam.mnCoursePktCount = 200;
        //msRxCalTestParam.mdCoarseStepSize = 2.0;
        msRxCalTestParam.mbLDPC = false;
        msRxCalTestParam.mbSTBC = false;
        return true;
    }

    bool CTSE_IPQ40xxRxCalTest::TSE_Run()
    {
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
        return true;
    }
}
