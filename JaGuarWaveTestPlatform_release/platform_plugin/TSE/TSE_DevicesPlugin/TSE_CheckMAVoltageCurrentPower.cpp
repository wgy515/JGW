#include "StdAfx.h"
#include "TSE_CheckMAVoltageCurrentPower.h"


namespace JGW
{
    CTSE_CheckMAVoltageCurrentPower::CTSE_CheckMAVoltageCurrentPower(void) : mbCheckVoltage(false),mbCheckCurrent(false),mbCheckPower(false),mfVoltage(-999.0f),mfCurrentMA(-999.0f),mfPower(-999.0f)
    {
    }


    CTSE_CheckMAVoltageCurrentPower::~CTSE_CheckMAVoltageCurrentPower(void)
    {
    }

    const wchar_t* CTSE_CheckMAVoltageCurrentPower::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckMAVoltageCurrentPower\",\
                \"TestName\": \"Micro Ammeter Check Voltage Current Power\",\
                }";
    }

    bool CTSE_CheckMAVoltageCurrentPower::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadRegContentEnvironment"))
        {
            mstrReadRegContentEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"VoltageRange"))
        {
            mbCheckVoltage = false;
            mvfVoltageRange.clear();
            JGW_ParserFloatW(strParamValue,L"~",mvfVoltageRange);
            if (!mvfVoltageRange.empty()) mbCheckVoltage = true;
            while (mvfVoltageRange.size() < 2) mvfVoltageRange.push_back(0);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CurrentRange"))
        {
            mbCheckCurrent = false;
            mvfCurrentRange.clear();
            JGW_ParserFloatW(strParamValue,L"~",mvfCurrentRange);
            if (!mvfCurrentRange.empty()) mbCheckCurrent = true;
            while (mvfCurrentRange.size() < 2) mvfCurrentRange.push_back(0);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PowerRange"))
        {
            mbCheckPower = false;
            mvfPowerRange.clear();
            JGW_ParserFloatW(strParamValue,L"~",mvfPowerRange);
            if (!mvfPowerRange.empty()) mbCheckPower = true;
            while (mvfPowerRange.size() < 2) mvfPowerRange.push_back(0);
        }
        else
        {
            return false;
        }
        return true;
    }
    /// 0x01 0x03 0x0A 0x30 0xF2 0x00 0x00 0x2D 0x55 0x00 0x89 0x00 0x01  0x23 0x76
    void CTSE_CheckMAVoltageCurrentPower::ParseReadRegContent(std::vector<uint8_t>& vuRegData)
    {
        mfCurrentMA = -999.0f;
        mfVoltage = -999.0f;
        mfPower = -999.0f;

        if (vuRegData.size() >= 7)
        {
            //int val = (((int)(vuRegData[3] & 0xFF) << 8) | ((int)(vuRegData[4] & 0xFF)));
            mfVoltage = (((int)(vuRegData[3] & 0xFF) << 8) | ((int)(vuRegData[4] & 0xFF))) / 1000.0f;
        }

        if (vuRegData.size() >= 11)
        {
            int val = (((int)(vuRegData[5] & 0xFF) << 24) | ((int)(vuRegData[6] & 0xFF) << 16));
            val += (((int)(vuRegData[7] & 0xFF) << 8) | ((int)(vuRegData[8] & 0xFF)));
            mfCurrentMA = val / 1000.0f;
        }

        if (vuRegData.size() >= 13)
        {
            mfPower = (((int)(vuRegData[9] & 0xFF) << 8) | ((int)(vuRegData[10] & 0xFF))) / 1000.0f;
        }
    }

    bool CTSE_CheckMAVoltageCurrentPower::TSE_Run()
    {
        std::wstring strValue = GetGlobalEnvironment()->GetString(mstrReadRegContentEnvironment);
        if (strValue.empty())
        {
            LogE(L"Voltage current power data is empty");
            return false;
        }

        std::vector<std::wstring> vstrTemp;
        std::vector<uint8_t> vuRegData;
        JGW_ParserStrW(strValue.c_str(),L" ",vstrTemp);
        for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
        {
            vuRegData.push_back((uint8_t)(_tcstol(it->c_str(),NULL,16) & 0xFF));
        }
        //uint8_t temp = 2 + 1 + 2 + (uint8_t)vuRegData[2];
        if (vuRegData.size() < 5 || vuRegData.size() != 2 + 1 + 2 + (uint8_t)vuRegData[2])
        {
            LogE(L"read length exception...");
            return false;
        }
        //
        if (0x03 != vuRegData[1])
        {
            LogE(L"Protocol header error");
            return false;
        }

        ParseReadRegContent(vuRegData);
        if (mbCheckVoltage)
        {
            LogU_F(E_TSE_SHOW_CONTENT_INFO,L"Voltage(V) %.4f <= %.4f <= %.4f",mvfVoltageRange[0],mfVoltage,mvfVoltageRange[1]);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Voltage(V) %.4f <= %.4f <= %.4f",mvfVoltageRange[0],mfVoltage,mvfVoltageRange[1]);
            if (mvfVoltageRange[0] > mfVoltage || mfVoltage > mvfVoltageRange[1])
            {
                return false;
            }
            
        }

        if (mbCheckCurrent)
        {
            LogU_F(E_TSE_SHOW_CONTENT_INFO,L"Current(mA) %.4f <= %.4f <= %.4f",mvfCurrentRange[0],mfCurrentMA,mvfCurrentRange[1]);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Current(mA) %.4f <= %.4f <= %.4f",mvfCurrentRange[0],mfCurrentMA,mvfCurrentRange[1]);
            if (mvfCurrentRange[0] > mfCurrentMA || mfCurrentMA > mvfCurrentRange[1])
            {
                return false;
            }
        }

        if (mbCheckPower)
        {
            LogU_F(E_TSE_SHOW_CONTENT_INFO,L"Power(W) %.4f <= %.4f <= %.4f",mvfPowerRange[0],mfPower,mvfPowerRange[1]);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Power(W) %.4f <= %.4f <= %.4f",mvfPowerRange[0],mfPower,mvfPowerRange[1]);
            if (mvfPowerRange[0] > mfPower || mfPower > mvfPowerRange[1])
            {
                return false;
            }
        }

        return true;
    }
}
