#include "StdAfx.h"
#include "TSE_SettingTruntable.h"
#include "SCLLibHelper.h"
#define PER_ANGLE_TURN 10000
#define TURNTABLE_NODE_ID 32
namespace JGW
{
    CTSE_SettingTruntable::CTSE_SettingTruntable(void) : mfVelocity(5),mfAcceleration(100),mfDeceleration(100),mnRelMoveAngle(1)
    {
    }


    CTSE_SettingTruntable::~CTSE_SettingTruntable(void)
    {
    }

    const wchar_t* CTSE_SettingTruntable::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SettingTruntable\",\
                \"TestName\": \"设置转台转角度\",\
                \"TruntableEnvironment\": \"Truntable environment\",\
                \"Velocity\": \"周转速度 max 10 default 5 rps\",\
                \"Acceleration\": \"加速度 rps/s\",\
                \"Deceleration\": \"减速度 rps/s\",\
                \"RelMoveAngle\": \"相对位置转动角度\"\
                }";
    }

    bool CTSE_SettingTruntable::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("TruntableEnvironment"),strParamName))
        {
            mstrTruntableEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("Velocity"),strParamName))
        {
            mfVelocity = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("Acceleration"),strParamName))
        {
            mfAcceleration = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("Deceleration"),strParamName))
        {
            mfDeceleration = static_cast<float>(_ttof(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RelMoveAngle"),strParamName))
        {
            mnRelMoveAngle = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SettingTruntable::TSE_Run()
    {
        SCLLibHelper* pSCLLibHelper = (SCLLibHelper*)(GetGlobalEnvironment()->GetInt(mstrTruntableEnvironment));
        if (NULL == pSCLLibHelper)
        {
            LogE(L"Not Find Truntable Instance");
            return false;
        }
        
        if (!pSCLLibHelper->IsOpen())
        {
            LogE(L"Truntable is not open");
           return false;
        }
        pSCLLibHelper->RelMove(TURNTABLE_NODE_ID,mnRelMoveAngle * PER_ANGLE_TURN,&mfVelocity,&mfAcceleration,&mfDeceleration);
        return true;
    }
}

