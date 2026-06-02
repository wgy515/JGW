// JGW_EncryptPassword.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>
#include <fstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <sstream>
//! 1F2C7BC7612391378BEF2F8D36592C8A

//void EncryptString(const std::string& strInput,std::string& strOutPut)
//{
//    try
//    {
//        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
//        // Creates a 256-bit AES cipher
//        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC","JaGuarWave","JaGuarWave"));
//        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
//        
//        //std::wstringstream
//        //std::istringstream strBuf;
//        //strInput >> strBuf;
//        //strBuf.str() = strInput;
//        std::ostringstream strOutPutBuf(std::ios_base::binary);
//        std::ifstream ifFile;
//        ifFile.imbue(std::locale(""));
//        ifFile.open(strInput,std::ios_base::binary | std::ios_base::in);
//
//        std::ofstream ofFile;
//        ofFile.imbue(std::locale(""));
//        ofFile.open("E:\\puma\\123.bin",std::ios_base::binary | std::ios_base::out);
//
//        //if (!ifFile.is_open() || !ofFile.is_open())
//        //{
//        //    return ;
//        //}
//        pCipher->decrypt(ifFile,strOutPutBuf,Poco::Crypto::Cipher::ENC_BASE64);
//        size_t count = strOutPutBuf.str().size();
//        char* buf = (char*)calloc(count,1);
//        for (size_t i = 0;i < count;i ++)
//        {
//            buf[i] = strOutPutBuf.str()[i];
//            ofFile << buf[i];
//        }
//        free(buf);
//        strOutPut = strOutPutBuf.str();
//    }
//    catch (...)
//    {
//        //std::cout << "Fail Encrypt File:" << strInputFilePath << std::endl;
//    }
//}

void EncryptFile(const std::string& strInputFilePath,const std::string& strOutPutFilePath)
{
    try
    {
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC","JaGuarWave","JaGuarWave"));
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

        std::ifstream ifFile;
        ifFile.imbue(std::locale(""));
        ifFile.open(strInputFilePath,std::ios_base::binary | std::ios_base::in);

        std::ofstream ofFile;
        ofFile.imbue(std::locale(""));
        ofFile.open(strOutPutFilePath,std::ios_base::binary | std::ios_base::out);

        if (!ifFile.is_open() || !ofFile.is_open())
        {
            return ;
        }
        pCipher->encrypt(ifFile,ofFile,Poco::Crypto::Cipher::ENC_BASE64);
    }
    catch (...)
    {
        std::cout << "Fail Encrypt File:" << strInputFilePath << std::endl;
    }
}

void DecryptFile(const std::string& strInputFilePath,const std::string& strOutPutFilePath)
{
    try
    {
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC","JaGuarWave","JaGuarWave"));
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

        std::ifstream ifFile;
        ifFile.imbue(std::locale(""));
        ifFile.open(strInputFilePath,std::ios_base::binary | std::ios_base::in);

        std::ofstream ofFile;
        ofFile.imbue(std::locale(""));
        ofFile.open(strOutPutFilePath,std::ios_base::binary | std::ios_base::out);
        if (!ifFile.is_open() || !ofFile.is_open())
        {
            return ;
        }

        pCipher->decrypt(ifFile,ofFile,Poco::Crypto::Cipher::ENC_BASE64);
        return /*true*/;
    }
    catch (...)
    {
        std::cout << "Fail Decrypt File:" << strInputFilePath << std::endl;
    }
}

typedef struct
{
    std::string mstrInputFolder;
    std::string mstrOutputFolder;
}S_SCANFILE_PARAM,*PS_SCANFILE_PARAM;

//void ScanPuamThroughputTestResultFile(const wchar_t* filepath,const wchar_t* filename,void* lpData)
//{
//    std::map<std::wstring,std::wstring>* mpMapFileMD5 = (std::map<std::wstring, std::wstring>*)lpData;
//
//    //if (NULL != JGW_WStrCaseWStr(gstrExec.c_str(),filepath) 
//    //    || NULL != JGW_WStrCaseWStr(gstrHelp.c_str(),filepath) 
//    //    || NULL != JGW_WStrCaseWStr(gstrDatabases.c_str(),filepath) 
//    //    || NULL != JGW_WStrCaseWStr(gstrJaGuarWaveTestPlatformInstall.c_str(),filepath))
//    //{
//    std::wstring strTmep (filepath);
//    strTmep += L"/";
//    strTmep += filename;
//    std::wstring strMD5 = JGW::CCJGW_CryptFileMD5::GetFileMD5(strTmep);
//    JGW_ReplaceStringW(strTmep,gstrFolder + L"/",L"");
//
//    mpMapFileMD5->insert(std::make_pair<std::wstring,std::wstring>(strTmep,strMD5));
//    //}
//    //! NULL != strstr(filename,"result") &&
//    /*if (NULL != strstr(filename,"P_"))
//    {
//    strFilePathTemp = filepath;
//    strFilePathTemp += "\\";
//    strFilePathTemp += filename;
//    gvQSEQHtmlFile.push_back(strFilePathTemp);
//    }*/
//}

void ScanEncryptFile(const char* filepath,const char* filename,void* lpData)
{
    PS_SCANFILE_PARAM psScanFileParam = (PS_SCANFILE_PARAM)lpData;
    std::string strInputFilePath,strOutputFilePath(psScanFileParam->mstrOutputFolder),strTemp;
    JGW::JGW_FormatString(strInputFilePath,"%s\\%s",filepath,filename);
    strTemp = strInputFilePath;
    if (0 != _strnicmp(strTemp.c_str(),psScanFileParam->mstrInputFolder.c_str(),psScanFileParam->mstrInputFolder.length()))
    {
        std::cout << "Fail Encrypt File:" << strInputFilePath << std::endl;
        return;
    }
    strOutputFilePath += "\\";
    strOutputFilePath += strTemp.erase(0,psScanFileParam->mstrInputFolder.length());
    JGW::JGW_CreateDirectoryA(strOutputFilePath.c_str(),false);
#if 1
    EncryptFile(strInputFilePath,strOutputFilePath);
#else
    DecryptFile(strInputFilePath,strOutputFilePath);
#endif
}

int main(int argc, char* argv[])
{
    //std::string strBuf;
    //EncryptString("E:\\puma\\persistent_database_0_otp.bin1",strBuf);
#if 1
    if (2 == argc)
    {
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::CipherKey key("AES-128-ECB","JaGuarWave","JaGuarWave");
        Poco::Crypto::Cipher* pCipher = factory.createCipher(key/*Poco::Crypto::CipherKey(std::string("aes-256"))*/);
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

        std::string plainText = argv[1];
        std::cout << pCipher->encryptString(plainText, Poco::Crypto::Cipher::ENC_BASE64) << std::endl;
    }
    else if (argc == 3)
    {
        std::string strInputFilePath = argv[1];
        std::string strOutPutFilePath = argv[2];
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("DES-EDE3-CBC","JaGuarWave","JaGuarWave"));
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

        std::ifstream ifFile;
        ifFile.imbue(std::locale(""));
        ifFile.open(strInputFilePath,std::ios_base::binary | std::ios_base::in);

        std::ofstream ofFile;
        ofFile.imbue(std::locale(""));
        ofFile.open(strOutPutFilePath,std::ios_base::binary | std::ios_base::out);

        if (!ifFile.is_open() || !ofFile.is_open())
        {
            return false;
        }

        pCipher->encrypt(ifFile,ofFile,Poco::Crypto::Cipher::ENC_BASE64);
    }
    else if (argc == 4)
    {
        std::string strInputFolder = argv[2];
        std::string strOutputFolder = argv[3];
        int numFiles = 0;
        JGW::JGW_EraseLastAndFristTwoChars(strInputFolder,'\\','/');
        JGW::JGW_ReplaceStringA(strInputFolder,"/","\\");
        JGW::JGW_ReplaceStringA(strOutputFolder,"/","\\");
        S_SCANFILE_PARAM sScanFileParam = {strInputFolder,strOutputFolder};

        std::cout << "Start Encrypt File ..." << std::endl;
        JGW::JGW_TraverseFolder_C(strInputFolder,numFiles,"\\*.*",ScanEncryptFile,&sScanFileParam);
        std::cout << "End Encrypt File" << std::endl;
    }
#else
     std::string strEncryptPassword = "fBL/Bgdpgw4MI9v59csOow==", passphrase = "JaGuarWave", salt = "JaGuarWave";
    std::string decrypted= "";
    try
    {
        Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
        // Creates a 256-bit AES cipher
        Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB",passphrase,salt));
        Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));

        strEncryptPassword = argv[1];
        decrypted = pCipher->decryptString(strEncryptPassword, Poco::Crypto::Cipher::ENC_BASE64);
        std::cout << decrypted << std::endl;
    }
    catch (...)
    {
        //PELOG4WW(L"密码解密失败");
    } 
   // return decrypted;
#endif
	return 0;
}

