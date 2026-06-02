#include "StdAfx.h"
#include "TSE_PutFileSFTPV2.h"
#include <JGW_SSHPlugin/CJGW_SSHSFTPV2.h>

namespace JGW
{
    CTSE_PutFileSFTPV2::CTSE_PutFileSFTPV2(void) : mnFTPFlags(LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC),mnFTPMode(LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IRGRP | LIBSSH2_SFTP_S_IROTH),mstrSFTPInstanceEnvironment(L"TSE_SFTP_INSTANCE")
    {
    }


    CTSE_PutFileSFTPV2::~CTSE_PutFileSFTPV2(void)
    {
    }

    const wchar_t* CTSE_PutFileSFTPV2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_PutFileSFTPV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SFTPInstanceEnvironment",strParamName))
        {
            mstrSFTPInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"LocalFilePathCommand",strParamName))
        {
            mstrLocalFilePathCommand = strParamValue;
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

    bool CTSE_PutFileSFTPV2::TSE_Run()
    {
        std::wstring strLocalFilePath(mstrLocalFilePathCommand),strRemoteFilePath(mstrRemoteFilePathCommand);

        GetEnvironmentCommandString(strLocalFilePath);
        GetEnvironmentCommandString(strRemoteFilePath);

        std::string strLocalFilePathA = JGW_W2A(JGW_RealativePathToAbsPath(strLocalFilePath.c_str()));
        CCJGW_SSHSFTPV2 *pSSHSFTPV2 = (CCJGW_SSHSFTPV2 *)GetGlobalEnvironment()->GetInt(mstrSFTPInstanceEnvironment);
        if (mstrSFTPInstanceEnvironment.empty() || NULL == pSSHSFTPV2)
        {
            LogE(L"Put SFTP Instance Fail");
            return false;
        }

        if (!pSSHSFTPV2->PutFileSFTP(JGW_W2A(strRemoteFilePath,CP_UTF8),strLocalFilePathA,mnFTPFlags,mnFTPMode))
        {
            LogE_F(L"Put SFTP File Fail(%s)",strRemoteFilePath.c_str());
            return false;
        }

        return true;
    }
}

