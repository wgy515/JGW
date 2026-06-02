#include "StdAfx.h"
#include "TSE_FTM_LcdColorTest.h"
#include "TSE_FTM_JSONCommand.h"
#include "CJGW_FTM_JsonSocket.h"

namespace JGW
{
    CTSE_FTM_LcdColorTest::CTSE_FTM_LcdColorTest(void) : mdwColor(0xF800)
        ,mstrTip(L"«ÎºÏ≤‚µ±«∞∫Ï∆¡ «∑Ò’˝≥£")
        ,mMainHwnd(NULL)
    {
    }


    CTSE_FTM_LcdColorTest::~CTSE_FTM_LcdColorTest(void)
    {
    }

    const wchar_t* CTSE_FTM_LcdColorTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_LcdColorTest\",\
                \"TestName\": \"LCD∆¡—’…´≤‚ ‘(LYNX FTM)\",\
                \"color\": \"…Ë÷√∆¡—’…´£∫0xF800 : ∫Ï∆¡£¨0x07E0£∫¬Ã∆¡£¨0x001F£∫¿∂∆¡£¨0xFFFF£∫∞◊∆¡£¨0x0000£∫∫⁄∆¡£¨0xEF5D£∫ª“∆¡\",\
                \"tip\": \"∂‘ª∞øÚÃ· æœ˚œ¢£¨≈–∂œLCD∆¡—’…´ «∑ÒOK\"\
                }";
    }

    bool CTSE_FTM_LcdColorTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"tip"))
        {
            mstrTip = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"color"))
        {
            _stscanf_s(strParamValue,L"%X",&mdwColor);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_LcdColorTest::TSE_Run()
    {
        if (NULL == mMainHwnd)
        {
            mMainHwnd = (HWND)GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
            if (NULL == mMainHwnd) mMainHwnd = FindWindow(NULL,NULL);
            GetGlobalEnvironment()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)mMainHwnd);
        }

        mnErrorRetryCount = 3;
        std::string json = CTSE_FTM_JSONCommand::GetLcdColorJson(mdwColor);
        if (!CCJGW_FTM_JsonSocket::WriteJsonSocket(json))
        {
            return false;
        }

        if (IDOK  == MessageBox(mMainHwnd,mstrTip.c_str(),L"LCD∆¡ƒª≤‚ ‘",MB_OKCANCEL))
        {
            return true;
        }
        mnErrorRetryCount = 1;
        return false;
    }
}