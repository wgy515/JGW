#pragma once
#include <string>
#include "JGW_CryptPlugin_Define.h"
namespace JGW
{
    class JGW_CRYPTPLUGIN_CLASS CCJGW_CryptPassword
    {
    public:
        CCJGW_CryptPassword(void);
        ~CCJGW_CryptPassword(void);
    public:
        static std::string EncryptPassword(const std::string& strPassword,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
        static std::string DecryptPassword(const std::string& strEncryptPassword,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
    };
}