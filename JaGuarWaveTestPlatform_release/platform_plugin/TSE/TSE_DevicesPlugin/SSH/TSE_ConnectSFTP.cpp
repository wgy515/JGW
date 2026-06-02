#include "StdAfx.h"
#include "TSE_ConnectSFTP.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    //! 26 420
    CTSE_ConnectSFTP::CTSE_ConnectSFTP(void) : mstrHostEnvironment(L"TSE_SSH_HOST"),mstrPasswordEnvironment(L"TSE_SSH_PASSWORD"),mstrSourceAddrEnvironment(L"TSE_SSH_SOURCE_IP"),mstrUserNameEnvironment(L"TSE_SSH_USER_NAME"),mnSSHPort(22),mnSFTPFlags(LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC),mnSFTPMode(LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH)
    {
    }


    CTSE_ConnectSFTP::~CTSE_ConnectSFTP(void)
    {
    }

    const wchar_t* CTSE_ConnectSFTP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectSFTP\",\
                \"TestName\": \"连接SFTP(AP)\",\
                \"SSHPort\": \"SFTP端口\",\
                \"SourceAddrEnvironment\": \"源IP地址对应的环境变量\",\
                \"HostEnvironment\": \"SFTP主机地址对应的环境变量\",\
                \"UserNameEnvironment\": \"登录用户名对应的环境变量 加密\",\
                \"PasswordEnvironment\": \"登录密码对应的环境变量 加密\",\
                \"SFTPPathEnvironment\": \"SFTP路径对应的环境变量\",\
                \"SFTPMode\": \"SFTP权限 写入模式下：420，读取模式下：0\",\
                \"SFTPFlags\": \"SFTP标识 写入模式下：26，读取模式下：1\"\
                }";
    }

    bool CTSE_ConnectSFTP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SourceAddrEnvironment",strParamName))
        {
            mstrSourceAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"HostEnvironment",strParamName))
        {
            mstrHostEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"UserNameEnvironment",strParamName))
        {
            mstrUserNameEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PasswordEnvironment",strParamName))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPPathEnvironment",strParamName))
        {
            mstrSFTPPathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SSHPort",strParamName))
        {
            mnSSHPort = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPFlags",strParamName))
        {
            mnSFTPFlags = _ttol(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPMode",strParamName))
        {
            mnSFTPMode = _ttol(strParamValue);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectSFTP::TSE_Run()
    {
        std::string strSourceAddr,strHost,strUserName,strPassword;

        strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        strHost = GetGlobalEnvironment()->GetAnsiString(mstrHostEnvironment);
        strUserName = GetGlobalEnvironment()->GetAnsiString(mstrUserNameEnvironment);
        strPassword = GetGlobalEnvironment()->GetAnsiString(mstrPasswordEnvironment);
        std::wstring strSFTPPath = GetGlobalEnvironment()->GetString(mstrSFTPPathEnvironment);
        std::string strSFTPPathA = JGW_W2A(strSFTPPath,CP_UTF8);

        strUserName = CCJGW_CryptPassword::DecryptPassword(strUserName);
        if (!strPassword.empty()) strPassword = CCJGW_CryptPassword::DecryptPassword(strPassword);

        if (!CTSE_DevicesGlobalResource::GetSSHSFTP()->ConnectSSHSFTP(strHost,strUserName,strPassword,strSFTPPathA,strSourceAddr.empty()?NULL:strSourceAddr.c_str(),mnSSHPort,mnSFTPFlags,mnSFTPMode))
        {
            LogE_F("Connect %s SSH Fail",strHost.c_str());
            return false;
        }

        return true;
    }
}