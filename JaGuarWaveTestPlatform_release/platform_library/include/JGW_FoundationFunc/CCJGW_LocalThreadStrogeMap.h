#pragma once
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
typedef boost::shared_lock<boost::shared_mutex> readLock;
typedef boost::unique_lock<boost::shared_mutex> writeLock;

template<class T>
namespace JGW
{
    class CCJGW_LocalThreadStrogeMap
    {
    public:
        std::shared_ptr<T> Get()
        {
            std::shared_ptr<T> ptr = GetLocal();
            if( !ptr )
            {
                ptr = Set();
            }
            return ptr;
        }
        std::shared_ptr<T> Set()
        {
            writeLock  wtlock( rwmutex );
            std::shared_ptr<T> ptr = std::make_shared<T>();
            local_map[GetCurrentThreadId()] = ptr;
            return ptr;
        }
    private:
        //! ªÒ»°
        std::shared_ptr<T> GetLocal()
        {
            readLock  rdlock( rwmutex );

            std::map<unsigned long,std::shared_ptr<T>>::iterator it = local_map.find(GetCurrentThreadId()) ;
            if( it != local_map.end() )
            {
                return it->second;
            }
            return NULL;
        }

    private:
        boost::shared_mutex  rwmutex;
        std::map<unsigned long,std::shared_ptr<T>> local_map;
    };
}