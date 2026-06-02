#include "StdAfx.h"
#include "TSE_PutFileSFTP.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_PutFileSFTP::CTSE_PutFileSFTP(void) : mstrLocalFilePathEnvironment(L"TSE_SFTP_LOCAL_FILE_PATH")
    {
    }


    CTSE_PutFileSFTP::~CTSE_PutFileSFTP(void)
    {
    }

    const wchar_t* CTSE_PutFileSFTP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PutFileSFTP\",\
                \"TestName\": \"SFTP上传文件(AP)\",\
                \"LocalFilePathEnvironment\": \"上传本地文件路径环境变量\"\
                }";
    }

    bool CTSE_PutFileSFTP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"LocalFilePathEnvironment",strParamName))
        {
            mstrLocalFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_PutFileSFTP::TSE_Run()
    {
        std::string strLocalFilePath = GetGlobalEnvironment()->GetAnsiString(mstrLocalFilePathEnvironment);

        if (!CTSE_DevicesGlobalResource::GetSSHSFTP()->PutFileSFTP(strLocalFilePath))
        {
            LogE_F(L"SFTP 文件上传失败\n");
            return false;
        }

        return true;
    }
}