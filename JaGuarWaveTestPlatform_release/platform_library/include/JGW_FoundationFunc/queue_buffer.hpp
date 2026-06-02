#pragma once
#include <boost/thread.hpp>
#include <queue>

namespace JGW
{
    //! 消费者模式，先入先出模式
	template< class T >
	class queue_buffer
	{
	private:
		size_t max_count;
		boost::mutex mu;
		boost::condition_variable_any mcond_put;
		boost::condition_variable_any mcond_get;
		std::queue<T> mqueue_buffer;
	public:
		queue_buffer( size_t max = 10 ):max_count(max)
		{

		}

		bool is_empty()
		{
			return mqueue_buffer.size() == 0;
		}

		bool is_full()
		{
			return mqueue_buffer.size() == max_count;
		}

		void put( T pqueue_buffer )
		{
			{
				boost::mutex::scoped_lock sp_lock(mu);
				mcond_put.wait( mu,!boost::bind(&queue_buffer::is_full,this) );
				mqueue_buffer.push( pqueue_buffer );
			}
			mcond_get.notify_one();
		}

		T get()
		{
			T pqueue_buffer /*= NULL*/;
			{
				boost::mutex::scoped_lock sp_lock(mu);
				mcond_get.wait( mu,!boost::bind(&queue_buffer::is_empty,this) );
				pqueue_buffer = mqueue_buffer.front();
				mqueue_buffer.pop();
			}
			mcond_put.notify_one();
			return pqueue_buffer;
		}

	};
};
