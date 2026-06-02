#include "StdAfx.h"
#include "TSE_RFConfigFormat.h"

namespace JGW
{
    CTSE_RFConfigFormat::CTSE_RFConfigFormat(void)
    {
    }


    CTSE_RFConfigFormat::~CTSE_RFConfigFormat(void)
    {
    }

    const wchar_t* CTSE_RFConfigFormat::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_RFConfigFormat::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("PrimaryKeyEnvironment"),strParamName))
        {
            mstrPrimaryKeyEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SecondKeyEnvironment"),strParamName))
        {
            mstrSecondKeyEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveSecondKeyEnvironment"),strParamName))
        {
            mstrSaveSecondKeyEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RFConfigFormat::TSE_Init()
    {
        return TSE_Run();
    }
    //! RF CONFIG 格式
    //！主键：AA,BB,CC
    //! 副键格式: BB:YY,AA:XX,CC:ZZ (XX,YY,ZZ是配置参数)
    //! 保存副键格式：XX,YY,ZZ (AA和XX,BB和YY，CC和ZZ必须一一对应)
    bool CTSE_RFConfigFormat::TSE_Run()
    {
        //! 分割主键配置参数
        std::wstring strPrimaryKey = GetGlobalEnvironment()->GetString(mstrPrimaryKeyEnvironment);
        std::vector<std::wstring> vstrPrimaryKey;
        JGW_ParserStrW(strPrimaryKey.c_str(),L",",vstrPrimaryKey);
        //! 分割副键环境变量组以及分割后保存的副键环境变量组
        std::vector<std::wstring> vstrSecondKeyEnvironment;
        std::vector<std::wstring> vstrSaveSecondKeyEnvironment;
        JGW_ParserStrW(mstrSecondKeyEnvironment.c_str(),L",",vstrSecondKeyEnvironment);
        JGW_ParserStrW(mstrSaveSecondKeyEnvironment.c_str(),L",",vstrSaveSecondKeyEnvironment);
        //! 副键环境变量数组必须与保存副键环境变量数组一致
        if (vstrSecondKeyEnvironment.size() != vstrSaveSecondKeyEnvironment.size() || vstrSaveSecondKeyEnvironment.empty())
        {
            LogE(L"当前测试参数配置错误");
            return false;
        }
        //! 开始解析取主键配置对应的副键配置信息
        for (size_t index = 0;index < vstrSecondKeyEnvironment.size();index ++)
        {
            //! 分割副键对应值
            std::wstring strSecondKeyContent = GetGlobalEnvironment()->GetString(vstrSecondKeyEnvironment[index]);
            std::vector<std::wstring> vstrSecondKeyContent;
            JGW_ParserStrW(strSecondKeyContent.c_str(),L",",vstrSecondKeyContent);
            if (vstrSecondKeyContent.empty()) continue;
            //! 解析副键对应值为KEY-VALUE MAP集合
            std::map<std::wstring,std::wstring> mmapPrimaryValueContent;
            for (std::vector<std::wstring>::iterator it = vstrSecondKeyContent.begin();
                it != vstrSecondKeyContent.end();++it)
            {
                std::vector<std::wstring> vstrTemp;
                JGW_ParserStrW(it->c_str(),L":",vstrTemp);
                if (vstrTemp.size() == 2) mmapPrimaryValueContent[vstrTemp[0]] = vstrTemp[1];
            }
            //! 获取主键对应的附件值
            std::wstring strContent;
            for (std::vector<std::wstring>::iterator it = vstrPrimaryKey.begin();
                it != vstrPrimaryKey.end();++it)
            {
                std::map<std::wstring,std::wstring>::iterator itContent = mmapPrimaryValueContent.find(it[0]);
                //! 如果未找到主键值对应的附键值则取首个副键值替代
                if (mmapPrimaryValueContent.end() == itContent)
                {
                    itContent = mmapPrimaryValueContent.begin();
                }
                if (!strContent.empty()) strContent += L",";
                strContent += itContent->second;
            }
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",vstrSaveSecondKeyEnvironment[index].c_str(),strContent.c_str());
            //! 将解析的RF CONFIG 格式对应附键值保存到相对环境变量中
            GetGlobalEnvironment()->PutString(vstrSaveSecondKeyEnvironment[index],strContent);
        }
        return true;
    }
}
