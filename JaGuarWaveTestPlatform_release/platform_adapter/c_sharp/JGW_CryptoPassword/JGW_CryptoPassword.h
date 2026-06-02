#pragma once
#include <string>

#ifdef __cplusplus
extern "C" 
{
#endif
    // º”√‹√‹¬Î
    __declspec(dllexport) const char* JGW_EncryptPassword(const char* password,const char* passphrase,const char* salt);
    // Ω‚√‹√‹¬Î
    __declspec(dllexport) const char* JGW_DecryptPassword(const char* encryptpassword,const char* passphrase,const char* salt);

    __declspec(dllexport) const char* JGW_GetFileMD5(const char* strFilePath);

    __declspec(dllexport) int JGW_GetDecryptPassword(const char* encryptpassword,char* password,int password_size,const char* passphrase,const char* salt);
#ifdef __cplusplus
};
#endif