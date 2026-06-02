#include "StdAfx.h"
#include "TSE_ConnectSFTPV2.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ConnectSFTPV2::CTSE_ConnectSFTPV2(void) : mstrSaveSFTPInstanceEnvironment(L"TSE_SFTP_INSTANCE"),mnSFTPPort(22),mstrHostEnvironment(L"TSE_SSH_HOST"),mstrPasswordEnvironment(L"TSE_SSH_PASSWORD"),mstrSourceAddrEnvironment(L"TSE_SSH_SOURCE_IP"),mstrUserNameEnvironment(L"TSE_SSH_USER_NAME")
    {
    }


    CTSE_ConnectSFTPV2::~CTSE_ConnectSFTPV2(void)
    {
    }

    const wchar_t* CTSE_ConnectSFTPV2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_ConnectSFTPV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"SaveSFTPInstanceEnvironment",strParamName))
        {
            mstrSaveSFTPInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SFTPPort",strParamName))
        {
            mnSFTPPort = _ttoi(strParamValue);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectSFTPV2::TSE_Run()
    {
        std::string strSourceAddr,strHost,strUserName,strPassword;

        strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        strHost = GetGlobalEnvironment()->GetAnsiString(mstrHostEnvironment);
        strUserName = GetGlobalEnvironment()->GetAnsiString(mstrUserNameEnvironment);
        strPassword = GetGlobalEnvironment()->GetAnsiString(mstrPasswordEnvironment);
        //strSaveSFTPInstance = GetGlobalEnvironment()->GetAnsiString(mstrSaveSFTPInstanceEnvironment);

        strUserName = CCJGW_CryptPassword::DecryptPassword(strUserName);
        if (!strPassword.empty()) strPassword = CCJGW_CryptPassword::DecryptPassword(strPassword);
        if (!mstrSaveSFTPInstanceEnvironment.empty()) GetGlobalEnvironment()->PutInt(mstrSaveSFTPInstanceEnvironment,(int)&mcSSHSFTPV2);

        if (!mcSSHSFTPV2.ConnectSSHSFTP(strHost,strUserName,strPassword,strSourceAddr.empty() ? NULL : strSourceAddr.c_str(),mnSFTPPort))
        {
            LogE_F("Connect %s SSH Fail",strHost.c_str());
            return false;
        }
        return true;
    }
}
