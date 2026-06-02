#include "StdAfx.h"
#include "TSE_FTM_CheckBreathingLed.h"

namespace JGW
{
    CTSE_FTM_CheckBreathingLed::CTSE_FTM_CheckBreathingLed(void) : mnGroupID(1)
        ,mstrTip(L"请检测第一组呼吸灯是否正常")
        ,mMainHwnd(NULL)
    {
    }


    CTSE_FTM_CheckBreathingLed::~CTSE_FTM_CheckBreathingLed(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckBreathingLed::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckBreathingLed\",\
                \"TestName\": \"检查LYNX呼吸灯闪烁(LYNX FTM)\",\
                \"Group\": \"呼吸灯ID 1组 :1 2组：2 3组：4 4组：8 闪烁：-1 关闭：0\",\
                \"tip\": \"检测呼吸灯结果判断对话框\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_FTM_CheckBreathingLed::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Group"))
        {
            mnGroupID = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"tip"))
        {
            mstrTip = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_CheckBreathingLed::TSE_Run()
    {
        if (NULL == mMainHwnd)
        {
            mMainHwnd = (HWND)GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
            if (NULL == mMainHwnd) mMainHwnd = FindWindow(NULL,NULL);
            GetGlobalEnvironment()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)mMainHwnd);
        }

        std::string json = CTSE_FTM_JSONCommand::GetBreathingLedJson(mnGroupID);

        if (!CCJGW_FTM_JsonSocket::WriteJsonSocket(json)) return false;
        if (mstrTip.empty()) return true;
        if (IDOK  == MessageBox(mMainHwnd,mstrTip.c_str(),L"呼吸灯测试",MB_OKCANCEL)) return true;

        return false;
    }

    bool CTSE_FTM_CheckBreathingLed::TSE_Exit()
    {
        //         std::string json = CTSE_FTM_JSONCommand::GetBreathingLedJson(0);
        //         CCJGW_FTM_JsonSocket::WriteJsonSocket(json);
        return true;
    }
}
