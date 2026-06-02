#pragma once
#include <string>
#include "JGW_CryptPlugin_Define.h"

namespace JGW
{
    class JGW_CRYPTPLUGIN_CLASS CCJGW_CryptFileMD5
    {
    public:
        CCJGW_CryptFileMD5(void);
        ~CCJGW_CryptFileMD5(void);
    public:
        static std::wstring GetFileMD5(const std::wstring& strFilePath);
        static std::string GetFileMD5A(const std::string& strFilePath);
    };
}