// JGW_SSHHelp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "JGW_SSHHelp.h"


namespace JGW
{
    CJGW_SSHHelp::CJGW_SSHHelp(void)
    {
    }


    CJGW_SSHHelp::~CJGW_SSHHelp(void)
    {
    }

    bool CJGW_SSHHelp::ConnectSSHShell(const char* server_name,const char* strUserName,const char* strPassword,const char* source_addr/* = NULL*/,unsigned int port/* = 22*/)
    {
        return mcSSHShell.ConnectSSHShell(server_name,strUserName,strPassword,source_addr,port);
    }

    bool CJGW_SSHHelp::IsConnected()
    {
        return mcSSHShell.IsConnected();
    }

    void CJGW_SSHHelp::DisConnectSSHShell()
    {
        mcSSHShell.DisConnectSSHShell();
    }

    int CJGW_SSHHelp::ReadCommandResult(char* read_buf,unsigned int read_buf_size)
    {
        return mcSSHShell.ReadCommandResult(read_buf,read_buf_size);
    }

    bool CJGW_SSHHelp::WriteCommand(const char* command,unsigned int len,int time_outSec)
    {
        return mcSSHShell.SendSSHShellCharacter(command,len,time_outSec);
    }
}

