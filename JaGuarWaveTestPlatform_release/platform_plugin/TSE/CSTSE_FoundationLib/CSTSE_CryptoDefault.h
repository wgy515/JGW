#pragma once
#include <CSTSE_FoundationLib/CSTSE_Crypto.h>

namespace JGW
{
    class CCSTSE_CryptoDefault : public CCSTSE_CryptoImp
    {
    public:
        static CCSTSE_CryptoDefault* GetCryptoDefault();
    private:
        CCSTSE_CryptoDefault(void);
        ~CCSTSE_CryptoDefault(void);
    private:
        //! ¼ÓÃÜ×Ö·û´®
        std::string Encryption(const std::string& strEncrypt);
        //! ½âÃÜ×Ö·û´®
        std::string Decryption(const std::string& strDecrypt);
    };
}


