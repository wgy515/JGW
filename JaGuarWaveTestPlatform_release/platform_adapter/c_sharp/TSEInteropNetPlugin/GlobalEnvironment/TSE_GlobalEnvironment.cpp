#include "StdAfx.h"
#include "TSE_GlobalEnvironment.h"
#include <sstream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include <boost/thread.hpp>    
#include <boost/thread/recursive_mutex.hpp>    
#include <boost/thread/mutex.hpp>    
#include <boost/shared_ptr.hpp>    

typedef boost::shared_mutex           WR_Mutex;    
typedef boost::unique_lock<WR_Mutex>   writeLock;    
typedef boost::shared_lock<WR_Mutex>   readLock;    

typedef boost::recursive_mutex         Rcs_Mutex;    
typedef boost::unique_lock<Rcs_Mutex>  recuLock;  

namespace JGW
{
    WR_Mutex grwMutex; 

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
        readLock lockWrite(grwMutex); 
        std::map<std::wstring,std::wstring>::iterator it = mmapKeyValue.find(itemName);
        if (mmapKeyValue.end() == it) return L"";
        return it->second;
    }

    void CTSE_GlobalEnvironment::ClearGlobalEnvironment()
    {
        readLock lockWrite(grwMutex); 
        mmapKeyValue.clear();
    }

    int CTSE_GlobalEnvironment::GetInt(const std::wstring& itemName)
    {
       /* return _wtoi(GetString(itemName).c_str());*/
        int temp = 0;
        std::wistringstream wstr(GetString(itemName));
        wstr >> temp;
        return temp;
    }

    bool CTSE_GlobalEnvironment::GetBool(const std::wstring& itemName)
    {
        return (1 == GetInt(itemName));
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

    std::string CTSE_GlobalEnvironment::GetAnsiString(const std::wstring& itemName)
    {
        return JGW_W2A(GetString(itemName));
    }

    void CTSE_GlobalEnvironment::PutDouble(const std::wstring& itemName,double itemValue)
    {
        std::wostringstream wstr;
        wstr << itemValue;
        PutString(itemName,wstr.str());
    }

    void CTSE_GlobalEnvironment::PutString(const std::wstring& itemName,const std::wstring& itemValue)
    {
        writeLock lockWrite(grwMutex); 
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

    void CTSE_GlobalEnvironment::PutBool(const std::wstring& itemName,bool itemValue)
    {
        PutInt(itemName,itemValue?1:0);
    }

    void CTSE_GlobalEnvironment::PutAnsiString(const std::wstring& itemName,const std::string& itemValue)
    {
        PutString(itemName,JGW_A2W(itemValue));
    }
}