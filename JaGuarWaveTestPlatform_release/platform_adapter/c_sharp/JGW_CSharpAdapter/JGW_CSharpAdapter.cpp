// JGW_CSharpAdapter.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>

static unsigned int gEncryptData[] = {
    0x00, 0x00, 0x00, 0x00,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
};


void x32123458(const wchar_t* moduleName)
{
    if (0 == JGW::JGW_WStrComparenoCaseWStr(moduleName,L"{6A287167-B986-480A-BFB2-0653BC2AECFC}"))
    {
        gEncryptData[0] = 0x60303050U;
        gEncryptData[1] = 0x02010103U;
        gEncryptData[2] = 0xce6767a9U;
        gEncryptData[3] = 0x562b2b7dU;
    }
}


#ifdef IS_ENCRYPT_MODULE
#define MAX_ENCRYPT_DATA_LEN 32
static unsigned int gDecryptData[MAX_ENCRYPT_DATA_LEN] = {0};

std::string EncryptDESCode(const std::string& strPassword,const std::string& passphrase /* = "JaGuarWave" */,const std::string& salt /* = "JaGuarWave" */)
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
        //PELOG4WW(L"密码加密失败");
    } 
    return encrypted;
}

extern "C" __declspec(dllexport) const unsigned int* x2x31313axaxazzadasdazz(const char* strManufacturer,int len)
{
    std::string strEncrypt;
    char szBuf[128] = {0};

    //! 处理加密字符串
    for (int x = 0;x < len && x < 128;x ++)
    {
        char temp = strManufacturer[x] - x * 2;
        szBuf[x] ^= temp;
    }
    szBuf[len - 12] = '\0';
    strEncrypt = szBuf;
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN * sizeof(unsigned int));

    //! "pgeTP4/X757K45bcMscg8LviTNtTJkey"
    std::string strDecrypt = EncryptDESCode("JaGuarWave202009041703","5DA8B82F4D48","71DC1F03");
    //! BhxD/bK/CbXOYox78hwQ0l4txW+eMpGQgjXRRXcEQzY=
    if (0 == JGW::JGW_StrComparenoCaseStr(strDecrypt.c_str(),strEncrypt.c_str()))
    {
        for (int i = 0;i < MAX_ENCRYPT_DATA_LEN;i ++)
        {
            gDecryptData[i] ^= gEncryptData[i];
            gDecryptData[i] += i;
        }
    }
    else
    {
        return gDecryptData;
    }
    return gDecryptData;
}

extern "C" __declspec(dllexport) const unsigned int* x2x31313acacazzadasdazz()
{
   EncryptDESCode("111111111111","JaGuarWave","JaGuarWave");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}

extern "C" __declspec(dllexport) const unsigned int* x2x3131ytyryaxaxazzadasdazz()
{
    EncryptDESCode("111111111111","JaGuarWave","JaGuarWave");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}


extern "C" __declspec(dllexport) const unsigned int* x2x3131ryryrjaxaxazzadasdazz()
{
    EncryptDESCode("111111111111","JaGuarWave","JaGuarWave");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}
#endif