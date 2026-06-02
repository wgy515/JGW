#pragma once
#include <string>

namespace JGW
{
    class CCSTSE_CryptoImp
    {
    public:
        //! ¼ÓÃÜ×Ö·û´®
        virtual std::string Encryption(const std::string& strEncrypt) = 0;
        //! ½âÃÜ×Ö·û´®
        virtual std::string Decryption(const std::string& strDecrypt) = 0;
    };
}