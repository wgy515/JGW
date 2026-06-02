#include "StdAfx.h"
#include "TSE_GetRFConfigLoss.h"
#include "TSE_LoadWlanLossConfig.h"
namespace JGW
{
    CTSE_GetRFConfigLoss::CTSE_GetRFConfigLoss(void)
    {
    }


    CTSE_GetRFConfigLoss::~CTSE_GetRFConfigLoss(void)
    {
    }

    const wchar_t* CTSE_GetRFConfigLoss::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_GetRFConfigLoss::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("FreqsEnvionment"),strParamName))
        {
            mstrFreqsEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("LossInstanceEnvionment"),strParamName))
        {
            mstrLossInstanceEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveFreqsLossEnvionment"),strParamName))
        {
            mstrSaveFreqsLossEnvionment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetRFConfigLoss::TSE_Run()
    {
        CTSE_LoadWlanLossConfig* pLoadWlanLossConfig = (CTSE_LoadWlanLossConfig*)GetGlobalEnvironment()->GetInt(mstrLossInstanceEnvionment);
        if (NULL == pLoadWlanLossConfig)
        {
            LogE(L"Get Loss Instance Fail");
            return false;
        }
        std::vector<std::wstring> vstrFreqs;
        std::wstring strLossContent;
        JGW_ParserStrW(GetGlobalEnvironment()->GetString(mstrFreqsEnvionment).c_str(),L",",vstrFreqs);
        for (size_t index = 0;index < vstrFreqs.size();index++)
        {
            float fLoss = pLoadWlanLossConfig->GetWlanLoss(L"BH0_LP",_ttoi(vstrFreqs[index].c_str()));
            if (!strLossContent.empty()) strLossContent += L",";
            strLossContent += JGW_GetFormatWString(L"%s:%.2f",vstrFreqs[index].c_str(),fLoss);
        }
        GetGlobalEnvironment()->PutString(mstrSaveFreqsLossEnvionment,strLossContent);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",mstrSaveFreqsLossEnvionment.c_str(),strLossContent.c_str());
        return true;
    }
}
