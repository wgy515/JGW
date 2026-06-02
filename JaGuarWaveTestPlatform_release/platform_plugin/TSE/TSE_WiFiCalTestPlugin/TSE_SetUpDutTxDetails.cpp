#include "StdAfx.h"
#include "TSE_SetUpDutTxDetails.h"

namespace JGW
{
    CTSE_SetUpDutTxDetails::CTSE_SetUpDutTxDetails(void) : m_channel(5180),m_cbState(primary20),m_rate(RATE_AC_MCS_8_20),m_pwrMode(TxPowerForce_CLPC),m_txMode(ContTx99),m_txChainMask(WLAN_CHAIN_1),m_powerLevel(16),m_gain(19),m_digitalGain(0),m_PAcfg(7),m_payLoadSize(65536),m_short11b_nGuard(false),m_ldpcRate(true),m_stbcMode(false),m_aggregation(1),m_dpdMode(DPD_Enabled),m_HC_Enable(HCS_Disabled),m_ifs(1),m_dutyCycle(10),m_nPattern(PN9_PATTERN),m_nPacket(0),m_broadcastMode(1),m_AIFSN(0),m_dutTxSettlingDelayMs(1000),m_scramblerDisable(false),m_channel2(0)
    {
        m_pattern.push_back(0);
    }


    CTSE_SetUpDutTxDetails::~CTSE_SetUpDutTxDetails(void)
    {
    }

    const wchar_t* CTSE_SetUpDutTxDetails::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_SetUpDutTxDetails::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (0 == wcslen(strParamValue))
        {
            return true;
        }
        if (TSE_PARAM_NAME_EQUAL(L"channel",strParamName))
        {
            m_channel =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"cbState",strParamName))
        {
            m_cbState =  (WLAN_Gen6_ChannelBondingState)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"rate",strParamName))
        {
            m_rate =  (WLAN_DataRate)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"pwrMode",strParamName))
        {
            m_pwrMode =  (TxPowerMode)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txMode",strParamName))
        {
            m_txMode =  (WLAN_TX_Mode)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"txChainMask",strParamName))
        {
            m_txChainMask =  (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"powerLevel",strParamName))
        {
            m_powerLevel =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"gain",strParamName))
        {
            m_gain =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"digitalGain",strParamName))
        {
            m_digitalGain =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PAcfg",strParamName))
        {
            m_PAcfg =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"payLoadSize",strParamName))
        {
            m_payLoadSize =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"short11b_nGuard",strParamName))
        {
            m_short11b_nGuard =  (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ldpcRate",strParamName))
        {
            m_ldpcRate =  (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"stbcMode",strParamName))
        {
            m_stbcMode =  (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"aggregation",strParamName))
        {
            m_aggregation =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"dpdMode",strParamName))
        {
            m_dpdMode =  (DPDSetting)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"HC_Enable",strParamName))
        {
            m_HC_Enable =  (HCSetting)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ifs",strParamName))
        {
            m_ifs =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"dutyCycle",strParamName))
        {
            m_dutyCycle =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"nPattern",strParamName))
        {
            m_nPattern =  (TxDataPattern)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"pattern",strParamName))
        {
            std::vector<int> vtemp;
            m_pattern.clear();
            JGW_ParserIntW(strParamValue,L",",vtemp);
            for (size_t i = 0;i < vtemp.size();i ++)
            {
                m_pattern.push_back((byte)vtemp[i]);
            }
            //m_pattern =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"nPacket",strParamName))
        {
            m_nPacket =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"broadcastMode",strParamName))
        {
            m_broadcastMode =  1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"AIFSN",strParamName))
        {
            m_AIFSN =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"dutTxSettlingDelayMs",strParamName))
        {
            m_dutTxSettlingDelayMs =  _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"scramblerDisable",strParamName))
        {
            m_scramblerDisable =  1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"channel2",strParamName))
        {
            m_channel2 =  _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SetUpDutTxDetails::TSE_Run()
    {
        std::vector<std::string> vParams;
        vParams.push_back("freq: ");
        vParams.push_back(JGW_GetFormatString("%d",m_channel));
        vParams.push_back("freq2: ");
        vParams.push_back(JGW_GetFormatString("%d",m_channel));
        vParams.push_back("Rate: ");
        vParams.push_back(JGW_GetFormatString("%d",(int)m_rate));
        vParams.push_back("ChainMask: ");
        vParams.push_back(JGW_GetFormatString("%d",(int)m_txChainMask));
        std::string strTemp;
        JGW_FormatString(strTemp,"%s:%s,%s:%s,%s:%s,%s:%s",vParams[0].c_str(),vParams[1].c_str()
            ,vParams[2].c_str(),vParams[3].c_str()
            ,vParams[4].c_str(),vParams[5].c_str()
            ,vParams[6].c_str(),vParams[7].c_str());
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,JGW_A2W(strTemp).c_str());
        if (m_pwrMode == ForcedDesiredGain || m_pwrMode == ForcedGain)
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"gain: %d,digitalGain:%d,PAcfg:%d",m_gain,m_digitalGain,m_PAcfg);
        }
        else if (m_pwrMode == ForcedGLUTIndex)
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"gain:%d",m_gain);
        }
        else
        {
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"powerLevel:%d",m_powerLevel);
        }
        if (m_dutyCycle < 0u || m_dutyCycle > 100u)
        {
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"dutyCycle parameter should be set within 0 to 100 precent");
            LogE(L"dutyCycle parameter should be set within 0 to 100 precent");
            return false;
        }
        double txPower = m_powerLevel;
        bool result = PHONE.SetUpTxDetail(m_channel, m_channel2, m_cbState, m_rate, m_pwrMode, m_txMode, m_txChainMask, m_powerLevel, m_gain, m_digitalGain, m_PAcfg, m_payLoadSize, m_short11b_nGuard, m_ldpcRate, m_stbcMode, m_aggregation, m_dpdMode, m_HC_Enable, m_ifs, m_dutyCycle, m_nPattern, m_pattern, m_broadcastMode, m_nPacket, m_AIFSN, m_scramblerDisable, txPower);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Tx Power = %d" ,txPower);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Waiting for dutTxSettlingDelayMs = %dms" ,m_dutTxSettlingDelayMs);
        return result;
    }
}
