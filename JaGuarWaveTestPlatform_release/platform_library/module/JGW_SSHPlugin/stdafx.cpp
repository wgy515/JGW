// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_SSHPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
const char *keyfile_ssh_shell1 = "~/.ssh/id_rsa.pub";
const char *keyfile_ssh_shell2 = "~/.ssh/id_rsa";
const char *password_ssh_shell = "password";

namespace JGW
{
    int LibSSH2_WaitSocket(int socket_fd, LIBSSH2_SESSION *session)
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
