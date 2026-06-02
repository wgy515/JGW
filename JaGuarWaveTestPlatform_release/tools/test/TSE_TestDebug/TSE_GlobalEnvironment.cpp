#include "StdAfx.h"
#include "TSE_GlobalEnvironment.h"
#include <sstream>

namespace JGW
{
    double CTSE_GlobalEnvironment::GetDouble(const std::wstring& itemName)
    {
        //! return _wtof(GetString(itemName).c_str());
        double dtemp = 0.00;
        std::wistringstream wstr(GetString(itemName));
        wstr >> dtemp;
        return dtemp;
    }

    std::wstring CTSE_GlobalEnvironment::GetString(const std::wstring& itemName)
    {
        std::map<std::wstring,std::wstring>::iterator it = mmapKeyValue.find(itemName);
        if (mmapKeyValue.end() == it) return L"";
        return it->second;
    }

    int CTSE_GlobalEnvironment::GetInt(const std::wstring& itemName)
    {
       /* return _wtoi(GetString(itemName).c_str());*/
        int temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    __int64 CTSE_GlobalEnvironment::GetInt64(const std::wstring& itemName)
    {
        /*return _wtoi64(GetString(itemName).c_str());*/
        __int64 temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    float CTSE_GlobalEnvironment::GetFloat(const std::wstring& itemName)
    {
        /*return static_cast<float>(GetDouble(itemName));*/
        float ftemp = 0.00f;
        std::wistringstream wstr(GetString(itemName));
        wstr >> ftemp;
        return ftemp;
    }

    void CTSE_GlobalEnvironment::PutDouble(const std::wstring& itemName,double itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironment::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        mmapKeyValue[itemName] = itemValue;
    }

    void CTSE_GlobalEnvironment::PutInt(const std::wstring& itemName,int itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironment::PutInt64(const std::wstring& itemName,__int64 itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironment::PutFloat(const std::wstring& itemName,float itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }
}


