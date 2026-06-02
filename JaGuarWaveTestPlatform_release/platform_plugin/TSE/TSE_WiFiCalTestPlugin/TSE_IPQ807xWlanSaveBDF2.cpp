#include "StdAfx.h"
#include "TSE_IPQ807xWlanSaveBDF2.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CTSE_IPQ807xWlanSaveBDF2::CTSE_IPQ807xWlanSaveBDF2(void) : meSaveOption(BDF_DataFlash),mstrBDFFileName("bdf.bin")
    {
    }


    CTSE_IPQ807xWlanSaveBDF2::~CTSE_IPQ807xWlanSaveBDF2(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanSaveBDF2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanSaveBDF2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("SaveOption"),strParamName))
        {
            meSaveOption = (BDF_OPTION)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("BDFFileName"),strParamName))
        {
            mstrBDFFileName = JGW_W2A(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanSaveBDF2::TSE_Run()
    {
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanSaveBDF2(meSaveOption,mstrBDFFileName);
    }
}
