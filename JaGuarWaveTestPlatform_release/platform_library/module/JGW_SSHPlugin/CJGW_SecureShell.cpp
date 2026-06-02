#include "StdAfx.h"
#include <JGW_SSHPlugin/CJGW_SecureShell.h>
#include "CJGW_SecureShellInit.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

extern JGW::CCJGW_SecureShellInit gSSHInit;

namespace JGW
{
    /*const char *keyfile1="~/.ssh/id_rsa.pub";
    const char *keyfile2="~/.ssh/id_rsa";
    const char *password="password";*/

    CCJGW_SecureShell::CCJGW_SecureShell(void): mpSession(NULL),mpChannel(NULL),mbConnectSSH(false),mpsftp_session(NULL),mpsftp_handle(NULL)
    {
        szSSHBuf = (char*)calloc(SSH_BUFSIZE,1);
    }


    CCJGW_SecureShell::~CCJGW_SecureShell(void)
    {
        DisConnectSFTP();
        CloseSSH();
        free(szSSHBuf);
    }

    static void kbd_callback(const char *name, int name_len,
        const char *instruction, int instruction_len,
        int num_prompts,
        const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
        LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
        void **abstract)
    {
        (void)name;
        (void)name_len;
        (void)instruction;
        (void)instruction_len;
        if (num_prompts == 1) {
            responses[0].text = strdup(password_ssh_shell);
            responses[0].length = strlen(password_ssh_shell);
        }
        (void)prompts;
        (void)abstract;
    } /* kbd_callback */

    bool CCJGW_SecureShell::ConnectSSHShell(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;
        password_ssh_shell = strPassword.c_str();
        if (mbConnectSSH)
        {
            DisConnectSFTP();
            CloseSSH();
        }
        //! 检测INIT 状态
        if (!gSSHInit.InitSecureShellBase() || !mbInitSocket)
        {
            PELOG4WW(L"Init SSH Library Or Init Socket Fail");
            return false;
        }

        //! 连接socket
        if (!ConnectSocketTimeOut(server_name,port,10 * 1000,10 * 1000,source_addr))
        {
            PELOG4WW_F(L"Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }
        int auth_pw = 0;
        const char *fingerprint;
        char *userauthlist;
        /* Create a session instance and start it up. This will trade welcome
        * banners, exchange keys, and setup crypto, compression, and MAC layers
        */
        mpSession = libssh2_session_init();
        if (libssh2_session_handshake(mpSession, mConnSocket)) 
        {
            PELOG4WW(L"Failure establishing SSH session");
            return false;
        }

        /* At this point we havn't authenticated. The first thing to do is check
        * the hostkey's fingerprint against our known hosts Your app may have it
        * hard coded, may go to a file, may present it to the user, that's your
        * call
        */
        fingerprint = libssh2_hostkey_hash(mpSession, LIBSSH2_HOSTKEY_HASH_SHA1);


        /* check what authentication methods are available */
        userauthlist = libssh2_userauth_list(mpSession, mstrUserName.c_str(), mstrUserName.length());
        PELOG4WW_F(L"Authentication methods: %s", JGW_A2W_A(userauthlist).c_str());

        if (NULL == userauthlist)
        {
            auth_pw = 0;
        }
        else
        {
            if (strstr(userauthlist, "password") != NULL) {
                auth_pw |= 1;
            }
            if (strstr(userauthlist, "keyboard-interactive") != NULL) {
                auth_pw |= 2;
            }
            if (strstr(userauthlist, "publickey") != NULL) {
                auth_pw |= 4;
            }
        }

        if (auth_pw & 1) 
        {
            /* We could authenticate via password */
            if (libssh2_userauth_password(mpSession, mstrUserName.c_str(), mstrPassword.c_str())) 
            {
                PELOG4WW(L"\tAuthentication by password failed!");
                return false;
            } 
            else 
            {
                PDLOG4WW(L"\tAuthentication by password succeeded.");
            }
        } 
        else if (auth_pw & 2) 
        {
            /* Or via keyboard-interactive */
            if (libssh2_userauth_keyboard_interactive(mpSession, mstrUserName.c_str(),
                &kbd_callback) ) 
            {
                PELOG4WW(L"\tAuthentication by keyboard-interactive failed!");
                return false;
            } 
            else 
            {
                PDLOG4WW(L"\tAuthentication by keyboard-interactive succeeded.");
            }
        } 
        else if (auth_pw & 4) 
        {
            /* Or by public key */
            if (libssh2_userauth_publickey_fromfile(mpSession, mstrUserName.c_str(), keyfile_ssh_shell1,
                keyfile_ssh_shell2, password_ssh_shell)) 
            {
                PELOG4WW(L"\tAuthentication by public key failed!");
                return false;
            } 
            else 
            {
                PDLOG4WW(L"\tAuthentication by public key succeeded.");
            }
        }
        else if (auth_pw == 0)
        {

        }
        else 
        {
            PELOG4WW(L"No supported authentication methods found!");
            return false;
        }

        /* Request a shell */
        if (!(mpChannel = libssh2_channel_open_session(mpSession))) {
            PELOG4WW(L"Unable to open a session");
            return false;
        }

        /* Some environment variables may be set,
        * It's up to the server which ones it'll allow though
        */
        libssh2_channel_setenv(mpChannel, "FOO", "bar");

        /* Request a terminal with 'vanilla' terminal emulation
        * See /etc/termcap for more options
        */
        if (libssh2_channel_request_pty(mpChannel, "vanilla")) 
        {
            PELOG4WW(L"Failed requesting pty");
            return false;
        }

        /* Open a SHELL on that pty */
        if (libssh2_channel_shell(mpChannel)) 
        {
            PELOG4WW(L"Unable to request shell on allocated pty");
            return false;
        }

         /* At this point the shell can be interacted with using
        * libssh2_channel_read()
        * libssh2_channel_read_stderr()
        * libssh2_channel_write()
        * libssh2_channel_write_stderr()
        *
        * Blocking mode may be (en|dis)abled with: libssh2_channel_set_blocking()
        * If the server send EOF, libssh2_channel_eof() will return non-0
        * To send EOF to the server use: libssh2_channel_send_eof()
        * A channel can be closed with: libssh2_channel_close()
        * A channel can be freed with: libssh2_channel_free()
        */
        return true;
    }

    bool CCJGW_SecureShell::ConnectSSH(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;
        if (mbConnectSSH)
        {
            DisConnectSFTP();
            CloseSSH();
        }
        //! 检测INIT 状态
        if (!gSSHInit.InitSecureShellBase() || !mbInitSocket)
        {
            PELOG4WW(L"Init SSH Library Or Init Socket Fail");
            return false;
        }
        //! 连接socket
        if (!ConnectSocketTimeOut(server_name,port,10 * 1000,10 * 1000,source_addr))
        {
            PELOG4WW_F(L"Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }
        //! 创建session
        if (!CreateSessionInstance() || !InitKnownHost(server_name,port) || !OpenSession(strUserName,strPassword))
        {
            return false;
        }
        mbConnectSSH = true;
        return true;
    }

    void CCJGW_SecureShell::CloseSSH()
    {
        int result = 0;
        char *exitsignal = (char *)"none";

        if (mpSession && CheckSocketIsConnected() && mpChannel)
        {
            while((result = libssh2_channel_close(mpChannel)) == LIBSSH2_ERROR_EAGAIN)
            {
                WaitSocket(mConnSocket, mpSession);
            }

            if(0 == result) 
            {
                libssh2_channel_get_exit_status(mpChannel);
                libssh2_channel_get_exit_signal(mpChannel, &exitsignal,NULL, NULL, NULL, NULL, NULL);
            }
        }

        if (mpChannel)
        {     
            libssh2_channel_free(mpChannel);
            mpChannel = NULL;
        }

        if (mpSession)
        {
            libssh2_session_disconnect(mpSession,"Normal Shutdown, Thank you for playing");
            libssh2_session_free(mpSession);
            mpSession = NULL;
        }
        CleanupSocket();
        mbConnectSSH = false;
    }

    void CCJGW_SecureShell::ReOpenChannel()
    {
        int result = 0;
        char *exitsignal = (char *)"none";
        if (mpSession && CheckSocketIsConnected() && mpChannel)
        {
            while((result = libssh2_channel_close(mpChannel)) == LIBSSH2_ERROR_EAGAIN)
            {
                WaitSocket(mConnSocket, mpSession);
            }

            if(0 == result) 
            {
                libssh2_channel_get_exit_status(mpChannel);
                libssh2_channel_get_exit_signal(mpChannel, &exitsignal,NULL, NULL, NULL, NULL, NULL);
            }
        }

        if (mpChannel)
        {     
            libssh2_channel_free(mpChannel);
            mpChannel = NULL;
        }

        OpenSession(mstrUserName,mstrPassword);
    }

    bool CCJGW_SecureShell::CreateSessionInstance()
    {
        mpSession = libssh2_session_init();
        if (!mpSession)
        {
            PELOG4WW(L"Create Session Instance Fail");
            return false;
        }

        int result = 0;
        /* tell libssh2 we want it all done non-blocking */
        libssh2_session_set_blocking(mpSession, 0);

        /* ... start it up. This will trade welcome banners, exchange keys,
        * and setup crypto, compression, and MAC layers
        */
        while ((result = libssh2_session_handshake(mpSession, mConnSocket)) == LIBSSH2_ERROR_EAGAIN);

        if (result) PELOG4WW_F(L"Failure establishing SSH session: %d", result);

        return (0 == result);
    }

    bool CCJGW_SecureShell::InitKnownHost(const std::string& server_name,size_t port /* = 22 */)
    {
        LIBSSH2_KNOWNHOSTS* pKnownhosts = libssh2_knownhost_init(mpSession);
        if (!pKnownhosts) 
        {
            PELOG4WW(L"Init Known Host Fail");
            return false;
        }

        const char* fingerprint = 0;
        size_t len = 0;
        int type = 0;
        bool bReturn = true;
        /* read all hosts from here */
        libssh2_knownhost_readfile(pKnownhosts, "known_hosts",LIBSSH2_KNOWNHOST_FILE_OPENSSH);

        /* store all known hosts to here */
        libssh2_knownhost_writefile(pKnownhosts, "dumpfile",LIBSSH2_KNOWNHOST_FILE_OPENSSH);

        fingerprint = libssh2_session_hostkey(mpSession, &len, &type);
        if (fingerprint) 
        {
            struct libssh2_knownhost *host;
            int check = libssh2_knownhost_checkp(pKnownhosts, server_name.c_str(), port,
                fingerprint, len,
                LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                LIBSSH2_KNOWNHOST_KEYENC_RAW,
                &host);

            PDLOG4WW_F(L"Host check: %d, key: %s", check,
                JGW_A2W_A((check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
                host->key:"<none>").c_str());

            /*****
            * At this point, we could verify that 'check' tells us the key is
            * fine or bail out.
            *****/
        }
        else 
        {
            bReturn = false;
        }
        libssh2_knownhost_free(pKnownhosts);
        pKnownhosts = NULL;
        return bReturn;
    }

    bool CCJGW_SecureShell::OpenSession(const std::string& strUserName,const std::string& strPassword)
    {
        int result = 0;
        if (strPassword.length() > 0) 
        {
            /* We could authenticate via password */
            while ((result = libssh2_userauth_password(mpSession, strUserName.c_str(), strPassword.c_str())) == LIBSSH2_ERROR_EAGAIN);
            if (result) 
            {
                PELOG4WW(L"Authentication by password failed.");
                return false;
            }
        }

        libssh2_trace(mpSession, LIBSSH2_TRACE_SOCKET);
        /* Exec non-blocking on the remove host */
        while((mpChannel = libssh2_channel_open_session(mpSession)) == NULL 
            && libssh2_session_last_error(mpSession,NULL,NULL,0) == LIBSSH2_ERROR_EAGAIN) 
            WaitSocket(mConnSocket, mpSession);

        if(NULL == mpChannel) 
        {
            PELOG4WW(L"Channel Open Serssion Error");
            return false;
        }

        return true;
    }

    //     void GetEnvironmentCommand(std::wstring& strCommand)
    //     {
    //         std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
    //         int nStart = strTemp.find(L"$$_"),nEnd = 0;
    //         /*
    //         JGW_ReplaceStringW(strTemp,L"$$_",L"");
    //         JGW_ReplaceStringW(strTemp,L"_$$",L"");
    //         strCommand = GetGlobalEnvironment()->GetString(strTemp.c_str());
    //         */
    //         while (std::wstring::npos != nStart)
    //         {
    //             nEnd = strTemp.find(L"_$$",nStart + 3);
    //             if (nEnd == std::wstring::npos) return ;
    //             strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
    //             strEnvironmentValue = GetGlobalEnvironment()->GetString(strEnvironmentName);
    //             JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
    //             JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
    //             nStart = strTemp.find(L"$$_",nEnd + 3);
    //         } 
    //     }

    bool CCJGW_SecureShell::ReadSSHShell(std::string& strRead,int time_out /* = 10 */)
    {
        jgw_timer jt; 
        int rc = 0;
        int n = 0;

        while (jt.elapsed() <= time_out)
        {
            memset(szSSHBuf,0x00,SSH_BUFSIZE);
            n = libssh2_channel_read(mpChannel, szSSHBuf, SSH_BUFSIZE);
            if (n == LIBSSH2_ERROR_EAGAIN)
            {
                strRead += szSSHBuf;
                continue;
            }
            else if (n < 0)
            {
                PELOG4WW(L"ssh read error");
                return false;
            }
            else
            {
                strRead += szSSHBuf;
                if (strRead.length() < 3)
                {
                    continue;
                }             
                if (('#' == strRead[strRead.length() - 2] && ']' == strRead[strRead.length() - 3])
                    || ('$' == strRead[strRead.length() - 2] && '~' == strRead[strRead.length() - 3]))
                {
                    return true;
                }
            }
        }

        if (0 == n)
        {
            ReOpenChannel();
        }

        return (n > 0);
    }

    bool CCJGW_SecureShell::WriteReadSSHShell(const std::string& strShell,std::string& strRead,int time_out /* = 10 */,int counts /* = 3 */)
    {
        jgw_timer tm;
        strRead = "";
        std::string strShellCommand = strShell;
        int i = 0;

        if ('\n' != strShellCommand[strShellCommand.length() - 1]) strShellCommand += "\n";

        while( tm.elapsed() < time_out && i++ < counts )
        {
            PDLOG4WW_F(L"shell: %s",JGW_A2W(strShellCommand).c_str());
            if (!SendSSH(strShellCommand,time_out)) { Sleep(100); continue;}
            //Sleep(200);
            if (!ReadSSHShell(strRead,time_out)) { Sleep(100); continue;}

            JGW_EraseFristToRemoveChar(strRead,L'\n');
            JGW_EraseAfterToRemoveChar(strRead,L'\n'); 
            JGW_EraseLastAndFristTwoChars(strRead);
            return true;
        }

        return false;
    }


    bool CCJGW_SecureShell::ExecSSH(const std::string& strCommandline)
    {
        int rc = 0;

        while( (rc = libssh2_channel_exec(mpChannel, strCommandline.c_str())) == LIBSSH2_ERROR_EAGAIN )
            WaitSocket(mConnSocket, mpSession);
        if( rc != 0 ) 
        {
            PELOG4WW(L"exec error");
            return false;
        }

        return true;
    }

    bool CCJGW_SecureShell::SendSSH(const std::string& strCommandline,int timeout /* = 10 */)
    {
        jgw_timer jt; 
        int result = 0;
        int n = 0;

        while (jt.elapsed() <= timeout)
        {
            n = libssh2_channel_write_ex(mpChannel, 0, strCommandline.c_str(), strCommandline.length());
            if (n == LIBSSH2_ERROR_EAGAIN) 
            {
                PELOG4WW(L"will write again");
                continue;
            }
            else if (n < 0) 
            {
                PELOG4WW(L"write failed");
                return false;
            }
            else 
            {
                break;
            }
        }
        return (n > 0);
    }

    bool CCJGW_SecureShell::ReadSSH(std::string& strReadBuf,int timeout /* = 10 */)
    {
        jgw_timer jt; 
        int rc = 0;
        int n = 0;

        while (jt.elapsed() <= timeout)
        {
            memset(szSSHBuf,0x00,SSH_BUFSIZE);
            n = libssh2_channel_read(mpChannel, szSSHBuf, SSH_BUFSIZE);
            if (n == LIBSSH2_ERROR_EAGAIN)
            {
                strReadBuf += szSSHBuf;
                continue;
            }
            else if (n < 0)
            {
                PELOG4WW(L"ssh read error");
                return false;
            }
            else
            {
                strReadBuf += szSSHBuf;
                break;
            }
        }
        return (n > 0);
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
            PELOG4WW(L"Unable to init SFTP session");
            return false;
        }

        mpsftp_handle = libssh2_sftp_open(mpsftp_session, strSFTPPath.c_str(),
            LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
            LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
            LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

        if (!mpsftp_handle) 
        {
            PELOG4WW(L"Unable to open file with SFTP");
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
            PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strLocaFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
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