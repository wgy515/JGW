#include "StdAfx.h"
#include "TSE_ContinousRxCommandFormat.h"

namespace JGW
{
    CTSE_ContinousRxCommandFormat::CTSE_ContinousRxCommandFormat(void) : muRxPower(-60),muRxChannel(7600)
    {
    }


    CTSE_ContinousRxCommandFormat::~CTSE_ContinousRxCommandFormat(void)
    {
    }

    std::wstring CTSE_ContinousRxCommandFormat::GetContinousRxCommandFormat(unsigned int rx_earfcn,short rx_power)
    {
        unsigned char szLitteEndian[4] = {0};
        memcpy(szLitteEndian,&rx_earfcn,4);

        short rxPower = rx_power * 256;
        unsigned char szLitteEndianPower[2] = {0};
        memcpy(szLitteEndianPower,&rxPower,2);

        std::wstring strTemp;
        JGW::JGW_FormatWString(strTemp,L"L%02X%02X%02X%02XFF00%02X%02X",szLitteEndian[0],szLitteEndian[1],szLitteEndian[2],szLitteEndian[3],szLitteEndianPower[0],szLitteEndianPower[1]);

        return strTemp;
    }

    const wchar_t* CTSE_ContinousRxCommandFormat::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ContinousRxCommandFormat::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"RxPower",strParamName))
        {
            muRxPower = static_cast<short>(_ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RxChannel",strParamName))
        {
            muRxChannel = static_cast<unsigned int>(_ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveContinousRxCommandEnvironment",strParamName))
        {
            mstrSaveContinousRxCommandEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ContinousRxCommandFormat::TSE_Run()
    {
        if (mstrSaveContinousRxCommandEnvironment.empty() || muRxChannel <= 0)
        {
            LogE(L"Param Config Fail");
            return false;
        }

        std::wstring strContinousRxCommand = GetContinousRxCommandFormat(muRxChannel,muRxPower);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",mstrSaveContinousRxCommandEnvironment.c_str(),strContinousRxCommand.c_str());
        GetGlobalEnvironment()->PutString(mstrSaveContinousRxCommandEnvironment,strContinousRxCommand);

        return true;
    }


}
