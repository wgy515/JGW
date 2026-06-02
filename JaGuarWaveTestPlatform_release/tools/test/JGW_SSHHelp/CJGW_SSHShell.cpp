#include "StdAfx.h"
#include "CJGW_SSHShell.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CJGW_SecureShellInit.h"
extern JGW::CCJGW_SecureShellInit gSSHInit;

namespace JGW
{
    const char* keyfile_ssh_shell1="~/.ssh/id_rsa.pub";
    const char* keyfile_ssh_shell2="~/.ssh/id_rsa";
    const char* password_ssh_shell="password";

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

    CCJGW_SSHShell::CCJGW_SSHShell(void) : mpChannel(NULL),mpSession(NULL),mbIsConnectSSH(false)
    {
        mszSSHBuf = (char*)calloc(SSH_BUFSIZE,1);
    }


    CCJGW_SSHShell::~CCJGW_SSHShell(void)
    {
        if (mbIsConnectSSH) DisConnectSSHShell();
        free(mszSSHBuf);
    }

    bool CCJGW_SSHShell::IsSSHConnected()
    {
        if (mbIsConnectSSH)
        {
            std::string strRead;
            if (!RWSSHShell("ls\n",strRead,true,0,2,1)) 
                DisConnectSSHShell();
        }
        return mbIsConnectSSH;
    }

    bool CCJGW_SSHShell::IsConnected()
    {
        return mbIsConnectSSH;
    }

    bool CCJGW_SSHShell::ConnectSSHShell(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;
        password_ssh_shell = mstrPassword.c_str();
        DisConnectSSHShell();

        //! ¼ì²âINIT ×´Ì¬
        if (!gSSHInit.InitSecureShellBase() || !mbInitSocket)
        {
            PELOG4WW(L"Init SSH Library Or Init Socket Fail");
            return false;
        }

        //! Á¬½Ósocket
        if (!ConnectSocketTimeOut(server_name,port,10 * 1000,10 * 1000,source_addr))
        {
            PELOG4WW_F(L"Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }

        if (!InitLibSSHConnect())
        {
            PELOG4WW_F(L"SSH Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }
        mbIsConnectSSH = true;
        return true;
    }

    void CCJGW_SSHShell::DisConnectSSHShell()
    {
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
        mbIsConnectSSH = false;
    }

    bool CCJGW_SSHShell::InitLibSSHConnect()
    {
        int auth_pw = 0;
        const char* fingerprint;
        char* userauthlist;
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
        if (strstr(userauthlist, "password") != NULL) {
            auth_pw |= 1;
        }
        if (strstr(userauthlist, "keyboard-interactive") != NULL) {
            auth_pw |= 2;
        }
        if (strstr(userauthlist, "publickey") != NULL) {
            auth_pw |= 4;
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

    int CCJGW_SSHShell::ReadSSHShell(std::wstring& strRead)
    {
        std::string astrRead;
        int result = ReadSSHShell(astrRead);
        strRead = JGW_A2W(astrRead);
        return result;
    }

    int CCJGW_SSHShell::ReadCommandResult(char* read_buf,unsigned int read_buf_size)
    {
        LIBSSH2_POLLFD pollfd;
        int rc = 0;
        pollfd.type = LIBSSH2_POLLFD_CHANNEL;
        pollfd.fd.channel = mpChannel;
        pollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

        libssh2_poll(&pollfd, 1, 10);
        if (pollfd.revents & LIBSSH2_POLLFD_POLLIN) rc = libssh2_channel_read(mpChannel, read_buf, read_buf_size);
        if (rc != LIBSSH2_ERROR_EAGAIN && rc < 0)  return -1;
        return rc;
    }

    int CCJGW_SSHShell::ReadSSHShell(std::string& strRead)
    {
        memset(mszSSHBuf,0x00,SSH_BUFSIZE);
        LIBSSH2_POLLFD pollfd;
        int rc = 0;
        pollfd.type = LIBSSH2_POLLFD_CHANNEL;
        pollfd.fd.channel = mpChannel;
        pollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

        libssh2_poll(&pollfd, 1, 10);
        if (pollfd.revents & LIBSSH2_POLLFD_POLLIN) rc = libssh2_channel_read(mpChannel, mszSSHBuf, SSH_BUFSIZE);
        if (rc > 0) { strRead += mszSSHBuf; PDLOG4WW(JGW_A2W_A(mszSSHBuf).c_str());}
        if (rc != LIBSSH2_ERROR_EAGAIN && rc < 0)  return -1;
        return rc;
    }

    bool CCJGW_SSHShell::RecvSSHShell(std::string& strRead,int time_out /* = 10 */)
    {
        jgw_timer jt; 
        int rc = 0;
        LIBSSH2_POLLFD pollfd;
        pollfd.type = LIBSSH2_POLLFD_CHANNEL;
        pollfd.fd.channel = mpChannel;
        pollfd.events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

        while (jt.elapsed() <= time_out)
        {
            memset(mszSSHBuf,0x00,SSH_BUFSIZE);
            /*rc = libssh2_channel_read(mpChannel, mszSSHBuf, SSH_BUFSIZE);*/
            libssh2_poll(&pollfd, 1, 10);
            if (pollfd.revents & LIBSSH2_POLLFD_POLLIN) rc = libssh2_channel_read(mpChannel, mszSSHBuf, SSH_BUFSIZE);
            if (rc > 0)
            {
                if (strRead.length() > 1024 * 1024) strRead = "";
                strRead += mszSSHBuf;
                //!PDLOG4WA(mszSSHBuf);
                if (strRead.length() >= 3)
                {
                    //JGW_EraseLastAndFristTwoChars(strRead);
                    //JGW_EraseLastAndFristTwoChars(strRead);
                    //JGW_EraseLastAndFristTwoChars(strRead,' ');  
                    JGW_EraseLastAndFristTwoChars(strRead,' ',' '); 
                    if (('#' == strRead[strRead.length() - 1] && ']' == strRead[strRead.length() - 2])
                        || ('$' == strRead[strRead.length() - 1] && '~' == strRead[strRead.length() - 2])
                        || ('#' == strRead[strRead.length() - 1] && '~' == strRead[strRead.length() - 2])
                        || ('$' == strRead[strRead.length() - 1] && ']' == strRead[strRead.length() - 2])
                        )
                    {
                        return true;
                    }
                } 
            }
            else  
            {
                /* no need to output this for the EAGAIN case */
                if (rc != LIBSSH2_ERROR_EAGAIN && rc < 0) PELOG4WW_F(L"libssh2_channel_read returned %d", rc);  
                if (rc < 0 && rc != LIBSSH2_ERROR_EAGAIN)
                {
                    return strRead.length() > 0;
                }
            }
            Sleep(200);
            rc = 0;
        }

        return false;
    }

    bool CCJGW_SSHShell::SendSSHShellCharacter(const char* szWrite,size_t len,int time_out/* = 10*/)
    {
        jgw_timer jt; 
        int result = 0;
        int n = 0;
//         std::string strShellCommand = strWrite;
// 
//         if ('\n' != strShellCommand[strShellCommand.length() - 1]) strShellCommand += "\n";
        while (jt.elapsed() <= time_out)
        {
            n = libssh2_channel_write_ex(mpChannel, 0,szWrite, len);
            if (n == len) return true;

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
            Sleep(200);
        }
        return false;
    }

    bool CCJGW_SSHShell::SendSSHShell(const std::string& strWrite,int time_out /* = 10 */)
    {
        jgw_timer jt; 
        int result = 0;
        int n = 0;
        std::string strShellCommand = strWrite;

        if ('\n' != strShellCommand[strShellCommand.length() - 1]) strShellCommand += "\n";
        while (jt.elapsed() <= time_out)
        {
            n = libssh2_channel_write_ex(mpChannel, 0, strShellCommand.c_str(), strShellCommand.length());
            if (n == strShellCommand.length()) return true;

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
            Sleep(200);
        }
        return false;
    }

    bool CCJGW_SSHShell::RWSSHShell(const std::string& strWrite,std::string& strRead,bool bRead /* = true */,int nTestWaitResult /* = 0 */,int rw_time_out /* = 10 */,int counts /* = 3 */)
    {
        jgw_timer tm;
        strRead = "";
        std::string strShellCommand = strWrite;
        int i = 0;

        if ('\n' != strShellCommand[strShellCommand.length() - 1]) strShellCommand += "\n";

        while( tm.elapsed() < rw_time_out && i++ < counts )
        {
            PDLOG4WW_F(L"shell: %s",JGW_A2W(strShellCommand).c_str());
            if (!SendSSHShell(strShellCommand,rw_time_out)) { Sleep(100); continue;}
            Sleep(nTestWaitResult);
            if (!bRead) return true;
            if (!RecvSSHShell(strRead,rw_time_out)) { Sleep(100); continue;}
            JGW_EraseFristToRemoveChar(strRead,L'\n');
            JGW_EraseAfterToRemoveChar(strRead,L'\n'); 
            JGW_EraseLastAndFristTwoChars(strRead);
            PDLOG4WW_F(L"shell: %s",JGW_A2W(strRead).c_str());
            return true;
        }

        return false;
    }
}