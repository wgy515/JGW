#pragma once

namespace JGW
{
    class CCJGW_SecureShellBase
    {
    public:
        CCJGW_SecureShellBase(void);
        ~CCJGW_SecureShellBase(void);
    public:
        bool InitSecureShellBase();
    private:
        int mInitResult;
    };
}