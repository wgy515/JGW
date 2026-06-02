#include "StdAfx.h"
#include "CSTSE_CryptoEmpty.h"

namespace JGW
{
    CCSTSE_CryptoEmpty::CCSTSE_CryptoEmpty(void)
    {
    }


    CCSTSE_CryptoEmpty::~CCSTSE_CryptoEmpty(void)
    {
    }

    CCSTSE_CryptoEmpty* CCSTSE_CryptoEmpty::GetCryptoEmpty()
    {
        static CCSTSE_CryptoEmpty cryptoEmpty;
        return &cryptoEmpty;
    }


    std::string CCSTSE_CryptoEmpty::Encryption(const std::string& strEncrypt)
    {
        return strEncrypt;
    }

    std::string CCSTSE_CryptoEmpty::Decryption(const std::string& strDecrypt)
    {
        return strDecrypt;
    }
}

