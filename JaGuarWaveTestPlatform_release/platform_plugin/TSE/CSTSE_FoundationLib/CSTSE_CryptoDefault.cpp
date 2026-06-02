#include "StdAfx.h"
#include "CSTSE_CryptoDefault.h"
#include <JGW_CryptPlugin/CJGW_CryptDes.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCSTSE_CryptoDefault::CCSTSE_CryptoDefault(void)
    {
    }


    CCSTSE_CryptoDefault::~CCSTSE_CryptoDefault(void)
    {
    }

    CCSTSE_CryptoDefault* CCSTSE_CryptoDefault::GetCryptoDefault()
    {
        static CCSTSE_CryptoDefault cryptoDefault;
        return &cryptoDefault;
    }


    std::string CCSTSE_CryptoDefault::Encryption(const std::string& strEncrypt)
    {
        std::string strEncryptTemp (strEncrypt);
        JGW_ReplaceStringA(strEncryptTemp,"\\\\","\\");
        JGW_ReplaceStringA(strEncryptTemp,"\\","\\\\");
        std::string strValue = CCJGW_CryptDes::EncryptDESCode(strEncryptTemp,"JaGuarWave_","JaGuarWave_");
        JGW_ReplaceStringA(strValue,"\r","");
        JGW_ReplaceStringA(strValue,"\n","");
        return strValue;
    }

    std::string CCSTSE_CryptoDefault::Decryption(const std::string& strDecrypt)
    {
         return CCJGW_CryptDes::DecryptDESCode(strDecrypt,"JaGuarWave_","JaGuarWave_");
    }
}

