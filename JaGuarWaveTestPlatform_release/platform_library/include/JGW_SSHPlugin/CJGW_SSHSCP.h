#pragma once
#include "JGW_SSHPlugin_Define.h"

#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>
#include <libssh2/libssh2_sftp.h>
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>

namespace JGW
{
    class JGW_SSHPLUGIN_CLASS CCJGW_SSHSCP : public CCJGW_AsyncSocket
    {
    public:
        CCJGW_SSHSCP(void);
        ~CCJGW_SSHSCP(void);
    public:
        bool ConnectSSHSCP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);
        void DisConnectSSHSCP();
        //! 发送SFTP文件 mode:文件权限
        bool PutFileSCP(const std::string& strScpPath,const std::string& strLocaFilePath,int mode = 644);
        //! 读取SFTP文件
        bool GetFileSCP(const std::string& strScpPath,const std::string& strSaveLocalFilePath);
    private:
        bool InitSSHSCP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);

    private:
        char* mszSSHBuf;
        LIBSSH2_SESSION* mpSession;
        LIBSSH2_CHANNEL* mpChannel;

        std::string mstrUserName;
        std::string mstrPassword;
    };
}


