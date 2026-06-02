#include "StdAfx.h"
#include "TSE_IPQ807xTxTest.h"

namespace JGW
{
    CTSE_IPQ807xTxTest::CTSE_IPQ807xTxTest(void) : meChannelBondingState(WLAN_Gen6_ChannelBondingState_none),meRateBw(RateBW_CCK),mePhyID(PhyID_A)
    {
    }


    CTSE_IPQ807xTxTest::~CTSE_IPQ807xTxTest(void)
    {
    }

    const wchar_t* CTSE_IPQ807xTxTest::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xTxTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
            mvChainMask.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvChainMask.push_back((WLAN_CHAIN_MASK2)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Rate",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvUniversalDataRate.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvUniversalDataRate.push_back((WLAN_UniversalDataRate)vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PowerLevel",strParamName))
        {
            muPowerLevel = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ExpectPower",strParamName))
        {
            muExpectPower = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RateBw",strParamName))
        {
            meRateBw = (WLAN_UniversalRateBW)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TestLabelName",strParamName))
        {
            mstrTestLabelName = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ChannelBondingState",strParamName))
        {
            meChannelBondingState = (WLAN_Gen6_ChannelBondingState)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxTestParamPathName",strParamName))
        {
            mstrTxTestParamPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxTestingStandardPathName",strParamName))
        {
            mstrTxTestingStandardPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxEvmTest",strParamName))
        {
            mbWlanTxEvmTest = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"WlanTxVerifyMaskTestSegmentMargins",strParamName))
        {
            mbWlanTxVerifyMaskTestSegmentMargins = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PhyID",strParamName))
        {
            mePhyID = (PhyID)_ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xTxTest::TSE_Run()
    {
        //! chainmask channel -  rate
        for (size_t chainMaskIndex = 0;chainMaskIndex < mvChainMask.size();chainMaskIndex++)
        {
            for (size_t channelIndex = 0;channelIndex < mvChannel.size();channelIndex++)
            {
                for (size_t rateIndex = 0;rateIndex < mvUniversalDataRate.size();rateIndex++)
                {

                }
            }
        }

        return false;
    }
}

