#include "StdAfx.h"
#include "TSE_ReaderQcomSN.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ReaderQcomSN::CTSE_ReaderQcomSN(void) : mSNLength(14),mstrEnvironment(L"TSE_READER_SN_NUMBER")
    {
    }


    CTSE_ReaderQcomSN::~CTSE_ReaderQcomSN(void)
    {
    }

    const wchar_t* CTSE_ReaderQcomSN::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReaderQcomSN\",\
                \"TestName\": \"读取SN(Qualcomm)\",\
                \"sn_length\": \"SN长度 默认：14\",\
                \"Environment\": \"读取SN后保存环境变量名称\"\
                }";
    }

    bool CTSE_ReaderQcomSN::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;

        if (TSE_PARAM_NAME_EQUAL(_T("sn_length"),strParamName))
        {
            mSNLength = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("Environment"),strParamName))
        {
            mstrEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReaderQcomSN::TSE_Run()
    {
        std::wstring strReadSN;

        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadSN(strReadSN,mSNLength))
        {
            LogE(L"读取SN失败");
            return false;
        }
#if 0
        std::wstring strWriteSN = GetGlobalEnvironment()->GetString(TSE_SN);
        if (NULL == JGW_WStrComparenoCaseWStr(strReadSN.c_str(),strWriteSN.c_str()))
        {
            return  true;
        }
        LogE_F(L"读取SN:%s,写入SN:%s，读取写入不一致\n",strReadSN.c_str(),strWriteSN.c_str());
        return false;
#endif
        LogI_F(L"读取SN:%s\n",strReadSN.c_str());
        LogU(E_TSE_SHOW_CONTENT_INFO,strReadSN.c_str());
        GetGlobalEnvironment()->PutString(mstrEnvironment,strReadSN);

        return true;
    }
}
