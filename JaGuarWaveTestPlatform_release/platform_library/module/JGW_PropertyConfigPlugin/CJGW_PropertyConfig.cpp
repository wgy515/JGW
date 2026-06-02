#include "StdAfx.h"
#include "CJGW_PropertyConfig.h"

#include <boost/thread.hpp>    
#include <boost/thread/recursive_mutex.hpp>    
#include <boost/thread/mutex.hpp>    
#include <boost/shared_ptr.hpp>    

typedef std::unordered_map<std::wstring,std::wstring> _mcommontestconfig;

typedef boost::shared_mutex           WR_Mutex;    
typedef boost::unique_lock<WR_Mutex>   writeLock;    
typedef boost::shared_lock<WR_Mutex>   readLock;    

typedef boost::recursive_mutex         Rcs_Mutex;    
typedef boost::unique_lock<Rcs_Mutex>  recuLock;  

namespace JGW
{
    CCJGW_PropertyConfig::CCJGW_PropertyConfig(void)
    {
    }

    CCJGW_PropertyConfig::~CCJGW_PropertyConfig(void)
    {
    }
}