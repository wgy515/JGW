#include "StdAfx.h"
#include "TSE_FTM_LcdBackLightTest.h"
#include "TSE_FTM_JSONCommand.h"
#include "CJGW_FTM_JsonSocket.h"

namespace JGW
{
    CTSE_FTM_LcdBackLightTest::CTSE_FTM_LcdBackLightTest(void) : mstrTip(L"请检测当前LCD背光是否正常"),mMainHwnd(NULL)
    {
    }


    CTSE_FTM_LcdBackLightTest::~CTSE_FTM_LcdBackLightTest(void)
    {
    }

    const wchar_t* CTSE_FTM_LcdBackLightTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_LcdBackLightTest\",\
                \"TestName\": \"LCD 背光测试(LYNX FTM)\",\
                \"tip\": \"对话框提示消息，判断LCD背光是否OK\"\
                }";
    }

    bool CTSE_FTM_LcdBackLightTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
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

    bool CTSE_FTM_LcdBackLightTest::TSE_Run()
    {
        if (NULL == mMainHwnd)
        {
            mMainHwnd = (HWND)GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
            if (NULL == mMainHwnd) mMainHwnd = FindWindow(NULL,NULL);
            GetGlobalEnvironment()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)mMainHwnd);
        }

        mnErrorRetryCount = 3;
        std::string json = CTSE_FTM_JSONCommand::GetLcdBackLightJson();
        if (!CCJGW_FTM_JsonSocket::WriteJsonSocket(json))
        {
            return false;
        }

        if (IDOK  == MessageBox(mMainHwnd,mstrTip.c_str(),L"LCD屏幕测试",MB_OKCANCEL))
        {
            return true;
        }
        mnErrorRetryCount = 1;
        return false;
    }
}