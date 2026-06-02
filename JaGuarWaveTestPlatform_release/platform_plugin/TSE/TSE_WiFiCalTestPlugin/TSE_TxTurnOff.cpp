#include "StdAfx.h"
#include "TSE_TxTurnOff.h"

namespace JGW
{
    CTSE_TxTurnOff::CTSE_TxTurnOff(void) : m_channel(5180),m_chain(WLAN_CHAIN_1)
    {
    }


    CTSE_TxTurnOff::~CTSE_TxTurnOff(void)
    {
    }

    const wchar_t* CTSE_TxTurnOff::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_TxTurnOff::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"channel",strParamName))
        {
            m_channel =  _ttoi(strParamValue);
        }
        if (TSE_PARAM_NAME_EQUAL(L"chain",strParamName))
        {
            m_chain =  (WLAN_CHAIN_MASK2)_ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_TxTurnOff::TSE_Run()
    {
        return PHONE.TurnOffTX(m_channel,m_chain);
    }
}

