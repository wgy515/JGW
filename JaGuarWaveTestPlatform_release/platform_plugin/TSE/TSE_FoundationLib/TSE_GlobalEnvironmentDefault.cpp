#include "StdAfx.h"
#include "TSE_GlobalEnvironmentDefault.h"
#include <sstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    double CTSE_GlobalEnvironmentDefault::GetDouble(const std::wstring& itemName)
    {
        //! return _wtof(GetString(itemName).c_str());
        double dtemp = 0.00;
        std::wistringstream wstr(GetString(itemName));
        wstr >> dtemp;
        return dtemp;
    }

    std::wstring CTSE_GlobalEnvironmentDefault::GetString(const std::wstring& itemName)
    {
        std::map<std::wstring,std::wstring>::iterator it = mmapKeyValue.find(itemName);
        if (mmapKeyValue.end() == it) return L"";
        return it->second;
    }

    int CTSE_GlobalEnvironmentDefault::GetInt(const std::wstring& itemName)
    {
        /* return _wtoi(GetString(itemName).c_str());*/
        int temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    bool CTSE_GlobalEnvironmentDefault::GetBool(const std::wstring& itemName)
    {
        return (1 == GetInt(itemName));
    }

    __int64 CTSE_GlobalEnvironmentDefault::GetInt64(const std::wstring& itemName)
    {
        /*return _wtoi64(GetString(itemName).c_str());*/
        __int64 temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    float CTSE_GlobalEnvironmentDefault::GetFloat(const std::wstring& itemName)
    {
        /*return static_cast<float>(GetDouble(itemName));*/
        float ftemp = 0.00f;
        std::wistringstream wstr(GetString(itemName));
        wstr >> ftemp;
        return ftemp;
    }

    std::string CTSE_GlobalEnvironmentDefault::GetAnsiString(const std::wstring& itemName)
    {
        return JGW_W2A(GetString(itemName));
    }

    void CTSE_GlobalEnvironmentDefault::PutDouble(const std::wstring& itemName,double itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironmentDefault::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        mmapKeyValue[itemName] = itemValue;
    }

    void CTSE_GlobalEnvironmentDefault::PutInt(const std::wstring& itemName,int itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironmentDefault::PutInt64(const std::wstring& itemName,__int64 itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironmentDefault::PutFloat(const std::wstring& itemName,float itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironmentDefault::PutBool(const std::wstring& itemName,bool itemValue)
    {
        PutInt(itemName,itemValue?1:0);
    }

    void CTSE_GlobalEnvironmentDefault::PutAnsiString(const std::wstring& itemName,const std::string& itemValue)
    {
        PutString(itemName,JGW_A2W(itemValue));
    }

    void CTSE_GlobalEnvironmentDefault::PutAnsiCopyDataMessageQueue(const std::string& strMessage)
    {
        if (!strMessage.empty())
        {
            mqueueCopyDataMessage.push(strMessage);
        }
    }

    std::string CTSE_GlobalEnvironmentDefault::GetAnsiCopyDataMessageQueue()
    {
        if (!mqueueCopyDataMessage.empty())
        {
            std::string strMessage = mqueueCopyDataMessage.front();
            mqueueCopyDataMessage.pop();
            return strMessage;
        }
        return "";
    }
}