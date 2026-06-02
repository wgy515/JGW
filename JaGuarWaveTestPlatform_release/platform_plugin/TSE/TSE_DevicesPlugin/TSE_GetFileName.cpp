#include "StdAfx.h"
#include "TSE_GetFileName.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_GetFileName::CTSE_GetFileName(void)
    {
    }


    CTSE_GetFileName::~CTSE_GetFileName(void)
    {
    }

    const wchar_t* CTSE_GetFileName::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetFileName\",\
                \"TestName\": \"Get File Name\",\
                \"FilePathEnvrionment\": \"File Path Envrionment\",\
                \"FileNameEnvrionment\": \"File Name Environment\"\
                }";
    }

    bool CTSE_GetFileName::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"FilePathEnvrionment",strParamName))
        {
            mstrFilePathEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"FileNameEnvrionment",strParamName))
        {
            mstrFileNameEnvrionment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetFileName::TSE_Run()
    {
        std::wstring strFilePath = GetGlobalEnvironment()->GetString(mstrFilePathEnvrionment);
        std::wstring strFileName = JGW_GetFileNameToFilePath(strFilePath.c_str());
        GetGlobalEnvironment()->PutString(mstrFileNameEnvrionment,strFileName);
        LogI_F(L"Get File Name %s=%s",mstrFileNameEnvrionment.c_str(),strFileName.c_str());
        return true;
    }
}
