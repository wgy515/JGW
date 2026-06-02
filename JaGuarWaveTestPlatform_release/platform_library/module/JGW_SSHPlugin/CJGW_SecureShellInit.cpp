#include "StdAfx.h"
#include "CJGW_SecureShellInit.h"
#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>

namespace JGW
{
    CCJGW_SecureShellInit::CCJGW_SecureShellInit(void)
    {
        InitSecureShellBase();
    }


    CCJGW_SecureShellInit::~CCJGW_SecureShellInit(void)
    {
        libssh2_exit();
        mInitResult = 0;
    }

    bool CCJGW_SecureShellInit::InitSecureShellBase()
    {
        if (0 == mInitResult) return true;
        return (0 == (mInitResult = libssh2_init(0)));
    }
}