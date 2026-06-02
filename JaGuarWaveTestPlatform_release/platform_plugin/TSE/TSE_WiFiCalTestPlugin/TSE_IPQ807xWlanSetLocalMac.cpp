#include "StdAfx.h"
#include "TSE_IPQ807xWlanSetLocalMac.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanSetLocalMac::CTSE_IPQ807xWlanSetLocalMac(void)
    {
    }


    CTSE_IPQ807xWlanSetLocalMac::~CTSE_IPQ807xWlanSetLocalMac(void)
    {
    }

    const wchar_t* CTSE_IPQ807xWlanSetLocalMac::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanSetLocalMac::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("MacAddr1Environment"),strParamName))
        {
            mstrMacAddr1Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr2Environment"),strParamName))
        {
            mstrMacAddr2Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr3Environment"),strParamName))
        {
            mstrMacAddr3Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr4Environment"),strParamName))
        {
            mstrMacAddr4Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr5Environment"),strParamName))
        {
            mstrMacAddr5Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr6Environment"),strParamName))
        {
            mstrMacAddr6Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr7Environment"),strParamName))
        {
            mstrMacAddr7Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr8Environment"),strParamName))
        {
            mstrMacAddr8Environment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("MacAddr9Environment"),strParamName))
        {
            mstrMacAddr9Environment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanSetLocalMac::TSE_Run()
    {
        std::string strMacAddr1 = "00.00.00.00.00.00";
        std::string strMacAddr2 = "00.00.00.00.00.00";
        std::string strMacAddr3 = "00.00.00.00.00.00";
        std::string strMacAddr4 = "00.00.00.00.00.00";
        std::string strMacAddr5 = "00.00.00.00.00.00";
        std::string strMacAddr6 = "00.00.00.00.00.00";
        std::string strMacAddr7 = "00.00.00.00.00.00";
        std::string strMacAddr8 = "00.00.00.00.00.00";
        std::string strMacAddr9 = "00.00.00.00.00.00";
        if (!mstrMacAddr1Environment.empty())
        {
            strMacAddr1 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr1Environment);
        }
        if (!mstrMacAddr2Environment.empty())
        {
            strMacAddr2 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr2Environment);
        }
        if (!mstrMacAddr3Environment.empty())
        {
            strMacAddr3 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr3Environment);
        }
        if (!mstrMacAddr4Environment.empty())
        {
            strMacAddr4 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr4Environment);
        }
        if (!mstrMacAddr5Environment.empty())
        {
            strMacAddr5 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr5Environment);
        }
        if (!mstrMacAddr6Environment.empty())
        {
            strMacAddr6 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr6Environment);
        }
        if (!mstrMacAddr7Environment.empty())
        {
            strMacAddr7 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr7Environment);
        }
        if (!mstrMacAddr8Environment.empty())
        {
            strMacAddr8 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr8Environment);
        }
        if (!mstrMacAddr9Environment.empty())
        {
            strMacAddr9 = GetGlobalEnvironment()->GetAnsiString(mstrMacAddr9Environment);
        }

        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanSetLocalMac(strMacAddr1,strMacAddr2,strMacAddr3,strMacAddr4,strMacAddr5,strMacAddr6,strMacAddr7,strMacAddr8,strMacAddr9);
    }
}
