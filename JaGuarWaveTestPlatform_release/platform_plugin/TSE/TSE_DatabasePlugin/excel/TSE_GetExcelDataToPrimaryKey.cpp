#include "StdAfx.h"
#include "TSE_GetExcelDataToPrimaryKey.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSE_GetExcelDataToPrimaryKey::CTSE_GetExcelDataToPrimaryKey(void) : mpsExcelContent(NULL),mnPrimaryKey(-1)
    {
    }


    CTSE_GetExcelDataToPrimaryKey::~CTSE_GetExcelDataToPrimaryKey(void)
    {
    }

    const wchar_t* CTSE_GetExcelDataToPrimaryKey::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetExcelDataToPrimaryKey\",\
                \"TestName\": \"根据主键获取EXCEL其他表头对应的数据\",\
                \"EnvironmentExcelSource\": \"excel数据解析后保存的环境变量\",\
                \"ExcelHeader\": \"主键获取的其他表头名称，多个表头以,作为分隔\",\
                \"EnvironmentValues\": \"主键获取EXCEL其他表头对应的数据保存在对应的环境变量中,多个数据以,作为分隔,ExcelHeader和EnvironmentValues必须一一对应\",\
                \"PrimaryKey\": \"主键表头名称\",\
                \"PrimaryValueEnvironment\": \"主键值保存的环境变量名称\"\
                }";
    }

    bool CTSE_GetExcelDataToPrimaryKey::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ExcelHeader"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrExcelHeader);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentValues"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrEnvironmentValues);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryKey"))
        {
            mstrPrimaryKey = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryValueEnvironment"))
        {
            mstrPrimaryValueEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentExcelSource"))
        {
            mstrEnvironmentExcelSource = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    int CTSE_GetExcelDataToPrimaryKey::GetPrimaryKeyToExcel()
    {
        for (size_t i = 0;i < mpsExcelContent->msvvExcelContent[0].size();i ++)
        {
            if (NULL == JGW_WStrComparenoCaseWStr(mpsExcelContent->msvvExcelContent[0][i].c_str(),mstrPrimaryKey.c_str()))
            {
                return i;
            }
        }
        LogE_F(L"Excel 表头未发现当前字符串(%s)",mstrPrimaryKey.c_str());
        return -1;
    }

    bool CTSE_GetExcelDataToPrimaryKey::InitExcelHeaderToExcel()
    {
        bool bFind = false;

        for (size_t i = 0;i < mvstrExcelHeader.size(); i ++)
        {
            bFind = false;
            for (size_t x = 0;x < mpsExcelContent->msvvExcelContent[0].size();x ++)
            {
                if (NULL == JGW_WStrComparenoCaseWStr(mpsExcelContent->msvvExcelContent[0][x].c_str(),mvstrExcelHeader.at(i).c_str()))
                {
                    mvExcelHeaderPos.push_back(x);
                    bFind = true;
                }
            }
            if (!bFind)
            {
                mnPrimaryKey = -1;
                LogE_F(L"Excel 表头未发现当前字符串(%s)",mvstrExcelHeader.at(i).c_str());
                return false;
            }
        }

        return true;
    }

    int CTSE_GetExcelDataToPrimaryKey::GetPrimaryValuePosToExcel(const std::wstring& strPrimartValue)
    {
        for (size_t x = 0;x < mpsExcelContent->msvvExcelContent.size();x ++)
        {
            //! 查找到当前的KEY
            if (NULL == JGW_WStrComparenoCaseWStr(mpsExcelContent->msvvExcelContent[x][mnPrimaryKey].c_str(),strPrimartValue.c_str()))
            {
                return x;
            }
        }
        return -1;
    }

    bool CTSE_GetExcelDataToPrimaryKey::TSE_Run()
    {
        if (mvstrExcelHeader.size() != mvstrEnvironmentValues.size())
        {
            LogE(L"EXCEL表头与对应变量名称数值必须一致");
            return false;
        }

        mpsExcelContent = (PS_EXCEL_CONTENT)GetGlobalEnvironment()->GetInt(mstrEnvironmentExcelSource);
        if (!mpsExcelContent)
        {
            LogE(L"当前EXCEL数据源并不存在，请检查EXCEL文件是否加载成功");
            return false;
        }
        //! 如果主KEY对应的列为-1，则重新定位主KEY，以及副KEY
        if (-1 == mnPrimaryKey)
        {
            mvExcelHeaderPos.clear();
            mnPrimaryKey = GetPrimaryKeyToExcel();
            if (-1 == mnPrimaryKey || !InitExcelHeaderToExcel()) return false;
        }
        //! 查找当前主KEY值对应的行
        mpsExcelContent->mnPrimaryKeyPos = GetPrimaryValuePosToExcel(GetGlobalEnvironment()->GetString(mstrPrimaryValueEnvironment));
        if (-1 == mpsExcelContent->mnPrimaryKeyPos)
        {
            LogE_F(L"当前主键值(%s)未在EXCEL表中查找到，请检测输入主键是否OK",GetGlobalEnvironment()->GetString(mstrPrimaryValueEnvironment).c_str());
            return false;
        }
        std::wstring strEnvName,strEnvValue;
        for (size_t i = 0;i < mvstrExcelHeader.size();i ++)
        {
            strEnvName = mvstrEnvironmentValues.at(i);
            strEnvValue = mpsExcelContent->msvvExcelContent[mpsExcelContent->mnPrimaryKeyPos][mvExcelHeaderPos[i]];
            GetGlobalEnvironment()->PutString(strEnvName,strEnvValue);
            LogI_F(L"%s\t%s\n",strEnvName.c_str(),strEnvValue.c_str());
        }

        return true;
    }
}