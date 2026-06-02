#include "StdAfx.h"
#include "TSE_GetFileSSHSCP.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_GetFileSSHSCP::CTSE_GetFileSSHSCP(void)
    {
    }


    CTSE_GetFileSSHSCP::~CTSE_GetFileSSHSCP(void)
    {
    }

    const wchar_t* CTSE_GetFileSSHSCP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetFileSSHSCP\",\
                \"TestName\": \"SCP下载(AP)\",\
                \"SaveLocalFilePathEnvironment\": \"保存到本地文件路径环境变量\",\
                \"RemoteFilePathEnvironment\": \"remote file path env\"\
                }";
    }

    bool CTSE_GetFileSSHSCP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SaveLocalFilePathEnvironment",strParamName))
        {
            mstrSaveLocalFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RemoteFilePathEnvironment",strParamName))
        {
            mstrRemoteFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetFileSSHSCP::TSE_Run()
    {
        std::string strSaveLocalFilePath = GetGlobalEnvironment()->GetAnsiString(mstrSaveLocalFilePathEnvironment);
        std::wstring strRemoteFilePath = GetGlobalEnvironment()->GetString(mstrRemoteFilePathEnvironment);
        std::string strRemoteFilePathA = JGW_W2A(strRemoteFilePath,CP_UTF8);

        if (!CTSE_DevicesGlobalResource::GetSSHSCP()->GetFileSCP(strRemoteFilePathA,strSaveLocalFilePath))
        {
            LogE_F(L"SCP 文件下载失败\n");
            return false;
        }

        return true;
    }

}
