#include "StdAfx.h"
#include <JGW_SSHPlugin/CJGW_SSHSFTPV2.h>
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

    extern int LibSSH2_WaitSocket(int socket_fd, LIBSSH2_SESSION *session);

    CCJGW_SSHSFTPV2::CCJGW_SSHSFTPV2(void) : mpsftp_session(NULL),mpsftp_handle(NULL),mpSession(NULL),mpChannel(NULL),mnFTPFlags(LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC),mnFTPMode(LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
        LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH)
    {
        mszSSHBuf = (char*)calloc(SSH_BUFSIZE,1);
    }


    CCJGW_SSHSFTPV2::~CCJGW_SSHSFTPV2(void)
    {
        free(mszSSHBuf);
    }

    bool CCJGW_SSHSFTPV2::ConnectSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const std::string& strSFTPPath,const char* source_addr /* = NULL */,size_t port /* = 22 */,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR| LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {

        mstrSFTPPath = strSFTPPath;
        mnFTPFlags = flags;
        mnFTPMode = mode;
        return ConnectSSHSFTP(server_name,strUserName,strPassword,source_addr,port);
    }

    bool CCJGW_SSHSFTPV2::ConnectSSHSFTP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;;
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

        if (!InitSSHSFTPV2())
        {
            PELOG4WW_F(L"SSH Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }

        return true;
    }

    bool CCJGW_SSHSFTPV2::InitSSHSFTPV2()
    {
        int rc = 0;
        const char* fingerprint = NULL;
        /* Create a session instance
        */
        mpSession = libssh2_session_init();
        if(!mpSession)
        {
            return false;
        }

        /* Since we have set non-blocking, tell libssh2 we are non-blocking */
        libssh2_session_set_blocking(mpSession, 0);

        /* ... start it up. This will trade welcome banners, exchange keys,
        * and setup crypto, compression, and MAC layers
        */
        while ((rc = libssh2_session_handshake(mpSession, mConnSocket)) == LIBSSH2_ERROR_EAGAIN);
        if (rc) 
        {
            //fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
            PELOG4WW_F(L"Failure establishing SSH session: %d", rc);
            return false;
        }

        /* At this point we havn't yet authenticated.  The first thing to do
        * is check the hostkey's fingerprint against our known hosts Your app
        * may have it hard coded, may go to a file, may present it to the
        * user, that's your call
        */
        do 
        {
            fingerprint = libssh2_hostkey_hash(mpSession, LIBSSH2_HOSTKEY_HASH_SHA1);
            if (NULL == fingerprint)
            {
                if (libssh2_session_last_errno(mpSession) == LIBSSH2_ERROR_EAGAIN)
                {
                    LibSSH2_WaitSocket(mConnSocket,mpSession);
                }
                else
                {
                    PELOG4WW(L"check the hostkey's fingerprint failed!");
                    return false;
                }
            }
        } while (!fingerprint);
        
        std::wstring strLog(L"Fingerprint: ");
        for (int i = 0; i < 20 && NULL != fingerprint; i++) 
        {
            strLog = JGW_GetFormatWString(L"%02X ", (unsigned char)fingerprint[i]);
        }
        PDLOG4WW(strLog.c_str());

        int auth_pw = 0;
        /* check what authentication methods are available */
        char* userauthlist = NULL;
        do 
        {
            userauthlist = libssh2_userauth_list(mpSession, mstrUserName.c_str(), mstrUserName.length());
            if (NULL == userauthlist)
            {
                rc = libssh2_session_last_errno(mpSession);
                if (rc == LIBSSH2_ERROR_EAGAIN)
                {
                    LibSSH2_WaitSocket(mConnSocket,mpSession);
                }
                else if (rc == LIBSSH2_ERROR_NONE)
                {
                    break;
                }
                else
                {
                    PELOG4WW(L"check what authentication methods are available failed!");
                    return false;
                }
            }
        } while (!userauthlist);
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
            while ((rc = libssh2_userauth_password(mpSession, mstrUserName.c_str(), mstrPassword.c_str())) == LIBSSH2_ERROR_EAGAIN);
            if (rc)
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
            while ((rc = libssh2_userauth_keyboard_interactive(mpSession, mstrUserName.c_str(),
                &kbd_callback)) == LIBSSH2_ERROR_EAGAIN);
            if (rc)
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
            while ((rc = libssh2_userauth_publickey_fromfile(mpSession, mstrUserName.c_str(), keyfile_ssh_shell1,
                keyfile_ssh_shell2, password_ssh_shell)) == LIBSSH2_ERROR_EAGAIN); 
            if (rc)
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

        return true;
    }

    void CCJGW_SSHSFTPV2::DisConnectSSHSFTP()
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

    bool CCJGW_SSHSFTPV2::ChangeSSHSFTPPath(const std::string& strSFTPPath,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR| LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {
        mstrSFTPPath = strSFTPPath;
        mnFTPFlags = flags;
        mnFTPMode = mode;
        return true;
    }

    bool CCJGW_SSHSFTPV2::SFTPInit()
    {
        PDLOG4WW(L"libssh2_sftp_init()!\n");
        do {
            mpsftp_session = libssh2_sftp_init(mpSession);

            if(!mpsftp_session) 
            {
                if(libssh2_session_last_errno(mpSession) == LIBSSH2_ERROR_EAGAIN) 
                {
                    PDLOG4WW(L"non-blocking init\n");
                    LibSSH2_WaitSocket(mConnSocket, mpSession); /* now we wait */
                }
                else 
                {
                    PELOG4WW(L"Unable to init SFTP session\n");
                    return false;
                }
            }
        } while (!mpsftp_session);

        return true;
    }

    bool CCJGW_SSHSFTPV2::SFTPOpen()
    {
        fprintf(stderr, "libssh2_sftp_open()!\n");
        /* Request a file via SFTP */
        do {
            mpsftp_handle = libssh2_sftp_open(mpsftp_session, mstrSFTPPath.c_str(), mnFTPFlags, mnFTPMode);

            if (!mpsftp_handle) 
            {
                if (libssh2_session_last_errno(mpSession) != LIBSSH2_ERROR_EAGAIN) 
                {
                    PELOG4WW(L"Unable to open file with SFTP\n");
                    return false;
                }
                else 
                {
                    PDLOG4WW(L"non-blocking open\n");
                    LibSSH2_WaitSocket(mConnSocket, mpSession); /* now we wait */
                }
            }
        } while (!mpsftp_handle);

        return true;
    }

    bool CCJGW_SSHSFTPV2::PutFileSFTP(const std::string& strLocaFilePath)
    {
        if (!SFTPInit())
        {
            return false;
        }

        if (!SFTPOpen())
        {
            libssh2_sftp_shutdown(mpsftp_session);
            return false;
        }

        size_t nread = 0;
        char* ptr = NULL;
        int rc = 0;
        long total = 0;
        FILE* local = NULL;

        fopen_s(&local,strLocaFilePath.c_str(), "rb");
        if (!local) 
        {
            PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strLocaFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
            return false;
        }

        do {
            nread = fread(mszSSHBuf, 1, SSH_BUFSIZE, local);
            if (nread <= 0) 
            {
                /* end of file */
                break;
            }
            ptr = mszSSHBuf;

            total += nread;

            do {
                /* write data in a loop until we block */
                while ((rc = libssh2_sftp_write(mpsftp_handle, ptr, nread)) == LIBSSH2_ERROR_EAGAIN) 
                {
                    LibSSH2_WaitSocket(mConnSocket, mpSession);;
                }
                if(rc < 0)
                {
                    break;
                }
                ptr += rc;
                nread -= rc;

            } while (nread);
        } while (rc > 0);

        if (local) fclose(local);
        libssh2_sftp_close(mpsftp_handle);

        return true;
    }

    bool CCJGW_SSHSFTPV2::PutFileSFTP(const std::string& strSFTPPath,const std::string& strLocaFilePath,unsigned long flags /* = LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC */,long mode /* = LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH */)
    {
        mstrSFTPPath = strSFTPPath;
        mnFTPFlags = flags;
        mnFTPMode = mode;
        return PutFileSFTP(strLocaFilePath);
    }

    bool CCJGW_SSHSFTPV2::GetFileSFTP(const std::string& strSaveLocalFilePath)
    {
        if (!SFTPInit())
        {
            return false;
        }

        if (!SFTPOpen())
        {
            libssh2_sftp_shutdown(mpsftp_session);
            return false;
        }

        int rc;
        int total = 0;
        int spin = 0;
        FILE* local = NULL;
        bool result = false;
        PDLOG4WW(L"libssh2_sftp_open() is done, now receive data!\n");

        fopen_s(&local,strSaveLocalFilePath.c_str(), "wb");
        if (!local) 
        {
            PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strSaveLocalFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
            return false;
        }

        do {
            /* loop until we fail */
            while ((rc = libssh2_sftp_read(mpsftp_handle, mszSSHBuf,RECV_SSH_BUFSIZE)) == LIBSSH2_ERROR_EAGAIN) 
            {
                spin++;
                LibSSH2_WaitSocket(mConnSocket, mpSession); /* now we wait */
            }
            if (rc > 0) 
            {
                total += rc;
                fwrite(mszSSHBuf, rc,1,local);
            }
            else if (rc == LIBSSH2_ERROR_NONE)
            {
                result = true;
                break;
            }
            else 
            {
                break;
            }
        } while (1);

        PDLOG4WW_F(L"sftp Got %d bytes spin: %d\n", total, spin);
        if (local) fclose(local);
        libssh2_sftp_close(mpsftp_handle);

        return result;
    }

    bool CCJGW_SSHSFTPV2::GetFileSFTP(const std::string& strSFTPPath,const std::string& strSaveLocalFilePath,unsigned long flags /* = LIBSSH2_FXF_READ */,long mode /* = 0 */)
    {
        mstrSFTPPath = strSFTPPath;
        mnFTPFlags = flags;
        mnFTPMode = mode;
        return GetFileSFTP(strSaveLocalFilePath);
    }
}