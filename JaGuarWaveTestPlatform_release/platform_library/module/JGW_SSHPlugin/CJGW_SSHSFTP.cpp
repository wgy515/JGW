#include "StdAfx.h"
#include <JGW_SSHPlugin/CJGW_SSHSFTP.h>
#include "CJGW_SecureShellInit.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

extern JGW::CCJGW_SecureShellInit gSSHInit;

namespace JGW
{
    extern void kbd_callback(const char *name, int name_len,
        const char *instruction, int instruction_len,
        int num_prompts,
        const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
        LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
        void **abstract);

    CCJGW_SSHSFTP::CCJGW_SSHSFTP(void) : mpsftp_session(NULL),mpsftp_handle(NULL),mpSession(NULL),mpChannel(NULL)
    {
        mszSSHBuf = (char*)calloc(SSH_BUFSIZE,1);
    }


    CCJGW_SSHSFTP::~CCJGW_SSHSFTP(void)
    {
        free(mszSSHBuf);
    }

    bool CCJGW_SSHSFTP::ConnectSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr /* = NULL */,size_t port /* = 22 */,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR| LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;
        DisConnectSSHSFTP();

        //! ¼ì²âINIT ×´Ì¬
        if (!gSSHInit.InitSecureShellBase() || !mbInitSocket)
        {
            PELOG4WW(L"Init SSH Library Or Init Socket Fail");
            return false;
        }

        //! Á¬½Ósocket
        if (!ConnectSocketTimeOut(server_name,port,10 * 1000,10 * 1000,source_addr))
        {
            PELOG4WW_F(L"Socket Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }

        if (!InitSSHSFTP(server_name,strUserName,strPassword,strSFTPPath,source_addr,port,flags,mode))
        {
            PELOG4WW_F(L"SSH Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }

        return true;
    }

    bool CCJGW_SSHSFTP::InitSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr /* = NULL */,size_t port /* = 22 */,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR| LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {
        int rc = 0;
        const char* fingerprint = NULL;
        /* Create a session instance
        */
        mpSession = libssh2_session_init();
        if(!mpSession)
            return false;

        /* Since we have set non-blocking, tell libssh2 we are blocking */
        libssh2_session_set_blocking(mpSession, 1);

        /* ... start it up. This will trade welcome banners, exchange keys,
        * and setup crypto, compression, and MAC layers
        */
        rc = libssh2_session_handshake(mpSession, mConnSocket);
        if(rc) 
        {
            PELOG4WW_F(L"Failure establishing SSH session: %d", rc);
            return false;
        }

        /* At this point we havn't yet authenticated.  The first thing to do
        * is check the hostkey's fingerprint against our known hosts Your app
        * may have it hard coded, may go to a file, may present it to the
        * user, that's your call
        */
        fingerprint = libssh2_hostkey_hash(mpSession, LIBSSH2_HOSTKEY_HASH_SHA1);
#if 0
        if (!strPassword.empty()) 
        {
            /* We could authenticate via password */
            if (libssh2_userauth_password(mpSession, mstrUserName.c_str(), mstrPassword.c_str())) 
            {
                PELOG4WW(L"Authentication by password failed.");
                return false;
            }
        } 
        else 
        {
            /* Or by public key */
            if (libssh2_userauth_publickey_fromfile(mpSession, mstrUserName.c_str(),
                "/home/username/.ssh/id_rsa.pub",
                "/home/username/.ssh/id_rsa",
                mstrPassword.c_str())) 
            {
                    PELOG4WW(L"\tAuthentication by public key failed");
                    return false;
            }
        }
#else
        int auth_pw = 0;
        char* userauthlist;
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
        else if (0 == auth_pw)
        {

        }
        else 
        {
            PELOG4WW(L"No supported authentication methods found!");
            return false;
        }
#endif
        mpsftp_session = libssh2_sftp_init(mpSession);
        if (!mpsftp_session) 
        {
            PELOG4WW(L"Unable to init SFTP session");
            return false;
        }

        mpsftp_handle = libssh2_sftp_open(mpsftp_session, strSFTPPath.c_str(),
            flags,
            mode);
        if (!mpsftp_handle) 
        {
            PELOG4WW(L"Unable to open file with SFTP");
            return false;
        }
        return true;
    }

    bool CCJGW_SSHSFTP::ChangeSSHSFTPPath(const std::string& strSFTPPath,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR| LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {
        if (mpsftp_handle)
        {
            libssh2_sftp_close(mpsftp_handle);
            mpsftp_handle = NULL;
        }

        mpsftp_handle = libssh2_sftp_open(mpsftp_session, strSFTPPath.c_str(),
            flags,
            mode);
        if (!mpsftp_handle) 
        {
            PELOG4WW(L"Unable to open file with SFTP");
            return false;
        }
        return true;
    }

    void CCJGW_SSHSFTP::DisConnectSSHSFTP()
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
            libssh2_session_disconnect(mpSession,
                "Normal Shutdown, Thank you for playing");
            libssh2_session_free(mpSession);
            mpSession = NULL;
        }
        CleanupSocket();
    }

    bool CCJGW_SSHSFTP::PutFileSFTP(const std::string& strLocaFilePath)
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
            nread = fread(mszSSHBuf, 1, SSH_BUFSIZE, local);
            if (nread <= 0) 
            {
                /* end of file */
                break;
            }
            ptr = mszSSHBuf;
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

    bool CCJGW_SSHSFTP::GetFileSFTP(const std::string& strSaveLocalFilePath)
    {
        FILE* local = NULL;
        size_t nread = 0;
        int rc = 0;
        char mem[1024] = {0};

        fopen_s(&local,strSaveLocalFilePath.c_str(), "wb");
        if (!local) 
        {
            PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strSaveLocalFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
            return false;
        }

        do 
        {
            /* write data in a loop until we block */
            rc = libssh2_sftp_read(mpsftp_handle, mem, sizeof(mem));
            if (rc > 0) {
                fwrite(mem, rc,1,local);
            } else {
                break;
            }

        } while (1);

        if (local) fclose(local);
        return true;

    }
}