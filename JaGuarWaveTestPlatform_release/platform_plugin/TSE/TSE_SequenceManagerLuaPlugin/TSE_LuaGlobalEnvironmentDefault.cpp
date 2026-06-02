#include "StdAfx.h"
#include "TSE_LuaGlobalEnvironmentDefault.h"
#include <sstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    double CTSE_LuaGlobalEnvironmentDefault::GetDouble(const std::wstring& itemName)
    {
        //! return _wtof(GetString(itemName).c_str());
        double dtemp = 0.00;
        std::wistringstream wstr(GetString(itemName));
        wstr >> dtemp;
        return dtemp;
    }

    std::wstring CTSE_LuaGlobalEnvironmentDefault::GetString(const std::wstring& itemName)
    {
        std::map<std::wstring,std::wstring>::iterator it = mmapKeyValue.find(itemName);
        if (mmapKeyValue.end() == it) return L"";
        return it->second;
    }

    int CTSE_LuaGlobalEnvironmentDefault::GetInt(const std::wstring& itemName)
    {
        /* return _wtoi(GetString(itemName).c_str());*/
        int temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    bool CTSE_LuaGlobalEnvironmentDefault::GetBool(const std::wstring& itemName)
    {
        return (1 == GetInt(itemName));
    }

    __int64 CTSE_LuaGlobalEnvironmentDefault::GetInt64(const std::wstring& itemName)
    {
        /*return _wtoi64(GetString(itemName).c_str());*/
        __int64 temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    float CTSE_LuaGlobalEnvironmentDefault::GetFloat(const std::wstring& itemName)
    {
        /*return static_cast<float>(GetDouble(itemName));*/
        float ftemp = 0.00f;
        std::wistringstream wstr(GetString(itemName));
        wstr >> ftemp;
        return ftemp;
    }

    std::string CTSE_LuaGlobalEnvironmentDefault::GetAnsiString(const std::wstring& itemName)
    {
        return JGW_W2A(GetString(itemName));
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutDouble(const std::wstring& itemName,double itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        mmapKeyValue[itemName] = itemValue;
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutInt(const std::wstring& itemName,int itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutInt64(const std::wstring& itemName,__int64 itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutFloat(const std::wstring& itemName,float itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutBool(const std::wstring& itemName,bool itemValue)
    {
        PutInt(itemName,itemValue?1:0);
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutAnsiString(const std::wstring& itemName,const std::string& itemValue)
    {
        PutString(itemName,JGW_A2W(itemValue));
    }

    void CTSE_LuaGlobalEnvironmentDefault::PutAnsiCopyDataMessageQueue(const std::string& strMessage)
    {
        if (!strMessage.empty())
        {
            mqueueCopyDataMessage.push(strMessage);
        }
    }

    std::string CTSE_LuaGlobalEnvironmentDefault::GetAnsiCopyDataMessageQueue()
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