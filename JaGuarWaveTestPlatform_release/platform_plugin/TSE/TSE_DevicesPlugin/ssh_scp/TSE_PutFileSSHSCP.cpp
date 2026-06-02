#include "StdAfx.h"
#include "TSE_PutFileSSHSCP.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_PutFileSSHSCP::CTSE_PutFileSSHSCP(void)
    {
    }


    CTSE_PutFileSSHSCP::~CTSE_PutFileSSHSCP(void)
    {
    }

    const wchar_t* CTSE_PutFileSSHSCP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PutFileSSHSCP\",\
                \"TestName\": \"SCP上传文件(AP)\",\
                \"LocalFilePathEnvironment\": \"上传本地文件路径环境变量\",\
                \"LocalFilePathCommand\": \"构造本地文件command\",\
                \"RemoteFilePathEnvironment\": \"remote file path env\"\
                }";
    }

    bool CTSE_PutFileSSHSCP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"LocalFilePathEnvironment",strParamName))
        {
            mstrLocalFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RemoteFilePathEnvironment",strParamName))
        {
            mstrRemoteFilePathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"LocalFilePathCommand",strParamName))
        {
            mstrLocalFilePathCommand = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_PutFileSSHSCP::TSE_Run()
    {
        std::string strLocalFilePath;
        std::wstring strRemoteFilePath = GetGlobalEnvironment()->GetString(mstrRemoteFilePathEnvironment);
        std::string strRemoteFilePathA = JGW_W2A(strRemoteFilePath,CP_UTF8);

        if (!mstrLocalFilePathEnvironment.empty())
        {
            strLocalFilePath = JGW_RealativePathToAbsPathA(GetGlobalEnvironment()->GetAnsiString(mstrLocalFilePathEnvironment));
        }
        else
        {
            std::wstring command(mstrLocalFilePathCommand);
            GetEnvironmentCommandString(command);
            strLocalFilePath = JGW_RealativePathToAbsPathA(JGW_W2A(command));
        }

        if (!CTSE_DevicesGlobalResource::GetSSHSCP()->PutFileSCP(strRemoteFilePathA,strLocalFilePath))
        {
            LogE_F(L"SFTP 文件上传失败\n");
            return false;
        }

        return true;
    }
}
