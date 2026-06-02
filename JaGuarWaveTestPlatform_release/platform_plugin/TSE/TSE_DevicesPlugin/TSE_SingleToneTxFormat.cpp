#include "StdAfx.h"
#include "TSE_SingleToneTxFormat.h"

namespace JGW
{
    CTSE_SingleToneTxFormat::CTSE_SingleToneTxFormat(void) : muTxPower(23),muTxChannel(25500),mstrSaveSingleToneTxEnvironment(L"TSE_SINGLE_TONE_TX")
    {
    }


    CTSE_SingleToneTxFormat::~CTSE_SingleToneTxFormat(void)
    {
    }

    const wchar_t* CTSE_SingleToneTxFormat::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_SingleToneTxFormat::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"TxPower",strParamName))
        {
            muTxPower = static_cast<short>(_ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TxChannel",strParamName))
        {
            muTxChannel = static_cast<unsigned int>(_ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveSingleToneTxEnvironment",strParamName))
        {
            mstrSaveSingleToneTxEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    std::wstring CTSE_SingleToneTxFormat::GetSingleToneTxCommandFormat(unsigned int channel,short txPower)
    {
        unsigned char szLitteEndian[4] = {0};
        memcpy(szLitteEndian,&channel,4);

        short tx_power = txPower * 256;
        unsigned char szLitteEndianPower[2] = {0};
        memcpy(szLitteEndianPower,&tx_power,2);

        std::wstring strTemp;
        JGW::JGW_FormatWString(strTemp,L"L%02X%02X%02X%02X00D9%02X%02X",szLitteEndian[0],szLitteEndian[1],szLitteEndian[2],szLitteEndian[3],szLitteEndianPower[0],szLitteEndianPower[1]);

        return strTemp;
    }

    bool CTSE_SingleToneTxFormat::TSE_Run()
    {
        if (mstrSaveSingleToneTxEnvironment.empty() || muTxChannel <= 0)
        {
            LogE(L"Param Config Fail");
            return false;
        }

        std::wstring strSingleToneTx = GetSingleToneTxCommandFormat(muTxChannel,muTxPower);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",mstrSaveSingleToneTxEnvironment.c_str(),strSingleToneTx.c_str());
        GetGlobalEnvironment()->PutString(mstrSaveSingleToneTxEnvironment,strSingleToneTx);

        return true;
    }
}
