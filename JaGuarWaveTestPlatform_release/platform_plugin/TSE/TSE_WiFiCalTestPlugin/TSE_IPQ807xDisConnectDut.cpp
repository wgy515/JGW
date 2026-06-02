#include "StdAfx.h"
#include "TSE_IPQ807xDisConnectDut.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xDisConnectDut::CTSE_IPQ807xDisConnectDut(void)
    {
    }


    CTSE_IPQ807xDisConnectDut::~CTSE_IPQ807xDisConnectDut(void)
    {
    }

    bool CTSE_IPQ807xDisConnectDut::TSE_Run()
    {
        CCJGW_IPQ807xPhone::GetIPQ807xPhone()->DisConnectDut();
        return true;
    }
}

