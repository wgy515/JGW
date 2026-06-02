#include "StdAfx.h"
#include "TSE_GetFileSFTPV2.h"
#include <JGW_SSHPlugin/CJGW_SSHSFTPV2.h>

namespace JGW
{
    CTSE_GetFileSFTPV2::CTSE_GetFileSFTPV2(void) : mnFTPFlags(LIBSSH2_FXF_READ),mnFTPMode(0),mstrSFTPInstanceEnvironment(L"TSE_SFTP_INSTANCE")
    {
    }


    CTSE_GetFileSFTPV2::~CTSE_GetFileSFTPV2(void)
    {
    }

    const wchar_t* CTSE_GetFileSFTPV2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_GetFileSFTPV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SFTPInstanceEnvironment",strParamName))
        {
            mstrSFTPInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveLocalFilePathCommand",strParamName))
        {
            mstrSaveLocalFilePathCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"RemoteFilePathCommand",strParamName))
        {
            mstrRemoteFilePathCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPMode",strParamName))
        {
            mnFTPMode = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPFlags",strParamName))
        {
            mnFTPFlags = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetFileSFTPV2::TSE_Run()
    {
        std::wstring strSaveLocalFilePath(mstrSaveLocalFilePathCommand),strRemoteFilePath(mstrRemoteFilePathCommand);

        GetEnvironmentCommandString(strSaveLocalFilePath);
        GetEnvironmentCommandString(strRemoteFilePath);

        std::string strSaveLocalFilePathA = JGW_W2A(JGW_RealativePathToAbsPath(strSaveLocalFilePath.c_str()));
        CCJGW_SSHSFTPV2 *pSSHSFTPV2 = (CCJGW_SSHSFTPV2 *)GetGlobalEnvironment()->GetInt(mstrSFTPInstanceEnvironment);
        if (mstrSFTPInstanceEnvironment.empty() || NULL == pSSHSFTPV2)
        {
            LogE(L"Get SFTP Instance Fail");
            return false;
        }

        if (!pSSHSFTPV2->GetFileSFTP(JGW_W2A(strRemoteFilePath,CP_UTF8),strSaveLocalFilePathA,mnFTPFlags,mnFTPMode))
        {
            LogE_F(L"Get SFTP File Fail(%s)",strRemoteFilePath.c_str());
            return false;
        }

        return true;
    }
}

