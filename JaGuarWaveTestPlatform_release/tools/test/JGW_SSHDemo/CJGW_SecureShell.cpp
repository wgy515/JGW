#include "StdAfx.h"
#include "CJGW_SecureShell.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    CCJGW_SecureShell::CCJGW_SecureShell(void) : mpSession(NULL),mnSocketPort(22),mpChannel(NULL),mbConnectSSH(false),mpsftp_session(NULL),mpsftp_handle(NULL)
    {
        mPollfd.type = LIBSSH2_POLLFD_CHANNEL;
        mPollfd.fd.channel = mpChannel;
        mPollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;
        szSSHBuf = (char*)malloc(SSH_BUFSIZE);
    }


    CCJGW_SecureShell::~CCJGW_SecureShell(void)
    {
        DisConnectSFTP();
        CloseSSH();
        free(szSSHBuf);
    }

    bool CCJGW_SecureShell::ConnectSSH(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        if (!mbInitSocket)
        {
            PELOG4WW(L"Init SSH Library Or Init Socket Fail\n");
            return false;
        }

        mstrHostName = server_name;
        mnSocketPort = port;

        if (!ConnectSocketTimeOut(server_name,port,10 * 1000,10 * 1000,source_addr))
        {
            PELOG4WA_F("Connect %s Fail\n",server_name.c_str());
            return false;
        }

        if (!CreateSessionInstance() || !InitKnownHost() || !OpenSession(strUserName,strPassword))
        {
            return false;
        }
        mbConnectSSH = true;
        return true;
    }

    void CCJGW_SecureShell::CloseSSH()
    {
        int rc = 0;
        int  exitcode = 127;
        char *exitsignal=(char *)"none";

        if (mpSession && CheckSocketIsConnected() && mpChannel)
        {
            while( (rc = libssh2_channel_close(mpChannel)) == LIBSSH2_ERROR_EAGAIN )
                WaitSocket(mConnSocket, mpSession);
            if( rc == 0 ) {
                exitcode = libssh2_channel_get_exit_status( mpChannel );
                libssh2_channel_get_exit_signal(mpChannel, &exitsignal,
                    NULL, NULL, NULL, NULL, NULL);
            }
        }

        if (mpChannel)
        {     
            libssh2_channel_free(mpChannel);
            mpChannel = NULL;
        }

        if (mpSession)
        {
            libssh2_session_disconnect(mpSession,
                "Normal Shutdown, Thank you for playing");
            libssh2_session_free(mpSession);
            mpSession = NULL;
        }

        CleanupSocket();    
        mbConnectSSH = false;
    }

    bool CCJGW_SecureShell::CreateSessionInstance()
    {
        mpSession = libssh2_session_init();
        if (!mpSession)
        {
            PELOG4WW(L"Create Session Instance Fail\n");
            return false;
        }

        int rc = 0;
        /* tell libssh2 we want it all done non-blocking */
        libssh2_session_set_blocking(mpSession, 0);

        /* ... start it up. This will trade welcome banners, exchange keys,
        * and setup crypto, compression, and MAC layers
        */
        while ((rc = libssh2_session_handshake(mpSession, mConnSocket)) ==
            LIBSSH2_ERROR_EAGAIN);
        if (rc) {
            PELOG4WW_F(L"Failure establishing SSH session: %d\n", rc);
            return false;
        }

        return true;
    }

    bool CCJGW_SecureShell::InitKnownHost()
    {
        LIBSSH2_KNOWNHOSTS *mpKnownhosts = libssh2_knownhost_init(mpSession);
        if(!mpKnownhosts) 
        {
            PELOG4WW(L"Init Known Host Fail\n");
            return false;
        }

        const char* fingerprint = 0;
        size_t len = 0;
        int type = 0;
        bool bReturn = true;
        /* read all hosts from here */
        libssh2_knownhost_readfile(mpKnownhosts, "known_hosts",
            LIBSSH2_KNOWNHOST_FILE_OPENSSH);

        /* store all known hosts to here */
        libssh2_knownhost_writefile(mpKnownhosts, "dumpfile",
            LIBSSH2_KNOWNHOST_FILE_OPENSSH);

        fingerprint = libssh2_session_hostkey(mpSession, &len, &type);
        if(fingerprint) 
        {
            struct libssh2_knownhost *host;
            int check = libssh2_knownhost_checkp(mpKnownhosts, mstrHostName.c_str(), mnSocketPort,
                fingerprint, len,
                LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                LIBSSH2_KNOWNHOST_KEYENC_RAW,
                &host);

            PDLOG4WA_F("Host check: %d, key: %s\n", check,
                (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
                host->key:"<none>");

            /*****
            * At this point, we could verify that 'check' tells us the key is
            * fine or bail out.
            *****/
        }
        else 
        {
            bReturn = false;
        }
        libssh2_knownhost_free(mpKnownhosts);
        mpKnownhosts = NULL;
        return bReturn;
    }

    bool CCJGW_SecureShell::OpenSession(const std::string& strUserName,const std::string& strPassword)
    {
        int rc = 0;

        if (strPassword.length() > 0) 
        {
            /* We could authenticate via password */
            while ((rc = libssh2_userauth_password(mpSession, strUserName.c_str(), strPassword.c_str())) ==
                LIBSSH2_ERROR_EAGAIN);
            if (rc) 
            {
                PELOG4WW(L"Authentication by password failed.\n");
                return false;
            }
        }

        libssh2_trace(mpSession, LIBSSH2_TRACE_SOCKET);
        /* Exec non-blocking on the remove host */
        while( (mpChannel = libssh2_channel_open_session(mpSession)) == NULL &&
            libssh2_session_last_error(mpSession,NULL,NULL,0) ==
            LIBSSH2_ERROR_EAGAIN ) {
                WaitSocket(mConnSocket, mpSession);
        }

        if( mpChannel == NULL ) 
        {
            PELOG4WW(L"Channel Open Serssion Error\n");
            return false;
        }

        return true;
        /*
        while( (rc = libssh2_channel_exec(mpChannel, commandline)) ==
        LIBSSH2_ERROR_EAGAIN )
        waitsocket(sock, session);

        if( rc != 0 ) {
        fprintf(stderr, "exec error\n");
        exit( 1 );
        }*/
    }

    bool CCJGW_SecureShell::ExecSSH(const std::string& strCommandline)
    {
        int rc = 0;

        while( (rc = libssh2_channel_exec(mpChannel, strCommandline.c_str())) == LIBSSH2_ERROR_EAGAIN )
            WaitSocket(mConnSocket, mpSession);
        if( rc != 0 ) 
        {
            PELOG4WW(L"exec error\n");
            char *errmsg;
            int errlen;
            int err = libssh2_session_last_error(mpSession, &errmsg, &errlen, 0);
            fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
            return false;
        }

        return true;
    }

    bool CCJGW_SecureShell::SendSSH(const std::string& strCommandline,int timeout /* = 10 */)
    {
        mPollfd.type = LIBSSH2_POLLFD_CHANNEL;
        mPollfd.fd.channel = mpChannel;
        mPollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;
        jgw_timer jt; 
        int rc = 0;
        int act = 0;

        while (jt.elapsed() <= timeout)
        {
            //             rc = (libssh2_poll(&mPollfd, 1, 10));
            //             if (mPollfd.revents & LIBSSH2_POLLFD_POLLOUT)
            //             {
            int n = libssh2_channel_write_ex(mpChannel, 0, strCommandline.c_str(), strCommandline.length());
            if (n == LIBSSH2_ERROR_EAGAIN) 
            {
                PELOG4WW(L"will write again\n");
                continue;
            }
            else if (n < 0) 
            {
                PELOG4WW(L"write failed\n");
                return false;
            }
            else if (n == 0)
            {
                char *errmsg;
                int errlen;
                int err = libssh2_session_last_error(mpSession, &errmsg, &errlen, 0);
                fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
            }
            else 
            {
                break;
            }
            //             }
            //             if (mPollfd.revents & LIBSSH2_POLLFD_CHANNEL_CLOSED)
            //             {
            //                 return false;
            //             }
        }
        return true;
    }

    bool CCJGW_SecureShell::ReadSSH(std::string& strReadBuf,int timeout /* = 10 */)
    {
        mPollfd.type = LIBSSH2_POLLFD_CHANNEL;
        mPollfd.fd.channel = mpChannel;
        mPollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;
        jgw_timer jt; 
        int rc = 0;

        while (jt.elapsed() <= timeout)
        {
            //             rc = (libssh2_poll(&mPollfd, 1, 10));
            //             if (mPollfd.revents & LIBSSH2_POLLFD_POLLIN)
            //             {
            memset(szSSHBuf,0x00,SSH_BUFSIZE);
            int n = libssh2_channel_read(mpChannel, szSSHBuf, SSH_BUFSIZE);
            if (n == LIBSSH2_ERROR_EAGAIN)
            {
                strReadBuf += szSSHBuf;
                continue;
            }
            else if (n < 0)
            {
                PELOG4WW(L"ssh read error\n");
                return false;
            }
            else if (n == 0)
            {
                char *errmsg;
                int errlen;
                int err = libssh2_session_last_error(mpSession, &errmsg, &errlen, 0);
                fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
            }
            else
            {
                strReadBuf += szSSHBuf;
                break;
            }
            //             }
            //             if (mPollfd.revents & LIBSSH2_POLLFD_CHANNEL_CLOSED)
            //             {
            //                 return false;
            //             }
        }
        return true;
    }

    bool CCJGW_SecureShell::ConnectSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        if (!mbConnectSSH)
        {
            CloseSSH();
            mbConnectSSH = ConnectSSH(server_name,strUserName,strPassword,source_addr,port);
        }

        if (!mbConnectSSH) return false;
        libssh2_session_set_blocking(mpSession, 1);

        mpsftp_session = libssh2_sftp_init(mpSession);
        if (!mpsftp_session) 
        {
            PELOG4WW(L"Unable to init SFTP session\n");
            return false;
        }

        mpsftp_handle =
            libssh2_sftp_open(mpsftp_session, strSFTPPath.c_str(),
            LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
            LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
            LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

        if (!mpsftp_handle) 
        {
            PELOG4WW(L"Unable to open file with SFTP\n");
            return false;
        }

        return true;
    }

    bool CCJGW_SecureShell::PutFileSFTP(const std::string& strLocaFilePath)
    {
        size_t nread = 0;
        char* ptr = NULL;
        int rc = 0;
        FILE* local = NULL;

        fopen_s(&local,strLocaFilePath.c_str(), "rb");
        if (!local) 
        {
            PELOG4WA_F("Can't open local file %s\n", strLocaFilePath.c_str());
            return false;
        }

        do 
        {
            nread = fread(szSSHBuf, 1, SSH_BUFSIZE, local);
            if (nread <= 0) 
            {
                /* end of file */
                break;
            }
            ptr = szSSHBuf;

            do 
            {
                /* write data in a loop until we block */
                rc = libssh2_sftp_write(mpsftp_handle, ptr, nread);
                if(rc < 0) break;
                ptr += rc;
                nread -= rc;
            } while (nread);

        } while (rc > 0);

        if (local) fclose(local);
        return true;
    }

    void CCJGW_SecureShell::DisConnectSFTP()
    {
        if (mpsftp_handle)
        {
            libssh2_sftp_close(mpsftp_handle);
            mpsftp_handle = NULL;
        }

        if (mpsftp_session)
        {
            libssh2_sftp_shutdown(mpsftp_session);
            mpsftp_session = NULL;
        }
        if (mpSession)
        {
            libssh2_session_set_blocking(mpSession, 0);
        }
        CloseSSH();
    }

    int CCJGW_SecureShell::WaitSocket(int socket_fd, LIBSSH2_SESSION *session)
    {
        struct timeval timeout;
        int rc;
        fd_set fd;
        fd_set *writefd = NULL;
        fd_set *readfd = NULL;
        int dir;

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        FD_ZERO(&fd);

        FD_SET(socket_fd, &fd);

        /* now make sure we wait in the correct direction */
        dir = libssh2_session_block_directions(session);

        if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
            readfd = &fd;

        if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
            writefd = &fd;

        rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

        return rc;
    }
}