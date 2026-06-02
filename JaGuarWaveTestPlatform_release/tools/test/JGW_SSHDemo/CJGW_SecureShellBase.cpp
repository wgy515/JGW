#include "StdAfx.h"
#include "CJGW_SecureShellBase.h"
#include <libssh2/libssh2_config.h>
#include <libssh2/libssh2.h>

namespace JGW
{
    CCJGW_SecureShellBase::CCJGW_SecureShellBase(void) : mInitResult(-1)
    {
        InitSecureShellBase();
    }


    CCJGW_SecureShellBase::~CCJGW_SecureShellBase(void)
    {
        libssh2_exit();
        mInitResult = 0;
    }

    bool CCJGW_SecureShellBase::InitSecureShellBase()
    {
        if (0 == mInitResult) return true;
        return (0 == (mInitResult = libssh2_init(0)));
    }
}