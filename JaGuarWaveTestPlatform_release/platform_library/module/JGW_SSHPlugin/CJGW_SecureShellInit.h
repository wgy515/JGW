#pragma once

namespace JGW
{
    class CCJGW_SecureShellInit
    {
    public:
        CCJGW_SecureShellInit(void);
        ~CCJGW_SecureShellInit(void);
    public:
        bool InitSecureShellBase();
    private:
        int mInitResult;
    };
}