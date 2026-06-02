#pragma once
#include "JGW_SSHPlugin_Define.h"
#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>
#include <libssh2/libssh2_sftp.h>
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#define SSH_BUFSIZE 102400

namespace JGW
{
    class JGW_SSHPLUGIN_CLASS CCJGW_SecureShell : public CCJGW_AsyncSocket
    {
    public:
        CCJGW_SecureShell(void);
        virtual ~CCJGW_SecureShell(void);
    public:
        bool ConnectSSHShell(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);
    public:
        //! 连接SSH服务
        bool ConnectSSH(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);
        //! time_out：超时时间  
        //! counts： 重试此时
        bool WriteReadSSHShell(const std::string& strShell,std::string& strRead,int time_out = 10,int counts = 3);
        //!
        bool ReadSSHShell(std::string& strRead,int time_out = 10);
        //! 执行SSH命令
        bool ExecSSH(const std::string& strCommandline);
        //! 发送SSH命令
        bool SendSSH(const std::string& strCommandline,int timeout = 10);
        //! 读取SSH命令
        bool ReadSSH(std::string& strReadBuf,int timeout = 10);
        //! 关闭SSH
        void CloseSSH();
        //! 连接SFTP
        bool ConnectSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr = NULL,size_t port = 22);
        //! 发送SFTP文件
        bool PutFileSFTP(const std::string& strLocaFilePath);
        //! 关闭SFTP
        void DisConnectSFTP();

    private:
        //! 
        bool CreateSessionInstance();
        //! 
        bool InitKnownHost(const std::string& server_name,size_t port = 22);
        //! 
        bool OpenSession(const std::string& strUserName,const std::string& strPassword);
        //!
        void ReOpenChannel();
    private:
        int WaitSocket(int socket_fd, LIBSSH2_SESSION *session);
    private:
        bool mbConnectSSH;
        size_t mnSocketPort;
        char* szSSHBuf;
        LIBSSH2_SESSION* mpSession;
        LIBSSH2_CHANNEL* mpChannel;
        //! SFTP
        LIBSSH2_SFTP* mpsftp_session;
        LIBSSH2_SFTP_HANDLE* mpsftp_handle;
        //!
        std::string mstrUserName;
        std::string mstrPassword;
    };
}