#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SettingTruntable : public CTSE_TestBase
    {
    public:
        CTSE_SettingTruntable(void);
        ~CTSE_SettingTruntable(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        double mfVelocity; //! 周转速度
        double mfAcceleration; //! 加速度
        double mfDeceleration; //! 减速度
        int mnRelMoveAngle;
        std::wstring mstrTruntableEnvironment;
    };

}
