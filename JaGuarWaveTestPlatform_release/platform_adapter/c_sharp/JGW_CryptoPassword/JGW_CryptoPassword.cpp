// JGW_CryptoPassword.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <poco/Crypto/DigestEngine.h>
#include <Poco/Crypto/RSAKey.h>
#include "JGW_CryptoPassword.h"
#include <fstream>

static std::string g_password = "";
static std::string g_md5 = "";

std::string EncryptPassword(const std::string& strPassword,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave")
{
    std::string encrypted= "";
    try
    {
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB",passphrase.c_str(),salt.c_str()));
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
        encrypted = pCipher->encryptString(strPassword, Poco::Crypto::Cipher::ENC_BASE64);
    }
    catch (...)
    {
        //PELOG4WW(L"密码加密失败");
    } 
    return encrypted;
}

std::string DecryptPassword(const std::string& strEncryptPassword,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave")
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
        //PELOG4WW(L"密码解密失败");
    } 
    return decrypted;
}

const char* JGW_GetFileMD5(const char* strFilePath)
{
    g_md5 = "00000000000000000000000000000000";
    try
    {
        Poco::Crypto::DigestEngine engine("MD5");
        std::ifstream fileStream;
        char szBuffer[1024];
        size_t read_size = 0;
        fileStream.imbue( std::locale("") );
        fileStream.open(strFilePath,std::ios_base::in | std::ios_base::binary );
        if (!fileStream.is_open()) return g_md5.c_str();

        while(!fileStream.eof())
        {
            fileStream.read(szBuffer,1024);
            read_size = static_cast<size_t>(fileStream.gcount());
            engine.update(szBuffer,read_size);
        }
        g_md5 = Poco::Crypto::DigestEngine::digestToHex(engine.digest());
    }
    catch (Poco::Exception&)
    {

    }
    return g_md5.c_str();
}

const char* JGW_EncryptPassword(const char* password,const char* passphrase,const char* salt)
{
    g_password = EncryptPassword(password,passphrase,salt);
    return g_password.c_str();
}

const char* JGW_DecryptPassword(const char* encryptpassword,const char* passphrase,const char* salt)
{
    g_password = DecryptPassword(encryptpassword,passphrase,salt);
    return g_password.c_str();
}

int JGW_GetDecryptPassword(const char* encryptpassword,char* password,int password_size,const char* passphrase,const char* salt)
{
    std::string pass = DecryptPassword(encryptpassword,passphrase,salt);
    strcpy_s(password,password_size,pass.c_str());
    return pass.length();
}