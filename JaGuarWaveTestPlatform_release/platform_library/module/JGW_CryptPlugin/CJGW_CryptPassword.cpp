#include "StdAfx.h"
#include <JGW_CryptPlugin/CJGW_CryptPassword.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>

namespace JGW
{
    CCJGW_CryptPassword::CCJGW_CryptPassword(void)
    {
    }


    CCJGW_CryptPassword::~CCJGW_CryptPassword(void)
    {
    }

    std::string CCJGW_CryptPassword::EncryptPassword(const std::string& strPassword,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        std::string encrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            encrypted = pCipher->encryptString(strPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            //PELOG4WW(L"√‹¬Îº”√‹ ß∞‹");
        } 
        return encrypted;
    }

    std::string CCJGW_CryptPassword::DecryptPassword(const std::string& strEncryptPassword,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        std::string decrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            decrypted = pCipher->decryptString(strEncryptPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            //PELOG4WW(L"√‹¬ÎΩ‚√‹ ß∞‹");
        } 
        return decrypted;
    }
}