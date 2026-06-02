#include "StdAfx.h"
#include "TSE_GetFileSFTP.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_GetFileSFTP::CTSE_GetFileSFTP(void)
    {
    }


    CTSE_GetFileSFTP::~CTSE_GetFileSFTP(void)
    {
    }

    const wchar_t* CTSE_GetFileSFTP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetFileSFTP\",\
                \"TestName\": \"SFTP下载(AP)\",\
                \"SaveLocalFilePathEnvironment\": \"保存到本地文件路径环境变量\"\
                }";
    }

    bool CTSE_GetFileSFTP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SaveLocalFilePathEnvironment",strParamName))
        {
            mstrSaveLocalFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetFileSFTP::TSE_Run()
    {
        std::string strSaveLocalFilePath = GetGlobalEnvironment()->GetAnsiString(mstrSaveLocalFilePathEnvironment);

        if (!CTSE_DevicesGlobalResource::GetSSHSFTP()->GetFileSFTP(strSaveLocalFilePath))
        {
            LogE_F(L"SFTP 文件下载失败\n");
            return false;
        }

        return true;
    }
}