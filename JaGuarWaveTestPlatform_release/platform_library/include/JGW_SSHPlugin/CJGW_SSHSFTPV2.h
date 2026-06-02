#pragma once
#include "JGW_SSHPlugin_Define.h"

#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>
#include <libssh2/libssh2_sftp.h>
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>

namespace JGW
{
    class JGW_SSHPLUGIN_CLASS CCJGW_SSHSFTPV2 : public CCJGW_AsyncSocket
    {
    public:
        CCJGW_SSHSFTPV2(void);
        ~CCJGW_SSHSFTPV2(void);
    public:
        //! 
        bool ConnectSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr = NULL,size_t port = 22,unsigned long flags = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,long mode = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
            LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
        //! 
        bool ConnectSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);
        //! 
        void DisConnectSSHSFTP();
        //!
        bool ChangeSSHSFTPPath(const std::string& strSFTPPath,unsigned long flags = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,long mode = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
            LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
        //! 发送SFTP文件
        bool PutFileSFTP(const std::string& strLocaFilePath);
        bool PutFileSFTP(const std::string& strSFTPPath,const std::string& strLocaFilePath,unsigned long flags = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,long mode = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
        //! 读取SFTP文件
        bool GetFileSFTP(const std::string& strSaveLocalFilePath);
        bool GetFileSFTP(const std::string& strSFTPPath,const std::string& strSaveLocalFilePath,unsigned long flags = LIBSSH2_FXF_READ,long mode = 0);
    private:
        bool SFTPInit();
        bool SFTPOpen();
        bool InitSSHSFTPV2();
    private:
        char* mszSSHBuf;
        unsigned long mnFTPFlags;
        long mnFTPMode;
        //! SFTP
        LIBSSH2_SFTP* mpsftp_session;
        LIBSSH2_SFTP_HANDLE* mpsftp_handle;

        LIBSSH2_SESSION* mpSession;
        LIBSSH2_CHANNEL* mpChannel;
        std::string mstrUserName;
        std::string mstrPassword;
        std::string mstrSFTPPath;
    };
}


