#pragma once
#include <unordered_map>
#include <JGW_PropertyConfigPlugin/CJGW_PropertyImpl.h>
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
    class CCJGW_PropertyMemoryConfig : public CCJGW_PropertyImpl
    {
    public:
        CCJGW_PropertyMemoryConfig(void);
        virtual ~CCJGW_PropertyMemoryConfig(void);
    protected:
        virtual double GetDouble(const std::wstring& itemName);

        virtual int GetInt(const std::wstring& itemName);

        virtual __int64 GetInt64(const std::wstring& itemName);

        virtual std::wstring GetString(const std::wstring& itemName);

        virtual float GetFloat(const std::wstring& itemName);

        virtual bool GetBool(const std::wstring& itemName);

        virtual void PutDouble(const std::wstring& itemName,double itemValue);

        virtual void PutString(const std::wstring& itemName,const std::wstring& itemValue);

        virtual void PutFloat(const std::wstring& itemName,float itemValue);

        virtual void PutInt64(const std::wstring& itemName,__int64 itemValue);

        virtual void PutInt(const std::wstring& itemName,int itemValue);

        virtual void PutBool(const std::wstring& itemName,bool itemValue);

        virtual void CopyProperty(std::unordered_map<std::wstring, std::wstring>& mapProperty);

        virtual const std::unordered_map<std::wstring,std::wstring>::iterator Begin();

        virtual const std::unordered_map<std::wstring,std::wstring>::iterator End();

        virtual void SetPropertyAppName(const std::wstring& strPropertyAppName = L"propety");

        virtual void ReloadPropertyFile();
    protected:
        WR_Mutex mrwMutex; 
        std::unordered_map<std::wstring,std::wstring> mmapPropertyConfig;
    };
}