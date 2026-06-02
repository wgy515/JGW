#include "StdAfx.h"
#include <string>
#include <JGW_CryptPlugin/CJGW_CryptDes.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#ifdef IS_ENCRYPT_MODULE
#define MAX_ENCRYPT_DATA_LEN 32

static const unsigned int gApplicationEncryptData[MAX_ENCRYPT_DATA_LEN] = {
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
};

static unsigned int gDecryptData[MAX_ENCRYPT_DATA_LEN] = {0};

extern "C" __declspec(dllexport) const unsigned int* x2x31313axaxazzadasdazz(const char* strManufacturer,int len)
{
    std::string strEncrypt;
    char szBuf[128] = {0};

    //! ´¦Àí¼ÓÃÜ×Ö·û´®
    for (int x = 0;x < len && x < 128;x ++)
    {
        char temp = strManufacturer[x] - x * 2;
        szBuf[x] ^= temp;
    }
    szBuf[len - 12] = '\0';
    strEncrypt = szBuf;
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN * sizeof(unsigned int));

    //! "pgeTP4/X757K45bcMscg8LviTNtTJkey"
    std::string strDecrypt = JGW::CCJGW_CryptDes::EncryptDESCode("JaGuarWave202009041703","5DA8B82F4D48","71DC1F03");
    //! BhxD/bK/CbXOYox78hwQ0l4txW+eMpGQgjXRRXcEQzY=
    if (0 == JGW::JGW_StrComparenoCaseStr(strDecrypt.c_str(),strEncrypt.c_str()))
    {
        for (int i = 0;i < MAX_ENCRYPT_DATA_LEN;i ++)
        {
            gDecryptData[i] ^= gApplicationEncryptData[i];
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
    JGW::CCJGW_CryptDes::EncryptDESCode("111111111111");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}

extern "C" __declspec(dllexport) const unsigned int* x2x3131ytyryaxaxazzadasdazz()
{
    JGW::CCJGW_CryptDes::EncryptDESCode("111111111111");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}


extern "C" __declspec(dllexport) const unsigned int* x2x3131ryryrjaxaxazzadasdazz()
{
    JGW::CCJGW_CryptDes::EncryptDESCode("111111111111");
    memset(gDecryptData,0x00,MAX_ENCRYPT_DATA_LEN);
    return gDecryptData;
}
#endif