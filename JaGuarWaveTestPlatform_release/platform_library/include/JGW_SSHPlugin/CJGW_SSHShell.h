#pragma once
#include "JGW_SSHPlugin_Define.h"

#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>
#include <libssh2/libssh2_sftp.h>
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>


namespace JGW
{
    class JGW_SSHPLUGIN_CLASS CCJGW_SSHShell : public CCJGW_AsyncSocket
    {
    public:
        CCJGW_SSHShell(void);
        ~CCJGW_SSHShell(void);
    public:
        //! Á¬½ÓSSH Shell
        bool ConnectSSHShell(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr = NULL,size_t port = 22);
        //!
        bool IsConnected();
        //!
        bool IsSSHConnected();
        //! ¶Ï¿ªSSH ShellÁ¬½Ó
        void DisConnectSSHShell();
        //! ¶ÁÐ´¿ØÖÆ¶Ë×Ö·û´®
        bool RWSSHShell(const std::string& strWrite,std::string& strRead,bool bRead = true,int nTestWaitResult = 0,int rw_time_out = 10,int counts = 3);
        //! ¶ÁÈ¡SSH SHELL¿ØÖÆ¶Ë×Ö·û´®
        bool RecvSSHShell(std::string& strRead,int time_out = 10);

        template <typename Func> 
        inline bool RecvSSHShellCallBack(std::string& strRead,Func func)
        {
            //! jgw_timer jt; 
            int rc = 0;
            LIBSSH2_POLLFD pollfd;
            pollfd.type = LIBSSH2_POLLFD_CHANNEL;
            pollfd.fd.channel = mpChannel;
            pollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

            while (1/*jt.elapsed() <= time_out*/)
            {
                memset(mszSSHBuf,0x00,SSH_BUFSIZE);
                /*rc = libssh2_channel_read(mpChannel, mszSSHBuf, SSH_BUFSIZE);*/
                libssh2_poll(&pollfd, 1, 10);
                if (pollfd.revents & LIBSSH2_POLLFD_POLLIN /*|| pollfd.revents &  LIBSSH2_POLLFD_POLLOUT*/) rc = libssh2_channel_read(mpChannel, mszSSHBuf, SSH_BUFSIZE);
                if (rc > 0)
                {
                    mszSSHBuf[rc] = L'\0';
                    func(mszSSHBuf,rc);
                    if (strRead.length() > 1024 * 1024) strRead = "";
                    strRead += mszSSHBuf;
                    //!PDLOG4WA(mszSSHBuf);
                    if (strRead.length() < 3) continue;
                    JGW_EraseLastAndFristTwoChars(strRead);
                    JGW_EraseLastAndFristTwoChars(strRead,' ');
                    if (('#' == strRead[strRead.length() - 1] && ']' == strRead[strRead.length() - 2])
                        || ('$' == strRead[strRead.length() - 1] && '~' == strRead[strRead.length() - 2])
                        || ('#' == strRead[strRead.length() - 1] && '~' == strRead[strRead.length() - 2])
                        || ('$' == strRead[strRead.length() - 1] && ']' == strRead[strRead.length() - 2])
                        )
                    {
                        return true;
                    }
                    /* if (strRead.length() < 3) continue;

                    if (('#' == strRead[strRead.length() - 2] && ']' == strRead[strRead.length() - 3])
                    || ('$' == strRead[strRead.length() - 2] && '~' == strRead[strRead.length() - 3]))
                    {
                    return true;
                    }*/
                }
                else  
                {
                    /* no need to output this for the EAGAIN case */
                   // if (rc != LIBSSH2_ERROR_EAGAIN) PELOG4WA_F("libssh2_channel_read returned %d", rc);   
                }
                Sleep(200);
                rc = 0;
            }

            return false;
        }

        //! ·¢ËÍSSH SHELL¿ØÖÆ¶Ë×Ö·û´®
        bool SendSSHShell(const std::string& strWrite,int time_out = 10);
        //!
        bool SendSSHShellCharacter(const char* szWrite,size_t len,int time_out = 10);
    public:
        int ReadSSHShell(std::string& strRead);
        int ReadSSHShell(std::wstring& strRead);
    private:
        bool InitLibSSHConnect();
    private:
        bool mbIsConnectSSH;
        char* mszSSHBuf;
        LIBSSH2_SESSION* mpSession;
        LIBSSH2_CHANNEL* mpChannel;
        std::string mstrUserName;
        std::string mstrPassword;
    };
}