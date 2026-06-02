#pragma once
#include <TSE_SequenceManagerPlugin/TSE_GlobalEnvironmentImpl.h>
#include <map>
#include <queue>
namespace JGW
{
    class CTSE_GlobalEnvironmentDefault : public CTSE_GlobalEnvironmentImpl
    {
    public:
        double GetDouble(const std::wstring& itemName);

        int GetInt(const std::wstring& itemName);

        __int64 GetInt64(const std::wstring& itemName);

        std::wstring GetString(const std::wstring& itemName);

        float GetFloat(const std::wstring& itemName);

        bool GetBool(const std::wstring& itemName);

        std::string GetAnsiString(const std::wstring& itemName);

        void PutDouble(const std::wstring& itemName,double itemValue);

        void PutString(const std::wstring& itemName,const std::wstring& itemValue);

        void PutFloat(const std::wstring& itemName,float itemValue);

        void PutInt64(const std::wstring& itemName,__int64 itemValue);

        void PutInt(const std::wstring& itemName,int itemValue);

        void PutBool(const std::wstring& itemName,bool itemValue);

        void PutAnsiString(const std::wstring& itemName,const std::string& itemValue);

        std::string GetAnsiCopyDataMessageQueue();

        void PutAnsiCopyDataMessageQueue(const std::string& strMessage);
    private:
        std::map<std::wstring,std::wstring> mmapKeyValue;
        std::queue<std::string> mqueueCopyDataMessage;
    };
}


