#include "StdAfx.h"
#include <poco/Crypto/DigestEngine.h>
#include <fstream>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_CryptFileMD5::CCJGW_CryptFileMD5(void)
    {
    }


    CCJGW_CryptFileMD5::~CCJGW_CryptFileMD5(void)
    {
    }

    std::wstring CCJGW_CryptFileMD5::GetFileMD5(const std::wstring& strFilePath)
    {
        return JGW_A2W(GetFileMD5A(JGW_W2A(strFilePath)));
    }

    std::string CCJGW_CryptFileMD5::GetFileMD5A(const std::string& strFilePath)
    {
        std::string md5 = "00000000000000000000000000000000";
        try
        {
            Poco::Crypto::DigestEngine engine("MD5");
            std::ifstream fileStream;
            char szBuffer[1024];
            size_t read_size = 0;
            fileStream.imbue( std::locale("chs") );
            fileStream.open(strFilePath,std::ios_base::in | std::ios_base::binary );
            if (!fileStream.is_open()) return md5;

            while(!fileStream.eof())
            {
                fileStream.read(szBuffer,1024);
                read_size = static_cast<size_t>(fileStream.gcount());
                engine.update(szBuffer,read_size);
            }
            md5 = Poco::Crypto::DigestEngine::digestToHex(engine.digest());
        }
        catch (Poco::Exception&)
        {
        	
        }
        return md5;
    }
}