#pragma once
#include <time.h>

#define PIPE_TIMEOUT_ ( 10 * 60)
namespace JGW
{
	class jgw_timer
	{
	public:
		jgw_timer() { _start_time = clock(); } // postcondition: elapsed()==0
		//         jgw_timer( const jgw_timer& src );      // post: elapsed()==src.elapsed()
		//        ~jgw_timer(){}
		//  timer& operator=( const jgw_timer& src );  // post: elapsed()==src.elapsed()
		void   restart() { _start_time = clock(); } // post: elapsed()==0
		double elapsed() const                  // return elapsed time in seconds
		{ return  double(clock() - _start_time) / CLOCKS_PER_SEC; }

		double elapsed_max() const   // return estimated maximum value for elapsed()
			// Portability warning: elapsed_max() may return too high a value on systems
			// where std::clock_t overflows or resets at surprising values.
		{
			return (double((std::numeric_limits<clock_t>::max)())
				- double(_start_time)) / double(CLOCKS_PER_SEC); 
		}

		double elapsed_min() const            // return minimum value for elapsed()
		{ return double(1)/double(CLOCKS_PER_SEC); }

	private:
		clock_t _start_time;
	}; // timer
}

