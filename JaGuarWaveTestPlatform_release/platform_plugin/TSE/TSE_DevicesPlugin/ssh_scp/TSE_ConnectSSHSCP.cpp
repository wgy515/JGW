#include "StdAfx.h"
#include "TSE_ConnectSSHSCP.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ConnectSSHSCP::CTSE_ConnectSSHSCP(void) : mstrHostEnvironment(L"TSE_SSH_HOST"),mstrPasswordEnvironment(L"TSE_SSH_PASSWORD"),mstrSourceAddrEnvironment(L"TSE_SSH_SOURCE_IP"),mstrUserNameEnvironment(L"TSE_SSH_USER_NAME"),mnSSHPort(22)
    {
    }


    CTSE_ConnectSSHSCP::~CTSE_ConnectSSHSCP(void)
    {
    }

    const wchar_t* CTSE_ConnectSSHSCP::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_ConnectSSHSCP\",\
				\"TestName\": \"Connect SCP(AP)\",\
				\"SSHPort\": \"SSH Port\",\
				\"SourceAddrEnvironment\": \"源IP地址对应的环境变量\",\
				\"HostEnvironment\": \"SFTP主机地址对应的环境变量\",\
				\"UserNameEnvironment\": \"登录用户名对应的环境变量 加密\",\
				\"PasswordEnvironment\": \"登录密码对应的环境变量 加密\"\
				}";
    }

    bool CTSE_ConnectSSHSCP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"SSHPort",strParamName))
        {
            mnSSHPort = _ttoi(strParamValue);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectSSHSCP::TSE_Run()
    {
        std::string strSourceAddr,strHost,strUserName,strPassword,strSFTPPath;

        strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        strHost = GetGlobalEnvironment()->GetAnsiString(mstrHostEnvironment);
        strUserName = GetGlobalEnvironment()->GetAnsiString(mstrUserNameEnvironment);
        strPassword = GetGlobalEnvironment()->GetAnsiString(mstrPasswordEnvironment);

        strUserName = CCJGW_CryptPassword::DecryptPassword(strUserName);
        if (!strPassword.empty()) strPassword = CCJGW_CryptPassword::DecryptPassword(strPassword);

        if (!CTSE_DevicesGlobalResource::GetSSHSCP()->ConnectSSHSCP(strHost,strUserName,strPassword,strSourceAddr.empty()?NULL:strSourceAddr.c_str(),mnSSHPort))
        {
            LogE_F("Connect %s SSH Fail",strHost.c_str());
            return false;
        }

        return true;
    }
}

