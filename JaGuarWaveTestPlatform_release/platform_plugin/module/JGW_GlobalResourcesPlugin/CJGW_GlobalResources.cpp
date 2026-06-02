#include "StdAfx.h"
#include "CJGW_GlobalResources.h"

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
    WR_Mutex grwGlobalResourcesMutex; 
    CCJGW_GlobalResources::CCJGW_GlobalResources(void)
    {
    }


    CCJGW_GlobalResources::~CCJGW_GlobalResources(void)
    {
    }

    void* CCJGW_GlobalResources::GetGlobalResourcesValue(const std::wstring& strGlobalResourcesName)
    {
        readLock lockWrite(grwGlobalResourcesMutex);
        return mmapGlobalResources[strGlobalResourcesName];
    }

    void CCJGW_GlobalResources::SetGlobalResourcesValue(const std::wstring& strGlobalResourcesName,void* pGlobalResourcesValue)
    {
        writeLock lockWrite(grwGlobalResourcesMutex);
        mmapGlobalResources[strGlobalResourcesName] = pGlobalResourcesValue;
    }
}