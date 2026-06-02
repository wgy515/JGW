#include "StdAfx.h"
#include "CJGW_PropertyMemoryConfig.h"
#include <sstream>

namespace JGW
{
    CCJGW_PropertyMemoryConfig::CCJGW_PropertyMemoryConfig()
    {

    }

    CCJGW_PropertyMemoryConfig::~CCJGW_PropertyMemoryConfig()
    {

    }

    double CCJGW_PropertyMemoryConfig::GetDouble(const std::wstring& itemName)
    {
        //! return _wtof(GetString(itemName).c_str());
        double dtemp = 0.00;
        std::wistringstream wstr(GetString(itemName));
        wstr >> dtemp;
        return dtemp;
    }

    std::wstring CCJGW_PropertyMemoryConfig::GetString(const std::wstring& itemName)
    {
        readLock lockWrite(mrwMutex);
        std::unordered_map<std::wstring,std::wstring>::iterator it = mmapPropertyConfig.find(itemName);
        if (mmapPropertyConfig.end() == it) return L"";
        return it->second;
    }

    int CCJGW_PropertyMemoryConfig::GetInt(const std::wstring& itemName)
    {
        /* return _wtoi(GetString(itemName).c_str());*/
        int temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    bool CCJGW_PropertyMemoryConfig::GetBool(const std::wstring& itemName)
    {
        return (1 == GetInt(itemName));
    }

    __int64 CCJGW_PropertyMemoryConfig::GetInt64(const std::wstring& itemName)
    {
        /*return _wtoi64(GetString(itemName).c_str());*/
        __int64 temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    float CCJGW_PropertyMemoryConfig::GetFloat(const std::wstring& itemName)
    {
        /*return static_cast<float>(GetDouble(itemName));*/
        float ftemp = 0.00f;
        std::wistringstream wstr(GetString(itemName));
        wstr >> ftemp;
        return ftemp;
    }

    void CCJGW_PropertyMemoryConfig::PutDouble(const std::wstring& itemName,double itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CCJGW_PropertyMemoryConfig::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        writeLock lockWrite(mrwMutex); 
        mmapPropertyConfig[itemName] = itemValue;
    }

    void CCJGW_PropertyMemoryConfig::PutInt(const std::wstring& itemName,int itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CCJGW_PropertyMemoryConfig::PutInt64(const std::wstring& itemName,__int64 itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CCJGW_PropertyMemoryConfig::PutFloat(const std::wstring& itemName,float itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CCJGW_PropertyMemoryConfig::PutBool(const std::wstring& itemName,bool itemValue)
    {
        PutInt(itemName,itemValue?1:0);
    }

    void CCJGW_PropertyMemoryConfig::CopyProperty(std::unordered_map<std::wstring, std::wstring>& mapProperty)
    {
        for (std::unordered_map<std::wstring, std::wstring>::iterator it = mmapPropertyConfig.begin();
            it != mmapPropertyConfig.end();
            ++ it)
        {
            mapProperty[it->first] = it->second;
        }
    }

    const std::unordered_map<std::wstring,std::wstring>::iterator CCJGW_PropertyMemoryConfig::Begin()
    {
        return mmapPropertyConfig.begin();
    }

    const std::unordered_map<std::wstring,std::wstring>::iterator CCJGW_PropertyMemoryConfig::End()
    {
        return mmapPropertyConfig.end();
    }

    void CCJGW_PropertyMemoryConfig::SetPropertyAppName(const std::wstring& strPropertyAppName/* = L"propety"*/)
    {
        /*SetPropertyAppName(strPropertyAppName);*/
    }

    void CCJGW_PropertyMemoryConfig::ReloadPropertyFile()
    {

    }
}