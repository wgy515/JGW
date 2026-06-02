#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <stdint.h>

namespace JGW
{
    class CTSE_CheckMAVoltageCurrentPower : public CTSE_TestBase
    {
    public:
        CTSE_CheckMAVoltageCurrentPower(void);
        ~CTSE_CheckMAVoltageCurrentPower(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        void ParseReadRegContent(std::vector<uint8_t>& vuRegData);
    private:
        bool mbCheckVoltage;
        bool mbCheckCurrent;
        bool mbCheckPower;
        std::vector<float> mvfVoltageRange;
        std::vector<float> mvfCurrentRange;
        std::vector<float> mvfPowerRange;
        float mfVoltage;
        float mfCurrentMA;
        float mfPower;
        std::wstring mstrReadRegContentEnvironment;
    };
}

