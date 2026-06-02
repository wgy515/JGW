#include "StdAfx.h"
#include "TSE_WriterQcomSN.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_WriterQcomSN::CTSE_WriterQcomSN() : mSNLength(14) , mstrSNPrefix(L""),mstrSNEnvironment(L"TSE_SN")
    {

    }

    const wchar_t* CTSE_WriterQcomSN::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriterQcomSN\",\
                \"TestName\": \"写SN(Qualcomm)\",\
                \"sn_length\": \"长度限制 默认：0 不限制长度\",\
                \"sn_prefix\": \"SN前缀校验\",\
                \"Environment\": \"要写入环境变量对应的值\"\
                }";
    }

    bool CTSE_WriterQcomSN::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;

        if (TSE_PARAM_NAME_EQUAL(_T("sn_length"),strParamName))
        {
            mSNLength = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("sn_prefix"),strParamName))
        {
            mstrSNPrefix = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("Environment"),strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WriterQcomSN::TSE_Run()
    {
        std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);

        if (mSNLength > 0 && mSNLength != strSN.length())
        {
            LogE_F(L"SN长度必须为:%d,当前输入长度:%d\n",mSNLength,strSN.length());
            return false;
        }

        if (!mstrSNPrefix.empty() && NULL == JGW_WStrCaseWStr(strSN.c_str(),mstrSNPrefix.c_str()))
        {
            LogE_F(L"SN前缀名称必须为:%s,当前输入SN:%s\n",mstrSNPrefix.c_str(),strSN.c_str());
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strSN.c_str());
        return CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriteSN(strSN.c_str(),mSNLength);
    }
}
