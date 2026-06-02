#pragma once
#include <CSTSE_FoundationLib/CSTSE_Crypto.h>
namespace JGW
{
    class CCSTSE_CryptoEmpty : public CCSTSE_CryptoImp
    {
    public:
        static CCSTSE_CryptoEmpty* GetCryptoEmpty();
    private:
        CCSTSE_CryptoEmpty(void);
        ~CCSTSE_CryptoEmpty(void);
    private:
        //! ¼ÓÃÜ×Ö·û´®
        std::string Encryption(const std::string& strEncrypt);
        //! ½âÃÜ×Ö·û´®
        std::string Decryption(const std::string& strDecrypt);
    };
}


