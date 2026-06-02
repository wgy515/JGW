#pragma once
#include <Windows.h>
//long 
namespace JGW
{
	class CCJGW_CHiPerfTimer
	{
	public:
		CCJGW_CHiPerfTimer(void);
		~CCJGW_CHiPerfTimer(void);
		void	InitTime();
		void	Start();
		void	Stop();
		double	GetTimeMs();
	private:
		LARGE_INTEGER startTime, stopTime,freq;
	};
}


