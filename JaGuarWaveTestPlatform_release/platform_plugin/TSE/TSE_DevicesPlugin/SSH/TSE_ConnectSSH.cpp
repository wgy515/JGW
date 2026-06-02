#include "StdAfx.h"
#include "TSE_ConnectSSH.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
//! netsh interface ip set address "本地连接 6" static 192.168.213.5 255.255.255.0
/*
echo off
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\Tcpip\Parameters\Interfaces\{5EDC9DE2-A000-49F1-A928-4C589078E2C9}
cls
title 设置IP
echo 正在设置IP地址，请稍候……
netsh interface ip set address "本地连接" static 192.168.1.100 255.255.255.0 192.168.1.1 1
echo 正在更改DNS设置，请稍候……
netsh interface ip set dns "本地连接" static 202.38.193.33
echo ******恭喜你，修改完成！******
pause

echo off
cls
title 清除IP设置
echo 正在清除IP地址，请稍候……
netsh interface ip set address name="本地连接" source=dhcp 
echo 正在清除DNS设置，请稍候……
netsh interface ip set dns name="本地连接" source=dhcp 
echo 删除IP设置，设置为自动。
echo            ***************    恭喜你，命令成功完成！*************
pause
//! 获取网卡设备名称
Name
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Network\{4D36E972-E325-11CE-BFC1-08002BE10318}\{5EDC9DE2-A000-49F1-A928-4C589078E2C9}\Connection
*/
namespace JGW
{
    CTSE_ConnectSSH::CTSE_ConnectSSH(void) : mstrHostEnvironment(L"TSE_SSH_HOST"),mstrPasswordEnvironment(L"TSE_SSH_PASSWORD"),mstrSourceAddrEnvironment(L"TSE_SSH_SOURCE_IP"),mstrUserNameEnvironment(L"TSE_SSH_USER_NAME"),mnSSHPort(22)
    {
    }


    CTSE_ConnectSSH::~CTSE_ConnectSSH(void)
    {
    }

    const wchar_t* CTSE_ConnectSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectSSH\",\
                \"TestName\": \"连接SSH(AP)\",\
                \"SSHPort\": \"SSH端口\",\
                \"EthernetGUIDEnvironment\": \"Net Card GUID\",\
                \"SourceAddrEnvironment\": \"源IP地址对应的环境变量\",\
                \"HostEnvironment\": \"SFTP主机地址对应的环境变量\",\
                \"UserNameEnvironment\": \"登录用户名对应的环境变量 加密\",\
                \"PasswordEnvironment\": \"登录密码对应的环境变量 加密\"\
                }";
    }

    bool CTSE_ConnectSSH::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EthernetGUIDEnvironment"))
        {
            mstrEthernetGUIDEnvironment = strParamValue;
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

    bool CTSE_ConnectSSH::TSE_Run()
    {
        std::string strSourceAddr,strHost,strUserName,strPassword,strRead,strPingAddr;

        strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        strHost = GetGlobalEnvironment()->GetAnsiString(mstrHostEnvironment);
        strUserName = GetGlobalEnvironment()->GetAnsiString(mstrUserNameEnvironment);
        strPassword = GetGlobalEnvironment()->GetAnsiString(mstrPasswordEnvironment);

        strUserName = CCJGW_CryptPassword::DecryptPassword(strUserName);

        if (!strPassword.empty()) strPassword = CCJGW_CryptPassword::DecryptPassword(strPassword);

        strPingAddr = strSourceAddr;
        if (!mstrEthernetGUIDEnvironment.empty())
        {
            std::wstring strEthernetGUID = GetGlobalEnvironment()->GetString(mstrEthernetGUIDEnvironment);
            std::string strIPAddr = JGW_W2A(JGW_GetIPV4AddrToAdpaterName(strEthernetGUID.c_str()));
            if (!strIPAddr.empty())
            {
                strPingAddr = strIPAddr;
                LogI_F("Get NetCard IPV4 Address:%s",strIPAddr.c_str());
                GetGlobalEnvironment()->PutAnsiString(mstrSourceAddrEnvironment,strPingAddr);
            }
        }

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->ConnectSSHShell(strHost,strUserName,strPassword,strPingAddr.empty()?NULL:strPingAddr.c_str(),mnSSHPort))
        {
            LogE_F("Connect %s SSH Fail",strHost.c_str());
            return false;
        }

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->RecvSSHShell(strRead))
        {
            LogE("Recv SSH Fail");
            return false;
        }

        return true;
    }
}