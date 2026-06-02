#include "StdAfx.h"
#include "TSE_BuildEnvironmentValue.h"

namespace JGW
{
    CTSE_BuildEnvironmentValue::CTSE_BuildEnvironmentValue(void)
    {
    }


    CTSE_BuildEnvironmentValue::~CTSE_BuildEnvironmentValue(void)
    {
    }

    const wchar_t* CTSE_BuildEnvironmentValue::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildEnvironmentValue\",\
                \"TestName\": \"根据规则构造环境变量值\",\
                \"BuildEnvironmentName\": \"按照规则生成的字符串保存的环境变量名\",\
                \"SrcEnvironmentValue\": \"需要构造的字符串,$$_xxxx_$$ xxxx表示环境变量\",\
                \"SrcFormat\": \"格式化规则 a,b  a：( 0 : 左边 -1 右边) b:(>0 : 取第几位字符串 < 0 取所有)  以;间隔多个环境变量\"\
                }";
    }

    bool CTSE_BuildEnvironmentValue::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("SrcEnvironmentValue"),strParamName))
        {
            mtrSrcEnvironmentValue = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("SrcFormat"),strParamName))
        {
            mstrSrcFormat = strParamValue;
            ParseStringFormat();
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("BuildEnvironmentName"),strParamName))
        {
            mstrBuildEnvironmentName = strParamValue;
        }
        else 
        {
            return false;
        }
        return true;
    }

    void CTSE_BuildEnvironmentValue::GetEnvironmentCommand(std::wstring& strCommand)
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;
        size_t i = 0;
        while (std::wstring::npos != nStart)
        {
            nEnd = strTemp.find(L"_$$",nStart + 3);
            if (nEnd == std::wstring::npos) return ;
            strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
            strEnvironmentValue = GetGlobalEnvironment()->GetString(strEnvironmentName);
            if (mvFormat.size() > i)
            {
                std::wstring strTempVal(strEnvironmentValue);
                s_left_right sLeftRight = mvFormat[i];
                if (sLeftRight.left >= 0)
                {
                    strEnvironmentValue = strTempVal.substr(sLeftRight.left,sLeftRight.right);
                }
                else
                {
                    if (sLeftRight.right > 0)
                        strEnvironmentValue = strTempVal.substr(strTempVal.size() - sLeftRight.right,sLeftRight.right);
                    else if (sLeftRight.right == 0)
                        strEnvironmentValue = L"";
                }
            }
            JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
            JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
            nStart = strTemp.find(L"$$_",nEnd + 3);
            i ++;
        } 
    }

    void CTSE_BuildEnvironmentValue::ParseStringFormat()
    {
        mvFormat.clear();
        std::vector<std::wstring> vTemp;
        std::wstring strTemp;
        JGW_ParserStrW(mstrSrcFormat.c_str(),L";",vTemp);
        s_left_right sLeftRight = {0,-1};

        for (std::vector<std::wstring>::iterator it = vTemp.begin();
            it != vTemp.end();
            ++ it)
        {
            sLeftRight.left = _ttoi(it->c_str());
            if (std::wstring::npos == it->find(L',')) 
            {
                sLeftRight.right = -1;
            }
            else
            {
                strTemp = it->substr(it->find(L',') + 1);
                sLeftRight.right = _ttoi(strTemp.c_str());
            }
            mvFormat.push_back(sLeftRight);
        }
    }

    bool CTSE_BuildEnvironmentValue::TSE_Run()
    {
        std::wstring strSrcEnvironmentValue = mtrSrcEnvironmentValue;
        GetEnvironmentCommand(strSrcEnvironmentValue);
        GetGlobalEnvironment()->PutString(mstrBuildEnvironmentName,strSrcEnvironmentValue);
        LogI_F(L"%s : %s",mstrBuildEnvironmentName.c_str(),strSrcEnvironmentValue.c_str());
        return true;
    }
}