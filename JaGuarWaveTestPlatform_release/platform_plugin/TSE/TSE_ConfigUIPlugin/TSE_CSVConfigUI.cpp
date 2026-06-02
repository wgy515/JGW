#include "StdAfx.h"
#include "TSE_CSVConfigUI.h"
#include "TSE_CSVConfigDlg.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>

namespace JGW
{
    CTSE_CSVConfigUI::CTSE_CSVConfigUI(void) : mnConfigCount(0),mstrCSVFilePathCommand(L"$$_TSE_CSV_PATH_$$"),mnPrimaryColumnNameRowIndex(0),mnPrimaryColumnValueIndex(0)
    {
    }


    CTSE_CSVConfigUI::~CTSE_CSVConfigUI(void)
    {
    }

    const wchar_t* CTSE_CSVConfigUI::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CSVConfigUI\",\
                \"TestName\": \"CSVÅäÖÃUI\",\
                \"CSVFilePathCommand\": \"csv file path Environment\",\
                \"PrimaryColumnNameRowIndex\": \"primary row index  Default: 0','\",\
                \"PrimaryColumnValueIndex\": \"primary column index  Default: 0\",\
                \"PrimaryValue\": \"primary value\",\
                \"ConfigCount\": \"config count\",\
                \"LabelName1\": \"label name\",\
                \"EditValueEnvironment1\": \"csv column header => value \"\
                }";
    }

    bool CTSE_CSVConfigUI::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("CSVFilePathCommand"),strParamName) )
        {
            mstrCSVFilePathCommand = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("PrimaryColumnNameRowIndex"),strParamName) )
        {
            mnPrimaryColumnNameRowIndex = _ttoi(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("PrimaryColumnValueIndex"),strParamName) )
        {
            mnPrimaryColumnValueIndex = _ttoi(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("PrimaryValue"),strParamName) )
        {
            mstrPrimaryValue = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("ConfigCount"),strParamName) )
        {
            mnConfigCount = _ttoi(strParamValue);
        }
        else
        {
            m_mParam[strParamName] = strParamValue;
        }  
        return true;
    }

    bool CTSE_CSVConfigUI::TSE_Init()
    {
        std::wstring strLabelNameKey;
        std::wstring strEditValueEnvironmentKey;
        S_TSE_CSV_CONFIG_ITEM sTSECSVConfigItem;
        mvsTSECSVConfigItems.clear();

        for (size_t i = 1;i <= mnConfigCount;i ++)
        {
            sTSECSVConfigItem.mbReadOnly = false;
            sTSECSVConfigItem.mstrLabelName = L"";
            sTSECSVConfigItem.mstrEditValueEnvironment = L"";
            sTSECSVConfigItem.mstrGlobalEnvironmentName = L"";
            sTSECSVConfigItem.mbUpdateCsv = false;

            JGW_FormatWString(strLabelNameKey,L"LabelName%d",i);
            JGW_FormatWString(strEditValueEnvironmentKey,L"EditValueEnvironment%d",i);
            if( m_mParam.end() != m_mParam.find(strLabelNameKey) )
            {
                sTSECSVConfigItem.mstrLabelName = m_mParam[strLabelNameKey];
            }
            if( m_mParam.end() != m_mParam.find(strEditValueEnvironmentKey) )
            {
                sTSECSVConfigItem.mstrEditValueEnvironment = m_mParam[strEditValueEnvironmentKey];
            }
            // LabelReadOnly%d GlobalEnvironmentName%d
            JGW_FormatWString(strLabelNameKey,L"LabelReadOnly%d",i);
            JGW_FormatWString(strEditValueEnvironmentKey,L"GlobalEnvironmentName%d",i);
            if( m_mParam.end() != m_mParam.find(strLabelNameKey) )
            {
                sTSECSVConfigItem.mbReadOnly = _ttoi(m_mParam[strLabelNameKey].c_str()) == 1;
            }
            if( m_mParam.end() != m_mParam.find(strEditValueEnvironmentKey) )
            {
                sTSECSVConfigItem.mstrGlobalEnvironmentName = m_mParam[strEditValueEnvironmentKey];
            }
            //if (!sTSECSVConfigItem.mstrGlobalEnvironmentName.empty()) sTSECSVConfigItem.mbReadOnly = true;

            mvsTSECSVConfigItems.push_back(sTSECSVConfigItem);
        }

        return true;
    }

    bool CTSE_CSVConfigUI::TSE_Run()
    {
        if (mnConfigCount != mvsTSECSVConfigItems.size()) TSE_Init();
        CTSE_CSVConfigDlg lanConfigDlg(GetGlobalMainHwnd(),mvsTSECSVConfigItems,mstrCSVFilePathCommand,mnPrimaryColumnNameRowIndex,mnPrimaryColumnValueIndex,mstrPrimaryValue);
        return (IDOK == lanConfigDlg.DoModal());
    }
}
