#pragma once
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>

namespace JGW
{
    class CCJGW_SecureShellFTP : public CCJGW_AsyncSocket
    {
    public:
        CCJGW_SecureShellFTP(void);
        ~CCJGW_SecureShellFTP(void);
    };
}