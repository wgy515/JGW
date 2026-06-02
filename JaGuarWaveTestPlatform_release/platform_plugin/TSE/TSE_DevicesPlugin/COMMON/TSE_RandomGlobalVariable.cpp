#include "StdAfx.h"
#include "TSE_RandomGlobalVariable.h"
#include <stdlib.h>

namespace JGW
{
    CTSE_RandomGlobalVariable::CTSE_RandomGlobalVariable(void) : mnGlobalVariableGroupCount(0)
    {
    }


    CTSE_RandomGlobalVariable::~CTSE_RandomGlobalVariable(void)
    {
    }

    const wchar_t* CTSE_RandomGlobalVariable::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RandomGlobalVariable\",\
                \"TestName\": \"随机全局变量\",\
                \"GlobalVariableGroupCount\": \"随机全局变量组数量，每隔全局变量组大小必须一致\",\
                \"RandomGlobalVariable1\": \"取随机的全局变量并保存在指定全局变量中,1是索引\",\
                \"GlobalVariableGroup1\": \"随机全局变量组参数 以,做分隔 1是索引\"\
                }";
    }

    bool CTSE_RandomGlobalVariable::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("GlobalVariableGroupCount"),strParamName) )
        {
            mnGlobalVariableGroupCount = _ttoi(strParamValue);
        }
        else
        {
            m_mParam[strParamName] = strParamValue;
        }      
        return true;
    }

    bool CTSE_RandomGlobalVariable::TSE_Init()
    {
        mvRandomGlobalVariable.clear();
        mvvGlobalVariableGroup.clear();
        std::wstring strTemp;
        std::vector<std::wstring> vGlobalVariableGroup;

        for (size_t i = 1;i <= mnGlobalVariableGroupCount;i ++)
        {
            JGW_FormatWString(strTemp,L"RandomGlobalVariable%d",i);
            if(m_mParam.end() != m_mParam.find(strTemp))
            {
                mvRandomGlobalVariable.push_back(m_mParam[strTemp]);
            }

            JGW_FormatWString(strTemp,L"GlobalVariableGroup%d",i);
            if(m_mParam.end() != m_mParam.find(strTemp))
            {
                vGlobalVariableGroup.clear();
                JGW_ParserStrW(m_mParam[strTemp].c_str(),L",",vGlobalVariableGroup); 
                mvvGlobalVariableGroup.push_back(vGlobalVariableGroup);
            }
        }

        return true;
    }

    bool CTSE_RandomGlobalVariable::TSE_Run()
    {
        //! 
        if (0 == mnGlobalVariableGroupCount)
        {
            LogI(L"空随机变量生成");
            return true;
        }
        //! 检查全局参数组合随机全局保存参数数量是否匹配
        if (mvvGlobalVariableGroup.size() != mvRandomGlobalVariable.size() || mnGlobalVariableGroupCount != mvvGlobalVariableGroup.size())
        {
            LogE("随机全局变量组数量与需要保存的全局变量组数量不一致，请检查配置是否OK");
            return false;
        }

        size_t nGlobalVariableCount = mvvGlobalVariableGroup.begin()->size();
        std::wstring strKey,strValue,strTemp;
        size_t nRandomGlobalVariableIndex = rand() % nGlobalVariableCount;

        for (size_t i = 0;i < mvRandomGlobalVariable.size();i ++)
        {
            size_t counts = mvvGlobalVariableGroup[i].size();
            if (mvvGlobalVariableGroup[i].size() != nGlobalVariableCount)
            {
                 LogE("每个索引的随机全局变量数量必须完全一致");
                return false;
            }
            strKey = mvRandomGlobalVariable[i];
            strValue = GetGlobalEnvironment()->GetString(mvvGlobalVariableGroup[i][nRandomGlobalVariableIndex]);
            strTemp += strValue;
            if (i < mvRandomGlobalVariable.size() - 1) strTemp += L",";
            GetGlobalEnvironment()->PutString(strKey,strValue);
            LogI_F(L"%s = %s(%s)",strKey.c_str(),strValue.c_str(),mvvGlobalVariableGroup[i][nRandomGlobalVariableIndex].c_str());
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());

        return true;
    }
}