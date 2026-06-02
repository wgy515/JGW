#pragma once
#include <string>
#include "JGW_CryptPlugin_Define.h"

namespace JGW
{
    class JGW_CRYPTPLUGIN_CLASS CCJGW_CryptDes
    {
    public:
        CCJGW_CryptDes();
        ~CCJGW_CryptDes();
    public:
        static std::string EncryptDESCode(const std::string& strCode,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
        static std::string DecryptDESCode(const std::string& strEncrypt,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
    public:
        static bool EncryptDESFile(const std::string& strInputFile,const std::string& strOutputFile,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
        static bool DecryptDESFile(const std::string& strInputFile,const std::string& strOutputFile,const std::string& passphrase = "JaGuarWave",const std::string& salt = "JaGuarWave");
    };

    /*extern "C" 
    {
    JGW_CRYPTPLUGIN_API 

    }*/
}

