#include "StdAfx.h"
#include "CJGW_Property.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <fstream>

namespace JGW
{
    CCJGW_Property::CCJGW_Property(const std::wstring& strPropertyFilleConfigName,std::unordered_map<std::wstring, std::wstring>& mapPropertyConfig) : mstrPropertyFilleConfigName(strPropertyFilleConfigName),mmapPropertyConfig(mapPropertyConfig),mstrPropertyAppName(L"propety")
    {
        mstrPropertyFilleConfigPath = JGW_RealativePathToAbsPath(strPropertyFilleConfigName.c_str());
        LoadPropertyFilleConfig();
    }

    CCJGW_Property::~CCJGW_Property()
    {

    }

    void CCJGW_Property::ReLoadPropertyFilleConfig()
    {
        mmapPropertyConfig.clear();
        LoadPropertyFilleConfig();
    }

    void CCJGW_Property::LoadPropertyFilleConfig()
    {
        std::wifstream inputFile;
        inputFile.imbue( std::locale("") );
        inputFile.open(mstrPropertyFilleConfigPath);
        if (!inputFile.is_open()) return;

        wchar_t str[260];  
        std::wstring strKey,strValue;
        int temp = 0;
        while (inputFile.getline(str,260))
        {
            temp = 0;
            strKey = L"";
            strValue = L"";
            if (NULL == wcsstr(str,L"=")) continue;
            while (str[temp] == L' ' && str[temp] != L'\0')  temp++;
            if (L';'== str[temp] || L'#' == str[temp]) continue;
            while (str[temp] != L'=') { strKey += str[temp]; temp++;}
            temp += 1;
            //! È¥µô¿Õ¸ñ
            while (L' ' == str[temp] && str[temp] != L'\0') temp++;
            //! 
            while (str[temp] != L'\0') { strValue += str[temp]; temp++;}
            
            while (!strKey.empty() && L' ' == strKey.at(strKey.length() - 1))
            {
                strKey.erase(strKey.end() - 1);
            }
            while (!strValue.empty() && L' ' == strValue.at(strValue.length() - 1))
            {
                strValue.erase(strValue.end() - 1);
            }
            mmapPropertyConfig[strKey] = strValue;
        }
        inputFile.close();
    }

    void CCJGW_Property::SetPropertyAppName(const std::wstring& strPropertyAppName /* = L"propety" */)
    {
        mstrPropertyAppName = strPropertyAppName;
    }

    bool CCJGW_Property::WritePropertyValueToKey(const std::wstring& strKey,const std::wstring& strValue)
    {
        return WritePrivateProfileString(mstrPropertyAppName.c_str(),strKey.c_str(),strValue.c_str(),mstrPropertyFilleConfigPath.c_str())>0?true:false;
    }
}