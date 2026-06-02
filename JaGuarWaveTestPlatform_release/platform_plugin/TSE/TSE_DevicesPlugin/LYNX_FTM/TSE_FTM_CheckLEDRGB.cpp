#include "StdAfx.h"
#include "TSE_FTM_CheckLEDRGB.h"

namespace JGW
{
    CTSE_FTM_CheckLEDRGB::CTSE_FTM_CheckLEDRGB(void) : mstrTip(L"依次闪烁红、蓝、绿灯，请检查LED闪烁顺序是否一致")
    {
    }


    CTSE_FTM_CheckLEDRGB::~CTSE_FTM_CheckLEDRGB(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckLEDRGB::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckLEDRGB\",\
                \"TestName\": \"检测LED闪烁是否正常(LYNX FTM)\",\
                \"tip\": \"默认依次闪烁红、蓝、绿灯，请检查LED闪烁顺序是否一致\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_FTM_CheckLEDRGB::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"tip"))
        {
            mstrTip = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_CheckLEDRGB::TSE_Run()
    {
        std::string json = CTSE_FTM_JSONCommand::GetLedRGBJson();
        if (!CCJGW_FTM_JsonSocket::WriteJsonSocket(json))
        {
            return false;
        }

        if (IDOK  == MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"LED RGB测试",MB_OKCANCEL))
        {
            return true;
        }

        return false;
    }
}