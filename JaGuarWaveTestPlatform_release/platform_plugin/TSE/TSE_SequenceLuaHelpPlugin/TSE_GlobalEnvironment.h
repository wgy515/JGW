#pragma once
#include <TSE_SequenceManagerPlugin/TSE_GlobalEnvironmentImpl.h>
#include <map>
#include <queue>

//#define USE_IPC_GLOBALVARIABLE

namespace JGW
{
    class CTSE_GlobalEnvironment:public CTSE_GlobalEnvironmentImpl
    {
    public:
		CTSE_GlobalEnvironment();
		~CTSE_GlobalEnvironment();
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
#ifndef USE_IPC_GLOBALVARIABLE
        std::map<std::wstring,std::wstring> mmapKeyValue;
#endif  
        std::queue<std::string> mqueueCopyDataMessage;
		void* grwMutex;
    };
}