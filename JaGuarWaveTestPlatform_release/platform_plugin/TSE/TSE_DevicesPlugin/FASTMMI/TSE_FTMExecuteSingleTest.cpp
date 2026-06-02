#include "StdAfx.h"
#include "TSE_FTMExecuteSingleTest.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_FTMExecuteSingleTest::CTSE_FTMExecuteSingleTest(void)
    {
    }

    CTSE_FTMExecuteSingleTest::~CTSE_FTMExecuteSingleTest(void)
    {
    }

    const wchar_t* CTSE_FTMExecuteSingleTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMExecuteSingleTest\",\
                \"TestName\": \"FASTMMI执行单个测试项目\",\
                \"Tip\": \"为空则不进行测试提示\",\
                \"TestCase\": \"FASTMMI测试项目名称TEST LIST，如LCD\"\
                }";
    }

    bool CTSE_FTMExecuteSingleTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Tip"))
        {
            mstrTip = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestCase"))
        {
            mstrTestCase = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTMExecuteSingleTest::TSE_Run()
    {
        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_EXECUTE_SINGLE_TEST(mstrTestCase.c_str()))
        {
            LogE(L"发送测试Case失败，请检查当前Case是否存在\n");
            Sleep(1000);
            return false;
        }

        if (!mstrTip.empty() && IDOK  != MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"测试提示",MB_OKCANCEL))
        {
            return false;
        }

        return true;
    }
}