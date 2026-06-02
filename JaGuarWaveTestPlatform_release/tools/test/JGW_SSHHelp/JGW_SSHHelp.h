#pragma once
#include "CJGW_SSHHelpBase.h"
#include "CJGW_SSHShell.h"
namespace JGW
{
    class CJGW_SSHHelpBase
    {
    public:
        //! 
        virtual bool ConnectSSHShell(const char* server_name,const char* strUserName,const char* strPassword,const char* source_addr = NULL,unsigned int port = 22) = 0;
        //!
        virtual bool IsConnected() = 0;
        //!
        virtual void DisConnectSSHShell() = 0;
        //!
        virtual int ReadCommandResult(char* read_buf,unsigned int read_buf_size) = 0;
        //!
        virtual bool WriteCommand(const char* command,unsigned int len,int time_outSec) = 0; 
    };

    class CJGW_SSHHelp : public CJGW_SSHHelpBase
    {
    public:
        CJGW_SSHHelp(void);
        ~CJGW_SSHHelp(void);
    public:
        //! 
        bool ConnectSSHShell(const char* server_name,const char* strUserName,const char* strPassword,const char* source_addr = NULL,unsigned int port = 22);
        //!
        bool IsConnected();
        //!
        void DisConnectSSHShell();
        //!
        int ReadCommandResult(char* read_buf,unsigned int read_buf_size);
        //!
        bool WriteCommand(const char* command,unsigned int len,int time_outSec);
    private:
        CCJGW_SSHShell mcSSHShell;
    };
}


