#include "StdAfx.h"
#include "TSE_ParseConfigInfo.h"
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
namespace JGW
{
    CTSE_ParseConfigInfo::CTSE_ParseConfigInfo(void) : m_nKeyValCount(0),mbIsFile(true)
    {
    }


    CTSE_ParseConfigInfo::~CTSE_ParseConfigInfo(void)
    {
    }

    const wchar_t* CTSE_ParseConfigInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ParseConfigInfo\",\
                \"TestName\": \"解析配置文件,格式(key=value)\",\
                \"ParseConfigInfoEnvironment\": \"配置文件路径对应的环境变量值或者待解析的配置文件字符串\",\
                \"IsFile\": \"是否是文件格式,1:文件 0: 则解析ParseConfigInfoEnvironment对应的字符串\",\
                \"KeyValCount\": \"解析Key-Val的数量\",\
                \"Key1Environment\": \"索引1对应的配置文件key值\",\
                \"Val1Environment\": \"全局变量对应的配置value\"\
                }";
    }

    bool CTSE_ParseConfigInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("ParseConfigInfoEnvironment"),strParamName))
        {
            mstrParseConfigInfoEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("IsFile"),strParamName))
        {
            mbIsFile = (_ttoi(strParamValue) == 1);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("KeyValCount"),strParamName))
        {
            m_nKeyValCount = _ttoi(strParamValue);
        }
        else
        {
            m_mParam[strParamName] = strParamValue;
        }

        return true;
    }

    bool CTSE_ParseConfigInfo::TSE_Init()
    {
        std::wstring strKeyTemp,strValTemp;
        for (int i = 1;i <= m_nKeyValCount; i ++)
        {
            JGW_FormatWString(strKeyTemp,L"Key%dEnvironment",i);
            JGW_FormatWString(strValTemp,L"Val%dEnvironment",i);
            mmapKeyVal[m_mParam[strKeyTemp]] = m_mParam[strValTemp];
        }
        return true;
    }

    bool CTSE_ParseConfigInfo::TSE_Run()
    {
        if (mbIsFile)
        {
            std::wstring strConfigInfoPath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrParseConfigInfoEnvironment).c_str());
            if (!JGW_FileExistsToFilePath(strConfigInfoPath.c_str()))
            {
                LogE_F(L"Config File Is Not Exists(%s)",strConfigInfoPath.c_str());
                return false;
            }

            CCJGW_PropertyImpl* pPropertyImpl = GetFilePropertyToFileName(strConfigInfoPath.c_str());
            for (std::map<std::wstring,std::wstring>::iterator it = mmapKeyVal.begin();
                it != mmapKeyVal.end();
                ++ it)
            {
                GetGlobalEnvironment()->PutString(it->second,pPropertyImpl->GetString(it->first));
            }
            DestroyFilePropertyToPropertyImplPtr(pPropertyImpl);
        }
        else
        {
            std::wstring strConfigInfo = GetGlobalEnvironment()->GetString(mstrParseConfigInfoEnvironment);
            std::map<std::wstring,std::wstring> mapProperty;
            JGW_PareserConfigString(strConfigInfo,mapProperty);

            for (std::map<std::wstring,std::wstring>::iterator it = mmapKeyVal.begin();
                it != mmapKeyVal.end();
                ++ it)
            {
                GetGlobalEnvironment()->PutString(it->second,mapProperty[it->first]);
            }
        }
        return true;
    }
}
