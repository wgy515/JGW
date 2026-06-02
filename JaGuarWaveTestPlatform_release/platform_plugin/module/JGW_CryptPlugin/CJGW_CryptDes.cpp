#include "StdAfx.h"
#include <JGW_CryptPlugin/CJGW_CryptDes.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>
#include <fstream>

namespace JGW
{
    CCJGW_CryptDes::CCJGW_CryptDes(void)
    {
    }


    CCJGW_CryptDes::~CCJGW_CryptDes(void)
    {
    }

    std::string CCJGW_CryptDes::EncryptDESCode(const std::string& strPassword,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        std::string encrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            encrypted = pCipher->encryptString(strPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            //PELOG4WW(L"√‹¬Îº”√‹ ß∞‹");
        } 
        return encrypted;
    }

    std::string CCJGW_CryptDes::DecryptDESCode(const std::string& strEncryptPassword,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        std::string decrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            decrypted = pCipher->decryptString(strEncryptPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            //PELOG4WW(L"√‹¬ÎΩ‚√‹ ß∞‹");
        } 
        return decrypted;
    }

    bool CCJGW_CryptDes::EncryptDESFile(const std::string& strInputFile,const std::string& strOutputFile,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        //unsigned char buf[] = {0xf1,0x0,0xfe,0x00};
        //std::string str((char*)buf,4);
        ////size_t str_size = str.size();
        ////cout << "str_size: " << str_size << endl;
        ////cout << (int)str[1] << endl;
        //std::istringstream iss(str,std::ios_base::in);
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
           
            std::ifstream ifFile;
            ifFile.imbue(std::locale("chs"));
            ifFile.open(strInputFile,std::ios_base::binary | std::ios_base::in);

            std::ofstream ofFile;
            ofFile.imbue(std::locale("chs"));
            ofFile.open(strOutputFile,std::ios_base::binary | std::ios_base::out);

            if (!ifFile.is_open() || !ofFile.is_open())
            {
                return false;
            }

            pCipher->encrypt(ifFile,ofFile,Poco::Crypto::Cipher::ENC_BASE64);

            return true;
        }
        catch (...)
        {
        }
        return false;
    }

    bool CCJGW_CryptDes::DecryptDESFile(const std::string& strInputFile,const std::string& strOutputFile,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
    {
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC",passphrase,salt));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

            std::ifstream ifFile;
            ifFile.imbue(std::locale("chs"));
            ifFile.open(strInputFile,std::ios_base::binary | std::ios_base::in);

            std::ofstream ofFile;
            ofFile.imbue(std::locale("chs"));
            ofFile.open(strOutputFile,std::ios_base::binary | std::ios_base::out);
            if (!ifFile.is_open() || !ofFile.is_open())
            {
                return false;
            }

            pCipher->decrypt(ifFile,ofFile,Poco::Crypto::Cipher::ENC_BASE64);
            return true;
        }
        catch (...)
        {
        }
        return false;
    }
}

