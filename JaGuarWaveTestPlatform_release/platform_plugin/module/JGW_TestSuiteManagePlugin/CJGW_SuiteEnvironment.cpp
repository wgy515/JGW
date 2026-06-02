#include "StdAfx.h"
#include "CJGW_SuiteEnvironment.h"
#include <unordered_map>

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
	static _mcommontestconfig						m_suiteCommonConfig;
	
	WR_Mutex										m_rwMutex; 

	void CCJGW_SuiteEnvironment::SetSuiteEnvironmentVar( const wchar_t* envirname,const wchar_t* envirvalue )
	{
		writeLock  lockWrite(m_rwMutex); 
		m_suiteCommonConfig[envirname] = envirvalue;
	}

	std::wstring CCJGW_SuiteEnvironment::GetSuiteEnvironmentVar( const wchar_t* envirname )
	{
		//! 判断当前key存不存在 不存在释放读锁
		{
			readLock lockRead(m_rwMutex);
			_mcommontestconfig::iterator it = m_suiteCommonConfig.find(envirname);
			if ( it != m_suiteCommonConfig.end() )
			{
				return it->second;
			}
		}
		//! 不存在	
		SetSuiteEnvironmentVar( envirname,L"" );
		return L"";
	}
}
