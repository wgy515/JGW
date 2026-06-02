#include "StdAfx.h"
#include "TSE_IPQ807xRxTest.h"

namespace JGW
{
    CTSE_IPQ807xRxTest::CTSE_IPQ807xRxTest(void)
    {
    }


    CTSE_IPQ807xRxTest::~CTSE_IPQ807xRxTest(void)
    {
    }

    const wchar_t* CTSE_IPQ807xRxTest::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xRxTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"RxTestParamPathName",strParamName))
        {
            mstrRxTestParamPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RxTestingStandardPathName",strParamName))
        {
            mstrRxTestingStandardPathName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PhyID",strParamName))
        {
            mePhyID = (PhyID)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PERUL",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvPerUL.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvPerUL.push_back(vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RssiLL",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvRssiLL.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvRssiLL.push_back(vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"StepSize",strParamName))
        {
            std::vector<int> vTemp;
            JGW_ParserIntW(strParamValue,L",",vTemp);
            mvStepSize.clear();
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                mvStepSize.push_back(vTemp[i]);
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PacketCount",strParamName))
        {
            muPacketCount = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xRxTest::TSE_Run()
    {
        return true;
    }
}
