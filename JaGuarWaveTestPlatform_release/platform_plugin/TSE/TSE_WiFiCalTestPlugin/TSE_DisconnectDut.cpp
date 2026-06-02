#include "StdAfx.h"
#include "TSE_DisconnectDut.h"
#include "JGW_Phone.h"
namespace JGW
{
    CTSE_DisconnectDut::CTSE_DisconnectDut(void)
    {
    }


    CTSE_DisconnectDut::~CTSE_DisconnectDut(void)
    {
    }

    const wchar_t* CTSE_DisconnectDut::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DisconnectDut\",\
                \"TestName\": \"DisConnect Dut\"\
                }";
    }

    bool CTSE_DisconnectDut::TSE_Run()
    {
        return PHONE->DisconnectDut();
    }
}
