#include "StdAfx.h"
#include "TSE_IPQ807xWlanSetDutTxMacAddressBssId.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanSetDutTxMacAddressBssId::CTSE_IPQ807xWlanSetDutTxMacAddressBssId(void) : mstrRxStation("FF.FF.FF.FF.FF.FF"),mstrTxStation("20.22.22.22.22.02"),mstrbssId("50.55.55.55.55.05")
    {
    }


    CTSE_IPQ807xWlanSetDutTxMacAddressBssId::~CTSE_IPQ807xWlanSetDutTxMacAddressBssId(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanSetDutTxMacAddressBssId::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanSetDutTxMacAddressBssId::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("RxStation"),strParamName))
        {
            mstrRxStation = JGW_W2A(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("TxStation"),strParamName))
        {
            mstrTxStation = JGW_W2A(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("DeviceIndex"),strParamName))
        {
            mnDeviceIndex = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("BssId"),strParamName))
        {
            mstrbssId = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanSetDutTxMacAddressBssId::TSE_Run()
    {
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanSetDutTxMacAddressBssId(mstrRxStation,mstrTxStation,mstrbssId,mnDeviceIndex);
    }
}
