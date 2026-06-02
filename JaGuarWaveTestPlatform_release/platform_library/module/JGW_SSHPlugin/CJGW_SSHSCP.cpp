#include "StdAfx.h"
#include <JGW_SSHPlugin/CJGW_SSHSCP.h>
#include "CJGW_SecureShellInit.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
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

    CCJGW_SSHSCP::CCJGW_SSHSCP(void) : mpSession(NULL),mpChannel(NULL)
    {
        mszSSHBuf = (char*)calloc(SSH_BUFSIZE,1);
    }


    CCJGW_SSHSCP::~CCJGW_SSHSCP(void)
    {
        if (mszSSHBuf)
        {
            free(mszSSHBuf);
            mszSSHBuf = NULL;
        }
    }

    bool CCJGW_SSHSCP::ConnectSSHSCP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
        mstrUserName = strUserName;
        mstrPassword = strPassword;
        DisConnectSSHSCP();

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

        if (!InitSSHSCP(server_name,strUserName,strPassword,source_addr,port))
        {
            PELOG4WW_F(L"SSH Connect %s Fail",JGW_A2W(server_name).c_str());
            return false;
        }

        return true;
    }

    bool CCJGW_SSHSCP::InitSSHSCP(const std::string& server_name,const std::string& strUserName,const std::string& strPassword,const char* source_addr /* = NULL */,size_t port /* = 22 */)
    {
#if 1
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
        PDLOG4WW_F(L"Authentication methods: %s", JGW_A2W_A(userauthlist).c_str());

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
#else
        int rc = 0;
        const char* fingerprint = NULL;
        /* Create a session instance
        */
        mpSession = libssh2_session_init();
        if(!mpSession)
            return false;

        /* Since we have set non-blocking, tell libssh2 we are blocking */
        libssh2_session_set_blocking(mpSession, 0);

        /* ... start it up. This will trade welcome banners, exchange keys,
        * and setup crypto, compression, and MAC layers
        */
        while ((rc = libssh2_session_handshake(mpSession, mConnSocket))
            == LIBSSH2_ERROR_EAGAIN);
        if(rc) {
            fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
            return false;
        }

        /*  rc = libssh2_session_handshake(mpSession, mConnSocket);
        if(rc) 
        {
        PELOG4WW_F(L"Failure establishing SSH session: %d", rc);
        return false;
        }*/

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
            while (rc = libssh2_userauth_password(mpSession, mstrUserName.c_str(), mstrPassword.c_str()));
            if (rc) 
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
#endif
        return true;
    }

    void CCJGW_SSHSCP::DisConnectSSHSCP()
    {
        if (mpSession)
        {
            libssh2_session_disconnect(mpSession,
                "Normal Shutdown, Thank you for playing");
            libssh2_session_free(mpSession);
            mpSession = NULL;
        }
        CleanupSocket();
    }

    //static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
    //{
    //    struct timeval timeout;
    //    int rc;
    //    fd_set fd;
    //    fd_set *writefd = NULL;
    //    fd_set *readfd = NULL;
    //    int dir;

    //    timeout.tv_sec = 10;
    //    timeout.tv_usec = 0;

    //    FD_ZERO(&fd);

    //    FD_SET(socket_fd, &fd);

    //    /* now make sure we wait in the correct direction */
    //    dir = libssh2_session_block_directions(session);

    //    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
    //        readfd = &fd;

    //    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
    //        writefd = &fd;

    //    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    //    return rc;
    //}

    bool CCJGW_SSHSCP::PutFileSCP(const std::string& strScpPath,const std::string& strLocaFilePath,int mode /* = 644 */)
    {
        struct stat fileinfo;
        size_t nread = 0;
        char* ptr = NULL;
        int rc = 0;
        long total = 0;
        size_t prev;
        //FILE* local = NULL;
        //fopen_s(&local,strLocaFilePath.c_str(), "rb");
        std::shared_ptr<FILE> fptr(fopen(strLocaFilePath.c_str(),"rb"),JGW_CloseFilePtr_C);
        if (!fptr) 
        {
             PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strLocaFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
            return false;
        }
        stat(strLocaFilePath.c_str(), &fileinfo);
        /* Send a file via scp. The mode parameter must only have permissions! */
        do {
            mpChannel = libssh2_scp_send(mpSession, strScpPath.c_str(), fileinfo.st_mode & 0777,
                (unsigned long)fileinfo.st_size);

            if ((!mpChannel) && (libssh2_session_last_errno(mpSession) !=
                LIBSSH2_ERROR_EAGAIN)) {
                    char *err_msg;
                    libssh2_session_last_error(mpSession, &err_msg, NULL, 0);
                    PELOG4WW_F(L"%s",JGW_A2W_A(err_msg).c_str());
                    //if (local) fclose(local);
                    return false;
            }
            //else
            //{
            //    PDLOG4WW(JGW_A2W_A("libssh2_scp_send() spin\n").c_str());
            //    LibSSH2_WaitSocket(mConnSocket, mpSession);
            //}
        } while (!mpChannel);

        do {
            nread = fread(mszSSHBuf, 1, SSH_BUFSIZE, fptr.get());
            if (nread <= 0) 
            {
                /* end of file */
                break;
            }
            ptr = mszSSHBuf;

            total += nread;

            prev = 0;
            do 
            {
                while ((rc = libssh2_channel_write(mpChannel, ptr, nread)) ==
                    LIBSSH2_ERROR_EAGAIN) 
                {
                    LibSSH2_WaitSocket(mConnSocket, mpSession);
                    prev = 0;
                }
                if (rc < 0) 
                {
                    PELOG4WW_F(L"ERROR %d total %ld / %d prev %d\n", rc,total, (int)nread, (int)prev);
                    //!fprintf(stderr, "ERROR %d total %ld / %d prev %d\n", rc,total, (int)nread, (int)prev);
                    break;
                }
                else 
                {
                    prev = nread;

                    /* rc indicates how many bytes were written this time */
                    nread -= rc;
                    ptr += rc;
                }
            } while (nread);
        } while (!nread); /* only continue if nread was drained */
        //do 
        //{
        //    nread = fread(mszSSHBuf, 1, SSH_BUFSIZE, fptr.get());
        //    if (nread <= 0) 
        //    {
        //        /* end of file */
        //        break;
        //    }
        //    ptr = mszSSHBuf;
        //    do 
        //    {
        //        /* write data in a loop until we block */
        //        rc = libssh2_channel_write(mpChannel, ptr, nread);
        //        if ((rc == LIBSSH2_ERROR_EAGAIN) && nread) 
        //        {
        //            /* this is due to blocking that would occur otherwise
        //            so we loop on this condition */
        //            //waitsocket(sock, session); /* now we wait */
        //            Sleep(1);
        //            continue;
        //        }
        //        else if(rc < 0)
        //        {
        //            break;
        //        }
        //        else
        //        {
        //            ptr += rc;
        //            nread -= rc;
        //        }
        //    } while (nread);
        //} while (rc > 0);

        //if (local) fclose(local);
        //fprintf(stderr, "Sending EOF\n");
        while (libssh2_channel_send_eof(mpChannel) == LIBSSH2_ERROR_EAGAIN);

        //fprintf(stderr, "Waiting for EOF\n");
        while (libssh2_channel_wait_eof(mpChannel) == LIBSSH2_ERROR_EAGAIN);

        //fprintf(stderr, "Waiting for channel to close\n");
        while (libssh2_channel_wait_closed(mpChannel) == LIBSSH2_ERROR_EAGAIN);

        libssh2_channel_free(mpChannel);
        mpChannel = NULL;

        return true;
    }

    bool CCJGW_SSHSCP::GetFileSCP(const std::string& strScpPath,const std::string& strSaveLocalFilePath)
    {
        std::shared_ptr<FILE> fptr(fopen(strSaveLocalFilePath.c_str(),"wb"),JGW_CloseFilePtr_C);
        /*FILE* local = NULL;
        fopen_s(&local,strSaveLocalFilePath.c_str(), "wb");*/
        if (!fptr) 
        {
             PELOG4WW_F(L"Can't open local file %s,error:%s",JGW_A2W(strSaveLocalFilePath).c_str(),JGW_A2W_A(strerror(errno)).c_str());
            return false;
        }

        libssh2_struct_stat fileinfo;
        do 
        {
            mpChannel = libssh2_scp_recv2(mpSession, strScpPath.c_str(), &fileinfo);
            if (!mpChannel) 
            {
                if(libssh2_session_last_errno(mpSession) != LIBSSH2_ERROR_EAGAIN) 
                {
                    char *err_msg;
                    libssh2_session_last_error(mpSession, &err_msg, NULL, 0);
                    PELOG4WW_F(L"%s",JGW_A2W_A(err_msg).c_str());
                    //! if (local) fclose(local);
                    return false;
                }
                else 
                {
                    //PDLOG4WW_F(stderr, "libssh2_scp_recv() spin\n");
                    PDLOG4WW(JGW_A2W_A("libssh2_scp_recv() spin\n").c_str());
                    LibSSH2_WaitSocket(mConnSocket, mpSession);
                }
            }
        } while (!mpChannel);

        libssh2_struct_stat_size got = 0;
        libssh2_struct_stat_size total = 0;
        while(got < fileinfo.st_size) 
        {
            int rc;

            do {
                int amount = RECV_SSH_BUFSIZE;
                if ((fileinfo.st_size - got) < amount) 
                {
                    amount = (int)(fileinfo.st_size - got);
                }
                /* loop until we block */
                rc = libssh2_channel_read(mpChannel, mszSSHBuf, amount);
                if (rc > 0) 
                {
                    fwrite(mszSSHBuf, rc,1,fptr.get());
                    ///write(1, mem, rc);
                    got += rc;
                    total += rc;
                }
            } while (rc > 0);

            if ((rc == LIBSSH2_ERROR_EAGAIN) && (got < fileinfo.st_size)) 
            {
                /* this is due to blocking that would occur otherwise
                so we loop on this condition */

                //! spin++;
                LibSSH2_WaitSocket(mConnSocket, mpSession); /* now we wait */
                continue;
            }
            break;
        }
        //int spin = 0;
        //libssh2_struct_stat_size got = 0;
        //libssh2_struct_stat_size total = 0;
        //bool result = true;

        //while(got < fileinfo.st_size) {
        //    char mem[1024*24] = {0};
        //    int rc;

        //    do {
        //        int amount=sizeof(mem);

        //        if ((fileinfo.st_size -got) < amount) {
        //            amount = (int)(fileinfo.st_size - got);
        //        }

        //        /* loop until we block */
        //        rc = libssh2_channel_read(mpChannel, mem, amount);
        //        if (rc > 0) {
        //            //write(1, mem, rc);
        //            fwrite(mem, rc,1,fptr.get());
        //            got += rc;
        //            total += rc;
        //        }
        //    } while (rc > 0);

        //    if ((rc == LIBSSH2_ERROR_EAGAIN) && (got < fileinfo.st_size)) {
        //        /* this is due to blocking that would occur otherwise
        //        so we loop on this condition */

        //        spin++;
        //        //waitsocket(sock, session); /* now we wait */
        //        Sleep(100);
        //        continue;
        //    }
        //    else if (rc < 0)
        //    {
        //        //result = false;
        //        break;
        //    }  
        //}
        //! if (local) fclose(local);
        libssh2_channel_free(mpChannel);
        mpChannel = NULL;
        return true;
    }
}
